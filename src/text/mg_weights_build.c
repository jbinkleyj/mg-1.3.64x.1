/**************************************************************************
 *
 * mg_weights_build.c -- Program to build the document weights file
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
 * $Id: mg_weights_build.c,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $
 *
 **************************************************************************/

#include "sysfuncs.h"
#include "memlib.h"
#include "messages.h"
#include "local_strings.h"
#include "bitio_gen.h"
#include "bitio_m.h"
#include "bitio_m_stdio.h"
#include "timing.h"
#include "netorder.h"  /* [RPAP - Jan 97: Endian Ordering] */

#include "mg_files.h"
#include "locallib.h"
#include "invf.h"
#include "text.h"
#include "words.h"

#define MAXBITS (sizeof (MG_u_long_t)  * 8)

/*
   $Log: mg_weights_build.c,v $
   Revision 1.2  2004/10/31 00:13:35  beebe
   Major update for version 1.3.64x to support 64-bit architectures.

   Revision 1.1  2004/10/30 21:11:34  beebe
   Initial revision

   * Revision 1.4  1994/11/29  00:32:05  tes
   * Committing the new merged files and changes.
   *
   * Revision 1.3  1994/10/20  03:57:00  tes
   * I have rewritten the boolean query optimiser and abstracted out the
   * components of the boolean query.
   *
   * Revision 1.2  1994/09/20  04:41:55  tes
   * For version 1.1
   *
 */

static char *RCSID = "$Id: mg_weights_build.c,v 1.2 2004/10/31 00:13:35 beebe Exp beebe $";

unsigned char bits = 8;
static char *file_name = "";
static MG_u_long_t NumPara = 0;
static MG_u_long_t StaticNumOfDocs = 0;

MG_u_long_t get_NumPara (void);
MG_u_long_t get_StaticNumOfDocs (void);
void GenerateWeights (void);
void Make_weight_approx (void);
void Make_text_idx_wgt (void);


int
main (int argc, char **argv)
{
  ProgTime StartTime;
  int ch;
  opterr = 0;
  msg_prefix = argv[0];
  while ((ch = getopt (argc, argv, "f:d:b:sh")) != -1)
    switch (ch)
      {
      case 'f':		/* input file */
	file_name = optarg;
	break;
      case 'd':
	set_basepath (optarg);
	break;
      case 'b':
	bits = atoi (optarg);
	if (bits > 32)
	  {
	    fprintf (stderr, "b may only take values 0-32\n");
	    exit (1);
	  }
	break;
      case 'h':
      case '?':
	fprintf (stderr, "usage: %s [-f input_file]"
		 "[-d data directory] [-b bits] [-s] [-h]\n", argv[0]);
	exit (1);
      }
  GetTime (&StartTime);

  GenerateWeights ();

  Make_weight_approx ();

  Make_text_idx_wgt ();

  Message ("%s", ElapsedTime (&StartTime, NULL));
  return (0);
}




