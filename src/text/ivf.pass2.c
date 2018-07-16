/**************************************************************************
 *
 * ivf.pass2.c -- Memory efficient pass 2 inversion
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
 * $Id: ivf.pass2.c,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/

/*
   $Log: ivf.pass2.c,v $
   Revision 1.2  2004/10/31 00:13:35  beebe
   Major update for version 1.3.64x to support 64-bit architectures.

   Revision 1.1  2004/10/30 21:11:34  beebe
   Initial revision

   * Revision 1.3  1994/10/20  03:56:49  tes
   * I have rewritten the boolean query optimiser and abstracted out the
   * components of the boolean query.
   *
   * Revision 1.2  1994/09/20  04:41:35  tes
   * For version 1.1
   *
 */

static char *RCSID = "$Id: ivf.pass2.c,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $";

#include "local_strings.h"
#include "sysfuncs.h"
#include "memlib.h"
#include "messages.h"
#include "stemmer.h"
#include "perf_hash.h"
#include "bitio_m.h"
#include "bitio_m_mems.h"
#include "bitio_gen.h"
#include "bitio_random.h"
#include "bitio_stdio.h"
#include "netorder.h"  /* [RPAP - Jan 97: Endian Ordering] */

#include "mg_files.h"
#include "invf.h"
#include "locallib.h"
#include "mg.h"
#include "build.h"
#include "words.h"
#include "hash.h"
#include "stemmer.h"

/* [RPAP - Feb 97: WIN32 Port] */
#ifdef __WIN32__
#include <io.h>
#endif

#ifndef RND_BUF_SIZE
#define RND_BUF_SIZE 8*1024
/*#define RND_BUF_SIZE 128 */
#endif

#define print_fsize(file)\
do\
  {\
    struct stat file_state;\
    fstat(fileno(invf_out), &file_state);\
    Message("len(invf) = %ld", file_state.st_size);\
  }while(0)

typedef struct word_rec
  {
    MG_u_long_t ptr;
    MG_u_long_t last;
  }
word_rec;

typedef struct invf_state_rec
  {
    MG_u_long_t Disk_Ptr;
    MG_u_long_t Disk_Last;
    MG_u_long_t Disk_B;
  }
invf_state_rec;

typedef struct chunk
  {
    MG_u_long_t start_doc;
    MG_u_long_t params_pos;
    MG_u_long_t disk_pos;
    MG_u_long_t N;
  }
chunk;


static FILE *dict;		/* Stemmed dictionary file */
static FILE *hash;		/* Stemmed dictionary hash file */
static FILE *invf;		/* Inverted file */
static FILE *invf_in;		/* Inverted file */
static FILE *invf_out;		/* Inverted file */
static FILE *invf_idx;		/* Inverted index file */
static FILE *count;		/* Count file */
static FILE *count_trans;	/* Count translation file */
static FILE *invf_state;	/* Inverted file State */
static FILE *chunk_state;	/* Chunk state */
static FILE *chunks;		/* Chunk state */
static FILE *invf_para = NULL;	/* Paragraph counts file */
static FILE *weights = NULL;	/* Weights file */

static stdio_bitio_state sbs;
static random_bitio_state crbs;
static chunk *chunk_data = NULL;
static random_bitio_state rbs, rbsp;

static int docs_left = 0, next_docs_left = 0;
static MG_u_long_t N;

static word_rec *WordRecs;
static u_char *lg_bs;
static float *idf = NULL;

static char *MemoryBuffer = NULL;
static MG_u_long_t MemBufSize;
static MG_u_long_t BufToUse;
static struct invf_dict_header idh;

static perf_hash_data *phd;

static MG_u_long_t *word_list = NULL;
static MG_u_long_t wl_size = 0;

static MG_u_long_t dict_size;
static MG_u_long_t no_of_ptrs = 0;
static MG_u_long_t chunks_read = 0;
static MG_u_long_t Disk_pos = 0;
static MG_u_long_t callnum = 0;
static MG_u_long_t wordnum = 0;

static MG_u_long_t totalIbytes = 0;
static MG_u_long_t totalDbytes = 0;
static MG_u_long_t totalHbytes = 0;

static MG_u_long_t MemInUse = 0;
static MG_u_long_t MaxMemInUse = 0;
static MG_u_long_t max_buffer_len;

