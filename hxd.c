#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include "xmalloc.h"
#include "hxd.h"
#include "hx.h"

#define _PATH_HXD_CONF "./hxd.conf"

char **hxd_environ = 0;

int hxd_open_max = 0;
struct hxd_file *hxd_files = 0;

int high_fd = 0;

fd_set hxd_rfds,
       hxd_wfds;

void
qbuf_set (struct qbuf *q, u_int32_t pos, u_int32_t len)
{
  int need_more = q->pos + q->len < pos + len;

  q->pos = pos;
  q->len = len;
  if (need_more) {
    q->buf = xrealloc(q->buf, q->pos + q->len + 1);  /* +1 for null */
  }
}

void
qbuf_add (struct qbuf *q, void *buf, u_int32_t len)
{
  size_t pos = q->pos + q->len;

  qbuf_set(q, q->pos, q->len + len);
  memcpy(&q->buf[pos], buf, len);
}

void
hxd_log (const char *fmt, ...)
{
  va_list ap;
  char buf[2048];
  int len;
  time_t t;
  struct tm tm;

  time(&t);
  localtime_r(&t, &tm);
  strftime(buf, 21, "%H:%M:%S %m/%d/%Y\t", &tm);
  va_start(ap, fmt);
  len = vsnprintf(&buf[20], sizeof(buf) - 24, fmt, ap);
  va_end(ap);
  if (len == -1) {
    len = sizeof(buf) - 24;
  }
  len += 20;
  buf[len++] = '\n';
  buf[len] = 0;
  hx_printf_prefix(&hx_htlc, 0, INFOPREFIX, "%s", buf);
}

static RETSIGTYPE
sig_alrm (int sig __attribute__((__unused__)))
{

}

#if !defined(ONLY_GTK)
struct timer {
  struct timer *next;
  struct timeval add_tv;
  struct timeval tv;
  int (*fn)();
  void *ptr;
  u_int8_t expire;
};

static struct timer *timer_list = 0;

void
timer_add (struct timeval *tv, int (*fn)(), void *ptr)
{
  struct timer *timer, *timerp;

  timer = xmalloc(sizeof(struct timer));
  timer->add_tv = *tv;
  timer->tv = *tv;
  timer->fn = fn;
  timer->ptr = ptr;

  timer->expire = 0;

  if (!timer_list || (timer_list->tv.tv_sec > timer->tv.tv_sec
                      || (timer_list->tv.tv_sec == timer->tv.tv_sec && timer_list->tv.tv_usec > timer->tv.tv_usec))) {
    timer->next = timer_list;
    timer_list = timer;
    return;
  }
  for (timerp = timer_list; timerp; timerp = timerp->next) {
    if (!timerp->next || (timerp->next->tv.tv_sec > timer->tv.tv_sec
                          || (timerp->next->tv.tv_sec == timer->tv.tv_sec && timerp->next->tv.tv_usec > timer->tv.tv_usec))) {
      timer->next = timerp->next;
      timerp->next = timer;
      return;
    }
  }
}

void
timer_delete_ptr (void *ptr)
{
  struct timer *timerp, *next;

  if (!timer_list) {
    return;
  }
  while (timer_list->ptr == ptr) {
    next = timer_list->next;
    xfree(timer_list);
    timer_list = next;
    if (!timer_list) {
      return;
    }
  }
  for (timerp = timer_list; timerp->next; timerp = next) {
    next = timerp->next;
    if (next->ptr == ptr) {
      next = timerp->next->next;
      xfree(timerp->next);
      timerp->next = next;
      next = timerp;
    }
  }
}

void
timer_add_secs (time_t secs, int (*fn)(), void *ptr)
{
  struct timeval tv;
  tv.tv_sec = secs;
  tv.tv_usec = 0;
  timer_add(&tv, fn, ptr);
}

static void
timer_check (struct timeval *before, struct timeval *after)
{
  struct timer *timer, *next, *prev;
  time_t secdiff, usecdiff;

  secdiff = after->tv_sec - before->tv_sec;
  if (before->tv_usec > after->tv_usec) {
    secdiff--;
    usecdiff = 1000000 - (before->tv_usec - after->tv_usec);
  } else {
    usecdiff = after->tv_usec - before->tv_usec;
  }
  for (timer = timer_list; timer; timer = timer->next) {
    if (secdiff > timer->tv.tv_sec
        || (secdiff == timer->tv.tv_sec && usecdiff >= timer->tv.tv_usec)) {
      timer->expire = 1;
      timer->tv.tv_sec = timer->add_tv.tv_sec
                         - (secdiff - timer->tv.tv_sec);
      if (usecdiff > (timer->tv.tv_usec + timer->add_tv.tv_usec)) {
        timer->tv.tv_sec -= 1;
        timer->tv.tv_usec = 1000000 - timer->add_tv.tv_usec
                            + timer->tv.tv_usec - usecdiff;
      } else {
        timer->tv.tv_usec = timer->add_tv.tv_usec
                            + timer->tv.tv_usec - usecdiff;
      }
    } else {
      timer->tv.tv_sec -= secdiff;
      if (usecdiff > timer->tv.tv_usec) {
        timer->tv.tv_sec -= 1;
        timer->tv.tv_usec = 1000000 - (usecdiff - timer->tv.tv_usec);
      } else {
        timer->tv.tv_usec -= usecdiff;
      }
    }
  }

  prev = 0;
  for (timer = timer_list; timer; timer = next) {
    next = timer->next;
    if (timer->expire) {
      int keep;
      int (*fn)() = timer->fn, *ptr = timer->ptr;

      if (prev) {
        prev->next = next;
      }
      if (timer == timer_list) {
        timer_list = next;
      }
      keep = fn(ptr);
      if (keep) {
        timer_add(&timer->add_tv, fn, ptr);
      }
      xfree(timer);
      next = timer_list;
    } else {
      prev = timer;
    }
  }
}

