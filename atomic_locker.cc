#include <locking_routines.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char ** argv)
{
  int argn=1;
  const char * record = argv[argn++];
  const char * indexfile=argv[argn++];
  const char * outfile=argv[argn++];
  char * xtra = NULL;
  if( argc > 4 )
    {
      xtra = argv[argn++];
    }
  FILE * ifp = fopen(indexfile,"a");
  int ifd = fileno(ifp);
  struct flock ilock = get_whole_flock();

  if (fcntl(ifd,F_SETLKW,&ilock) == -1)
    {
      cerr << "ERROR: could not obtain lock on " << indexfile << '\n';
      exit(10);
    }

  FILE * ofp = fopen(outfile,"a");
  int ofd = fileno(ofp);
  struct flock olock = get_whole_flock();
  if (fcntl(ifd,F_SETLKW,&ilock) == -1)
    {
      cerr << "ERROR: could not obtain lock on " << outfile << '\n';
      exit(10);
    }

  if(xtra == NULL)
    {
      fprintf(ifp,"%s\t%li\n",record,lseek(ofd,0,SEEK_CUR));
    }
  else
    {
      fprintf(ifp,"%s\t%li\t%s\n",record,lseek(ofd,0,SEEK_CUR),xtra);
    }

  int rv;
  char ch;
  do
    {
      rv = fscanf(stdin,"%c",&ch);
      if(rv==0){exit(10);}
      fprintf(ofp,"%c",ch);
    }
  while(rv != EOF);

  olock.l_type = F_UNLCK;
  if (fcntl(ofd,F_UNLCK,&olock) == -1)
    {
      cerr << "ERROR: could not release lock on " << outfile << '\n';
      exit(10);
    }
  fclose(ofp);

  ilock.l_type = F_UNLCK;
  if (fcntl(ifd,F_UNLCK,&ilock) == -1)
    {
      cerr << "ERROR: could not release lock on " << indexfile << '\n';
      exit(10);
    }
  fclose(ifp);
}
