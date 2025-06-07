#include <stddef.h>
#include "getopt.h"

static int do_getopt_long_r(int argc, char *const argv[], const char *optstring,
                            const struct option *longopts, int *longind,
                            int long_only, struct opt_r *opt);

int getopt_r(int argc, char *const argv[], const char *optstring,
             struct opt_r *opt);

int _getopt_r_internal(int argc, char *const argv[], const char *optstring,
                       const struct option *longopts, int *longind,
                       int long_only, struct opt_r *opt);

static int
do_getopt_long_r(int argc, char *const argv[], const char *optstring,
                 const struct option *longopts, int *longind, int long_only,
                 struct opt_r *opt)
  opterr = 0;
  optarg = NULL;
  int ret;
  if (longopts == NULL) {
    ret = getopt(argc, argv, optstring);
  } else if (long_only) {
    ret = getopt_long_only(argc, argv, optstring, longopts, longind);
  } else {
    ret = getopt_long(argc, argv, optstring, longopts, longind);
  opt->arg = optarg;
  opt->ind = optind;
  opt->opt = ret;
  return ret;
getopt_r(int argc, char *const argv[], const char *optstring,
         struct opt_r *opt)
  return do_getopt_long_r(argc, argv, optstring, NULL, NULL, 0, opt);
_getopt_r_internal(int argc, char *const argv[], const char *optstring,
                   const struct option *longopts, int *longind, int long_only,
                   struct opt_r *opt)
  return do_getopt_long_r(argc, argv, optstring, longopts, longind,
                          long_only, opt);
    }

  if (opt.ind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (opt.ind < argc)
	printf ("%s ", argv[opt.ind++]);
      printf ("\n");
    }

  exit (0);
}

#endif /* TEST */
