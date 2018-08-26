#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<algorithm>
#include<time.h>
#include <bits/stdc++.h>
using namespace std;

#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))
#define clearsc() printf("\033[H\033[J")
#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x0A
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108
#define KEY_BACKSPACE 0x0008
#define KEY_HOME    0x68

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);
map<int,string> mp;
stack<string> BackdirNames;
stack<string> FwddirNames;
int counter=0;
string rootDir(void){
    return "/home/sayan";
}
std::string GetCurrentWorkingDir(void) {
  char buff[FILENAME_MAX];
  getcwd( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}
void listd(int c){
    struct stat stn_buf ;
    int status ;
    struct dirent *dir;
    DIR *d;
    string y;
    bool flag=true;
    if(c==KEY_ENTER){
        y=mp[counter];
        if(is_dir(y.c_str())){

        d = opendir(y.c_str()) ;
        BackdirNames.push(y);
        }
        else{
            flag=false;
            //fopen(y.c_str(),"r");
            string total = "xdg-open "+y+" &";
            system(total.c_str());
        }
    }
    else if(c==KEY_RIGHT){
        string x=BackdirNames.top();
        FwddirNames.push(x);
        BackdirNames.pop();
        x=BackdirNames.top();
        d=opendir(x.c_str());
        y=x;
    }
    else if(c==KEY_LEFT){
        string x=FwddirNames.top();
        FwddirNames.pop();
        BackdirNames.push(x);
        d=opendir(x.c_str());
        y=x;
    }
    else if(c==KEY_BACKSPACE){
        string x=BackdirNames.top();
        FwddirNames.push(x);
        BackdirNames.pop();
        x=BackdirNames.top();
        d=opendir(x.c_str());
        
    }
    else 
    {
        if(c==KEY_HOME)
        d=opendir(rootDir().c_str()); // KEY H
    }      
    
    if (d && flag)
    {
        clearsc();
        mp.clear();
        counter=0;
        //int counter=0;
        printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &stn_buf) ;
            string x = y+"/"+dir->d_name;
            mp[counter++]= x;
            printf("\033[0;33m %-30s%10ld\t",dir->d_name , stn_buf.st_size);
            printf( (S_ISDIR(stn_buf.st_mode)) ? "d" : "-");
            printf( (stn_buf.st_mode & S_IRUSR) ? "r" : "-");
            printf( (stn_buf.st_mode & S_IWUSR) ? "w" : "-");
            printf( (stn_buf.st_mode & S_IXUSR) ? "x" : "-");
            printf( (stn_buf.st_mode & S_IRGRP) ? "r" : "-");
            printf( (stn_buf.st_mode & S_IWGRP) ? "w" : "-");
            printf( (stn_buf.st_mode & S_IXGRP) ? "x" : "-");
            printf( (stn_buf.st_mode & S_IROTH) ? "r" : "-");
            printf( (stn_buf.st_mode & S_IWOTH) ? "w" : "-");
            printf( (stn_buf.st_mode & S_IXOTH) ? "x" : "-");
            printf("\t%30s", ctime(&stn_buf.st_mtime)) ;
        }
        closedir(d);
    }
    
}
int main(int argc, char **argv)
{
    FILE *file;
    file =fopen("/home/sayan/curspos.cpp","r");
    if (file == NULL) {
      perror("Error");
    } 
    else
        printf("YAY");
    struct stat st_buf ;
    int status ;
    DIR *d;
    struct dirent *dir;
    string s=GetCurrentWorkingDir();
    BackdirNames.push(s);
    d = opendir(s.c_str()) ;
    if (d)
    {
        
        printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &st_buf) ;
            string x = s+"/"+dir->d_name;
            mp[counter++]= x;
            // if(S_ISDIR (st_buf.st_mode)) 
            // {
            // printf("\033[0;32m %-30s%10ld\t", dir->d_name, st_buf.st_size );
            // printf( (S_ISDIR(st_buf.st_mode)) ? "d" : "-");}
            // else 
            printf("\033[0;33m %-30s%10ld\t",dir->d_name , st_buf.st_size);
            printf( (S_ISDIR(st_buf.st_mode))  ? "d" : "-");
            printf( (st_buf.st_mode & S_IRUSR) ? "r" : "-");
            printf( (st_buf.st_mode & S_IWUSR) ? "w" : "-");
            printf( (st_buf.st_mode & S_IXUSR) ? "x" : "-");
            printf( (st_buf.st_mode & S_IRGRP) ? "r" : "-");
            printf( (st_buf.st_mode & S_IWGRP) ? "w" : "-");
            printf( (st_buf.st_mode & S_IXGRP) ? "x" : "-");
            printf( (st_buf.st_mode & S_IROTH) ? "r" : "-");
            printf( (st_buf.st_mode & S_IWOTH) ? "w" : "-");
            printf( (st_buf.st_mode & S_IXOTH) ? "x" : "-");
            printf("\t%30s", ctime(&st_buf.st_mtime)) ;
        }
        closedir(d);
    }
    else printf("there seems some error with your path?") ;

    int c;
    
    while (1) {
        c = kbget();
        if (c == KEY_ESCAPE) {
            break;
        } else if(c== KEY_BACKSPACE){
            clearsc();
            listd(c);
        }
        else if(c== KEY_LEFT){
            clearsc();
            listd(c);
        }
        else if(c== KEY_RIGHT){
            clearsc();
            listd(c);
        }
        else if(c== KEY_ENTER){
            listd(c);
        }
          else
        if (c == KEY_DOWN) {
            counter++;
            cursordownward(1);
        } else
        if (c == KEY_UP) {
            counter--;
            cursorupward(1);
        } else {
            putchar(c);
        }
    }

    //printf("\n");
    return 0;
}