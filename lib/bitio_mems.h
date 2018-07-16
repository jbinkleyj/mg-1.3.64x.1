/**************************************************************************
 *
 * bitio_mems.h -- Functions for bitio to memory (random access)
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
 * $Id: bitio_mems.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
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

#ifndef H_BITIO_MEMS
#define H_BITIO_MEMS



typedef struct mems_bitio_state
  {
    unsigned char *Base;
    MG_u_long_t pos;
  }
mems_bitio_state;


/* NOTE : All bytes are filled high bit first */


void BIO_Mems_Encode_Start (char *buf, MG_u_long_t pos, mems_bitio_state * bs);
void BIO_Mems_Encode_Done (mems_bitio_state * bs);



void BIO_Mems_Decode_Start (void *buf, MG_u_long_t pos, mems_bitio_state * bs);



void BIO_Mems_Unary_Encode (MG_u_long_t val, mems_bitio_state * bs,
			    MG_u_long_t *bits);
MG_u_long_t BIO_Mems_Unary_Decode (mems_bitio_state * bs,
				     MG_u_long_t *bits);



void BIO_Mems_Binary_Encode (MG_u_long_t val, MG_u_long_t b,
			     mems_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Mems_Binary_Decode (MG_u_long_t b, mems_bitio_state * bs,
				      MG_u_long_t *bits);



void BIO_Mems_Gamma_Encode (MG_u_long_t val, mems_bitio_state * bs,
			    MG_u_long_t *bits);
MG_u_long_t BIO_Mems_Gamma_Decode (mems_bitio_state * bs, MG_u_long_t *bits);



void BIO_Mems_Delta_Encode (MG_u_long_t val, mems_bitio_state * bs,
			    MG_u_long_t *bits);
MG_u_long_t BIO_Mems_Delta_Decode (mems_bitio_state * bs, MG_u_long_t *bits);


void BIO_Mems_Elias_Encode (MG_u_long_t val, MG_u_long_t b, double s,
			    mems_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Mems_Elias_Decode (MG_u_long_t b, double s,
				mems_bitio_state * bs, MG_u_long_t *bits);


void BIO_Mems_Bblock_Encode (MG_u_long_t val, MG_u_long_t b,
			     mems_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Mems_Bblock_Decode (MG_u_long_t b, mems_bitio_state * bs,
				      MG_u_long_t *bits);


void BIO_Mems_Decode_Seek (MG_u_long_t pos, mems_bitio_state * bs);


#endif
