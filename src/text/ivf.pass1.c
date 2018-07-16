/**************************************************************************
 *
 * ivf.pass1.c -- Memory efficient pass 1 inversion
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
 * $Id: ivf.pass1.c,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/

#include "sysfuncs.h"

#include "memlib.h"
#include "messages.h"
#include "bitio_m.h"
#include "bitio_m_stdio.h"
#include "bitio_gen.h"
#include "local_strings.h"
#include "netorder.h"  /* [RPAP - Jan 97: Endian Ordering] */

#include "mg_files.h"
#include "invf.h"
#include "mg.h"
#include "build.h"
#include "locallib.h"
#include "words.h"
#include "stemmer.h"
#include "hash.h"


/*
   $Log: ivf.pass1.c,v $
   Revision 1.2  2004/10/31 00:13:35  beebe
   Major update for version 1.3.64x to support 64-bit architectures.

   Revision 1.1  2004/10/30 21:11:34  beebe
   Initial revision

   * Revision 1.6  1995/01/16  03:57:05  tes
   * Fixed bug for index_string_bytes which was calculated
   * by adding the suffix lengths except for every lookback (block)
   * number of words. This was incorrect as invf.dict is fully
   * front coded - perhaps it was partially coded in the past.
   * This would only affect a statistic - hence the reason why the
   * bug could exist ;-)
   *
   * Revision 1.5  1994/11/29  00:31:59  tes
   * Committing the new merged files and changes.
   *
   * Revision 1.3  1994/10/20  03:56:48  tes
   * I have rewritten the boolean query optimiser and abstracted out the
   * components of the boolean query.
   *
   * Revision 1.2  1994/09/20  04:41:34  tes
   * For version 1.1
   *
 */

static char *RCSID = "$Id: ivf.pass1.c,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $";

#define LOGLOOKBACK	2
#define POOL_SIZE 1024*1024
#define HASH_POOL_SIZE 8192
#define INITIAL_HASH_SIZE 7927

#define INIT_CHECK_FRAC 0.10
#define CHECK_FRAC 0.75
#define CHECK_CLOSE 0.999
#define PARA_DIV 1.5
#define NORM_DIV 1.0


typedef struct hash_rec
  {
    MG_u_long_t fcnt;		/* fragment frequency */
    MG_u_long_t lfcnt;	/* local fragment frequency */
    MG_u_long_t fnum;		/* last fragment to use stem */
    MG_u_long_t wcnt;		/* stem frequency */
    MG_u_long_t lwcnt;	/* local stem frequency */
    u_char *word;
  }
hash_rec;








static MG_u_long_t words_read = 0, words_diff = 0, bytes_diff = 0;
static MG_u_long_t outputbytes = 0;
static MG_u_long_t inputbytes = 0;
static MG_u_long_t MaxMemInUse = 0;
static MG_u_long_t MemInUse = 0;
static MG_u_long_t ChunksWritten = 0;


static FILE *ic;		/* The invf block file */
static stdio_bitio_state sbs;

static hash_rec **HashTable;
static MG_u_long_t HashSize;
static MG_u_long_t HashUsed;
static u_char *Pool;
static int PoolLeft;

static hash_rec *hr_pool;
static int hr_PoolLeft;

static hash_rec **first_occr;
static int max_first_occr;

static MG_u_long_t L1_bits = 0, L1_ohead = 0;
static MG_u_long_t L2_bits = 0, L2_ohead = 0;
static MG_u_long_t L3_bits = 0, L3_ohead = 0;
static MG_u_long_t callnum = 0, lcallnum = 0, wordnum = 0, lwordnum = 0;
static MG_u_long_t ptrcnt = 0;
static MG_u_long_t checknum;
static MG_long_t max_mem = 0;


static void 
ChangeMem (int Change)
{
  MemInUse += Change;
  if (MemInUse > MaxMemInUse)
    MaxMemInUse = MemInUse;
}



