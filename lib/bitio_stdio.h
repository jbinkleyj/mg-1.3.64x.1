/**************************************************************************
 *
 * bitio_stdio.h -- Functions for bitio to a file
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
 * $Id: bitio_stdio.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************
 *
 *  This file contains function definitions for doing bitwise input and output
 *  of number on a FILE*. With these function you cannot mix reads and writes
 *  on the FILE, or multiple writes, at the same time and guarantee them to 
 *  work, also you  cannot seek to a point and do a write. The decode function
 *  can detect when you run off the end of the file and will produce an 
 *  approate error message.
 *
 *
 **************************************************************************/

#ifndef H_BITIO_STDIO
#define H_BITIO_STDIO


typedef struct stdio_bitio_state
  {
    FILE *File;
    unsigned char Buff;
    unsigned char Btg;
  }
stdio_bitio_state;


/* NOTE : All bytes are filled high bit first */


void BIO_Stdio_Encode_Start (FILE * f, stdio_bitio_state * bs);
void BIO_Stdio_Encode_Done (stdio_bitio_state * bs);



void BIO_Stdio_Decode_Start (FILE * f, stdio_bitio_state * bs);



void BIO_Stdio_Encode_Bit (int bit, stdio_bitio_state * bs);
int BIO_Stdio_Decode_Bit (stdio_bitio_state * bs);


void BIO_Stdio_Unary_Encode (MG_u_long_t val, stdio_bitio_state * bs,
			     MG_u_long_t *bits);
MG_u_long_t BIO_Stdio_Unary_Decode (stdio_bitio_state * bs,
				      MG_u_long_t *bits);



void BIO_Stdio_Binary_Encode (MG_u_long_t val, MG_u_long_t b,
			      stdio_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Stdio_Binary_Decode (MG_u_long_t b, stdio_bitio_state * bs,
				       MG_u_long_t *bits);



void BIO_Stdio_Gamma_Encode (MG_u_long_t val, stdio_bitio_state * bs,
			     MG_u_long_t *bits);
MG_u_long_t BIO_Stdio_Gamma_Decode (stdio_bitio_state * bs,
				      MG_u_long_t *bits);



void BIO_Stdio_Delta_Encode (MG_u_long_t val, stdio_bitio_state * bs,
			     MG_u_long_t *bits);
MG_u_long_t BIO_Stdio_Delta_Decode (stdio_bitio_state * bs,
				      MG_u_long_t *bits);



void BIO_Stdio_Elias_Encode (MG_u_long_t val, MG_u_long_t b, double s,
			     stdio_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Stdio_Elias_Decode (MG_u_long_t b, double s,
				      stdio_bitio_state * bs,
				      MG_u_long_t *bits);


void BIO_Stdio_Bblock_Encode (MG_u_long_t val, MG_u_long_t b,
			      stdio_bitio_state * bs, MG_u_long_t *bits);
MG_u_long_t BIO_Stdio_Bblock_Decode (MG_u_long_t b, stdio_bitio_state * bs,
				       MG_u_long_t *bits);


void BIO_Stdio_Decode_Seek (MG_u_long_t pos, stdio_bitio_state * bs);

#endif
