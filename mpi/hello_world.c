#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
   // initialize varible types
   int size, rank;
   // initialize MPI instance
   MPI_Init(&argc, &argv);
   // get processor size and rank, save to variables
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   // print info and shut down MPI instance
   printf("size = %d rank = %d\n",size,rank);
   MPI_Finalize();   
   return(0);
}