int 
init_ivf_1 (char *file_name)
{
  if (!(ic = create_file (file_name, INVF_CHUNK_SUFFIX, "wb", MAGIC_CHUNK,
			  MG_MESSAGE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return (COMPERROR);
  fwrite ("    ", sizeof (MG_u_long_t), 1, ic);	/* Space for the maxmem */
  ENCODE_START (ic)
    ENCODE_PAUSE (sbs)

    if (!(Pool = Xmalloc (POOL_SIZE)))
    {
      Message ("Unable to allocate memory for pool");
      return (COMPERROR);
    }
  PoolLeft = POOL_SIZE;
  ChangeMem (POOL_SIZE);

  if (!(hr_pool = Xmalloc (HASH_POOL_SIZE * sizeof (hash_rec))))
    {
      Message ("Unable to allocate memory for pool");
      return (COMPERROR);
    }
  hr_PoolLeft = HASH_POOL_SIZE;
  ChangeMem (HASH_POOL_SIZE * sizeof (hash_rec));

  HashSize = INITIAL_HASH_SIZE;
  HashUsed = 0;
  if (!(HashTable = Xmalloc (sizeof (hash_rec *) * HashSize)))
    {
      Message ("Unable to allocate memory for table");
      return (COMPERROR);
    }
  ChangeMem (sizeof (hash_rec *) * HashSize);
  bzero ((char *) HashTable, sizeof (hash_rec *) * HashSize);
  max_first_occr = 8192;
  if (!(first_occr = Xmalloc (sizeof (hash_rec *) * max_first_occr)))
    {
      Message ("Unable to allocate memory for first_occur");
      return (COMPERROR);
    }
  ChangeMem (sizeof (hash_rec *) * max_first_occr);

  checknum = (invf_buffer_size * INIT_CHECK_FRAC) /
    (InvfLevel == 3 ? PARA_DIV : NORM_DIV);

  return (COMPALLOK);
}



static MG_u_long_t 
mem_reqd (void)
{
  register int i;
  register MG_u_long_t total = 0;
/*  register MG_u_long_t N = InvfLevel == 3 ? lwordnum : lcallnum; */
  register MG_u_long_t N = lcallnum;
  for (i = 0; i < HashUsed; i++)
    {
      register hash_rec *ent = first_occr[i];
/*      register MG_u_long_t p = InvfLevel == 3 ? ent->lwcnt : ent->lfcnt; */
      register MG_u_long_t p = ent->lfcnt;
      if (p)
	total += BIO_Bblock_Bound (N, p);
      if (InvfLevel >= 2)
	total += ent->lwcnt;
    }

  total = (total + 7) >> 3;
  return total;
}

static MG_u_long_t 
max_mem_reqd (void)
{
  register int i;
  register MG_u_long_t total = 0;
/*  register MG_u_long_t N = InvfLevel == 3 ? wordnum : callnum; */
  register MG_u_long_t N = callnum;
  for (i = 0; i < HashUsed; i++)
    {
      register hash_rec *ent = first_occr[i];
/*      register MG_u_long_t p = InvfLevel == 3 ? ent->wcnt : ent->fcnt; */
      register MG_u_long_t p = ent->fcnt;
      if (p)
	total += BIO_Bblock_Bound (N, p);
      if (InvfLevel >= 2)
	total += ent->wcnt;
    }

  total = (total + 7) >> 3;
  return total;
}


static void 
dump_dict (MG_u_long_t mem)
{
  int i;

  ChunksWritten++;

  ENCODE_CONTINUE (sbs)

    GAMMA_ENCODE (lcallnum + 1);
  if (mem > max_mem)
    max_mem = mem;
  GAMMA_ENCODE (mem + 1);
  lwordnum = lcallnum = 0;
  GAMMA_ENCODE (HashUsed + 1);
  for (i = 0; i < HashUsed; i++)
    {
      hash_rec *ent = first_occr[i];
      GAMMA_ENCODE (ent->lwcnt + 1);
      if (ent->lwcnt >= 2)
	GAMMA_ENCODE (ent->lfcnt);
      ent->lwcnt = ent->lfcnt = 0;
    }
  ptrcnt = 0;
  ENCODE_PAUSE (sbs)
}



static int 
process_doc (u_char * s_in, int l_in)
{
  u_char *end = s_in + l_in - 1;

  callnum++;
  lcallnum++;
  inputbytes += l_in;

  if (!INAWORD (*s_in))
    if (SkipSGML)
      PARSE_NON_STEM_WORD_OR_SGML_TAG (s_in, end);
    else
      PARSE_NON_STEM_WORD (s_in, end);
  /*
   ** Alternately parse off words and non-words from the input
   ** stream beginning with a non-word. Each token is then
   ** inserted into the set if it does not exist or has it's
   ** frequency count incremented if it does.
   */
  while (s_in <= end)
    {
      u_char Word[MAXSTEMLEN + 1];

      PARSE_STEM_WORD (Word, s_in, end);
      stemmer (stem_method, Word);
      if (SkipSGML)
	PARSE_NON_STEM_WORD_OR_SGML_TAG (s_in, end);
      else
	PARSE_NON_STEM_WORD (s_in, end);

      words_read++;
      wordnum++;
      lwordnum++;
      /* Search the hash table for Word */
      {
	register MG_u_long_t hashval, step;
	register int hsize = HashSize;
	HASH (hashval, step, Word, hsize);
	for (;;)
	  {
	    register u_char *s1;
	    register u_char *s2;
	    register int len;
	    register hash_rec *ent;
	    ent = HashTable[hashval];
	    if (!ent)
	      {
		int len = *Word + 1;
		if (!hr_PoolLeft)
		  {
		    if (!(hr_pool = Xmalloc (HASH_POOL_SIZE *
					     sizeof (hash_rec))))
		      {
			Message ("Unable to allocate memory for pool");
			return (COMPERROR);
		      }
		    hr_PoolLeft = HASH_POOL_SIZE;
		    ChangeMem (HASH_POOL_SIZE * sizeof (hash_rec));
		  }
		ent = hr_pool++;
		hr_PoolLeft--;
		if (len > PoolLeft)
		  {
		    if (!(Pool = Xmalloc (POOL_SIZE)))
		      {
			Message ("Unable to allocate memory for pool");
			return (COMPERROR);
		      }
		    PoolLeft = POOL_SIZE;
		    ChangeMem (POOL_SIZE);
		  }
		ent->wcnt = 1;
		ent->fcnt = 1;
		ent->lwcnt = 1;
		ent->lfcnt = 1;
		ent->fnum = callnum;
		ent->word = Pool;
		memcpy (Pool, Word, len);
		Pool += len;
		PoolLeft -= len;
		if (HashUsed == max_first_occr - 1)
		  {
		    ChangeMem (-sizeof (hash_rec *) * max_first_occr);
		    max_first_occr *= 2;
		    if (!(first_occr = Xrealloc (first_occr, sizeof (hash_rec *) *
						 max_first_occr)))
		      {
			Message ("Unable to allocate memory for first_occr");
			return (COMPERROR);
		      }
		    ChangeMem (sizeof (hash_rec *) * max_first_occr);
		  }
		first_occr[HashUsed] = ent;
		HashUsed++;
		HashTable[hashval] = ent;
		bytes_diff += Word[0];
		break;
	      }

	    /* Compare the words */
	    s1 = Word;
	    s2 = ent->word;
	    len = *s1 + 1;
	    for (; len; len--)
	      if (*s1++ != *s2++)
		break;

	    if (len)
	      {
		hashval = (hashval + step);
		if (hashval >= hsize)
		  hashval -= hsize;
	      }
	    else
	      {
		ent->wcnt++;
		ent->lwcnt++;
		if (callnum > ent->fnum)
		  {
		    ptrcnt++;
		    ent->fcnt++;
		    ent->lfcnt++;
		    ent->fnum = callnum;
		  }
		break;
	      }
	  }
      }

      if (HashUsed >= HashSize >> 1)
	{
	  hash_rec **ht;
	  MG_u_long_t size;
	  MG_u_long_t i;
	  size = prime (HashSize * 2);
	  if (!(ht = Xmalloc (sizeof (hash_rec *) * size)))
	    {
	      Message ("Unable to allocate memory for table");
	      return (COMPERROR);
	    }
	  bzero ((char *) ht, sizeof (hash_rec *) * size);
	  ChangeMem (sizeof (hash_rec *) * size);

	  for (i = 0; i < HashSize; i++)
	    if (HashTable[i])
	      {
		register u_char *wptr;
		hash_rec *ent;
		register MG_u_long_t hashval, step;

		wptr = HashTable[i]->word;
		HASH (hashval, step, wptr, size);
		ent = ht[hashval];
		while (ent)
		  {
		    hashval += step;
		    if (hashval >= size)
		      hashval -= size;
		    ent = ht[hashval];
		  }
		ht[hashval] = HashTable[i];
	      }
	  Xfree (HashTable);
	  ChangeMem (-sizeof (hash_rec *) * HashSize);
	  HashTable = ht;
	  HashSize = size;
	}

    }

  if (ptrcnt >= checknum)
    {
      MG_u_long_t mem;
      /*fprintf(stderr, "Checking at %u . . . ", ptrcnt); */
      mem = mem_reqd ();
      if (mem >= invf_buffer_size * CHECK_CLOSE)
	{
	  /*fprintf(stderr, "Got a match\n"); */
	  dump_dict (mem);
	  checknum = (invf_buffer_size * INIT_CHECK_FRAC) /
	    (InvfLevel == 3 ? PARA_DIV : NORM_DIV);
	}
      else
	{
	  checknum = checknum * ((CHECK_FRAC * (invf_buffer_size - mem)) / mem) +
	    checknum;
	  if (checknum <= ptrcnt)
	    checknum = ptrcnt + 1;
	  /*fprintf(stderr, "Next check at %u\n", checknum); */
	}
    }
  return (COMPALLOK);

}				/* encode */


int 
process_ivf_1 (u_char * s_in, int l_in)
{
  if (InvfLevel <= 2)
    return process_doc (s_in, l_in);
  else
    {
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
	    }
	  pos++;
	}
      if (start < s_in + pos)
	return process_doc (start, pos + s_in - start);
    }
  return COMPALLOK;
}

static int 
PackHashTable (void)
{
  int s, d;
  for (s = d = 0; s < HashSize; s++)
    if (HashTable[s])
      HashTable[d++] = HashTable[s];
  ChangeMem (-sizeof (hash_rec *) * HashSize);
  ChangeMem (sizeof (hash_rec *) * HashUsed);
  if (!(HashTable = Xrealloc (HashTable, sizeof (hash_rec *) * HashUsed)))
    {
      Message ("Out of memory");
      return COMPERROR;
    }
  HashSize = HashUsed;
  return COMPALLOK;
}





static int 
ent_comp (const void *A, const void *B)
{
  u_char *s1 = (*((hash_rec **) A))->word;
  u_char *s2 = (*((hash_rec **) B))->word;

  return (casecompare (s1, s2)); /* [RPAP - Jan 97: Stem Index Change] */
}				/* stem_comp */





/*
 * void count_text()
 *
 * The maths used in this function is described in the paper "Coding for 
 * Compression in Full-Text Retrieval Systems"
 *
 */
static void 
count_text ()
{
  int i;
  for (i = 0; i < HashUsed; i++)
    {
      hash_rec *wrd = HashTable[i];
      /* estimate size of a level 1 inverted file */
      L1_bits += (int) (0.99 + wrd->fcnt * (1.6 + log2 (1.0 * callnum / wrd->fcnt)));
      L1_ohead += BIO_Gamma_Length (wrd->fcnt);

      L2_bits += BIO_Unary_Length (wrd->wcnt);
      L2_ohead += BIO_Gamma_Length (wrd->wcnt - wrd->fcnt + 1);

      L3_bits += (int) (0.99 + wrd->wcnt *
		  (1.6 + log2 (1.0 * words_read / (wrd->wcnt + callnum))));
      L3_ohead += 0;
    }
  L3_bits = (L3_bits + L2_bits + L1_bits + 7) / 8;
  L3_ohead = (L3_ohead + L2_ohead + L1_ohead + 7) / 8;
  L2_bits = (L2_bits + L1_bits + 7) / 8;
  L2_ohead = (L2_ohead + L1_ohead + 7) / 8;
  L1_bits = (L1_bits + 7) / 8;
  L1_ohead = (L1_ohead + 7) / 8;
}				/* count_text */



/*
 *    write_stem_file():
 *              writes out the stemmed dictionary file
 *              in the following format 
 *                      lookback value (int)
 *                      totalbytes value (int)
 *                      indexstringbytes (int)
 *                      for each word 
 *                        wordlen (4 bits)
 *                        prefix match (4 bits)
 *                        word (wordlen bytes)
 *                        word frequency (int)
 *                        word count (int)
 *
 *      Accesses outside variables:     
 *
 *      Return value...:                
 */

static void 
write_stem_file (char *file_name)
{
  MG_long_t j;
  struct invf_dict_header idh;
  MG_long_t lookback = (1 << LOGLOOKBACK);	/* ???? */
  MG_long_t totalbytes = 0;		/* The sum of the length of all words, including 
				   the length byte */
  MG_long_t indexstringbytes = 0;	/* The amount of space required to store the 
				   words in the diction, this takes into account 
				   the prefixes */
  u_char *lastword = NULL;	/* A pointer to the last word processed */
  FILE *sp;


  /* Calculate the size of certain things */
  for (j = 0; j < HashSize; j++)
    {
      u_char *word = HashTable[j]->word;
      indexstringbytes += word[0] + 2;
      totalbytes += word[0] + 1;
      if (lastword)
	indexstringbytes -= prefixlen (lastword, word);
      lastword = word;
    }

  lastword = NULL;

  if (!(sp = create_file (file_name, INVF_DICT_SUFFIX, "wb", MAGIC_STEM_BUILD,
			  MG_MESSAGE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return;

  /* [RPAP - Jan 97: Endian Ordering] */
  HTONUL2(lookback, idh.lookback);
  HTONUL2(HashSize, idh.dict_size);
  HTONUL2(totalbytes, idh.total_bytes);
  HTONUL2(indexstringbytes, idh.index_string_bytes);
  HTONUL2(inputbytes, idh.input_bytes);
  HTONUL2(callnum, idh.num_of_docs);
  HTONUL2(callnum, idh.static_num_of_docs);
  HTONUL2(words_read, idh.num_of_words);
  HTONUL2(stem_method, idh.stem_method);

  fwrite ((char *) &idh, sizeof (idh), 1, sp);
  outputbytes += sizeof (idh);

  for (j = 0; j < HashSize; j++)
    {
      int i;
      MG_u_long_t wcnt, fcnt;  /* [RPAP - Jan 97: Endian Ordering] */
      hash_rec *ent = HashTable[j];
      if (lastword != NULL)
	/* look for prefix match with prev string */
	i = prefixlen (lastword, ent->word);
      else
	i = 0;
      fputc (i, sp);
      fputc (ent->word[0] - i, sp);
      fwrite ((char *) ent->word + i + 1, sizeof (u_char), ent->word[0] - i, sp);
      outputbytes += ent->word[0] - i + 1;

      /* [RPAP - Jan 97: Endian Ordering] */
      HTONUL2(ent->fcnt, fcnt);
      fwrite ((char *) &(fcnt), sizeof (fcnt), 1, sp);

      outputbytes += sizeof (ent->fcnt);

      /* [RPAP - Jan 97: Endian Ordering] */
      HTONUL2(ent->wcnt, wcnt);
      fwrite ((char *) &(wcnt), sizeof (wcnt), 1, sp);

      outputbytes += sizeof (ent->wcnt);
      lastword = ent->word;
    }

  fclose (sp);
}				/* write_stem_file() */



/*
 *    write_codes():
 *              calls functions to assign and write out codes and
 *              then prints out stats about execution results.
 *
 *      Accesses outside variables:     z
 *
 *      Return value...:                z
 */

static void 
write_codes (char *file_name)
{
  MG_u_long_t dicts = 0;
  outputbytes = 0;
  write_stem_file (file_name);
  dicts = outputbytes;

#ifndef SILENT
  Message ("Chunks written      : %d\n",
	   ChunksWritten);
  if (InvfLevel == 3)
    Message ("Paragraphs          : %8d\n", callnum);
  Message ("Peak memory usage   : %10.1f Mb\n",
	   (double) MaxMemInUse / 1024.0 / 1024.0);
  Message ("Stems size          : %10.1f kB  %5.2f%%\n",
	   (double) dicts / 1024, (double) 100 * dicts / inputbytes);
  Message ("Lev 1 inverted file : %10.1f kB  %5.2f%%\n",
	   (double) (L1_bits + L1_ohead) / 1024,
	   (double) 100 * (L1_bits + L1_ohead) / inputbytes);
  Message ("Lev 2 inverted file : %10.1f kB  %5.2f%%\n",
	   (double) (L2_bits + L2_ohead) / 1024,
	   (double) 100 * (L2_bits + L2_ohead) / inputbytes);
  Message ("Lev 3 inverted file : %10.1f kB  %5.2f%%\n",
	   (double) (L3_bits + L3_ohead) / 1024,
	   (double) 100 * (L3_bits + L3_ohead) / inputbytes);
  Message ("Record addresses    : %10.1f kB  %5.2f%%\n",
	   (double) words_diff * 4 / 1024, (double) 100 * words_diff * 4 / inputbytes);
#endif
}				/* write_codes() */



void 
write_num_file (char *file_name)
{

  int i;
  FILE *f;

  for (i = 0; i < HashSize; i++)
    HashTable[i]->fnum = i;

  if (!(f = create_file (file_name, INVF_CHUNK_TRANS_SUFFIX, "wb",
			 MAGIC_CHUNK_TRANS, MG_MESSAGE)))  /* [RPAP - Feb 97: WIN32 Port] */
    return;

#if 1
  ENCODE_START (f)

    for (i = 0; i < HashSize; i++)
    BINARY_ENCODE (first_occr[i]->fnum + 1, HashSize + 1);

  ENCODE_DONE;
#else
  for (i = 0; i < HashSize; i++)
    {
      /* [RPAP - Jan 97: Endian Ordering] */
      MG_u_long_t fnum;
      HTONUL2(first_occur[i]->fnum, fnum);
      fwrite ((char *) &fnum, sizeof  (MG_u_long_t) , 1, f);
    }
#endif

  fclose (f);
}




int 
done_ivf_1 (char *FileName)
{
  char *temp_str = msg_prefix;
  msg_prefix = "ivf.pass1";
#ifndef SILENT
  Message ("Mem reqd for 1 chunk: %8u bytes\n",
	   max_mem_reqd ());
#endif
  if (lcallnum)
    dump_dict (mem_reqd ());

  if (PackHashTable () == COMPERROR)
    return COMPERROR;

  qsort (HashTable, HashUsed, sizeof (hash_rec *), ent_comp);

  count_text ();
  write_codes (FileName);

  ENCODE_CONTINUE (sbs)
    GAMMA_ENCODE (1);
  ENCODE_DONE
    fseek (ic, sizeof  (MG_long_t) , 0);
  
  HTONSL(max_mem);  /* [RPAP - Jan 97: Endian Ordering] */
  fwrite (&max_mem, sizeof (max_mem), 1, ic);
  NTOHSL(max_mem);  /* [RPAP - Jan 97: Endian Ordering] */

  fclose (ic);

  write_num_file (FileName);

  msg_prefix = temp_str;

  return (COMPALLOK);
}				/* done_encode */
