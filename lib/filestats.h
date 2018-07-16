/**************************************************************************
 *
 * filestats.h -- Functions for keeping stats on file accesses
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
 * $Id: filestats.h,v 1.2 2004/10/31 00:11:04 beebe Exp beebe $
 *
 **************************************************************************/

#ifndef H_FILESTATS
#define H_FILESTATS

#include "sysfuncs.h"

typedef struct FileStats
  {
    MG_u_long_t NumSeeks;
    MG_u_long_t NumReads;
    MG_u_long_t NumBytes;
  }
FileStats;



typedef struct
  {
    FILE *f;
    char *pathname;
    char *name;
    FileStats Current;
    FileStats Cumulative;
  }
File;

/* if magic is 0 no magic number is read or written */
File *Fopen (char *name, char *mode, MG_u_long_t magic);

size_t Fread (void *ptr, size_t size, size_t nitems, File * F);

int Fseek (File * F, MG_long_t offset, int ptrname);

void Rewind (File * F);

int Fclose (File * F);

#define Getc(F) (F->Current.NumReads++, F->Current.NumBytes++, getc(F->f))

/* This adds the Current file stats to the Cumulative stats and then zeros
   the Current stats */
void ZeroFileStats (File * F);


#endif
