/**************************************************************************
 *
 * sum.c -- byte summing program
 * Copyright (C) 1995  Tim Shimmin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: simple_sum.c,v 1.2 2004/10/31 00:09:16 beebe Exp beebe $
 *
 **************************************************************************/

/*
$Log: simple_sum.c,v $
Revision 1.2  2004/10/31 00:09:16  beebe
Major update for version 1.3.64x to support 64-bit architectures.

*/

/*
 * If given -l option then just prints the length.
 * Normally prints the sum and the length for the file.
 */

static int length_only = 0;

#include <stdio.h>
#include "mg_types.h"

void 
output_sum(fname, input_file, output_file)
char *fname;
FILE *input_file;
FILE *output_file;
{
  int ch = '\0';
  MG_u_long_t sum = 0;
  MG_u_long_t num_bytes = 0;

  while((ch = getc(input_file))!=EOF){
    sum += ch;
    num_bytes++;
  }

  if (length_only)
    fprintf(output_file, "%s: %ld\n", fname, (long)num_bytes);
  else
    fprintf(output_file, "%s: %ld %ld\n", fname, (long)sum, (long)num_bytes);
}

int
main(argc, argv)
int argc;
char *argv[];
{
  int i = 0;

  /* use stdin */
  if (argc == 1){
    output_sum("-", stdin, stdout);
    return(0);
  }

  /* use args as file names for input */
  for (i=1;i<argc; i++){
    char *fname = argv[i];
    FILE *input_file = NULL;
    
    if (!length_only && (argv[i][0] == '-') && (argv[i][1] =='l') ) 
      { 
        length_only = 1; 
        continue;
      }

    input_file = fopen(fname, "r");
    if (!input_file){
      fprintf(stderr,"Could not open %s\n", fname);
      continue;
    }
    else{
      output_sum(fname, input_file, stdout);
      fclose(input_file);
    }
  }/*for*/
  return (0);
}
