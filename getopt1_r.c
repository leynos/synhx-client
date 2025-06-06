/* getopt_long and getopt_long_only entry points for GNU getopt.
   Copyright (C) 1987,88,89,90,91,92,93,94,96,97,98
     Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */
  #include "config.h"

  /* This is a separate conditional since some stdc systems
  reject `defined (const)'.  */
  #ifndef const
    #define const
  #endif

  #include <gnu-versions.h>
  #if _GNU_GETOPT_R_INTERFACE_VERSION == GETOPT_R_INTERFACE_VERSION
    #define ELIDE_CODE
  #endif
  #include <stdlib.h>
#endif
  #define NULL 0

int argc;
char *const *argv;
const char *options;
const struct option *long_options;
int *opt_index;
struct opt_r *opt;
int argc;
char *const *argv;
const char *options;
const struct option *long_options;
int *opt_index;
struct opt_r *opt;
int argc;
char **argv;
  while (1) {
    int this_option_optind = opt.ind ? opt.ind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"add", 1, 0, 0},
      {"append", 0, 0, 0},
      {"delete", 1, 0, 0},
      {"verbose", 0, 0, 0},
      {"create", 0, 0, 0},
      {"file", 1, 0, 0},
      {0, 0, 0, 0}
    };

    c = getopt_long_r (argc, argv, "abc:d:0123456789",
                       long_options, &option_index, &opt);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 0:
        printf ("option %s", long_options[option_index].name);
        if (opt.arg) {
          printf (" with arg %s", opt.arg);
        }
        printf ("\n");
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (digit_optind != 0 && digit_optind != this_option_optind) {
          printf ("digits occur in two different argv-elements.\n");
        }
        digit_optind = this_option_optind;
        printf ("option %c\n", c);
        break;

      case 'a':
        printf ("option a\n");
        break;

      case 'b':
        printf ("option b\n");
        break;

      case 'c':
        printf ("option c with value `%s'\n", opt.arg);
        break;

      case 'd':
        printf ("option d with value `%s'\n", opt.arg);
        break;

      case '?':
        break;

      default:
        printf ("?? getopt returned character code 0%o ??\n", c);
  }
  if (opt.ind < argc) {
    printf ("non-option ARGV-elements: ");
    while (opt.ind < argc) {
      printf ("%s ", argv[opt.ind++]);
    printf ("\n");
  }
	{"verbose", 0, 0, 0},
	{"create", 0, 0, 0},
	{"file", 1, 0, 0},
	{0, 0, 0, 0}
      };

      c = getopt_long_r (argc, argv, "abc:d:0123456789",
		         long_options, &option_index, &opt);
      if (c == -1)
	break;

      switch (c)
	{
	case 0:
	  printf ("option %s", long_options[option_index].name);
	  if (opt.arg)
	    printf (" with arg %s", opt.arg);
	  printf ("\n");
	  break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  if (digit_optind != 0 && digit_optind != this_option_optind)
	    printf ("digits occur in two different argv-elements.\n");
	  digit_optind = this_option_optind;
	  printf ("option %c\n", c);
	  break;

	case 'a':
	  printf ("option a\n");
	  break;

	case 'b':
	  printf ("option b\n");
	  break;

	case 'c':
	  printf ("option c with value `%s'\n", opt.arg);
	  break;

	case 'd':
	  printf ("option d with value `%s'\n", opt.arg);
	  break;

	case '?':
	  break;

	default:
	  printf ("?? getopt returned character code 0%o ??\n", c);
	}
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
