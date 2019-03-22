#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

  int len;
  if (argc<2){
    printf("Give a length.\n");
    return 1;
  }
  //grab the second command line argument:
  sscanf(argv[1],"%d",&len);

  //make an array that is len long.
  int my_array[len];

  //add up the entries in my_array.
  int i;
  int total=0;
  for (i=0;i<len;i++){
    total=total+my_array[i];
  }
  printf("The total was %d\n", total);
  return 0;

}
