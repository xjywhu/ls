
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

void printlongformat(struct filename f[],int count){

 struct stat st;
char str[10];

for(int i=0;i<count;i++){
 if(lstat(f[i].name, &st) < 0) {
  perror(f[i].name);
  exit(1);
 }

mode_to_letters(st.st_mode,str);
printf("%s",str);
printf("%4d",(int)st.st_nlink);
printf("%8s ",uid_to_name(st.st_uid));
printf("%8s ",gid_to_name(st.st_gid));

//printf("%8d ",st.st_uid);
//printf("%8d ",st.st_gid);

printf("%8ld ",(long)st.st_size);
printf("%.12s ",ctime(&st.st_mtime)+4);
printf("%s\n",f[i].name);

}

}
