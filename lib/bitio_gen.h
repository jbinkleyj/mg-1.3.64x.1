/**************************************************************************
 *
 * bitio_gen.h -- General supoport routines for bitio
 * Copyright (C) 1994  Neil Sharman and Alistair Moffat
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
 * $Id: bitio_gen.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************
 *
 *  This file contains function definitions for doing general bitio operations.
 *
 *  This file defines functions for calculation the length in bits of a number
 *  coded unary, binary, delta, gamma, elias, and  bblock coding. It defines 
 *  routines for calculating the parameter and bounds for bblock coding. 
 *  Routines for calculating ceil and floor lg are also defined.
 *
 *
 **************************************************************************/

#ifndef H_BITIO_GEN
#define H_BITIO_GEN

MG_u_long_t BIO_Unary_Length (MG_u_long_t val);

MG_u_long_t BIO_Binary_Length (MG_u_long_t val, MG_u_long_t b);

MG_u_long_t BIO_Gamma_Length (MG_u_long_t val);

MG_u_long_t BIO_Delta_Length (MG_u_long_t val);

MG_u_long_t BIO_Elias_Length (MG_u_long_t val, MG_u_long_t b, double s);

MG_u_long_t BIO_Bblock_Length (MG_u_long_t val, MG_u_long_t b);

int BIO_Bblock_Init (int N, int p);

int BIO_Bblock_Init_W (int N, int p);

int BIO_Bblock_Bound_b (int N, int p, int b);

int BIO_Bblock_Bound (int N, int p);

int BIO_Gamma_Bound (int N, int p);

int floorlog_2 (int b);

int ceillog_2 (int b);

#endif
