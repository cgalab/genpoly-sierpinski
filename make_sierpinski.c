/*****************************************************************************/
/*                                                                           */
/*                       1990-2020      Martin Held                          */
/*                                                                           */
/*                                                                           */
/* E-Mail:      held@cs.sbg.ac.at                                            */
/* Snail Mail:  Universitaet Salzburg, FB Computerwissenschaften,            */
/*              Jakob-Haringer Str. 2, A-5020 Salzburg, Austria (Europe).    */
/*                                                                           */
/*****************************************************************************/


/* get standard i/o library */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef BOOL_DEFINED
typedef bool boolean;
#else
#define false 0
#define true  (!false)
typedef unsigned char  boolean;
#endif


boolean first = true;
int start_x, start_y;

void Top(int Depth, FILE *output, int *x, int *y, int Length,
         int *Counter);
void Bottom(int Depth, FILE *output, int *x, int *y, int Length,
            int *Counter);
void Left(int Depth, FILE *output, int *x, int *y, int Length,
          int *Counter);
void Right(int Depth, FILE *output, int *x, int *y, int Length,
           int *Counter);


void MakeLine(FILE *output, int *x, int *y, int dx, int dy, 
              int Length, int *Counter)
{
   *x = *x + dx * Length; 
   *y = *y + dy * Length;

   fprintf(output, "%d %d\n", *x, *y);
   if (first) {
      start_x = *x;
      start_y = *y;
      first   = false;
   }
   *Counter = *Counter + 1;

   return;
}


void Top(int Depth, FILE *output, int *x, int *y, int Length,
         int *Counter)
{
   if (Depth > 0){
	  Top(Depth-1, output, x, y, Length, Counter); 
	  MakeLine(output, x, y, 1, -1, Length, Counter);
	  Right(Depth-1, output, x, y, Length, Counter);
	  MakeLine(output, x, y, 1, 0, 2 * Length, Counter);
	  Left(Depth-1, output, x, y, Length, Counter);  
	  MakeLine(output, x, y, 1, 1, Length, Counter);
	  Top(Depth-1, output, x, y, Length, Counter);
   }

   return;
}


void Right(int Depth, FILE *output, int *x, int *y, int Length,
           int *Counter)
{
   if (Depth > 0) {
	  Right(Depth-1, output, x, y, Length, Counter); 
	  MakeLine(output, x, y, -1, -1, Length, Counter);
	  Bottom(Depth-1, output, x, y, Length, Counter);
	  MakeLine(output, x, y, 0, -1, 2 * Length, Counter);
	  Top(Depth-1, output, x, y, Length, Counter);   
	  MakeLine(output, x, y, 1, -1, Length, Counter);
	  Right(Depth-1, output, x, y, Length, Counter);
   }

   return;
}


void Bottom(int Depth, FILE *output, int *x, int *y, int Length,
            int *Counter)
{
   if (Depth > 0) {
	  Bottom(Depth-1, output, x, y, Length, Counter);
	  MakeLine(output,x, y, -1, 1, Length, Counter);
	  Left(Depth-1, output, x, y, Length, Counter);  
	  MakeLine(output, x, y, -1, 0, 2 * Length, Counter);
	  Right(Depth-1, output, x, y, Length, Counter);  
	  MakeLine(output, x, y, -1, -1, Length, Counter);
	  Bottom(Depth-1, output, x, y, Length, Counter);
   }

   return;
}


void Left(int Depth, FILE *output, int *x, int *y, int Length,
         int *Counter)
{
   if (Depth > 0) {
	  Left(Depth-1, output, x, y, Length, Counter);  
	  MakeLine(output, x, y, 1, 1, Length, Counter);
	  Top(Depth-1, output, x, y, Length, Counter);   
	  MakeLine(output, x, y, 0, 1, 2 * Length, Counter);
	  Bottom(Depth-1, output, x, y, Length, Counter);
	  MakeLine(output, x, y, -1, 1, Length, Counter);
	  Left(Depth-1, output, x, y, Length, Counter);
   }

   return;
}



void PrintHeader(void)
{
   printf("\n");
   printf("***********************************************************\n");
   printf("*                                                         *\n");
   printf("*                   make_sierpinski                       *\n");
   printf("*                                                         *\n");
   printf("* Generation of a Sierpinski curve.                       *\n");
   printf("*                                                         *\n");
   printf("*      Martin Held 1990-2020        held@cs.sbg.ac.at     *\n");
   printf("*                                                         *\n");
   printf("***********************************************************\n");

   return;
}


void OpenFile(FILE  **output, const char *file_name)
{
   /* open file */
   if((*output = fopen(file_name, "w")) == NULL) {
      fprintf(stderr,"*** Output file not created! ***\n");
      exit(1);
   }
   
   return;
}   


int main(int argc, char **argv)
{
   int     count = 1;
   boolean success = true, name_read = false;
   int     x, y, Length;
   int     Depth = -1, Counter;
   FILE    *output;
   int     i, j;
   char    *file_name = "sierp.line";

   PrintHeader();

   /*                                                                        */
   /* parse command-line arguments                                           */
   /*                                                                        */
   while ((count < argc)  &&  success)    {

      if (strcmp(argv[count],"--number") == 0) {
         ++count;
         if ((success = (count < argc)))  Depth = atoi(argv[count]);
      }
      else if (strcmp(argv[count],"--output") == 0) {
         ++count;
         if ((success = (count < argc))) {
            file_name = argv[count];
            name_read = true;
         }
      }
      else {
         success = false;
      }
      ++count;
   }

   if (!success  ||  !name_read  ||  (Depth < 0)) {
      if (!success) 
         printf("\nUnrecognized command-line option: %s\n", argv[count-1]); 
      printf("\nUsage: make_sierpinski --number N --output XYZ\n       where N is a non-negative integer.\n"); 
      exit(2);
   }

   /*                                                                        */
   /* initialization                                                         */
   /*                                                                        */
   OpenFile(&output, file_name);
   i = 4;
   for (j = 0; j < Depth;  ++j)  i *= 4;
   fprintf(output,"%d\n", i + 1);   
   x       = 0;
   y       = 0;
   Length  = 1;
   Counter = 0;

   /*                                                                        */
   /* recursive generation of the Sierpinski curve: output                   */
   /*                                                                        */
   Top(Depth, output, &x, &y, Length, &Counter);   
   MakeLine(output, &x, &y, 1, -1, Length, &Counter);
   Right(Depth, output, &x, &y, Length, &Counter); 
   MakeLine(output, &x, &y, -1, -1, Length, &Counter);
   Bottom(Depth, output, &x, &y, Length, &Counter);
   MakeLine(output, &x, &y, -1, 1, Length, &Counter);
   Left(Depth, output, &x, &y, Length, &Counter);  
   MakeLine(output, &x, &y, 1, 1, Length, &Counter);

   /*                                                                        */
   /* close the polygon                                                      */
   /*                                                                        */
   fprintf(output, "%d %d\n", start_x, start_y);

   fclose(output);

   exit(0);
}
