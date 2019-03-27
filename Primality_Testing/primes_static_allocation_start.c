#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers_static.h"

int is_prime(int n)
{

  int i;
  if (n <= 1)
  {
    return 0;
  }
  for (i = 2; i < n; i++)
  {
    if (n % i == 0)
    {
      return 0;
    }
  }

  return i;
}

void send_results(int *results, int min, int max)
{
}

// recieve results from rank rank, put results into results buffer, which is result_buffer_len long
// return number of primes recieved in result_len.
void recv_results(int rank, int *results, int result_buffer_len, int *result_len)
{
  MPI_Status stat;
  MPI_Recv(result, result_buffer_len, MPI_INT, rank, 0, MPI_COMM_WORLD, &stat);
  MPI_Get_count(stat, MPI_INT, result_len);
  printf("Rank 0 recieved %d primes from rank %d\n", *result_len, rank);
}

void workerstuff(options *o)
{
}

void masterstuff(options *o)
{
  // Determine which numbers to check for primality
  int my_low = 2; // inclusive
  int total_nums_to_check = o->maxn - 1;
  int my_high = my_low + total_nums_to_check / o->numprocs; // inclusive

  if (total_nums_to_check % o->numprocs != 0)
  {
    my_high++;
  }

  printf("Rank 0: Check %d -- %d\n", my_low, my_high);

  int j;
  int *my_prime_list = malloc(((my_high - my_low + 1) / 2 + 1) * sizeof(int));
  int my_num_primes = 0;

  for (j = my_low; j <= my_high; j++)
  {
    // printf("Rank 0: %d is %s\n", j, is_prime(j) != 0 ? "prime" : "not prime");
    if (is_prime(j))
    {
      my_prime_list[my_num_primes] = j;
      my_num_primes++;
    }
    // printf("Rank 0 found: ");
    // print_int_arr(my_prime_list, my_num_primes);

    int *all_prime_list = malloc(o->maxn * sizeof(int));
    int total_primes_found = 0;

    memcpy(all_prime_list, my_prime_list, my_num_primes * sizeof(int));
    total_primes_found = my_num_primes;

    int *temp_results = malloc(sizeof(int) * (total_nums_to_check / o->numprocs + 1));
    int temp_num_primes;

    for (j = 1; j < o->numprocs; j++)
    {
      // recv_results(j, temp_results, &temp_num_primes);
    }

    printf("Rank 0: Total prime list is:");
    print_int_arr(all_prime_list, total_primes_found);
  }
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank;
  int numproc;

  MPI_Comm_size(MPI_COMM_WORLD, &numproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  options o;
  set_opts(argc, argv, &o);
  if (rank == 0)
  {
    print_opts(&o);
    masterstuff(&o);
  }
  else
  {
    workerstuff(&o);
  }

  MPI_Finalize();
  return 0;
}
