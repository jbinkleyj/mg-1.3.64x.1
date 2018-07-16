/**************************************************************************
 *
 * stem_search.h -- Functions for searching the blocked stemmed dictionary
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
 * $Id: stem_search.h,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/

#ifndef H_STEM_SEARCH
#define H_STEM_SEARCH

stemmed_dict *ReadStemDictBlk (File * stem_file);

stemmed_idx *ReadStemIdxBlk (File * stem_idx_file);

int FindWord (stemmed_dict * sd, u_char * Word, MG_u_long_t *count,
	      MG_u_long_t *doc_count, MG_u_long_t *invf_ptr,
	      MG_u_long_t *invf_len);

/* [RPAP - Jan 97: Stem Index Change] */
int FindWords (stemmed_dict * sd, u_char * sWord, int stem_method, TermList ** tl);

void FreeStemDict (stemmed_dict * sd);

/* [RPAP - Jan 97: Stem Index Change] */
void FreeStemIdx (stemmed_idx * si);

#endif
