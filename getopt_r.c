/* Reentrant wrappers around getopt functions.
 *
 * These helpers call getopt(), getopt_long() or getopt_long_only() and copy
 * the global results into an opt_r structure. They are minimal replacements
 * for the GNU getopt_r API used by older code.
 */

#include <stddef.h>
#include "getopt.h"

static int do_getopt_long_r(int argc, char *const argv[], const char *optstring,
                            const struct option *longopts, int *longind,
                            int long_only, struct opt_r *opt)
{
  int ret;

  opterr = 0;
  optarg = NULL;

  if (longopts == NULL) {
    ret = getopt(argc, argv, optstring);
  } else if (long_only) {
    ret = getopt_long_only(argc, argv, optstring, longopts, longind);
  } else {
    ret = getopt_long(argc, argv, optstring, longopts, longind);
  }

  opt->arg = optarg;
  opt->ind = optind;
  opt->opt = ret;

  return ret;
}

int getopt_r(int argc, char *const argv[], const char *optstring,
             struct opt_r *opt)
{
  return do_getopt_long_r(argc, argv, optstring, NULL, NULL, 0, opt);
}

/*
 * Provide the _getopt_r_internal symbol expected by the legacy
 * getopt wrapper functions in getopt1_r.c.  The prototype for this
 * function is declared in getopt.h, so the definition here must not
 * be static; otherwise GCC emits a conflicting declaration error when
 * the header is included.  Keeping it non-static matches the header
 * and allows older code to link correctly.
 */
int _getopt_r_internal(int argc, char *const argv[], const char *optstring,
                       const struct option *longopts, int *longind,
                       int long_only, struct opt_r *opt)
{
  return do_getopt_long_r(argc, argv, optstring, longopts, longind, long_only,
                          opt);
}

