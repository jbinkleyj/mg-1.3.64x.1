/**************************************************************************
 *
 * random.h -- pseudo random number generator
 * Copyright (C) 1994  Chris Wallace (csw@bruce.cs.monash.edu.au)
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
 * $Id: random.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************/

/*
$Log: random.h,v $
Revision 1.2  2004/10/31 00:11:04  beebe
Major update for version 1.3.64x to support 64-bit architectures.

Revision 1.1  2004/10/30 21:11:14  beebe
Initial revision

*/

#ifndef RANDOM_H
#define RANDOM_H

/*
 *	A random number generator called as a function by
 *	random (iseed)	or	irandm (iseed)
 *	The parameter should be a pointer to a 2-element MG_long_t vector.
 *	The first function returns a double uniform in 0 .. 1.
 *	The second returns a MG_long_t integer uniform in 0 .. 2**31-1
 *	Both update iseed[] in exactly the same way.
 *	iseed[] must be a 2-element integer vector.
 *	The initial value of the second element may be anything.
 *
 *	The period of the random sequence is 2**32 * (2**32-1)
 *	The table mt[0:127] is defined by mt[i] = 69069 ** (128-i)
 */

double random  (MG_long_t is [2]);
MG_long_t irandm  (MG_long_t is [2]);

#endif