MG_u_long_t 
get_NumPara (void)
{
  struct invf_dict_header idh;
  FILE *invf_dict;
  if (NumPara)
    return (NumPara);
  invf_dict = open_file (file_name, INVF_DICT_SUFFIX, "rb", MAGIC_STEM_BUILD,
			 MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */
  fread ((char *) &idh, sizeof (idh), 1, invf_dict);
  fclose (invf_dict);
  NTOHUL2(idh.num_of_docs, NumPara);  /* [RPAP - Jan 97: Endian Ordering] */
  return NumPara;
}



MG_u_long_t 
get_StaticNumOfDocs (void)
/* the static number of documents is the N parameter used to
 * decode document gaps in the inverted file encoded using
 * the Bblock method.
 */
{
  struct invf_dict_header idh;
  FILE *invf_dict;
  if (StaticNumOfDocs)
    return (StaticNumOfDocs);
  invf_dict = open_file (file_name, INVF_DICT_SUFFIX, "rb", MAGIC_STEM_BUILD,
			 MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */
  fread ((char *) &idh, sizeof (idh), 1, invf_dict);
  fclose (invf_dict);
  NTOHUL2(idh.static_num_of_docs, StaticNumOfDocs);  /* [RPAP - Jan 97: Endian Ordering] */
  return StaticNumOfDocs;
}



void 
GenerateWeights (void)
{
  FILE *dict, *invf, *f, *idx;
  struct invf_dict_header idh;
  struct invf_file_header ifh;
  int i;
  double logN;
  float *DocWeights;

  get_NumPara ();
  get_StaticNumOfDocs ();

  if ((f = open_file (file_name, WEIGHTS_SUFFIX, "rb", MAGIC_WGHT,
		      MG_CONTINUE)) != NULL)
    {
      fclose (f);
      return;
    }

  logN = log ((double) NumPara);

  Message ("The file \"%s.weight\" does not exist.", file_name);
  Message ("Building the weight data from the file \"%s.invf\".", file_name);

  if (!(DocWeights = Xmalloc (sizeof (float) * (NumPara + 1))))
      FatalError (1, "No memory for doc weights");
  bzero ((char *) DocWeights, sizeof (float) * (NumPara + 1));

  dict = open_file (file_name, INVF_DICT_SUFFIX, "rb", MAGIC_STEM_BUILD,
		    MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */
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

  idx = open_file (file_name, INVF_IDX_SUFFIX, "rb", MAGIC_INVI,
		   MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */

  invf = open_file (file_name, INVF_SUFFIX, "rb", MAGIC_INVF,
		    MG_ABORT);
  fread ((char *) &ifh, sizeof (ifh), 1, invf);

  /* [RPAP - Jan 97: Endian Ordering] */
  NTOHUL(ifh.no_of_words);
  NTOHUL(ifh.no_of_ptrs);
  NTOHUL(ifh.skip_mode);
  for (i = 0; i <= 15; i++)
    NTOHUL(ifh.params[i]);
  NTOHUL(ifh.InvfLevel);

  if (ifh.skip_mode != 0)
    FatalError (0, "Can\'t make weights file from a skipped inverted file.");
  if (ifh.InvfLevel == 1)
    FatalError (0, "Can\'t make weights file from level 1 inverted file.");

  DECODE_START (invf)

    for (i = 0; i < ifh.no_of_words; i++)
    {
      u_char dummy1, dummy2[MAXSTEMLEN + 1];
      MG_u_long_t fcnt, wcnt, blk, CurrDoc, p, j;
      float idf;

      if ((i & 0xfff) == 0)
	fprintf (stderr, ".");
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

      idf = logN - log ((double) fcnt);
      blk = BIO_Bblock_Init (StaticNumOfDocs, p);
      CurrDoc = 0;

      {
	MG_u_long_t loc;
	fread ((char *) &loc, sizeof (loc), 1, idx);
	NTOHUL(loc);  /* [RPAP - Jan 97: Endian Ordering] */
	if (ftell (invf) != loc)
	  {
	    FatalError (1, "Word %d  %d != %d", i, ftell (invf), loc);
	  }
      }
      for (j = 0; j < p; j++)
	{
	  MG_u_long_t x, tf;
	  BBLOCK_DECODE (x, blk);
	  CurrDoc += x;
	  if (ifh.InvfLevel >= 2)
	    {
	      double weight;
	      GAMMA_DECODE (tf);
	      weight = tf * idf;
	      DocWeights[CurrDoc - 1] += weight * weight;
	    }
	}
      while (__btg)
	DECODE_BIT;
    }
  DECODE_DONE
    fclose (dict);
  fclose (invf);
  fprintf (stderr, "\n");

  /* [RPAP - Jan 97: Endian Ordering] */
  for (i = 0; i < NumPara; i++)
    HTONF(DocWeights[i]);

  f = create_file (file_name, WEIGHTS_SUFFIX, "wb", MAGIC_WGHT,
		   MG_ABORT);

  fwrite ((char *) DocWeights, sizeof (float), NumPara, f);
  fclose (f);
  Xfree (DocWeights);
}











void 
Make_weight_approx (void)
{
  int i, pos, max;
  MG_u_long_t buf;
  double U, L, B;
  FILE *approx, *exact;

  exact = open_file (file_name, WEIGHTS_SUFFIX, "rb", MAGIC_WGHT,
		     MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */

  /* calculate U and L */
  L = 1e300;
  U = 0;
  for (i = 0; i < NumPara; i++)
    {
      float wgt;
      fread ((char *) &wgt, sizeof (wgt), 1, exact);
      NTOHF(wgt);  /* [RPAP - Jan 97: Endian Ordering] */
      wgt = sqrt (wgt);
      if (wgt > U)
	U = wgt;
      if (wgt > 0 && wgt < L)
	L = wgt;

    }
  fseek (exact, sizeof (MG_u_long_t), SEEK_SET);

  B = pow (U / L, pow (2.0, -(double) bits));

  fprintf (stderr, "L = %f\n", L);
  fprintf (stderr, "U = %f\n", U);
  fprintf (stderr, "B = %f\n", B);



  approx = create_file (file_name, APPROX_WEIGHTS_SUFFIX, "wb",
			MAGIC_WGHT_APPROX, MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */

  fwrite ((char *) &bits, sizeof (bits), 1, approx);
  HTOND(L);  /* [RPAP - Jan 97: Endian Ordering] */
  HTOND(B);  /* [RPAP - Jan 97: Endian Ordering] */
  fwrite ((char *) &L, sizeof (L), 1, approx);
  fwrite ((char *) &B, sizeof (B), 1, approx);
  NTOHD(L);  /* [RPAP - Jan 97: Endian Ordering] */
  NTOHD(B);  /* [RPAP - Jan 97: Endian Ordering] */

  max = bits == 32 ? 0xffffffff : (1 << bits) - 1;
  for (buf = pos = i = 0; i < NumPara; i++)
    {
      MG_u_long_t fx;
      float wgt;
      fread ((char *) &wgt, sizeof (wgt), 1, exact);
      NTOHF(wgt);  /* [RPAP - Jan 97: Endian Ordering] */
      wgt = sqrt (wgt);
      if (wgt == 0.0F)
	{
	  wgt = L;
	  Message ("Warning: Document %d had a weight of 0.", i);
	}
      fx = (int) floor (log (wgt / L) / log (B));

      if (fx > max)
	fx = max;

      buf |= (fx << pos);
      pos += bits;

      if (pos >= MAXBITS)
	{
	  HTONUL(buf);
	  fwrite ((char *) &buf, sizeof (buf), 1, approx);
	  buf = fx >> (bits - (pos - MAXBITS));
	  pos = pos - MAXBITS;
	}
    }
  if (pos > 0)
    {
      /* [RPAP - Jan 97: Endian Ordering] */
      HTONUL(buf);
      fwrite ((char *) &buf, sizeof (buf), 1, approx);
    }

  fclose (approx);
  fclose (exact);
}





void 
Make_text_idx_wgt (void)
{
  compressed_text_header cth;
  int i;
  FILE *idx_wgt, *idx, *para, *exact;

  idx_wgt = create_file (file_name, TEXT_IDX_WGT_SUFFIX, "wb", MAGIC_TEXI_WGT,
			 MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */

  idx = open_file (file_name, TEXT_IDX_SUFFIX, "rb", MAGIC_TEXI,
		   MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */
  if (fread (&cth, sizeof (cth), 1, idx) != 1)
    FatalError (1, "Unable to read header of index file");

  /* [RPAP - Jan 97: Endian Ordering] */
  NTOHUL(cth.num_of_docs);
  NTOHUL(cth.num_of_bytes);
  NTOHUL(cth.num_of_words);
  NTOHUL(cth.length_of_longest_doc);
  NTOHD(cth.ratio);

  exact = open_file (file_name, WEIGHTS_SUFFIX, "rb", MAGIC_WGHT,
		     MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */

  get_NumPara ();
  if (cth.num_of_docs != NumPara)
    {
      Message ("The number of documents does not equal "
	       "the number of paragraphs.");
      Message ("Using the \"%s.invf.paragraph\" file\n", file_name);
      para = open_file (file_name, INVF_PARAGRAPH_SUFFIX, "rb", MAGIC_PARAGRAPH,
			MG_ABORT);  /* [RPAP - Feb 97: WIN32 Port] */
    }
  else
    para = NULL;

  {
    struct
      {
	MG_u_long_t Start;
	float Weight;
      }
    data;
    for (i = 0; i < cth.num_of_docs; i++)
      {
	int count;
	fread ((char *) &data.Start, sizeof  (MG_u_long_t) , 1, idx);
	if (para && i < cth.num_of_docs)
	  {
	    /* [RPAP - Jan 97: Endian Ordering] */
	    fread ((char *) &count, sizeof (count), 1, para);
	    NTOHSI(count);
	  }
	else
	  count = 1;
	while (count--)
	  {
	    fread ((char *) &data.Weight, sizeof (float), 1, exact);
	    NTOHF(data.Weight);  /* [RPAP - Jan 97: Endian Ordering] */
	    data.Weight = sqrt (data.Weight);
	    HTONF(data.Weight);  /* [RPAP - Jan 97: Endian Ordering] */
	    fwrite ((char *) &data, sizeof (data), 1, idx_wgt);
	  }
      }
    /* Write out the extra entry for the idx file */
    fread ((char *) &data.Start, sizeof  (MG_u_long_t) , 1, idx);
    data.Weight = 0;
    fwrite((char*)&data, sizeof(data), 1, idx_wgt);
  }

  fclose (idx_wgt);
  fclose (idx);
  fclose (exact);
  if (para)
    fclose (para);
}
