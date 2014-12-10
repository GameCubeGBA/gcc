/* Copyright (C) 2002-2014 Free Software Foundation, Inc.
   Contributed by Andy Vaught and Paul Brook <paul@nowt.org>

This file is part of the GNU Fortran runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "libgfortran.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* Stupid function to be sure the constructor is always linked in, even
   in the case of static linking.  See PR libfortran/22298 for details.  */
void
stupid_function_name_for_static_linking (void)
{
  return;
}

options_t options;

/* This will be 0 for little-endian
   machines and 1 for big-endian machines.

   Currently minimal libgfortran only runs on little-endian devices
   which don't support constructors so this is just a constant.  */
int big_endian = 0;

static int argc_save;
static char **argv_save;

static const char *exe_path;

/* recursion_check()-- It's possible for additional errors to occur
 * during fatal error processing.  We detect this condition here and
 * exit with code 4 immediately. */

#define MAGIC 0x20DE8101

static void
recursion_check (void)
{
  static int magic = 0;

  /* Don't even try to print something at this point */
  if (magic == MAGIC)
    sys_abort ();

  magic = MAGIC;
}

#define STRERR_MAXSZ 256

void
os_error (const char *message)
{
  recursion_check ();
  printf ("Operating system error: ");
  printf ("%s\n", message);
  exit (1);
}
iexport(os_error);

void
runtime_error (const char *message, ...)
{
  va_list ap;

  recursion_check ();
  printf ("Fortran runtime error: ");
  va_start (ap, message);
  vprintf (message, ap);
  va_end (ap);
  printf ("\n");
  exit (2);
}
iexport(runtime_error);

/* void runtime_error_at()-- These are errors associated with a
 * run time error generated by the front end compiler.  */

void
runtime_error_at (const char *where, const char *message, ...)
{
  va_list ap;

  recursion_check ();
  printf ("Fortran runtime error: ");
  va_start (ap, message);
  vprintf (message, ap);
  va_end (ap);
  printf ("\n");
  exit (2);
}
iexport(runtime_error_at);

/* Return the full path of the executable.  */
char *
full_exe_path (void)
{
  return (char *) exe_path;
}


/* Set the saved values of the command line arguments.  */

void
set_args (int argc, char **argv)
{
  argc_save = argc;
  argv_save = argv;
  exe_path = argv[0];
}
iexport(set_args);


/* Retrieve the saved values of the command line arguments.  */

void
get_args (int *argc, char ***argv)
{
  *argc = argc_save;
  *argv = argv_save;
}

/* sys_abort()-- Terminate the program showing backtrace and dumping
   core.  */

void
sys_abort (void)
{
  printf ("Abort called.\n");
  abort();
}
