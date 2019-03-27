typedef struct options{
  long long int maxn;           //fine all the primes between 2 and maxn
  int printresults;   //print the results or not
  int numprocs;       //the number of processes that are running
  int numworkers;     //the number of workers that are running
  long long int taskchunksize;
}options;


//prints an integer array
void print_int_arr(long long int* arr, long long int n){
  long long int i;
  for (i=0;i<n-1;i++){
    printf("%d,",arr[i]);
  }
  printf("%d\n",arr[n-1]);
}

//prints out an options struct
void print_opts(options* o){

  printf("N=2..%lld\n",o->maxn);
  printf("taskchunksize: %lld\n",o->taskchunksize);
  printf("Print Results: %s\n", o->printresults!=0? "Yes":"No");
  printf("Number of processors: %d\n",o->numprocs);
  printf("Number of workers: %d\n",o->numworkers);
}

//Takes the command line arguments and fills up the proper options in the provided options struct. 
void set_opts(int argc, char** argv, options* o){
  int i;
  long long int dummyllint;
  int dummyint;
  o->maxn=1000;
  o->printresults=0;
  o->taskchunksize=50;
  MPI_Comm_size(MPI_COMM_WORLD,&dummyint);
  o->numprocs=dummyint;
  o->numworkers=o->numprocs-1;
  int forcestatic=0;
  for (i=0;i<argc;i++){
    if(sscanf(argv[i],"--printresults=%d",&dummyint)==1){
      o->printresults=dummyint;
    }
    if (sscanf(argv[i],"--taskchunksize=%lld",&dummyllint)==1){
      o->taskchunksize=dummyllint;
    }
    if (strcmp(argv[i],"--forcestatic")==0){
      forcestatic=1;
    }
    else if (sscanf(argv[i],"%lld",&dummyllint)==1){
      o->maxn=dummyllint;
    }
    
  }

  if (o->maxn<=1000){
    o->printresults=1;
  }

  if (forcestatic){
    o->taskchunksize=o->maxn/o->numworkers;
  }

 
  
}

