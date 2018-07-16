#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <direct.h>
#include <windows.h>

#define default_memory 4  /*megabytes for working*/
#define separator '\\'
#define path_length 256
#define command_length 256

char database_name[path_length],         /*base name for database files*/
     source_directory[path_length],      /*first level directory with */
     source_wildcard[path_length],       /*final file name/wildcard*/
     cl_source_directory[path_length],   /*copy for command line use*/
     cl_source_wildcard[path_length],    /*copy for command line use*/
     command_dir[path_length],           /*base name for executables*/
     indexed[path_length];               /*indexed reply str*/
char single_file[3] = "";
int  verbose, 
     memory;

void give_up(void)
{
  cprintf("\n");
  cprintf("Program closing down - please press <ENTER>\n");
  getchar();
  exit(1);
}
	
void usage(char *pname)
{
  cprintf("Usage:    %s   collection_name   source_directory\n", pname);
  give_up();
}

int last_sep(char *name)
{
  int at = 0, sep = -1;
  while (*name != 0)
    {
      if (*name == separator) sep = at;
      name++;  at++;
    }
  return sep;
}

void just_directory(char *name)
{
  int sep = last_sep(name);
  if (sep < 0)
    name[0] = 0;    /*No directory*/
  else 
    name[sep+1] = 0;
}

void locate_database(char *name)
{
  char new_name[path_length];
  long dirtag;  _finddata_t dirinfo;
  int res, sep;
  strcpy(new_name, name);
  dirtag = _findfirst(new_name, &dirinfo);
  if (dirtag < 0)
    {
      res = mkdir(name);
      if (res != 0) {
	cprintf("\n");
	cprintf("Couldn't create directory for collection \"%s\"\n", name);
	give_up();
      }
    }
  else
    {
      if ((dirinfo.attrib & _A_SUBDIR) == 0) {
	cprintf("\n");
	cprintf("The name \"%s\" is already in use\n", new_name);
	give_up();
      }
      _findclose(dirtag);
    }
  strcat(new_name, "\\");
  sep = last_sep(name);
  if (sep < 0)
    strcat(new_name, name);
  else
    strcat(new_name, &name[sep+1]);
  strcpy(name, new_name);
}

void command_line_name(char *name_out, char *name_in)
/*Prepare a version of the input string(file name) to use*/
/*on a command line - putting in double quotes and doubling*/
/*the last backslash character if there are blanks in the name*/
{
  int blank = 0;  char *scan;

  scan = name_in;
  while (*scan != 0) if (*scan++ == ' ') blank = 1;
  if (blank == 0)
    strcpy(name_out, name_in);
  else
    {
      *name_out++ = '"';
      while (*name_in != 0)
	{
	  *name_out++ = *name_in++;
	}
      if (*(name_out-1) == '\\')
	*name_out++ = '\\';
      *name_out++ = '"';
      *name_out = 0;
    }
}

int iscol(char *name)
{
  int l = strlen(name);
  if (l < 4) return 0;
  if (name[l-4] == '.' &&
      (name[l-3] == 'c' || name[l-3] == 'C') &&
      (name[l-2] == 'o' || name[l-2] == 'O') &&
      (name[l-1] == 'l' || name[l-1] == 'L'))
    return 1;
  return 0;
}