void
hxd_fd_add (int fd)
{
  if (high_fd < fd) {
    high_fd = fd;
  }
}

void
hxd_fd_del (int fd)
{
  if (high_fd == fd) {
    for (fd--; fd && !FD_ISSET(fd, &hxd_rfds); fd--)
      ;
    high_fd = fd;
  }
}

void
hxd_fd_set (int fd, int rw)
{
  if (rw & FDR) {
    FD_SET(fd, &hxd_rfds);
  }
  if (rw & FDW) {
    FD_SET(fd, &hxd_wfds);
  }
}

void
hxd_fd_clr (int fd, int rw)
{
  if (rw & FDR) {
    FD_CLR(fd, &hxd_rfds);
  }
  if (rw & FDW) {
    FD_CLR(fd, &hxd_wfds);
  }
}

static void loopZ (void) __attribute__((__noreturn__));

static void
loopZ (void)
{
  fd_set rfds, wfds;
  struct timeval before, tv;

  gettimeofday(&tv, 0);
  for (;;) {
    register int n, i;

    if (timer_list) {
      gettimeofday(&before, 0);
      timer_check(&tv, &before);
      if (timer_list) {
        tv = timer_list->tv;
      }
    }
    rfds = hxd_rfds;
    wfds = hxd_wfds;
    n = select(high_fd + 1, &rfds, &wfds, 0, timer_list ? &tv : 0);
    if (n < 0 && errno != EINTR) {
      hxd_log("loopZ: select: %s", strerror(errno));
      exit(1);
    }
    gettimeofday(&tv, 0);
    if (timer_list) {
      timer_check(&before, &tv);
    }
    if (n <= 0) {
      continue;
    }
    for (i = 0; i < high_fd + 1; i++) {
      if (FD_ISSET(i, &rfds) && FD_ISSET(i, &hxd_rfds)) {
        if (hxd_files[i].ready_read) {
          hxd_files[i].ready_read(i);
        }
        if (!--n) {
          break;
        }
      }
      if (FD_ISSET(i, &wfds) && FD_ISSET(i, &hxd_wfds)) {
        if (hxd_files[i].ready_write) {
          hxd_files[i].ready_write(i);
        }
        if (!--n) {
          break;
        }
      }
    }
  }
}
#endif /* ONLY_GTK */

#if defined(CONFIG_CIPHER)
#include "cipher.h"

#if USE_OPENSSL
#include <openssl/rand.h>

static char *egd_path = 0;

static void
set_egd_path (char **egd_pathp, const char *str)
{
  int r;

  if (*egd_pathp) {
    xfree(*egd_pathp);
  }
  *egd_pathp = xstrdup(str);
  #if 0 /* old openssl */
  r = -1;
  #else
  r = RAND_egd(str);
  #endif
  if (r == -1) {
    hx_printf_prefix(&hx_htlc, 0, INFOPREFIX,
                     "failed to get entropy from egd socket %s\n", str);
  }
}
#endif

static void
cipher_init (void)
{
  #if USE_OPENSSL
  variable_add(&egd_path, set_egd_path, "egd_path");
  #else
  srand(getpid()*clock());
  #endif
}
#endif /* CONFIG_CIPHER */

extern void hlserver_reap_pid (pid_t pid, int status);
extern void hlclient_reap_pid (pid_t pid, int status);

RETSIGTYPE
sig_chld (int sig __attribute__((__unused__)))
{
  int status, serrno = errno;
  pid_t pid;

  #ifndef WAIT_ANY
#define WAIT_ANY -1
  #endif

  for (;;) {
    pid = waitpid(WAIT_ANY, &status, WNOHANG);
    if (pid < 0) {
      if (errno == EINTR) {
        continue;
      }
      goto ret;
    }
    if (!pid) {
      goto ret;
    }
    hlclient_reap_pid(pid, status);
  }

ret:
  errno = serrno;
}

