/**************************************************************************
 *
 * bitio_random.c -- Functions for bitio to a file (random access)
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
 * $Id: bitio_random.c,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************/

/*
   $Log: bitio_random.c,v $
   Revision 1.2  2004/10/31 00:11:04  beebe
   Major update for version 1.3.64x to support 64-bit architectures.

   Revision 1.1  2004/10/30 21:11:14  beebe
   Initial revision

   * Revision 1.1  1994/08/22  00:24:40  tes
   * Initial placement under CVS.
   *
 */

static char *RCSID = "$Id: bitio_random.c,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $";



#include "sysfuncs.h"
#include "memlib.h"

#include "bitio_m_random.h"
#include "bitio_m.h"


void 
BIO_Random_Start (FILE * f, MG_u_long_t len,
		  random_bitio_state * bs)
{
  ENCODE_START (f, len)
    ENCODE_PAUSE (*bs)
}

void 
BIO_Random_Done (random_bitio_state * bs)
{
  ENCODE_CONTINUE (*bs)
    ENCODE_DONE
}

void 
BIO_Random_Encode_Bit (int bit, random_bitio_state * bs)
{
  ENCODE_CONTINUE (*bs)
    ENCODE_BIT (bit);
  ENCODE_PAUSE (*bs)
}

int 
BIO_Random_Decode_Bit (random_bitio_state * bs)
{
  register int val;
  DECODE_CONTINUE (*bs)
    val = DECODE_BIT;
  DECODE_PAUSE (*bs)
    return (val);
}


void 
BIO_Random_Unary_Encode (MG_u_long_t val, random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    UNARY_ENCODE_L (val, *bits);
  else
    UNARY_ENCODE (val);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Random_Unary_Decode (random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  register MG_u_long_t val;
  DECODE_CONTINUE (*bs)
    if (bits)
    UNARY_DECODE_L (val, *bits);
  else
    UNARY_DECODE (val);
  DECODE_PAUSE (*bs)
    return (val);
}







void 
BIO_Random_Binary_Encode (MG_u_long_t val, MG_u_long_t b,
			  random_bitio_state * bs, MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    BINARY_ENCODE_L (val, b, *bits);
  else
    BINARY_ENCODE (val, b);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Random_Binary_Decode (MG_u_long_t b,
			  random_bitio_state * bs,
			  MG_u_long_t *bits)
{
  register MG_u_long_t val;
  DECODE_CONTINUE (*bs)
    if (bits)
    BINARY_DECODE_L (val, b, *bits);
  else
    BINARY_DECODE (val, b);
  DECODE_PAUSE (*bs)
    return (val);
}







void 
BIO_Random_Gamma_Encode (MG_u_long_t val, random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    GAMMA_ENCODE_L (val, *bits);
  else
    GAMMA_ENCODE (val);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Random_Gamma_Decode (random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  register MG_u_long_t val;
  DECODE_CONTINUE (*bs)
    if (bits)
    GAMMA_DECODE_L (val, *bits);
  else
    GAMMA_DECODE (val);
  DECODE_PAUSE (*bs)
    return (val);
}




void 
BIO_Random_Delta_Encode (MG_u_long_t val, random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    DELTA_ENCODE_L (val, *bits);
  else
    DELTA_ENCODE (val);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Random_Delta_Decode (random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  register MG_u_long_t val;
  DECODE_CONTINUE (*bs)
    if (bits)
    DELTA_DECODE_L (val, *bits);
  else
    DELTA_DECODE (val);
  DECODE_PAUSE (*bs)
    return (val);
}

void 
BIO_Random_Elias_Encode (MG_u_long_t val, MG_u_long_t b, double s,
			 random_bitio_state * bs, MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    ELIAS_ENCODE_L (val, b, s, *bits);
  else
    ELIAS_ENCODE (val, b, s);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Random_Elias_Decode (MG_u_long_t b, double s,
			 random_bitio_state * bs,
			 MG_u_long_t *bits)
{
  register MG_u_long_t val;
  DECODE_CONTINUE (*bs)
    if (bits)
    ELIAS_DECODE_L (val, b, s, *bits);
  else
    ELIAS_DECODE (val, b, s);
  DECODE_PAUSE (*bs)
    return (val);
}

void 
BIO_Random_Bblock_Encode (MG_u_long_t val, MG_u_long_t b,
			  random_bitio_state * bs, MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    BBLOCK_ENCODE_L (val, b, *bits);
  else
    BBLOCK_ENCODE (val, b);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Random_Bblock_Decode (MG_u_long_t b,
			  random_bitio_state * bs,
			  MG_u_long_t *bits)
{
  register MG_u_long_t val;
  DECODE_CONTINUE (*bs)
    if (bits)
    BBLOCK_DECODE_L (val, b, *bits);
  else
    BBLOCK_DECODE (val, b);
  DECODE_PAUSE (*bs)
    return (val);
}

void 
BIO_Random_Seek (MG_u_long_t pos, random_bitio_state * bs)
{
  ENCODE_CONTINUE (*bs)
    ENCODE_SEEK (pos);
  ENCODE_PAUSE (*bs)
}
void 
BIO_Random_Flush (random_bitio_state * bs)
{
  ENCODE_CONTINUE (*bs)
    ENCODE_FLUSH;
  ENCODE_PAUSE (*bs)
}

MG_u_long_t 
BIO_Random_Tell (random_bitio_state * bs)
{
  register MG_u_long_t t;
  ENCODE_CONTINUE (*bs)
    t = ENCODE_TELL;
  ENCODE_PAUSE (*bs)
    return (t);
}
