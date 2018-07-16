/**************************************************************************
 *
 * invf.h -- Data structures for inverted files
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
 * $Id: invf.h,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/



#ifndef H_INVF
#define H_INVF

/* NOTE: This does not include the magic number */
struct invf_dict_header
  {
    MG_u_long_t lookback;
    MG_u_long_t dict_size;
    MG_u_long_t total_bytes;
    MG_u_long_t index_string_bytes;
    MG_u_long_t input_bytes;
    MG_u_long_t num_of_docs;
    MG_u_long_t static_num_of_docs;
    MG_u_long_t num_of_words;
    MG_u_long_t stem_method;
  };

struct stem_dict_header
  {
    MG_u_long_t lookback;
    MG_u_long_t block_size;
    MG_u_long_t num_blocks;
    MG_u_long_t blocks_start;
    MG_u_long_t index_chars;
    MG_u_long_t num_of_docs;
    MG_u_long_t static_num_of_docs;
    MG_u_long_t num_of_words;
    MG_u_long_t stem_method;
    MG_u_long_t indexed;       /* [RPAP - Jan 97: Stem Index Change] */
  };

struct invf_file_header
  {
    MG_u_long_t no_of_words;
    MG_u_long_t no_of_ptrs;
    MG_u_long_t skip_mode;
    MG_u_long_t params[16];
    MG_u_long_t InvfLevel;
  };

/* [RPAP - Jan 97: Stem Index Change] */
struct stem_idx_header
  {
    MG_u_long_t lookback;
    MG_u_long_t block_size;
    MG_u_long_t num_blocks;
    MG_u_long_t blocks_start;
    MG_u_long_t index_chars;
    MG_u_long_t num_of_words;
  };

#endif