void 
ChangeMemInUse (int mem)
{
  MemInUse += mem;
  if (MemInUse > MaxMemInUse)
    MaxMemInUse = MemInUse;
}




static int 
open_files (char *file_name)
{
  char FName[200];

  if (!(dict = open_file (file_name, INVF_DICT_SUFFIX, "rb",
			  MAGIC_STEM_BUILD, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);

  if (!(hash = open_file (file_name, INVF_DICT_HASH_SUFFIX, "rb",
			  MAGIC_HASH, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);

  if (!(count = open_file (file_name, INVF_CHUNK_SUFFIX, "rb",
			   MAGIC_CHUNK, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);
  fread (&max_buffer_len, sizeof (max_buffer_len), 1, count);
  NTOHUL(max_buffer_len);  /* [RPAP - Jan 97: Endian Ordering] */

  BIO_Stdio_Decode_Start (count, &sbs);
  next_docs_left = BIO_Stdio_Gamma_Decode (&sbs, NULL) - 1;

  if (!(count_trans = open_file (file_name, INVF_CHUNK_TRANS_SUFFIX, "rb",
				 MAGIC_CHUNK_TRANS, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);

  if (!(invf = create_file (file_name, INVF_SUFFIX, "w+b",
			    MAGIC_INVF, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);
  fflush (invf);
  if (!(invf_in = open_file (file_name, INVF_SUFFIX, "rb",
			     MAGIC_INVF, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);
  if (!(invf_out = create_file (file_name, INVF_SUFFIX, "wb",
				MAGIC_INVF, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);
  BIO_Random_Start (invf, RND_BUF_SIZE, &rbs);
  BIO_Random_Start (invf, RND_BUF_SIZE, &rbsp);
  ChangeMemInUse (RND_BUF_SIZE * 2);

  if (!(invf_idx = create_file (file_name, INVF_IDX_SUFFIX, "wb",
				MAGIC_INVI, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);

  if (InvfLevel == 3)
    if (!(invf_para = create_file (file_name, INVF_PARAGRAPH_SUFFIX, "wb",
				   MAGIC_PARAGRAPH, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
      return (COMPERROR);

  sprintf (FName, FILE_NAME_FORMAT ".%ld", get_basepath (), file_name,
	   ".invf.state",  (long)  getpid ());  /* [RPAP - Feb 97: WIN32 Port] */
  if (!(invf_state = fopen (FName, "w+b")))  /* [RPAP - Feb 97: WIN32 Port] */
    {
      Message ("Unable to create \"%s\"", FName);
      return (COMPERROR);
    }
  unlink (FName);

  sprintf (FName, FILE_NAME_FORMAT ".%ld", get_basepath (), file_name,
	   ".chunk.state",  (long)  getpid ());  /* [RPAP - Feb 97: WIN32 Port] */
  if (!(chunk_state = fopen (FName, "w+b")))  /* [RPAP - Feb 97: WIN32 Port] */
    {
      Message ("Unable to create \"%s\"", FName);
      return (COMPERROR);
    }
  unlink (FName);
  BIO_Random_Start (chunk_state, RND_BUF_SIZE, &crbs);
  ChangeMemInUse (RND_BUF_SIZE);

  sprintf (FName, FILE_NAME_FORMAT ".%ld", get_basepath (), file_name,
	   ".chunks",  (long)  getpid ());  /* [RPAP - Feb 97: WIN32 Port] */
  if (!(chunks = fopen (FName, "w+b")))  /* [RPAP - Feb 97: WIN32 Port] */
    {
      Message ("Unable to create \"%s\"", FName);
      return (COMPERROR);
    }
  unlink (FName);

  return (COMPALLOK);
}





#define ISR_CACHE 1024
#define ISR_ENTRY_SIZE (sizeof (MG_u_long_t) *2 + sizeof (MG_u_long_t) )

invf_state_rec *
in_cache (int pos)
{
  static char isr_data[ISR_CACHE * ISR_ENTRY_SIZE];
  static invf_state_rec isr;
  static int isr_base = 0, isr_num = -1, isr_pos = -1;
  if (isr_pos >= 0)
    bcopy ((char *) &isr, &isr_data[isr_pos * ISR_ENTRY_SIZE], ISR_ENTRY_SIZE);
  if (pos < isr_base || pos >= isr_base + isr_num)
    {
      if (isr_num >= 0)
	{
	  fseek (invf_state, isr_base * ISR_ENTRY_SIZE, 0);
	  fwrite (isr_data, 1, ISR_ENTRY_SIZE * isr_num, invf_state);
	}
      isr_base = pos;
      fseek (invf_state, isr_base * ISR_ENTRY_SIZE, 0);
      fread (isr_data, 1, ISR_ENTRY_SIZE * ISR_CACHE, invf_state);
      isr_num = ISR_CACHE;
    }
  isr_pos = pos - isr_base;
  bcopy (&isr_data[isr_pos * ISR_ENTRY_SIZE], (char *) &isr, ISR_ENTRY_SIZE);
  return &isr;
}





MG_u_long_t 
occur_to_lexical  (MG_long_t occ)
{
  static MG_long_t pos = -1;
  static random_bitio_state rbs;
  static int val = 0;
  if (pos == -1)
    {
      BIO_Random_Start (count_trans, RND_BUF_SIZE, &rbs);
      pos = 0x7fffffff;
    }
  if (occ < pos)
    {
      if (occ == -1)
	{
	  BIO_Random_Done (&rbs);
	  return 0;
	}
      BIO_Random_Seek (32, &rbs);
      pos = 0;
    }
  while (pos <= occ)
    {
      val = BIO_Random_Binary_Decode (dict_size + 1, &rbs, NULL) - 1;
      pos++;
    }
  return (val);
}


void 
add_chunk_state (MG_u_long_t pos, MG_u_long_t start_doc,
		 MG_u_long_t N)
{
  chunk_data[chunks_read].params_pos = pos;
  chunk_data[chunks_read].start_doc = start_doc;
  chunk_data[chunks_read].N = N;
  chunks_read++;
}


int 
init_ivf_2 (char *file_name)
{
  u_char prev[MAXSTEMLEN + 1];
  int i;
  MG_u_long_t totalIbits;
  double logN = 0.0;

  if (open_files (file_name) == COMPERROR)
    return COMPERROR;


  /* Read in the stemmed dictionary file header */
  fread ((char *) &idh, sizeof (idh), 1, dict);

  /* [RPAP - Jan 97: Endian Ordering] */
  NTOHUL(idh.lookback);
  NTOHUL(idh.dict_size);
  NTOHUL(idh.total_bytes);
  NTOHUL(idh.index_string_bytes);
  NTOHUL(idh.input_bytes);
  NTOHUL(idh.num_of_docs);
  NTOHUL(idh.static_num_of_docs);
  NTOHUL(idh.num_of_words);
  NTOHUL(idh.stem_method);

  dict_size = idh.dict_size;

  N = idh.num_of_docs;

  if (!(phd = read_perf_hash_data (hash)))
    {
      Message ("Unable to read in hash data");
      return COMPERROR;
    }
  totalHbytes = sizeof (perf_hash_data) + sizeof (u_char) * 256 +
    sizeof (int) * (phd->MAX_N + 1) + sizeof (int *) * 3 * phd->MAX_CH +
    sizeof  (MG_long_t)  * phd->MAX_CH * phd->MAX_L;

  if (!(WordRecs = Xmalloc (sizeof (word_rec) * idh.dict_size)))
    {
      Message ("No memory for word entries");
      return COMPERROR;
    }
  totalDbytes += sizeof (word_rec) * idh.dict_size;

  /* separate storage for the log(b) values, one byte each */
  if (!(lg_bs = Xmalloc (sizeof (u_char) * idh.dict_size)))
    {
      Message ("No memory for lg b's");
      return COMPERROR;
    }
  totalDbytes += sizeof (u_char) * idh.dict_size;

  if (MakeWeights)
    {
      /* separate storage for the idf values, one single each */
      if (!(idf = Xmalloc (sizeof (float) * idh.dict_size)))
	{
	  Message ("No memory for idf's");
	  return COMPERROR;
	}
      totalDbytes += sizeof (float) * idh.dict_size;

      if (!(weights = open_file (file_name, WEIGHTS_SUFFIX, "wb",
				 MAGIC_WGHT, MG_CONTINUE)))  /* [RPAP - Feb 97: WIN32 Port] */
	return (COMPERROR);
    }
  else
    {
      unlink (make_name (file_name, WEIGHTS_SUFFIX, NULL));
    }

  chunk_data = Xmalloc (sizeof (chunk) * (ChunkLimit + 2));
  totalDbytes += sizeof (chunk) * (ChunkLimit + 2);

  totalIbits = 32;		/* The magic number */
  totalIbits += 8 * 100;	/* A 100 byte gap */

  if (MakeWeights)
    {
      wl_size = 1024;
      if (!(word_list = Xmalloc (sizeof (*word_list) * wl_size)))
	{
	  Message ("No memory for word_list");
	  return COMPERROR;
	}

      logN = log ((double) N);
    }

  for (i = 0; i < idh.dict_size; i++)
    {
      invf_state_rec *isr;
      register MG_u_long_t copy, suff, p;
      MG_u_long_t fcnt, wcnt;

      copy = fgetc (dict);
      suff = fgetc (dict);
      *prev = copy + suff;
      fread (prev + copy + 1, sizeof (u_char), suff, dict);

      fread ((char *) &fcnt, sizeof (fcnt), 1, dict);
      fread ((char *) &wcnt, sizeof (wcnt), 1, dict);

      /* [RPAP - Jan 97: Endian Ordering] */
      NTOHUL(fcnt);
      NTOHUL(wcnt);

      WordRecs[i].last = 0;
      WordRecs[i].ptr = 0;

      p = fcnt;

      if (MakeWeights)
	idf[i] = logN - log ((double) fcnt);


      isr = in_cache (i);

      isr->Disk_Last = 0;
      isr->Disk_Ptr = totalIbits;

      isr->Disk_B = BIO_Bblock_Init (N, p);

      totalIbits += BIO_Bblock_Bound_b (N, p, isr->Disk_B);

      if (InvfLevel >= 2)
	totalIbits += BIO_Gamma_Bound (wcnt, fcnt);

      totalIbits = (totalIbits + 7ul) & 0xfffffff8ul;

    }


  /* now convert to bytes, and actually get the space */
  totalIbytes = (totalIbits + 7ul) >> 3ul;


  return (COMPALLOK);

}





static void 
LoadCounts (void)
{
  MG_u_long_t numwords, i, last_total;
  static MG_u_long_t local_N = 0;
  MG_u_long_t totalIbits, crbs_pos;
  word_rec *wr;
  MG_u_long_t *counts;

  if (MemoryBuffer == NULL)
    {
      MemBufSize = sizeof  (MG_u_long_t)  * dict_size;
      if (max_buffer_len > MemBufSize)
	MemBufSize = max_buffer_len;
      if (!(MemoryBuffer = Xmalloc (MemBufSize)))
	FatalError (1, "Unable to allocate memory for buffer");
      ChangeMemInUse (MemBufSize);
    }

  counts = (MG_u_long_t *) MemoryBuffer;
  bzero ((char *) counts, sizeof  (MG_u_long_t)  * dict_size);

  docs_left = next_docs_left;
  if (!docs_left)
    FatalError (1, "The number of docs in the current chunk is 0");

  BufToUse = BIO_Stdio_Gamma_Decode (&sbs, NULL) - 1;

  numwords = BIO_Stdio_Gamma_Decode (&sbs, NULL) - 1;

  local_N = docs_left;



  for (wr = WordRecs, i = 0; i < dict_size; i++, wr++)
    wr->ptr = 0;

  bzero ((char *) lg_bs, dict_size);

  for (i = 0; i < numwords; i++)
    {
      MG_u_long_t word_num, wcnt, fcnt, p;
      word_num = occur_to_lexical (i);

      wr = &WordRecs[word_num];

      wcnt = BIO_Stdio_Gamma_Decode (&sbs, NULL) - 1;
      if (wcnt >= 2)
	fcnt = BIO_Stdio_Gamma_Decode (&sbs, NULL);
      else
	fcnt = wcnt;

      p = fcnt;

      if (wcnt)
	{
	  register MG_u_long_t length;
	  counts[word_num] = p;
	  length = BIO_Bblock_Bound (local_N, p);
	  if (InvfLevel >= 2)
	    length += wcnt;
	  wr->ptr = length;
	  lg_bs[word_num] = floorlog_2 (BIO_Bblock_Init_W (local_N, p));
	}

    }

  crbs_pos = BIO_Random_Tell (&crbs);

  totalIbits = 0;
  last_total = 0;
  for (wr = WordRecs, i = 0; i < dict_size; i++, wr++)
    {
      register MG_u_long_t length;
      length = wr->ptr;
      wr->last = callnum;
      BIO_Random_Gamma_Encode (counts[i] + 1, &crbs, NULL);
      if (counts[i])
	{
	  if (i)
	    BIO_Random_Delta_Encode (totalIbits - last_total + 1, &crbs, NULL);
	  else
	    BIO_Random_Delta_Encode (1, &crbs, NULL);

	  last_total = totalIbits;
	}
      wr->ptr = totalIbits;
      totalIbits += length;
    }
  add_chunk_state (crbs_pos, callnum, local_N);

  if ((totalIbits + 7ul) >> 3ul > BufToUse)
    FatalError (1, "Pointers exceed buffer size");

  next_docs_left = BIO_Stdio_Gamma_Decode (&sbs, NULL) - 1;
}




static void 
DumpChunk (void)
{
  chunk_data[chunks_read - 1].disk_pos = Disk_pos << 3;
  fseek (chunks, Disk_pos, 0);
  fwrite (MemoryBuffer, sizeof (char), BufToUse, chunks);
  Disk_pos += BufToUse;
}




static void 
DiskMerge (void)
{
  random_bitio_state *rbsi;
  random_bitio_state *chks = NULL;
  MG_u_long_t *chunk_ptrs;
  int i;

  BIO_Random_Flush (&crbs);

  chunk_ptrs = Xmalloc (chunks_read * sizeof  (MG_u_long_t) );
  ChangeMemInUse (chunks_read * sizeof  (MG_u_long_t) );
  bzero ((char *) chunk_ptrs, chunks_read * sizeof  (MG_u_long_t) );

  rbsi = Xmalloc (chunks_read * sizeof (random_bitio_state));
  ChangeMemInUse (chunks_read * sizeof (random_bitio_state));
  for (i = 0; i < chunks_read; i++)
    {
      rbsi[i] = crbs;
      rbsi[i].Buf = Xmalloc (rbsi[i].len);
      ChangeMemInUse (rbsi[i].len);
      bcopy ((char *) (crbs.Buf), (char *) (rbsi[i].Buf), rbsi[i].len);
      BIO_Random_Seek (chunk_data[i].params_pos, &rbsi[i]);
    }

  if (chunks_read > 1)
    {
      int j;
      chks = Xmalloc ((chunks_read - 1) * sizeof (random_bitio_state));
      ChangeMemInUse ((chunks_read - 1) * sizeof (random_bitio_state));
      BIO_Random_Start (chunks, RND_BUF_SIZE, &chks[0]);
      ChangeMemInUse (RND_BUF_SIZE);
      for (j = 1; j < chunks_read - 1; j++)
	{
	  chks[j] = chks[0];
	  chks[j].Buf = Xmalloc (chks[0].len);
	  ChangeMemInUse (chks[0].len);
	  bcopy ((char *) (chks[0].Buf), (char *) (chks[j].Buf), chks[0].len);
	}
    }
  for (i = 0; i < dict_size; i++)
    {
      int j;
      invf_state_rec *isr = in_cache (i);
      register int B;

      BIO_Random_Seek (isr->Disk_Ptr, &rbs);	/* Position in invf file */

      B = isr->Disk_B;

      for (j = 0; j < chunks_read; j++)
	{
	  int p;
	  p = BIO_Random_Gamma_Decode (&rbsi[j], NULL) - 1;

	  if (p)
	    {
	      int ptr, b;
	      chunk_ptrs[j] += BIO_Random_Delta_Decode (&rbsi[j], NULL) - 1;
	      ptr = chunk_ptrs[j];
	      b = 1 << floorlog_2 (BIO_Bblock_Init_W (chunk_data[j].N, p));

	      if (j == chunks_read - 1)
		{
		  int k, CurrDoc;
		  DECODE_START ((u_char *) MemoryBuffer, ptr)
		    CurrDoc = isr->Disk_Last;
		  for (k = 0; k < p; k++)
		    {
		      register MG_u_long_t x, tf;
		      BBLOCK_DECODE (x, b);
		      if (k == 0)
			x = x + chunk_data[j].start_doc - isr->Disk_Last;
		      CurrDoc += x;
		      BIO_Random_Bblock_Encode (x, B, &rbs, NULL);
		      if (InvfLevel >= 2)
			{
			  UNARY_DECODE (tf);
			  BIO_Random_Gamma_Encode (tf, &rbs, NULL);
			}
		    }
		  DECODE_DONE
		    isr->Disk_Last = CurrDoc;
		}
	      else
		{
		  int k, CurrDoc;
		  random_bitio_state *Chks = chks + j;
		  BIO_Random_Seek (chunk_data[j].disk_pos + ptr, Chks);
		  CurrDoc = isr->Disk_Last;
		  for (k = 0; k < p; k++)
		    {
		      register MG_u_long_t x, tf;
		      x = BIO_Random_Bblock_Decode (b, Chks, NULL);
		      if (k == 0)
			x = x + chunk_data[j].start_doc - isr->Disk_Last;
		      CurrDoc += x;
		      BIO_Random_Bblock_Encode (x, B, &rbs, NULL);
		      if (InvfLevel >= 2)
			{
			  tf = BIO_Random_Unary_Decode (Chks, NULL);
			  BIO_Random_Gamma_Encode (tf, &rbs, NULL);
			}
		    }
		  isr->Disk_Last = CurrDoc;
		}
	    }
	}

      isr->Disk_Ptr = BIO_Random_Tell (&rbs);

    }
  if (chunks_read > 1)
    {
      int j;
      for (j = 0; j < chunks_read - 1; j++)
	{
	  Xfree (chks[j].Buf);
	  ChangeMemInUse (-chks[j].len);
	}
      Xfree (chks);
      ChangeMemInUse (-(chunks_read - 1) * sizeof (random_bitio_state));
    }

  for (i = 0; i < chunks_read; i++)
    {
      Xfree (rbsi[i].Buf);
      ChangeMemInUse (-rbsi[i].len);
    }
  Xfree (rbsi);
  ChangeMemInUse (-chunks_read * sizeof (random_bitio_state));
  chunks_read = 0;
  Xfree (chunk_ptrs);
  ChangeMemInUse (-chunks_read * sizeof  (MG_u_long_t) );
  Disk_pos = 0;
  BIO_Random_Seek (0, &crbs);
}

static void 
MergeIn (void)
{
  static int disk_chunks = 0;
  static int header = 0;
  if (!header)
    {
      fprintf (stderr, "ivf.pass2 : ");
      header = 1;
    }
  if (disk_chunks == ChunkLimit || next_docs_left == 0)
    {
      fprintf (stderr, "M");
      DiskMerge ();
      disk_chunks = 0;
    }
  else
    {
      fprintf (stderr, "-");
      DumpChunk ();
      disk_chunks++;
    }
  if (next_docs_left == 0)
    fprintf (stderr, "\n");
}


static int 
wl_comp (const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}

static int 
process_doc (u_char * s_in, int l_in)
{
  int res;
  u_char *end = s_in + l_in - 1;
  MG_u_long_t tocode;
  MG_u_long_t wl_pos = 0;

  if (!docs_left)
    LoadCounts ();

  callnum++;

  if (!INAWORD (*s_in))
    if (SkipSGML)
      PARSE_NON_STEM_WORD_OR_SGML_TAG (s_in, end);
    else
      PARSE_NON_STEM_WORD (s_in, end);

  while (s_in <= end)
    {
      u_char Word[MAXSTEMLEN + 1];

      PARSE_STEM_WORD (Word, s_in, end);
      stemmer (idh.stem_method, Word);
      if (SkipSGML)
	PARSE_NON_STEM_WORD_OR_SGML_TAG (s_in, end);
      else
	PARSE_NON_STEM_WORD (s_in, end);

      if (*Word == 0)
	continue;

      res = perf_hash (phd, Word);

      {
	word_rec *arr = &WordRecs[res];
	int b = 1 << lg_bs[res];
	wordnum++;

	tocode = callnum;

	ENCODE_START ((u_char *) MemoryBuffer, arr->ptr)

	  if (tocode > arr->last)
	  {
	    register int x;
	    x = tocode - arr->last - 1;
	    BBLOCK_ENCODE (x + 1, b);
	    if (InvfLevel >= 2)
	      ENCODE_BIT (1);
	    no_of_ptrs++;
	    arr->last = tocode;
	  }
	else if (InvfLevel >= 2)
	  {
	    __pos--;
	    ENCODE_BIT (0);
	    ENCODE_BIT (1);
	  }
	arr->ptr = __pos;
	ENCODE_DONE
      }

      if (MakeWeights)
	{
	  if (wl_pos >= wl_size)
	    {
	      wl_size += (wl_size >> 1);
	      word_list = Xrealloc (word_list, sizeof (*word_list) * wl_size);
	    }
	  word_list[wl_pos++] = res;
	}
    }
  if (MakeWeights)
    {
      float doc_weight = 0.0;
      if (wl_pos)
	{
	  MG_u_long_t *wl = word_list;
	  MG_u_long_t i, count, val;
	  qsort (wl, wl_pos, sizeof (*wl), wl_comp);
	  count = 1;
	  val = *wl++;
	  for (i = 1; i <= wl_pos; i++, wl++)
	    if (i == wl_pos || val != *wl)
	      {
		double weight = count * idf[val];
		doc_weight += weight * weight;
		count = 1;
		val = *wl;
	      }
	    else
	      count++;
	}
      HTONF(doc_weight);  /* [RPAP - Jan 97: Endian Ordering] */
      fwrite ((char *) &doc_weight, sizeof (doc_weight), 1, weights);
    }
  docs_left--;
  if (!docs_left)
    MergeIn ();

  return COMPALLOK;
}

int 
process_ivf_2 (u_char * s_in, int l_in)
{
  if (InvfLevel <= 2)
    return process_doc (s_in, l_in);
  else
    {
      int count = 0;
      int pos = 0;
      u_char *start = s_in;
      while (pos < l_in)
	{
	  if (s_in[pos] == TERMPARAGRAPH)
	    {
	      int len = pos + s_in + 1 - start;
	      if (process_doc (start, len) != COMPALLOK)
		return (COMPERROR);
	      start = s_in + pos + 1;
	      count++;
	    }
	  pos++;
	}
      if (start < s_in + pos)
	{
	  if (process_doc (start, pos + s_in - start) != COMPALLOK)
	    return (COMPERROR);
	  count++;
	}
      HTONSI(count);  /* [RPAP - Jan 97: Endian Ordering] */
      fwrite ((char *) &count, sizeof (count), 1, invf_para);
    }
  return COMPALLOK;
}





static void 
stats (MG_u_long_t len)
{
#ifndef SILENT
  fseek (count, 0, 2);
  fseek (count_trans, 0, 2);
  fseek (invf_state, 0, 2);
  fseek (invf, 0, 0);
  fseek (invf, 0, 2);
  fseek (chunks, 0, 2);
  fseek (chunk_state, 0, 2);
  Message ("File sizes\n");
  Message ("  Chunk desc    : %10u bytes\n", ftell (count));
  Message ("  Chunk trans   : %10u bytes\n", ftell (count_trans));
  Message ("  Chunks        : %10u bytes\n", ftell (chunks));
  Message ("  Chunk state   : %10u bytes\n", ftell (chunk_state));
  Message ("  Invf state    : %10u bytes\n", ftell (invf_state));
  Message ("  Peak invf     : %10u bytes\n", len);
  Message ("  Final invf    : %10u bytes\n", ftell (invf));
  Message ("Peak disk usage : %10.2f %%\n",
	   (double) (ftell (count) + ftell (count_trans) +
		     ftell (invf_state) + ftell (chunks) +
		     ftell (chunk_state) + len) / ftell (invf) * 100.0);
#endif
}


/* ARGSUSED */
int 
done_ivf_2 (char *FileName)
{
  MG_long_t i;
  MG_u_long_t totalIbits;
  MG_u_long_t invf_len;
  MG_u_long_t bytes_output;
  struct invf_file_header ifh;

  if (weights)
    fclose (weights);
  if (invf_para)
    fclose (invf_para);

  free_perf_hash (phd);

  free (MemoryBuffer);
  ChangeMemInUse (-MemBufSize);

  BIO_Random_Done (&rbs);
  BIO_Random_Done (&rbsp);
  fflush (invf);

  fseek (invf, 0, 2);
  invf_len = ftell (invf);

  fseek (invf_out, sizeof  (MG_long_t) , 0);
  /* [RPAP - Jan 97: Endian Ordering] */
  HTONUL2(dict_size, ifh.no_of_words);
  HTONUL2(no_of_ptrs, ifh.no_of_ptrs);
  ifh.skip_mode = 0;
  bzero ((char *) ifh.params, sizeof (ifh.params));
  HTONUL2(InvfLevel, ifh.InvfLevel);
  fwrite ((char *) &ifh, sizeof (ifh), 1, invf_out);

  bytes_output = ftell (invf_out);

  totalIbits = 32;		/* The magic number */
  totalIbits += 8 * 100;	/* A 100 byte gap */

  /* find the right place in the file to start reading p values */
  fseek (dict, sizeof  (MG_u_long_t)  + sizeof (struct invf_dict_header), 0);
  for (i = 0; i < dict_size; i++)
    {
      invf_state_rec *isr;
      MG_u_long_t fcnt, wcnt, s, e;
      register MG_u_long_t p;
      u_char dummy1, dummy2[MAXSTEMLEN + 1];

      HTONUL(bytes_output);  /* [RPAP - Jan 97: Endian Ordering] */
      fwrite ((char *) &bytes_output, sizeof (bytes_output), 1, invf_idx);
      NTOHUL(bytes_output);  /* [RPAP - Jan 97: Endian Ordering] */

      /* read an entry for a word, just to get p value */
      dummy1 = fgetc (dict);
      dummy1 = fgetc (dict);
      fread (dummy2, sizeof (u_char), dummy1, dict);
      fread ((char *) &fcnt, sizeof (fcnt), 1, dict);
      fread ((char *) &wcnt, sizeof (wcnt), 1, dict);

      /* [RPAP - Jan 97: Endian Ordering] */
      NTOHUL(fcnt);
      NTOHUL(wcnt);

      p = fcnt;

      isr = in_cache (i);

      e = (isr->Disk_Ptr + 7ul) >> 3ul;
      s = totalIbits >> 3;

      fseek (invf_in, s, 0);
      while (s < e)
	{
	  u_char c = getc (invf_in);
	  if (s == e - 1)
	    {
	      u_char ands[8] =
	      {0xff, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
	      c &= ands[isr->Disk_Ptr & 7ul];
	    }
	  putc (c, invf_out);
	  bytes_output++;
	  s++;
	}

      totalIbits += BIO_Bblock_Bound_b (N, p, isr->Disk_B);
      if (InvfLevel >= 2)
	totalIbits += BIO_Gamma_Bound (wcnt, fcnt);
      totalIbits = (totalIbits + 7ul) & 0xfffffff8ul;

    }

  fclose (invf_in);

  /* [RPAP - Feb 97: WIN32 Port] */
#ifdef __WIN32__
  if (!(_chsize (_fileno (invf_out), bytes_output)))
    Message ("Could not truncate invf.");
#else
  ftruncate (fileno (invf_out), bytes_output);
#endif

  fclose (invf_out);

  HTONUL(bytes_output);  /* [RPAP - Jan 97: Endian Ordering] */
  fwrite ((char *) &bytes_output, sizeof (bytes_output), 1, invf_idx);
  NTOHUL(bytes_output);  /* [RPAP - Jan 97: Endian Ordering] */

  fclose (invf_idx);

#ifndef SILENT
  {
    char *temp_str = msg_prefix;
    MG_u_long_t total;
    msg_prefix = "ivf.pass2";
    stats (invf_len);
    Message ("Pass two data structures      : %6.3f Mbyte\n",
	     (double) totalDbytes / 1024 / 1024);
    total = totalDbytes;
    Message ("Pass two hash structure(s)    : %6.3f Mbyte\n",
	     (double) totalHbytes / 1024 / 1024);
    total += totalHbytes;
    Message ("Peak extra memory in use      : %6.3f Mbyte\n",
	     (double) MaxMemInUse / 1024 / 1024);
    total += MaxMemInUse;
    Message ("Peak total memory in use      : %6.3f Mbyte\n",
	     (double) total / 1024 / 1024);
    msg_prefix = temp_str;
  }
#endif

  Xfree (WordRecs);
  Xfree (lg_bs);

  /* Free the memory allocated for the BIO_Random */
  occur_to_lexical (-1);

  BIO_Random_Done (&crbs);

  fclose (invf);
  fclose (dict);
  fclose (hash);
  fclose (count);
  fclose (count_trans);
  fclose (chunk_state);
  fclose (chunks);
  fclose (invf_state);
  return (COMPALLOK);
}
