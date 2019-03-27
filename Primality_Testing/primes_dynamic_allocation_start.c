#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "helpers_dynamic.h"


int is_prime(long long int n){

  long long int i;
  if (n<=1){
  	return 0;
  }
  for (i=2;i<n;i++){
    if (n%i==0){
	return 0;
      }
  }

  return 1;

}


void send_results(long long int* results,int num_primes){
}

void recv_results(long long int** primes,int* num_primes, int* primes_capacity, int* worker_rank){
}

void recv_assignment(long long int* min,long long int* max){
}

void send_assignment(int rank, long long int min, long long int max){
}


void workerstuff(options* o){
}

void send_quit(int rank){
}

long long int min(long long int a,long long int b){

	return a<b ? a:b;

}



void masterstuff(options* o){


}


int main(int argc, char** argv){
  MPI_Init(&argc,&argv);
  int rank;
  int numproc;

  MPI_Comm_size(MPI_COMM_WORLD,&numproc);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
  options o;
  set_opts(argc,argv,&o);
  if (rank==0){
    print_opts(&o);
    masterstuff(&o);
  }
  else{
    workerstuff(&o);
  }
  
  

  MPI_Finalize();
  return 0;
}
