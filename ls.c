#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<dirent.h>
#include <unistd.h>
#include<string.h>
#include "cmp.h"
#include "print.h"
#include "ls.h"

struct filename fdir[200];
 int num=0;
 int ind=-1;


char*connectPathAndName(char*filepath,char*fileName){
char*tempName;
struct stat st;
if(lstat(filepath, &st) < 0) {
perror(filepath);
exit(1);
}
/*如果是目录文件则需要拼接，否则不需要拼接*/
if(S_ISDIR(st.st_mode)){
if(filepath[strlen(filepath)-1]=='/'){
tempName=(char*)malloc(strlen(filepath)+strlen(fileName)+1);
strcpy(tempName,filepath);
strcat(tempName,fileName);
}
else{
tempName=(char*)malloc(strlen(filepath)+strlen(fileName)+strlen("/")+1);
strcpy(tempName,filepath);
strcat(tempName,"/");
strcat(tempName,fileName);
}
}
else{
tempName=(char*)malloc(strlen(filepath)+1);
strcpy(tempName,filepath);
}
return tempName;
}
/*得到存放文件的数组，参数优先级d,a,A default*/
int getFileNameArrayWithMode(char*file_path,struct filename f[],char* paramater,int* count){
 DIR *dp;
 struct dirent *dirp;
 struct stat st;
 struct filename fn;
 int tempcount=0;
 	if(lstat(file_path, &st) < 0) {
    perror(file_path);
    exit(1);
    }
    /*如果目标是普通文件，则需要对该文件操作*/
	if(S_ISREG(st.st_mode)){
    fn.name=(char*)malloc(sizeof(char)*50);
	strcpy(fn.name,file_path);
	fn.type=isFile;
	f[tempcount]=fn;
	tempcount++;
	*count=tempcount;
	return 0;
	}
	/*如果目标是目录文件则需要对目录下面的所有文件操作*/
	if(S_ISDIR(st.st_mode)){
    if ((dp = opendir(file_path)) == NULL){
	printf("can't open %s",file_path);
	exit(1);
	}
	}

    /*有参数d则只需要操作当前目录*/
    if(strrchr(paramater,'d')!=NULL){
    fn.name=(char*)malloc(sizeof(char)*50);
	strcpy(fn.name,".");
	fn.type=isDirectory;
	f[tempcount]=fn;
	tempcount++;
	*count=tempcount;
	return 0;
    }
while ((dirp = readdir(dp)) != NULL){

    /*如果有参数A且没有a和f，则A起作用*/
    if(strrchr(paramater,'A')!=NULL&&strrchr(paramater,'a')==NULL&&strrchr(paramater,'f')==NULL){
        if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
	    continue;
    }
    /*没有参数的时候会跳过隐藏文件（以.开头）*/
    if(strlen(paramater)==0){
      if(dirp->d_name[0]=='.')
	    continue;
    }

	fn.name=(char*)malloc(sizeof(char)*50);
	strcpy(fn.name,dirp->d_name);
	fn.fullname=(char*)malloc(sizeof(char)*100);
	strcpy(fn.fullname,file_path);
	strcat(fn.fullname,"/");
	strcat(fn.fullname,fn.name);

    char*temp=connectPathAndName(file_path,fn.name);
	if(lstat(temp, &st) < 0) {
    perror(dirp->d_name);
    exit(1);
    }
	if(S_ISREG(st.st_mode)){
	fn.type=isFile;
	}
	if(S_ISDIR(st.st_mode)){
    fn.type=isDirectory;
	}
	f[tempcount]=fn;
	tempcount++;
	}
    closedir(dp);
    *count=tempcount;
	return 0;
}



