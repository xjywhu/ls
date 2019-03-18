#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>

int doCurrent(){




}
int doTarget(char*filepath){
struct stat buf;

if(lstat(filepath, &buf) < 0) {
  printf("lstat error when manage the filepath\n");
  exit(1);
 }



}


int main(int argc,char*argv[])
{
    switch(argc){
    case'1':
    break;
    case'2':
    if(doTarget(argv[1])==1)
    exit(1);
    break;
    case'3':
    break;

    default:
    printf("The format of the arguement is not correct");
    exit(1);

    }

}
