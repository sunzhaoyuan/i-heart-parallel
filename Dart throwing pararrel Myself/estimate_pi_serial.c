#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>


int main(int argc, char** argv){

  int numtrials;
  if (argc<2){
    numtrials=1e7;
  }
  else{
    numtrials=atoi(argv[1]);
  }

  //seed the random number generator
  int basetime=time(NULL);
  srand(basetime);

  //Throw a dart at the square [0,1] x [0,1] numtrials times.  Record how many times
  //the dart lands inside the unit circle. 
  double x,y;
  int successes=0;
  int i;
  for (i=0;i<numtrials;i++){
    //generate random coordinates
    x=(double)rand()/RAND_MAX;
    y=(double)rand()/RAND_MAX;
    //see if they happen to be in unit circle
    if (x*x+y*y < 1){
      //record success
      successes+=1;
    }
  }
  //A random dart should land in that circle with probability pi/4.  So, we can estmate pi as
  //pi_est =4*successes/numtrials
  //watch out!  C does integer division by default, so include a double here to make it realize that I want a double division. 
  double pi_est=4.0*successes/numtrials;
  printf("Using %d samples we got pi is approximately %lf\n",numtrials,pi_est);
    
  return(0);
}
