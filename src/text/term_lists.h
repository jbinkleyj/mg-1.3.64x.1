/**************************************************************************
 *
 * term_lists.h -- description
 * Copyright (C) 1994  Authors
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
 * $Id: term_lists.h,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/

/*
   $Log: term_lists.h,v $
   Revision 1.2  2004/10/31 00:13:35  beebe
   Major update for version 1.3.64x to support 64-bit architectures.

   Revision 1.1  2004/10/30 21:11:34  beebe
   Initial revision

   * Revision 1.1  1994/10/20  03:57:08  tes
   * I have rewritten the boolean query optimiser and abstracted out the
   * components of the boolean query.
   *
 */

#ifndef TERM_LISTS_H
#define TERM_LISTS_H

#include "sysfuncs.h"

#define MAXTERMSTRLEN 1023	/* maximum number of characters in term string */

typedef struct WordEntry
  {
    int word_num;		/* Unique number for each different word */
    MG_u_long_t count;		/* Number of time the word occurs in the text */
    MG_u_long_t doc_count;		/* Number of documents that contain the word */
    MG_u_long_t max_doc_count;       /* [RPAP - Jan 97: Stem Index Change] */
    MG_u_long_t invf_ptr;		/* This is a byte position of the  
				   inverted file entry corresponding to the word */
    MG_u_long_t invf_len;		/* This is the length of the inverted 
				   file entry in bytes */
  }
WordEntry;

typedef struct TermEntry
  {
    WordEntry WE;
    int Count;			/* The number of times the word occurs in the query */
    u_char *Word;		/* The word. */
    u_char *Stem;               /* [RPAP - Jan 97: Stem Index Change] - The stemmed non-cf version of Word */
  }
TermEntry;

typedef struct TermList
  {
    int list_size;
    int num;
    TermEntry TE[1];
  }
TermList;

#define GetNthWE(term_list, n) (&((term_list)->TE[(n)].WE))

/* --- prototypes --- */
void ConvertTermsToString (TermList * query_term_list, char *str);
int AddTermEntry (TermList ** query_term_list, TermEntry * te);
int AddTerm (TermList ** query_term_list, u_char * Word, int Count, int word_num,
	     MG_u_long_t count, MG_u_long_t doc_count, MG_u_long_t invf_ptr, MG_u_long_t invf_len);  /* [RPAP - Feb 97: Term Frequency] */
void ResetTermList (TermList ** tl);
void FreeTermList (TermList ** the_tl);
void PrintWordEntry (WordEntry * we, FILE * file);
void PrintTermEntry (TermEntry * te, FILE * file);
void PrintTermList (TermList * tl, FILE * file);
TermList *MakeTermList (int n);


#endif