void fix_source(char *name)
{
  int len = strlen(name), sep = last_sep(name), nstart;
  long dirtag;  _finddata_t dirinfo;
  char name_copy[path_length];

  strcpy(name_copy, name);
  if (sep >= 0 && sep == len-1)
    {
      /*Directory if \ char at end*/
      strcpy(source_directory, name_copy);
      strcpy(source_wildcard, "*.*");
    }
  else
    {                           /*No \ or text after \*/
      if (sep >= 0) nstart = sep+1; else nstart = 0;
      if (strrchr(&name_copy[nstart],'*') != NULL  ||
	  strrchr(&name_copy[nstart],'?') != NULL)
	{     /*Wildcard*/
	  strcpy(source_directory, name_copy);
	  strcpy(source_wildcard, &name_copy[nstart]);
	  source_directory[nstart] = 0;
	}
      else
	{                      /*Plain name at end*/
	  dirtag = _findfirst(name_copy, &dirinfo);
	  if (dirtag < 0)
	    {         /*File not found*/
	      cprintf("\n");
	      cprintf("Cannot find file or directory called \"%s\"\n", name_copy);
	      give_up();
	    }
	  else 
	    {                    /*File found*/
	      if ((dirinfo.attrib & _A_SUBDIR) != 0)
		{   /*File was directory*/
		  strcpy(source_directory, name_copy);
	      strcat(source_directory, "\\");
	      strcpy(source_wildcard, "*.*");
		}
	      else
		{          /*Plain name was an ordinary file (single file)*/
		  strcpy(source_directory, name_copy);
		  strcpy(source_wildcard, &name_copy[nstart]);
		  source_directory[nstart] = 0;
		  if (iscol(source_wildcard) == 1) 
		    strcpy(single_file, "-H");
		  else
		    strcpy(single_file,"-p");
		}
	      _findclose(dirtag);
	    }
	}
    }
  command_line_name(cl_source_directory, source_directory);
  command_line_name(cl_source_wildcard, source_wildcard);
}

void do_args(int argc, char *argv[])
{
  int i, l;

  strcpy(command_dir, argv[0]);
  just_directory(command_dir);
  database_name[0] = 0;
  source_directory[0] = 0;
  verbose = 0;  memory = default_memory;

  for (i = 1;  i < argc;  i++)
    {
      if (argv[i][0] == '-')
	{
	  l = strlen(argv[i]);
	  if (l > 1) {
	    if (argv[i][1] == 'v')
	      verbose = 1;
	    else if (argv[i][1] == 'm')
	      {
		if (l > 2)
		  memory = atoi(&argv[i][2]);
		else if (i+1 < argc) {
		  i++;
		  memory = atoi(argv[i]);
		}
		if (memory < 1 || memory > 50)
		  memory = default_memory;
	      }
	  }
	}
      else if (database_name[0] == 0)
	strcpy(database_name,argv[i]);
      else if (source_directory[0] == 0)
	strcpy(source_directory,argv[i]);
      else usage(argv[0]);
    }
  if (database_name[0] == 0)
    {
      cprintf("Collection name <help>: ");
      gets(database_name);
      if (database_name[0] == 0)
	{
	  cprintf("\n");
	  cprintf("Each collection must have a name. Names are from 1 to 8 characters\n");
	  cprintf("in length, and consist of letters and digits only.  Eg:  alice, t31\n");
	  cprintf("\n");
	  cprintf("Collection name: ");
	  gets(database_name);
	}
    }
  if (database_name[0] != 0)
    {
      if (source_directory[0] == 0)
	{
	  cprintf("\n");
	  cprintf("File(s) to collect <help>: ");
	  gets(source_directory);
	  if (source_directory[0] == 0)
	    {
	      cprintf("\n");
	      cprintf("A collection is built from text files.  These files will be read,\n");
	      cprintf("indexed and compressed to form the collection \"%s\".  If you\n",database_name);
	      cprintf("have only one text file, just provide its (path)name.  If you have\n");
	      cprintf("a number of files to use, provide the name of the directory\n");
	      cprintf("they are stored in.  You may enter a wild card name to control\n");
	      cprintf("the selection of files if required.  Eg:  c:\\data\\mine\\*.txt\n");
	      cprintf("\n");
	      cprintf("File(s) to collect: ");
	      gets(source_directory);
	    }
	}
    }
  if (database_name[0] == 0  ||  source_directory[0] == 0)
    {
      cprintf("\n");
      cprintf("Cannot proceed without collection name and file location\n");
      give_up();
    }

  cprintf("\n");
  cprintf("Build with indexes <Y>: ");
  gets(indexed);
  if (toupper(indexed[0]) != 'N' && toupper(indexed[0]) != 'Y')
    {
      cprintf("Invalid response - building collection with indexes.\n");
      indexed[0] = 'Y';
      indexed[1] = 0;
    }
  locate_database(database_name);
  fix_source(source_directory);
}

