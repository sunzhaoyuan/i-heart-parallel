#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<mpi.h>


int main(int argc, char** argv){
  MPI_Init(&argc,&argv);

  int nprocs;
  int rank;

  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  double start_time;
  double end_time;
  start_time=MPI_Wtime();

  int total_num_darts;
  if (argc<2){
    total_num_darts=1e7;
  }
  else{
    total_num_darts=atoi(argv[1]);
  }


  int my_num_darts=total_num_darts/nprocs;

  if (rank==nprocs-1){
    my_num_darts+=total_num_darts % nprocs;
  }

  //printf("Rank %d/%d: %d darts should be thrown in total, I'll throw: %d\n",rank,nprocs,total_num_darts,my_num_darts);


  // //seed the random number generator
  int basetime=time(NULL);
  srand(basetime+1000*rank);

  // //Throw a dart at the square [0,1] x [0,1] numtrials times.  Record how many times
  // //the dart lands inside the unit circle. 
  double x,y;
  int my_successes=0;
  int i;
  for (i=0;i<my_num_darts;i++){
    //generate random coordinates
    x=1.00*rand()/RAND_MAX;
    y=(double)rand()/RAND_MAX;
    //see if they happen to be in unit circle
    if (x*x+y*y < 1){
      //record success
      my_successes+=1;
    }
  }
  printf("Rank %d: %d/%d successes\n",rank,my_successes,my_num_darts);

  if (rank==0){

    int i;
    int buffer;
    MPI_Status status;
    int total_successes=my_successes;
    for (i=1;i<nprocs;i++){
      MPI_Recv(&buffer,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
      //printf("Rank 0 received data %d from rank %d\n",buffer,status.MPI_SOURCE);
      total_successes+=buffer;
    }

    double pi_est=4.0*total_successes/total_num_darts;
    printf("Using %d darts on %d processes we estimate pi to be %lf\n",total_num_darts,nprocs,pi_est);
  }
  else{
    MPI_Send(&my_successes,1,MPI_INT,0,0,MPI_COMM_WORLD);
  }
  //A random dart should land in that circle with probability pi/4.  So, we can estmate pi as
  // //pi_est =4*successes/numtrials
  // //watch out!  C does integer division by default, so include a double here to make it realize that I want a double division. 
  // double pi_est=4.0*successes/numtrials;
  // printf("Using %d samples we got pi is approximately %lf\n",numtrials,pi_est);

  end_time=MPI_Wtime();
  if (rank==0){
    printf("Throwing %d darts with %d procceses took %lf seconds.\n",total_num_darts,nprocs,end_time-start_time);
  }
  MPI_Finalize();    
  return(0);
}
