#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int rank, nprocs;

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double start_time;
  double end_time;

  start_time = MPI_Wtime();

  int num_darts;
  if (argc < 2)
  {
    num_darts = 1e7;
  }
  else
  {
    num_darts = atoi(argv[1]);
  }

  int my_num_darts = num_darts / nprocs;
  // add up leftovers to rank 0
  if (rank == 0)
  {
    my_num_darts += num_darts % nprocs;
  }

  // printf("Rank: %d of %d, the total number of darts is: %d, I will throw: %d\n", rank, nprocs, num_darts, my_num_darts);

  //seed the random number generator using current time. 
  // Important! However, since we fire up each process at same time, they seeded same. To fix this, we add up current time with its rank.
  int basetime = time(NULL);
  srand(basetime + rank);

  //Throw a dart at the square [0,1] x [0,1] num_darts times.  Record how many times
  //the dart lands inside the unit circle.
  double x, y;
  int successes = 0;
  int i;
  for (i = 0; i < my_num_darts; i++)
  {
    //generate random coordinates
    x = (double)rand() / RAND_MAX;
    y = (double)rand() / RAND_MAX;
    //see if they happen to be in unit circle
    if (x * x + y * y < 1)
    {
      //record success
      successes += 1;
    }
  }

  printf("Rank: %d had %d/%d\n", rank, successes, my_num_darts);

  /**
  send 1 MPI_INT starting from memory position &successes to rank 0 on the world communication
  give it tag 0. 
  */
  if (rank != 0)
  {
    MPI_Send(&successes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  else
  {
    int buffer;
    int total_successes = successes;
    int i;
    MPI_Status stat;
    // There are nprocs-1 workers
    for (i = 1; i <= nprocs - 1; i++)
    {
      MPI_Recv(&buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
      printf("Rank 0 received: %d of successes\n", buffer);
      total_successes += buffer;
      //A random dart should land in that circle with probability pi/4.  So, we can estmate pi as
      //pi_est =4*successes/num_darts
      //watch out!  C does integer division by default, so include a double here to make it realize that I want a double division.
      double pi_est = 4.0 * successes / num_darts;
      printf("Using %d darts over %d processes we estimate pi to be %lf\n", num_darts, nprocs, pi_est);
    }
  }

  end_time = MPI_Wtime();
  if (rank==0) {
    printf("Elapsed time for %d darts with %d processes is %lf\n", end_time-start_time, num_darts, nprocs);
  }
  MPI_Finalize();
  return (0);
}
