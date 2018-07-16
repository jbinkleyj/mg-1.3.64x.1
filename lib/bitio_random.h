/**************************************************************************
 *
 * bitio_random.h -- Functions for bitio to a file (random access)
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
 * $Id: bitio_random.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************
 *
 *  This file contains function definitions for doing bitwise input and output
 *  of numbers on an array of chars. These routines are slower than the ones 
 *  in "mem" files. but with these routines you can mix reads and writes, or 
 *  multiple writes,  on the array of chars at the same time and guarantee
 *  them to work, also you can seek to a point and do a write. The decode and 
 *  encode functions cannot detect when the end off the character is reached
 *  and just continue processing.
 *
 **************************************************************************/

#ifndef H_BITIO_RANDOM
#define H_BITIO_RANDOM



typedef struct random_bitio_state
  {
    FILE *File;
    unsigned char *Buf;
    MG_u_long_t Base;
    MG_u_long_t Used;
    MG_u_long_t pos;
    MG_u_long_t len;
    MG_u_long_t sft;
  }
random_bitio_state;


/* NOTE : All bytes are filled high bit first */


void BIO_Random_Start (FILE * f, MG_u_long_t len,
		       random_bitio_state * bs);
void BIO_Random_Done (random_bitio_state * bs);



void BIO_Random_Decode_Start (void *buf, MG_u_long_t pos,
			      random_bitio_state * bs);

void BIO_Random_Encode_Bit (int bit, random_bitio_state * bs);

int BIO_Random_Decode_Bit (random_bitio_state * bs);


void BIO_Random_Unary_Encode (MG_u_long_t val, random_bitio_state * bs,
			      MG_u_long_t *bits);
MG_u_long_t BIO_Random_Unary_Decode (random_bitio_state * bs,
				       MG_u_long_t *bits);



void BIO_Random_Binary_Encode (MG_u_long_t val, MG_u_long_t b,
			       random_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Random_Binary_Decode (MG_u_long_t b, random_bitio_state * bs,
					MG_u_long_t *bits);



void BIO_Random_Gamma_Encode (MG_u_long_t val, random_bitio_state * bs,
			      MG_u_long_t *bits);
MG_u_long_t BIO_Random_Gamma_Decode (random_bitio_state * bs,
				       MG_u_long_t *bits);



void BIO_Random_Delta_Encode (MG_u_long_t val, random_bitio_state * bs,
			      MG_u_long_t *bits);
MG_u_long_t BIO_Random_Delta_Decode (random_bitio_state * bs,
				       MG_u_long_t *bits);


void BIO_Random_Elias_Encode (MG_u_long_t val, MG_u_long_t b, double s,
			      random_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Random_Elias_Decode (MG_u_long_t b, double s,
				       random_bitio_state * bs,
				       MG_u_long_t *bits);


void BIO_Random_Bblock_Encode (MG_u_long_t val, MG_u_long_t b,
			       random_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Random_Bblock_Decode (MG_u_long_t b,
					random_bitio_state * bs,
					MG_u_long_t *bits);


void BIO_Random_Seek (MG_u_long_t pos, random_bitio_state * bs);

void BIO_Random_Flush (random_bitio_state * bs);

MG_u_long_t BIO_Random_Tell (random_bitio_state * bs);


#endif
