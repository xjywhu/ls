#include "print.h"
#include<sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<dirent.h>
#include<string.h>
#include<grp.h>
#include<time.h>
#include <pwd.h>
#include <sys/types.h>
#include<math.h>



void mode_to_letters(int mode,char str[])
{
	strcpy(str,"----------");
	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';


	//3 bits for user
	if(mode&S_IRUSR) str[1] = 'r';
	if(mode&S_IWUSR) str[2] = 'w';
	if(mode&S_IXUSR) str[3] = 'x';

	//3 bits for group
	if(mode&S_IRGRP) str[4] = 'r';
	if(mode&S_IWGRP) str[5] = 'w';
	if(mode&S_IXGRP) str[6] = 'x';

	//3 bits for other
	if(mode&S_IROTH) str[7] = 'r';
	if(mode&S_IWOTH) str[8] = 'w';
	if(mode&S_IXOTH) str[9] = 'x';
}

char *gid_to_name(gid_t gid)
{
	struct group*grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

char *uid_to_name( uid_t uid )
{
	struct	passwd* pw_ptr;
	static  char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;

	}
	else
		return pw_ptr->pw_name;

}

void printlongformat(struct filename f[],int count,char *paramater){
long int total=0;
struct stat st;
char str[10];
int isPrint=0;

for(int i=0;i<count;i++){
 if(lstat(f[i].name, &st) < 0) {
  perror(f[i].name);
  exit(1);
 }
  total+=st.st_blocks/2;
 }

 /*print total*/
if(strrchr(paramater,'h')!=NULL)
 printf("total %ldk\n",total);
else
 printf("total %ld\n",total);

for(int i=0;i<count;i++){
 if(lstat(f[i].name, &st) < 0) {
  perror(f[i].name);
  exit(1);
 }

mode_to_letters(st.st_mode,str);
/*print i-node number*/
if(strrchr(paramater,'i')!=NULL)
printf("%ld ",st.st_ino);

/*print the number of blocks*/
if(strrchr(paramater,'s')!=NULL){
if(strrchr(paramater,'h')!=NULL)
printf("%.1fk ",(float)st.st_blocks/2);
else
printf("%ld ",st.st_blocks/2);
}


printf("%s",str);
printf("%4d",(int)st.st_nlink);

/*print the id*/
if(strrchr(paramater,'n')!=NULL){
printf("%8d ",st.st_uid);
printf("%8d ",st.st_gid);
}else{
printf("%8s ",uid_to_name(st.st_uid));
printf("%8s ",gid_to_name(st.st_gid));
}

/*print the size*/
/*h*/
if(strrchr(paramater,'h')!=NULL){
if(st.st_size<1024)
printf("%8ld ",(long)st.st_size);
else{
if(st.st_size%1024==0)
printf("%8.1fk ",(float)(st.st_size)/1024);
else
//printf("%8.1fk ",(float)(st.st_size+102.3)/1024);
printf("%8.1fk ",(float)ceil((float)(st.st_size)/102.4)/10);
}

}
else
printf("%8ld ",(long)st.st_size);

/*print time*/
if(strrchr(paramater,'u')==NULL&&strrchr(paramater,'c')!=NULL){
printf("%.12s ",ctime(&st.st_ctime)+4);
}
else if(strrchr(paramater,'u')!=NULL&&strrchr(paramater,'c')==NULL){
printf("%.12s ",ctime(&st.st_atime)+4);
}
else if(strrchr(paramater,'u')!=NULL&&strrchr(paramater,'c')!=NULL){
if(strrchr(paramater,'u')>strrchr(paramater,'c'))
printf("%.12s ",ctime(&st.st_atime)+4);
else
printf("%.12s ",ctime(&st.st_ctime)+4);
}else{
printf("%.12s ",ctime(&st.st_mtime)+4);
}


if(strrchr(paramater,'F')!=NULL){
if(S_ISREG(st.st_mode)&&(S_IXUSR&st.st_mode)){printf("%s*\n",f[i].name); isPrint=1;}
if(S_ISDIR(st.st_mode)){printf("%s/\n",f[i].name);isPrint=1;}
if(S_ISLNK(st.st_mode)){printf("%s@\n",f[i].name);isPrint=1;}
if(S_ISSOCK(st.st_mode)){printf("%s=\n",f[i].name);isPrint=1;}
if(S_ISFIFO(st.st_mode)){printf("%s|\n",f[i].name);isPrint=1;}

}
if(isPrint==0)
printf("%s\n",f[i].name);
isPrint=0;

}

}
/*relative to the width of screen*/
void printNormalformat(struct filename f[],int count,char* paramater){
struct stat st;
int isPrint=0;
for(int i=0;i<count;i++){
 if(lstat(f[i].name, &st) < 0) {
  perror(f[i].name);
  exit(1);
 }

/*print i node*/
if(strchr(paramater,'i')!=NULL)
printf("%ld ",st.st_ino);
/*print the number of blocks*/
if(strchr(paramater,'s')!=NULL){
if(strchr(paramater,'h')!=NULL)
printf("%.1fk ",(float)st.st_blocks/2);
else
printf("%ld ",st.st_blocks/2);

}

/*print the type of the file*/

if(strrchr(paramater,'F')!=NULL){
if(S_ISREG(st.st_mode)&&(S_IXUSR&st.st_mode)){printf("%s*\n",f[i].name); isPrint=1;}
if(S_ISDIR(st.st_mode)){printf("%s/\n",f[i].name);isPrint=1;}
if(S_ISLNK(st.st_mode)){printf("%s@\n",f[i].name);isPrint=1;}
if(S_ISSOCK(st.st_mode)){printf("%s=\n",f[i].name);isPrint=1;}
if(S_ISFIFO(st.st_mode)){printf("%s|\n",f[i].name);isPrint=1;}

}
if(isPrint==0)
printf("%s\n",f[i].name);
isPrint=0;


}


}
char getPrintFormat(char*paramater){
/*null??*/
return paramater[strlen(paramater)];
}
/*f can close the l when f is after l*/
void printWithMode(struct filename f[],int count,char* paramater,char format){
/*if(strchr(paramater,'l')==NULL||(strchr(paramater,'l')!=NULL&&strchr(paramater,'f')!=NULL&&(strchr(paramater,'f')>strchr(paramater,'l'))))
printNormalformat(f,count,paramater);
else
printlongformat(f,count,paramater);*/
if(format=='l'||format=='n')
printlongformat(f,count,paramater);
else
printNormalformat(f,count,paramater);
}
