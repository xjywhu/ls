#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
#include <unistd.h>
#include<string.h>

struct filename{
char*name;
int type;
};



void dicSort(struct filename f[],int count){
char* temp;
int type;
for(int i=0;i<count-1;i++){
  for(int k=i+1;k<count;k++){
if(strcasecmp(f[i].name,f[k].name)>0||(strcasecmp(f[i].name,f[k].name)==0&&f[i].type==1)){
temp=(char*)malloc(sizeof(char)*50);
strcpy(temp,f[i].name);
strcpy(f[i].name,f[k].name);
strcpy(f[k].name,temp);
type=f[i].type;
f[i].type=f[k].type;
f[k].type=type;
       }
    }
  }
for(int k=0;k<count;k++){
printf("%s\n", f[k].name);
}
}

int do_ls(char*file_name){
 DIR *dp;
 struct dirent *dirp;
 struct stat st;
 struct filename f[20];
 int count=0;
 if(lstat(file_name, &st) < 0) {
  perror(file_name);
  exit(1);
 }
 	if ((dp = opendir(file_name)) == NULL){
	printf("can't open %s",file_name);
	exit(1);
	}
	while ((dirp = readdir(dp)) != NULL){
	if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0){
	continue;
	}
	struct filename fn;
	fn.name=dirp->d_name;
	if(lstat(file_name, &st) < 0) {
    perror(file_name);
    exit(1);
    }
	if(S_ISREG(st.st_mode)){
	fn.type=0;
	}
	if(S_ISDIR(st.st_mode)){
    fn.type=1;
	}
	f[count]=fn;
	count++;
	}
	dicSort(f,count);
	closedir(dp);
	return 0;
}



int doCurrent(){
 char buf[80];
 char*currentpath;
 if((currentpath=getcwd(buf,sizeof(buf)))==NULL){

 exit(1);
 }
 else{
  if(do_ls(currentpath)==1)
  exit(1);
 }
 return 0;
}


int doTarget(char*filepath){
struct stat st;

if(lstat(filepath, &st) < 0) {
  perror(filepath);
  exit(1);
 }

 if(S_ISREG(st.st_mode)){
   printf("%s\n",filepath);
   return 0;
 }

 if(S_ISDIR(st.st_mode)){
    if(do_ls(filepath)==1)
    exit(1);
 }
return 0;
}


int main(int argc,char*argv[])
{
    switch(argc){
    case 1:
    if(doCurrent()==1)
    exit(1);
    break;
    case 2:
    if(doTarget(argv[1])==1)
    exit(1);
    break;
    case 3:
    break;

    default:
    printf("The format of the arguement is not correct\n");
    exit(1);
    }

}
