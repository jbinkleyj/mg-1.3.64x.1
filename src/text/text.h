/**************************************************************************
 *
 * text.h -- Header file for compression related stuff
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
 * $Id: text.h,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/



#ifndef H_TEXT
#define H_TEXT

#include "huffman.h"





/*****************************************************************************
 *
 * There are several different methods of compressing the text of the
 * database. The following defines list the different methods of text
 * compression
 *
 */


/* The dictionary contains all the fragments that occur in the collection
   i.e. escapes are not possible */
#define MG_COMPLETE_DICTIONARY 0


/* Certain words have been deleted from the dictionary. The words deleted
   have been used to create the frequency huffman codes of the characters. 
   This dictionary has an escape code and may be used to compress novel
   words. This dictionary may fail if there is a novel character. */
#define MG_PARTIAL_DICTIONARY 1


/* This dictionary has an escape so that novel words and non-words can be 
   coded. The method for coding the novel words and non-words is determined
   by a dictionary parameter. */
#define MG_SEED_DICTIONARY 2









/*****************************************************************************
 *
 * With a seed dictionary there are several methods for coding the novel
 * words and non-words the following defined values specify the different
 * methods of coding.
 *
 */


/* Code novel words and non-words character by character using huffman codes.
   The huffman codes for the word and non-word lengths and characters are 
   generated from the distribution of lengths and characters in the
   dictionary. */
#define MG_NOVEL_HUFFMAN_CHARS 0


/* This method codes novel words using binary codes. The novel words are stored
   in a auxillary dictionary which is built by pass two. */
#define MG_NOVEL_BINARY 1


/* This method codes novel words using delta codes. The novel words are stored
   in a auxillary dictionary which is built by pass two. */
#define MG_NOVEL_DELTA 2


/* This method codes novel words using hybrid version of delta. The novel
   words are stored in a auxillary dictionary which is built by pass two. */
#define MG_NOVEL_HYBRID 3


/* This method codes novel words using hybrid version of delta and a MTF
   operation. The novel words are stored in a auxillary dictionary which
   is built by pass two. */
#define MG_NOVEL_HYBRID_MTF 4






/* This specified an amount of extra space allocated in the
   compression_dict_header for adding new parameters. As new
   parameters are added this should be decreased. */
#define TEXT_PARAMS 15



typedef struct compression_dict_header
  {
    MG_u_long_t dict_type;
    MG_u_long_t novel_method;
    MG_u_long_t params[TEXT_PARAMS];
    MG_u_long_t num_words[2];
    MG_u_long_t num_word_chars[2];
    MG_u_long_t lookback;
  }
compression_dict_header;


typedef struct comp_frags_header
  {
    huff_data hd;
    MG_u_long_t uncompressed_size;
    MG_u_long_t huff_words_size[MAX_HUFFCODE_LEN + 1];
  }
comp_frags_header;



typedef struct compressed_text_header
  {
    MG_u_long_t num_of_docs;
    MG_u_long_t num_of_bytes;
    MG_u_long_t num_of_words;	/* number of words in collection */
    MG_u_long_t length_of_longest_doc;	/* (characters) */
    double ratio;
  }
compressed_text_header;


typedef struct compression_stats_header
  {
    MG_u_long_t num_docs;
    MG_u_long_t num_bytes;
  }
compression_stats_header;

typedef struct frags_stats_header
  {
    MG_u_long_t num_frags;
    MG_u_long_t mem_for_frags;
  }
frags_stats_header;


typedef struct aux_frags_header
  {
    MG_u_long_t num_frags;
    MG_u_long_t mem_for_frags;
  }
aux_frags_header;


#endif
