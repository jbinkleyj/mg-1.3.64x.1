/**************************************************************************
 *
 * bitio_mems.c -- Functions for bitio to memory (random access)
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
 * $Id: bitio_mems.c,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************/

/*
   $Log: bitio_mems.c,v $
   Revision 1.2  2004/10/31 00:11:04  beebe
   Major update for version 1.3.64x to support 64-bit architectures.

   Revision 1.1  2004/10/30 21:11:14  beebe
   Initial revision

   * Revision 1.1  1994/08/22  00:24:39  tes
   * Initial placement under CVS.
   *
 */

static char *RCSID = "$Id: bitio_mems.c,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $";


#include "sysfuncs.h"

#include "bitio_m_mems.h"
#include "bitio_m.h"

int fprintf (FILE *, const char *,...);



void 
BIO_Mems_Encode_Start (unsigned char *buf, MG_u_long_t pos,
		       mems_bitio_state * bs)
{
  ENCODE_START (buf, pos)
    ENCODE_PAUSE (*bs)
}

void 
BIO_Mems_Encode_Done (mems_bitio_state * bs)
{
  ENCODE_CONTINUE (*bs)
    ENCODE_DONE
}


void 
BIO_Mems_Decode_Start (unsigned char *buf, MG_u_long_t pos,
		       mems_bitio_state * bs)
{
  DECODE_START (buf, pos)
    DECODE_PAUSE (*bs)
}




void 
BIO_Mems_Unary_Encode (MG_u_long_t val, mems_bitio_state * bs,
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
BIO_Mems_Unary_Decode (mems_bitio_state * bs,
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
BIO_Mems_Binary_Encode (MG_u_long_t val, MG_u_long_t b,
			mems_bitio_state * bs, MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    BINARY_ENCODE_L (val, b, *bits);
  else
    BINARY_ENCODE (val, b);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Mems_Binary_Decode (MG_u_long_t b, mems_bitio_state * bs,
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
BIO_Mems_Gamma_Encode (MG_u_long_t val, mems_bitio_state * bs,
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
BIO_Mems_Gamma_Decode (mems_bitio_state * bs, MG_u_long_t *bits)
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
BIO_Mems_Delta_Encode (MG_u_long_t val, mems_bitio_state * bs,
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
BIO_Mems_Delta_Decode (mems_bitio_state * bs, MG_u_long_t *bits)
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
BIO_Mems_Elias_Encode (MG_u_long_t val, MG_u_long_t b, double s,
		       mems_bitio_state * bs, MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    ELIAS_ENCODE_L (val, b, s, *bits);
  else
    ELIAS_ENCODE (val, b, s);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Mems_Elias_Decode (MG_u_long_t b, double s,
		       mems_bitio_state * bs, MG_u_long_t *bits)
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
BIO_Mems_Bblock_Encode (MG_u_long_t val, MG_u_long_t b,
			mems_bitio_state * bs, MG_u_long_t *bits)
{
  ENCODE_CONTINUE (*bs)
    if (bits)
    BBLOCK_ENCODE_L (val, b, *bits);
  else
    BBLOCK_ENCODE (val, b);
  ENCODE_PAUSE (*bs)
}


MG_u_long_t 
BIO_Mems_Bblock_Decode (MG_u_long_t b, mems_bitio_state * bs,
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
BIO_Mems_Decode_Seek (MG_u_long_t pos, mems_bitio_state * bs)
{
  DECODE_CONTINUE (*bs)
    DECODE_SEEK (pos);
  DECODE_PAUSE (*bs)
}

void 
BIO_Mems_Encode_Seek (MG_u_long_t pos, mems_bitio_state * bs)
{
  ENCODE_CONTINUE (*bs)
    ENCODE_SEEK (pos);
  ENCODE_PAUSE (*bs)
}