void report_parameters(void)
{
  cprintf("\n");
  cprintf("Collection name:   %s\n", database_name);
  cprintf("Source directory:  %s%s\n", source_directory, source_wildcard);
  cprintf("Memory available:  %d MBytes\n", memory);
}

void delete_temp_group(char *suffix)
{
  long dirtag;  _finddata_t dirinfo;  int sep;
  char search_name[path_length], base_name[path_length], rem_name[path_length];

  strcpy(base_name, database_name);
  sep = last_sep(base_name);
  if (sep >= 0) base_name[sep+1] = 0;
  sprintf(search_name, "%s%s", database_name, suffix);
  dirtag = _findfirst(search_name, &dirinfo);
  if (dirtag < 0)
    {
      cprintf("\n");
      cprintf("Couldn't find temporary file %s\n", search_name);
      give_up();
    }
  do {
    sprintf(rem_name,"%s%s",base_name,dirinfo.name);
    if (remove(rem_name) != 0)
      {
	cprintf("\n");
	cprintf("Couldn't remove temporary file %s\n", rem_name);
	give_up();
      }
  } while (_findnext(dirtag, &dirinfo) == 0);
  _findclose(dirtag);
}

void delete_temp_files(void)
{
  delete_temp_group(".invf.state.*");
  delete_temp_group(".chunk.state.*");
  delete_temp_group(".chunks.*");
  delete_temp_group(".trace");
  delete_temp_group(".tmp");
  delete_temp_group(".text.stats");
  delete_temp_group(".text.idx");
  delete_temp_group(".invf.chunk.trans");
  delete_temp_group(".invf.chunk");
  delete_temp_group(".invf.dict");
  delete_temp_group(".invf.dict.hash");
  delete_temp_group(".invf.idx");
  delete_temp_group(".weight");
}

