/**************************************************************************
 *
 * hash.h -- Hash function
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
 * $Id: hash.h,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/


#ifndef H_HASH
#define H_HASH



#define HASH(h, step, wd, tablesize)			\
	do {						\
		register u_char *p=wd;			\
		register MG_long_t len = *p++;		\
		register MG_u_long_t hval;		\
		if (len>25) len = 25;			\
		for (hval=0; len; len--)		\
		  hval = (hval << 5) - hval + *p++;	\
		step = hval % (tablesize - 2) + 1;	\
		h = hval % tablesize;			\
	} while (0)



#endif
