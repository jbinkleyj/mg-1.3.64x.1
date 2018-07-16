/**************************************************************************
 *
 * huffman.h -- Huffman coding functions
 * Copyright (C) 1994  Neil Sharman
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
 * $Id: huffman.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************/

#ifndef H_HUFFMAN
#define H_HUFFMAN

#include "sysfuncs.h"

#include "filestats.h"


#define MAX_HUFFCODE_LEN 31

typedef struct huff_data
  {
    int num_codes;
    int mincodelen;
    int maxcodelen;
    int lencount[MAX_HUFFCODE_LEN + 1];
    MG_u_long_t min_code[MAX_HUFFCODE_LEN + 1];
    char *clens;
  }
huff_data;


huff_data *Generate_Huffman_Data (int num, MG_long_t *freqs, huff_data * data,
				  MG_u_long_t * mem);

MG_u_long_t *Generate_Huffman_Codes (huff_data * data, MG_u_long_t * mem);

MG_u_long_t **Generate_Huffman_Vals (huff_data * data, MG_u_long_t * mem);

int Write_Huffman_Data (FILE * f, huff_data * hd);

int Read_Huffman_Data (FILE * f, huff_data * hd, MG_u_long_t * mem, MG_u_long_t * disk);

int F_Read_Huffman_Data (File * f, huff_data * hd, MG_u_long_t * mem, MG_u_long_t * disk);

/* Calculate the number of bits required to code the data with the
   specified frequencies. Normally freqs and counts should point to
   the same array. */
double Calculate_Huffman_Size (int num, MG_long_t *freqs, MG_long_t *counts);



#define HUFF_ENCODE(x, codes, lens)					\
  do {									\
    register int __i;							\
    register int __clen = (lens)[x];					\
    register MG_u_long_t __code = (codes)[x];				\
    for (__i=__clen-1; __i>=0; __i--)					\
      ENCODE_BIT((__code >> __i) & 1);					\
  } while(0)

#define HUFF_ENCODE_L(x, codes, lens, count)				\
  do {									\
    HUFF_ENCODE(x, codes, lens);		   			\
    (count) += (lens)[x];						\
  } while(0)


#define HUFF_DECODE(x, mcodes, values)					\
  do {									\
    register MG_u_long_t *__min_code = (mcodes);			\
    register MG_u_long_t *__mclen = __min_code;			\
    register MG_u_long_t **__values = (values);			\
    register MG_u_long_t __code = 0;					\
    do									\
      {									\
        DECODE_ADD(__code);						\
      }									\
    while (__code < *++__mclen);					\
    (x) = __values[__mclen - __min_code][__code - *__mclen];		\
  } while(0);


#define HUFF_DECODE_L(x, mcodes, values, count)				\
  do {									\
    register MG_u_long_t *__min_code = (mcodes);			\
    register MG_u_long_t *__mclen = __min_code;			\
    register MG_u_long_t **__values = (values);			\
    register MG_u_long_t __code = 0;					\
    do									\
      {									\
        DECODE_ADD(__code);						\
	(count)++;							\
      }									\
    while (__code < *++__mclen);					\
    (x) = __values[__mclen - __min_code][__code - *__mclen];		\
  } while(0);



#endif
