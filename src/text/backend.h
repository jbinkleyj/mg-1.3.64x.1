/**************************************************************************
 *
 * backend.h -- Underlying routines and datastructures for mgquery
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
 * $Id: backend.h,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/


#ifndef BACKEND_H
#define BACKEND_H

#include "sysfuncs.h"

#include "timing.h"
#include "lists.h"
#include "term_lists.h"
#include "query_term_list.h"  /* [RPAP - Feb 97: Term Frequency] */
#include "mg.h"
#include "invf.h"
#include "text.h"


typedef struct invf_data
  {
    File *InvfFile;
    MG_u_long_t N;
    MG_u_long_t Nstatic;	/* N parameter for decoding inverted file entries */
    struct invf_file_header ifh;
  }
invf_data;

typedef struct text_data
  {
    File *TextFile;
    File *TextIdxFile;
    File *TextIdxWgtFile;
    MG_long_t current_pos;
    struct
      {
	MG_u_long_t Start;
	float Weight;
      }
     *idx_data;
    compressed_text_header cth;
  }
text_data;


typedef struct auxiliary_dict
  {
    aux_frags_header afh[2];
    u_char *word_data[2];
    u_char **words[2];
    int blk_start[2][33], blk_end[2][33];	/* blk_start and blk_end are required
						   for the hybrid methods */
  }
auxiliary_dict;


typedef struct compression_dict
  {
    compression_dict_header cdh;
    comp_frags_header *cfh[2];
    MG_u_long_t MemForCompDict;
    u_char ***values[2];
    u_char *escape[2];
    huff_data *chars_huff[2];
    MG_u_long_t **chars_vals[2];
    huff_data *lens_huff[2];
    MG_u_long_t **lens_vals[2];
    auxiliary_dict *ad;
    int fast_loaded;
  }
compression_dict;


typedef struct stemmed_idx   /* [RPAP - Jan 97: Stem Index Change] */
  {
    File *stem_idx_file;
    struct stem_idx_header sih;
    u_char **index;
    MG_u_long_t *pos;
    int active;
    u_char *buffer;
    MG_u_long_t MemForStemIdx;
  }
stemmed_idx;


typedef struct stemmed_dict
  {
    File *stem_file;
    struct stem_dict_header sdh;
    u_char **index;
    MG_u_long_t *pos;
    int active;
    u_char *buffer;
    MG_u_long_t MemForStemDict;

    /* [RPAP - Jan 97: Stem Index Change] */
    stemmed_idx *stem1;
    stemmed_idx *stem2;
    stemmed_idx *stem3;
  }
stemmed_dict;


typedef struct approx_weights_data
  {
    double L;
    double B;
    MG_u_long_t *DocWeights;
    char bits;
    float *table;
    MG_u_long_t mask;
    MG_u_long_t MemForWeights;
    MG_u_long_t num_of_docs;
  }
approx_weights_data;


typedef struct RankedQueryInfo
  {
    int QueryFreqs;
    int Exact;           /* use exact weights for ranking or not */
    MG_long_t MaxDocsToRetrieve;	/* may be -1 for all */
    MG_long_t MaxParasToRetrieve;
    int Sort;        
    char AccumMethod;		/* 'A' = array,  'S' = splay tree,  'H' = hash_table */
    MG_long_t MaxAccums;		/* may be -1 for all */
    MG_long_t MaxTerms;		/* may be -1 for all */
    int StopAtMaxAccum; /* Stop at maximum accumulator or not */
    MG_long_t HashTblSize;
    char *skip_dump;
  }
RankedQueryInfo;



typedef struct BooleanQueryInfo
  {
    MG_long_t MaxDocsToRetrieve;
  }
BooleanQueryInfo;


/* [TS:24/Aug/94] - maximum number of characters in term string */
#define MAXTERMSTRLEN 1023

typedef struct query_data
  {
    stemmed_dict *sd;
    compression_dict *cd;
    approx_weights_data *awd;
    invf_data *id;
    text_data *td;
    char *pathname;
    File *File_text;
    File *File_comp_dict;
    File *File_aux_dict;
    File *File_fast_comp_dict;
    File *File_text_idx_wgt;
    File *File_text_idx;
    File *File_stem;

    /* [RPAP - Jan 97: Stem Index Change] */
    File *File_stem1;
    File *File_stem2;
    File *File_stem3;

    File *File_invf;
    File *File_weight_approx;
    MG_u_long_t mem_in_use, max_mem_in_use;
    MG_u_long_t num_of_ptrs, tot_num_of_ptrs;
    MG_u_long_t num_of_terms, tot_num_of_terms;
    MG_u_long_t num_of_accum, tot_num_of_accum;
    MG_u_long_t num_of_ans, tot_num_of_ans;
    MG_u_long_t hops_taken, tot_hops_taken;
    MG_u_long_t text_idx_lookups, tot_text_idx_lookups;
    MG_u_long_t max_buffers;
    unsigned doc_pos;
    unsigned buf_in_use;
    DocList *DL;
    TermList *TL;		/* [TS:Oct/94] - so term list for query can easily be accessed */
    u_char *TextBuffer;
    int TextBufferLen;
    QueryTermList *QTL;    /* [RPAP - Feb 97: Term Frequency] */
  }
query_data;



typedef struct InitQueryTimes
  {
    ProgTime Start;
    ProgTime StemDict;
    ProgTime ApproxWeights;
    ProgTime CompDict;
    ProgTime Invf;
    ProgTime Text;
  }
InitQueryTimes;



query_data *InitQuerySystem (char *dir, char *name, InitQueryTimes * iqt);

void ChangeMemInUse (query_data * qd, MG_long_t delta);

void FinishQuerySystem (query_data * qd);

void ResetFileStats (query_data * qd);

void TransFileStats (query_data * qd);

void ChangeMemInUse (query_data * qd, MG_long_t delta);

void RankedQuery (query_data * qd, char *Query, RankedQueryInfo * rqi);

void BooleanQuery (query_data * qd, char *Query, BooleanQueryInfo * bqi,
		   int stem_method);

void DocnumsQuery (query_data * qd, char *QueryLine);

void FreeTextBuffer (query_data * qd);

void FreeQueryDocs (query_data * qd);

int LoadCompressedText (query_data * qd, int max_mem);

int GetDocNum (query_data * qd);

float GetDocWeight (query_data * qd);

MG_long_t GetDocCompLength (query_data * qd);

u_char *GetDocText (query_data * qd, MG_u_long_t *len);

DocEntry *GetDocChain (query_data * qd);

int NextDoc (query_data * qd);

#endif
