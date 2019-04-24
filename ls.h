#ifndef LS_H_INCLUDED
#define LS_H_INCLUDED
#define isDirectory 1
#define isFile 0
#define firstBig 2
#define equals 3
#define secondBig 4
struct filename{
char*name;//filename
int type;//type=1 is directory type=0 is file
char*fullname;//fullfilename include the path.
};

int getFileNameArrayWithMode(char*file_path,struct filename f[],char*paramater,int*count);
char*connectPathAndName(char*filepath,char*fileName);
#endif // LS_H_INCLUDED