int do_ls(char*file_path,char*fileNumber,char*sort,char*print,char format,int isRecursion,int isCurrent){

int count=0;
 struct filename f[30];
 struct stat st;
 if(isRecursion==1){
 /*递归处理*/
 printf("%s:\n",file_path);
 if(getFileNameArrayWithMode(file_path,f,fileNumber,&count)==1)
 exit(1);
 for(int i=0;i<count;i++){
     if(lstat(f[i].fullname, &st) < 0) {
    perror(f[i].fullname);
    exit(1);
    }
    if(S_ISDIR(st.st_mode)){

    struct filename fn;
    fn.type=isDirectory;
    fn.name=(char*)malloc(sizeof(char)*50);
    strcpy(fn.name,f[i].name);
    fn.fullname=(char*)malloc(sizeof(char)*100);

	if(ind==-1){
	if(isCurrent==1){
	strcpy(fn.fullname,"./");
	strcat(fn.fullname,fn.name);
	}else{
	strcpy(fn.fullname,file_path);
	strcat(fn.fullname,"/");
	strcat(fn.fullname,fn.name);
	}
	}else{
	strcpy(fn.fullname,fdir[ind].fullname);
	strcat(fn.fullname,"/");
	strcat(fn.fullname,fn.name);
	}

    fdir[num]=fn;
    num++;
    }

 }
 sortWithMode(f,count,sort,format);
 printWithMode(file_path,f,count,print,format);
 printf("\n");
 ind++;
    while(ind<num){
    if(do_ls(fdir[ind].fullname,fileNumber,sort,print,format,isRecursion,isCurrent)==1)
    exit(1);
    }
 }
 else{

 if(getFileNameArrayWithMode(file_path,f,fileNumber,&count)==1)
 exit(1);
 sortWithMode(f,count,sort,format);
 printWithMode(file_path,f,count,print,format);
 }
 return 0;
}



int doCurrent(char*fileNumber,char*sort,char*print,char format,int isRecursion){
 char buf[80];
 char*currentpath;
 int isCurrent=1;
 if((currentpath=getcwd(buf,sizeof(buf)))==NULL){
 exit(1);
 }
 else{
  if(do_ls(currentpath,fileNumber,sort,print,format,isRecursion,isCurrent)==1)
  exit(1);
 }
 return 0;
}


int doTarget(char*filepath,char*fileNumber,char*sort,char*print,char format,int isRecursion){
struct stat st;
int isCurrent=0;
if(lstat(filepath, &st) < 0) {
  perror(filepath);
  exit(1);
 }

if(do_ls(filepath,fileNumber,sort,print,format,isRecursion,isCurrent)==1)
exit(1);

return 0;
}


int main(int argc,char*argv[])
{

char*arguments="AaCcdFfhiklnRrSstuqx1w";
char*targetFile[10];
int args;
int fileNumberParamaterCount=0;
int sortParamaterCount=0;
int printParamaterCount=0;
int formatParamaterCount=0;
int isRecursion=0;
char*fileNumber=(char*)malloc(sizeof(char)*10);
char*sort=(char*)malloc(sizeof(char)*10);
char*print=(char*)malloc(sizeof(char)*10);
char*formatNumber=(char*)malloc(sizeof(char)*10);
/*把各类参数放在相应的数组
文件数目相关：A,a,d,f:fileNumber
排序相关：c,f,r,s,t,u:sort
输出内容相关:k,F,s,i,h,n,q:print
输出格式相关:n,l,f,1,x,c,w:format
是否递归处理：R:isRecursion
*/
while((args=getopt(argc,argv,arguments))!=-1){
if(args=='A'||args=='a'||args=='d'||args=='f')
fileNumber[fileNumberParamaterCount++]=args;
if(args=='c'||args=='f'||args=='r'||args=='S'||args=='t'||args=='u')
sort[sortParamaterCount++]=args;
if(args=='k'||args=='F'||args=='s'||args=='i'||args=='h'||args=='n'||args=='q'||args=='w')
print[printParamaterCount++]=args;
if(args=='n'||args=='l'||args=='1'||args=='x'||args=='C')
formatNumber[formatParamaterCount++]=args;
if(args=='R')
isRecursion=1;
}
int num=0;
for(int i=1;i<argc;i++){
if(argv[i][0]!='-'){
targetFile[num]=(char*)malloc(sizeof(char)*50);
targetFile[num]=argv[i];
num++;
}
}
//the last pamater work
//the f will close l
char format;
char*tempformat=(char*)malloc(sizeof(char)*3);
tempformat[1]=formatNumber[formatParamaterCount-1];
tempformat[0]=formatNumber[formatParamaterCount-2];
if(tempformat[1]!='f'){
format=tempformat[1];
}
else{
if(tempformat[0]=='l')
format=tempformat[3];
else
format=tempformat[0];
}


/*当目标文件数组为空，则对当前路径操作，否则对目标路径操作*/
if(num==0)
doCurrent(fileNumber,sort,print,format,isRecursion);
else{
for(int i=0;i<num;i++){
doTarget(targetFile[i],fileNumber,sort,print,format,isRecursion);
}
}
}