static RETSIGTYPE
sig_bus (int sig __attribute__((__unused__)))
{
  hxd_log("\n\
caught SIGBUS -- mail ran@krazynet.com with output from:\n\
$ gcc -v hxd.c\n\
$ cc -v hxd.c\n\
$ gdb hxd core\n\
(gdb) backtrace\n\
and any other information you think is useful");
  abort();
}

extern void hotline_server_init (void);
extern void hotline_client_init (int argc, char **argv);
extern void tracker_server_init (void);
extern void tracker_register_init (void);

static RETSIGTYPE
sig_tstp (int sig __attribute__((__unused__)))
{
  hxd_log("SIGTSTP");
}

#if !defined(_SC_OPEN_MAX) && defined(HAVE_GETRLIMIT)
  #include <sys/time.h>
  #include <sys/resource.h>
#endif

#if 0
static struct timeval tv1, tv2, tv3, ctv1,ctv2,ctv3;
int
tfn (struct timeval *tv)
{
  struct timeval *ctv;
  time_t s, us, secdiff, usecdiff;

  hxd_log("timer: %u, %u", tv->tv_sec, tv->tv_usec);
  if (tv==&tv1) {
    ctv=&ctv1;
  } else if (tv==&tv2) {
    ctv=&ctv2;
  } else if (tv==&tv3) {
    ctv=&ctv3;
  }
  s = ctv->tv_sec;
  us = ctv->tv_usec;
  gettimeofday(ctv,0);
  secdiff = ctv->tv_sec - s;
  if (us > ctv->tv_usec) {
    secdiff--;
    usecdiff = 1000000 - (us - ctv->tv_usec);
  } else {
    usecdiff = ctv->tv_usec - us;
  }
  hxd_log("real: %u, %u",secdiff,usecdiff);
  return 1;
}
void tfark () __attribute__((__constructor__));
void tfark ()
{
  tv1.tv_sec = 2;
  tv1.tv_usec = 100000;
  gettimeofday(&ctv1,0);
  timer_add(&tv1, tfn, &tv1);
  tv2.tv_sec = 1;
  tv2.tv_usec = 700000;
  gettimeofday(&ctv2,0);
  timer_add(&tv2, tfn, &tv2);
  tv3.tv_sec = 4;
  tv3.tv_usec = 000000;
  gettimeofday(&ctv3,0);
  timer_add(&tv3, tfn, &tv3);
}
#endif

static RETSIGTYPE
sig_fpe (int sig)
{
  hxd_log("SIGFPE (%d)", sig);
  abort();
}

#if XMALLOC_DEBUG
  extern void DTBLINIT (void);
#endif

int
main (int argc __attribute__((__unused__)), char **argv __attribute__((__unused__)), char **envp)
{
  struct sigaction act;

  #if XMALLOC_DEBUG
  DTBLINIT();
  #endif
  #if defined(_SC_OPEN_MAX)
  hxd_open_max = sysconf(_SC_OPEN_MAX);
  #elif defined(RLIMIT_NOFILE)
  {
    struct rlimit rlimit;

    if (getrlimit(RLIMIT_NOFILE, &rlimit)) {
      exit(1);
    }
    hxd_open_max = rlimit.rlim_max;
  }
  #elif defined(HAVE_GETDTABLESIZE)
  hxd_open_max = getdtablesize();
  #elif defined(OPEN_MAX)
  hxd_open_max = OPEN_MAX;
  #else
  hxd_open_max = 16;
  #endif
  if (hxd_open_max > FD_SETSIZE) {
    hxd_open_max = FD_SETSIZE;
  }
  hxd_files = xmalloc(hxd_open_max * sizeof(struct hxd_file));
  memset(hxd_files, 0, hxd_open_max * sizeof(struct hxd_file));
  FD_ZERO(&hxd_rfds);
  FD_ZERO(&hxd_wfds);

  hxd_environ = envp;

  act.sa_handler = SIG_IGN;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGHUP, &act, 0);
  sigaction(SIGPIPE, &act, 0);
  act.sa_handler = sig_fpe;
  sigaction(SIGFPE, &act, 0);
  act.sa_handler = sig_bus;
  sigaction(SIGBUS, &act, 0);
  act.sa_handler = sig_alrm;
  sigaction(SIGALRM, &act, 0);
  act.sa_handler = sig_chld;
  act.sa_flags |= SA_NOCLDSTOP;
  sigaction(SIGCHLD, &act, 0);

  sigaction(SIGTSTP, &act, 0);
  act.sa_handler = sig_tstp;

  #if defined(HAVE_LIBHPPA)
  allow_unaligned_data_access();
  #endif

  #if defined(CONFIG_CIPHER)
  cipher_init();
  #endif
  hotline_client_init(argc, argv);

  hx_output.loop();

  #if !defined(ONLY_GTK)
  loopZ();
  #endif

  return 0;
}
