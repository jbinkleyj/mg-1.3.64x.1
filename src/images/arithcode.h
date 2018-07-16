/**************************************************************************
 *
 * arithcode.h -- Arithmetic coding
 * Copyright (C) 1994  Alistair Moffat
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
 * $Id: arithcode.h,v 1.2 2004/10/31 00:12:38 beebe Exp beebe $
 *
 **************************************************************************/

#ifndef _arithcode_h
#define _arithcode_h

extern FILE *arith_in, *arith_out;

#define codevaluebits 16
#if (codevaluebits<16)
typedef unsigned short codevalue;
#else
typedef MG_u_long_t codevalue;
#endif

#define topvalue ((codevalue)((1<<codevaluebits)-1))
#define maxfrequency ((topvalue+1)/4 + 1)

#define firstqtr (topvalue/4+1)
#define half     (2*firstqtr)
#define thirdqtr (3*firstqtr)

#define	escape_event	U->totalcnt-U->notfound, U->totalcnt, U->totalcnt

void arithmetic_encode ();
codevalue arithmetic_decode_target ();

#define	arithmetic_decode_target(totl)	\
	(  ((S_value-S_low+1)*(totl)-1) / (S_high-S_low+1)  )

void arithmetic_decode ();


extern codevalue S_low, S_high, S_value;
extern MG_long_t S_bitstofollow;
extern int S_buffer, S_bitstogo;

extern MG_long_t cmpbytes, rawbytes;

extern MG_long_t CountOfBitsOut;

void EncodeGammaDist (int Off);
int DecodeGammaDist ();

void EncodeGammaSigned (int snum, int *pos, int *neg);
int DecodeGammaSigned (int *pos, int *neg);


void InitArithEncoding (void);
void InitArithDecoding (void);

void CloseDownArithEncoding (void);
void CloseDownArithDecoding (void);

void EncodeChecksum ();
void DecodeChecksum (char str[]);

#endif