void build_database(void)
{
  char command[command_length];

  if (toupper(indexed[0]) == 'Y')
    cprintf("MG with indexes runs in 11 stages.  It may take several minutes.\n");
  else
    cprintf("MG runs in 8 stages.  It may take several minutes.\n");
//  sprintf(command, "%sbin\\finder %s %s %s | %sbin\\mg_passes -f %s -2 -m %d -s 3 -G -t 10 -T1 -I1",
//	      command_dir, single_file, 
//		  cl_source_directory, cl_source_wildcard,
//		  command_dir, database_name, memory);

  if (toupper(indexed[0]) == 'Y')
    sprintf(command, "%sbin\\mgpass -f %s -2 -m %d -s 0 -G -t 10 -T1 -I1 %s %s %s",
       	    command_dir, database_name, memory, single_file, 
	    cl_source_directory, cl_source_wildcard);
  else
    sprintf(command, "%sbin\\mgpass -f %s -2 -m %d -s 3 -G -t 10 -T1 -I1 %s %s %s",
            command_dir, database_name, memory, single_file,
            cl_source_directory, cl_source_wildcard);

  cprintf("  1 mg_passes ...");
  if (system(command) != 0)
    {
      cprintf("\n\n");
      cprintf("Collection building failed (mg_passes 1)\n");
      give_up();
    }
  cprintf(" done                  ");

  sprintf(command, "%sbin\\mg_perf_hash_build -f %s",
	  command_dir, database_name);
  cprintf("2 mg_perf_hash_build ...");
  if (system(command) != 0)
    {
      cprintf("\n\n");
      cprintf("Collection building failed (mg_perf_hash_build)\n");
      give_up();
    }
  cprintf(" done\n");

  sprintf(command, "%sbin\\mg_compression_dict -f %s -S",
	  command_dir, database_name);
  cprintf("  3 mg_compression_dict ...");
  if (system(command) != 0)
    {
      cprintf("\n\n");
      cprintf("Collection building failed (mg_compression_dict)\n");
      give_up();
    }
  cprintf(" done        ");

  sprintf(command, "%sbin\\mgpass -f %s -2 -c 3 -G -t 10 -T2 -I2 %s %s %s",
	  command_dir, database_name, 
	  single_file, cl_source_directory, cl_source_wildcard);
  cprintf("4 mg_passes ...");
  if (system(command) != 0)
    {
      cprintf("\n\n");
      cprintf("Collection building failed (mg_passes 2)\n");
      give_up();
    }
  cprintf(" done\n");
  
  sprintf(command, "%sbin\\mg_weights_build -f %s -b 6",
	  command_dir, database_name);
  cprintf("  5 mg_weights_build ...");
  if (system(command) != 0)
    {
      cprintf("\n\n");
      cprintf("Collection building failed (mg_weights_build)\n");
      give_up();
    }
  cprintf(" done           ");
  sprintf(command, "%sbin\\mg_invf_dict -f %s -b 4096", 
	  command_dir, database_name);
  cprintf("6 mg_invf_dict ...");
  if (system(command) != 0)
    {
      cprintf("\n\n");
      cprintf("Collection building failed (mg_invf_dict)\n");
      give_up();
    }
  cprintf(" done\n");

  if (toupper(indexed[0]) == 'Y')
    {
      sprintf(command, "%sbin\\mg_stem_idx -f %s -b 4096 -s1",
	      command_dir, database_name);
      cprintf("  7 mg_stem_idx ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_stem_idx)\n");
	  give_up();
	}
      cprintf(" done                ");

      sprintf(command, "%sbin\\mg_stem_idx -f %s -b 4096 -s2",
	      command_dir, database_name);
      cprintf("8 mg_stem_idx ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_stem_idx)\n");
	  give_up();
	}
      cprintf(" done\n");

      sprintf(command, "%sbin\\mg_stem_idx -f %s -b 4096 -s3",
	      command_dir, database_name);
      cprintf("  9 mg_stem_idx ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_stem_idx)\n");
	  give_up();
	}
      cprintf(" done               ");

      sprintf(command, "%sbin\\mgstat -f %s -E",
	      command_dir, database_name);
      cprintf("10 mgstat ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_stat)\n");
	  give_up();
	}
      cprintf(" done\n");

      sprintf(command, "%sbin\\mg_fast_comp_dict -f %s",
	      command_dir, database_name);
      cprintf(" 11 mg_fast_comp_dict ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_fast_comp_dict)\n");
	  give_up();
	}
      cprintf(" done\n");
    }
  else
    {
      sprintf(command, "%sbin\\mgstat -f %s -E",
	      command_dir, database_name);
      cprintf("  7 mgstat ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_stat)\n");
	  give_up();
	}
      cprintf(" done                     ");

      sprintf(command, "%sbin\\mg_fast_comp_dict -f %s",
	      command_dir, database_name);
      cprintf("8 mg_fast_comp_dict ...");
      if (system(command) != 0)
	{
	  cprintf("\n\n");
	  cprintf("Collection building failed (mg_fast_comp_dict)\n");
	  give_up();
	}
      cprintf(" done\n");
    }

  delete_temp_files();
}

int main(int argc, char *argv[])
{
  char out_logfile_name[path_length], err_logfile_name[path_length];

  cprintf("University of Waikato MG System (Nov 1996)\n\n");
  do_args(argc,argv);
  report_parameters();

  if (verbose == 0)
    {
      sprintf(out_logfile_name,"%s.outlog.txt",database_name);
      sprintf(err_logfile_name,"%s.errlog.txt",database_name);
      if (freopen(out_logfile_name,"w",stdout) == NULL ||
	  freopen(err_logfile_name,"w",stderr) == NULL) 
	{
	  cprintf("\n");
  	  cprintf("Couldn't open log files (%s and %s)\n", 
		  out_logfile_name, err_logfile_name);
  	  give_up();
	}
      cprintf("\n");
      cprintf("Log files are %s and %s\n", 
	      out_logfile_name, err_logfile_name);
    }
  
  build_database();
  
  cprintf("\n");
  cprintf("Finished - please press <ENTER>\n");
  getchar();
  return 0;
}
