#include "getopt.h"

extern int _getopt_r_internal(int argc, char *const argv[], const char *optstring,
                              const struct option *longopts, int *longind,
                              int long_only, struct opt_r *opt);

int
getopt_long_r(int argc, char *const argv[], const char *options,
              const struct option *long_options, int *opt_index,
              struct opt_r *opt)
{
  return _getopt_r_internal(argc, argv, options, long_options, opt_index, 0,
                            opt);
}

int
getopt_long_only_r(int argc, char *const argv[], const char *options,
                   const struct option *long_options, int *opt_index,
                   struct opt_r *opt)
{
  return _getopt_r_internal(argc, argv, options, long_options, opt_index, 1,
                            opt);
}
