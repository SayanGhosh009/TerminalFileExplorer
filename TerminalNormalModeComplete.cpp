#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <string>
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
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <time.h>
#include <bits/stdc++.h>
#include <vector>
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
#define KEY_BACKSPACE 127
#define KEY_HOME    104

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

map<int,string> mp;
map<int,string> outputmp;
vector<string> f_name;
vector<string> f_size;
vector<string> f_perm;
vector<string> f_modTime;
stack<string> BackdirNames;
stack<string> FwddirNames;
int counter=0;
int MaxRecords=0;
int lower_limit=0;
int upper_limit=20;
void printRecords(){
    //int last_row_no=counter-1;
    int curr_row=lower_limit;
    printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
    //curr_row=last_row_no - upper_limit + 1;
     while(curr_row<=upper_limit){
        printf("%s",outputmp[curr_row].c_str());
        curr_row++;
      }
    
}
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
            case 'h':
                c = KEY_HOME;
                break;
            default:
                c = KEY_BACKSPACE;
                break;
        }
    } else {
        c = KEY_BACKSPACE;
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
    int slide_count=0;
    struct stat stn_buf ;
    int status ;
    struct dirent *dir;
    DIR *d;
    string y;
    bool flag=true;
    if(c==KEY_ENTER){
        //printf("I am at enter");
        y=mp[counter];
        //printf("%s",y.c_str());
        if(is_dir(y.c_str())){

        d = opendir(y.c_str());
        BackdirNames.push(y);
        }
        else{
            flag=false;
            //fopen(y.c_str(),"r");
            string total = "xdg-open "+y+" &";
            system(total.c_str());
        }
    }
    else if(c==KEY_RIGHT){ //KEY LEFT ACTUALLY
        string x=BackdirNames.top();
        FwddirNames.push(x);
        BackdirNames.pop();
        x=BackdirNames.top();
        d=opendir(x.c_str());
        y=x;
    }
    else if(c==KEY_LEFT){ //KEY RIGHT ACTUALLY
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
    else if(c==KEY_HOME){
        while(!BackdirNames.empty()){
            BackdirNames.pop();
        }
        while(!FwddirNames.empty()){
            FwddirNames.pop();
        }
        d=opendir(rootDir().c_str()); // KEY h
        y=rootDir();
    }
    else if(c==127){ //Backspace
        BackdirNames.pop();
        string x=BackdirNames.top();
        d=opendir(x.c_str());
        y=x;
    }
    else
    { exit(0); }     
    if (d && flag)
    {
        printf("i am in");
        clearsc();
        mp.clear();
        outputmp.clear();
        counter=0;
        //printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &stn_buf) ;
            string d_name=dir->d_name;
            string z = y+ "/" +dir->d_name;
            mp[counter]= z;
            // printf("\033[0;33m %-30s%10ld\t",dir->d_name , stn_buf.st_size);
            // printf( (S_ISDIR(stn_buf.st_mode)) ? "d" : "-");
            // printf( (stn_buf.st_mode & S_IRUSR) ? "r" : "-");
            // printf( (stn_buf.st_mode & S_IWUSR) ? "w" : "-");
            // printf( (stn_buf.st_mode & S_IXUSR) ? "x" : "-");
            // printf( (stn_buf.st_mode & S_IRGRP) ? "r" : "-");
            // printf( (stn_buf.st_mode & S_IWGRP) ? "w" : "-");
            // printf( (stn_buf.st_mode & S_IXGRP) ? "x" : "-");
            // printf( (stn_buf.st_mode & S_IROTH) ? "r" : "-");
            // printf( (stn_buf.st_mode & S_IWOTH) ? "w" : "-");
            // printf( (stn_buf.st_mode & S_IXOTH) ? "x" : "-");
            // printf("\t%30s", ctime(&stn_buf.st_mtime)) ;
            string x;
            x = ((S_ISDIR(stn_buf.st_mode))  ? "d" : "-");
            x=x+((stn_buf.st_mode & S_IRUSR) ? "r" : "-");
            x=x+((stn_buf.st_mode & S_IWUSR) ? "w" : "-");
            x=x+((stn_buf.st_mode & S_IXUSR) ? "x" : "-");
            x=x+((stn_buf.st_mode & S_IRGRP) ? "r" : "-");
            x=x+((stn_buf.st_mode & S_IWGRP) ? "w" : "-");
            x=x+((stn_buf.st_mode & S_IXGRP) ? "x" : "-");
            x=x+((stn_buf.st_mode & S_IROTH) ? "r" : "-");
            x=x+((stn_buf.st_mode & S_IWOTH) ? "w" : "-");
            x=x+((stn_buf.st_mode & S_IXOTH) ? "x" : "-");
            string res;
            int sz=stn_buf.st_size;
            string sz_s = std::to_string(sz);
            //usleep(500000);
            // string time= ctime(&stn_buf.st_mtime);
            // if(time.empty() || time =="")
            //     time= " ";
            // string t = &stn_buf.st_mtime;
            // if(t.empty() || t.equals("(null)") || t.size()==0)
            //     continue;
            res = d_name+ "\t"+ sz_s + "\t"+ x+ "\t" + ctime(&stn_buf.st_mtime);
            outputmp[counter]=res;
            counter++;
            //return;
            // f_name.push_back(d_name);
            // f_size.push_back(sz_s);
            // f_perm.push_back(x);
            // f_modTime.push_back(ctime(&stn_buf.st_mtime));
        }
        MaxRecords=counter;
        upper_limit=MaxRecords-1;
        lower_limit=upper_limit-10;//Total 11 records
        if(lower_limit<0)
            lower_limit=0;
        printRecords();
        closedir(d);
    }
    
}
int main(int argc, char **argv)
{
    struct stat st_buf ;
    int status ;
    DIR *d;
    struct dirent *dir;
    string s=GetCurrentWorkingDir();
    BackdirNames.push(s);
    d = opendir(s.c_str()) ;
    clearsc();
    if (d)
    {
        
        //printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &st_buf) ;
            string z = s+"/"+dir->d_name;
            string d_name=dir->d_name;
            mp[counter]= z;
            string x;
            x = ((S_ISDIR(st_buf.st_mode))  ? "d" : "-");
            x=x+((st_buf.st_mode & S_IRUSR) ? "r" : "-");
            x=x+((st_buf.st_mode & S_IWUSR) ? "w" : "-");
            x=x+((st_buf.st_mode & S_IXUSR) ? "x" : "-");
            x=x+((st_buf.st_mode & S_IRGRP) ? "r" : "-");
            x=x+((st_buf.st_mode & S_IWGRP) ? "w" : "-");
            x=x+((st_buf.st_mode & S_IXGRP) ? "x" : "-");
            x=x+((st_buf.st_mode & S_IROTH) ? "r" : "-");
            x=x+((st_buf.st_mode & S_IWOTH) ? "w" : "-");
            x=x+((st_buf.st_mode & S_IXOTH) ? "x" : "-");
            string res;
            int sz=st_buf.st_size;
            string sz_s = std::to_string(sz);
            res = d_name+ "\t"+ sz_s + "\t"+ x+ "\t" + ctime(&st_buf.st_mtime);
            //printf("%d  %s",counter,res.c_str());
            outputmp[counter]=res;
            counter++;

            // printf("\033[0;33m %-30s%10ld\t",dir->d_name , st_buf.st_size);
            // printf( (S_ISDIR(st_buf.st_mode))  ? "d" : "-");
            // printf( (st_buf.st_mode & S_IRUSR) ? "r" : "-");
            // printf( (st_buf.st_mode & S_IWUSR) ? "w" : "-");
            // printf( (st_buf.st_mode & S_IXUSR) ? "x" : "-");
            // printf( (st_buf.st_mode & S_IRGRP) ? "r" : "-");
            // printf( (st_buf.st_mode & S_IWGRP) ? "w" : "-");
            // printf( (st_buf.st_mode & S_IXGRP) ? "x" : "-");
            // printf( (st_buf.st_mode & S_IROTH) ? "r" : "-");
            // printf( (st_buf.st_mode & S_IWOTH) ? "w" : "-");
            // printf( (st_buf.st_mode & S_IXOTH) ? "x" : "-");
            // printf("\t%30s", ctime(&st_buf.st_mtime)) ;

        }
        MaxRecords=counter;
        upper_limit=MaxRecords-1;
        lower_limit=upper_limit-10;//Total 11 records
        printRecords();
        //printf("%d          ",counter);
        closedir(d);
    }
    else printf("there seems some error with your path?") ;

    int c;
    while (1) {
        c = kbget();
        if (c == KEY_ESCAPE) {
            break;
        } 
        else if(c== KEY_LEFT){ //KEY RIGHT
            if(FwddirNames.size()==0)
                continue;
            clearsc();
            listd(c);
        }
        else if(c== KEY_RIGHT){ // KEY LEFT
            if(BackdirNames.size()<=1)
                continue;
            clearsc();
            listd(c);
        }
        else if(c== KEY_ENTER){
            listd(c);
        }
          else
        if (c == KEY_DOWN) {
            if(counter==MaxRecords)
                continue;
            if(counter==upper_limit){
                
                if(upper_limit!=MaxRecords){

                    clearsc();
                    lower_limit++;
                    upper_limit++;
                    printRecords();
                    printf("\033[%d;1H",upper_limit-lower_limit+2);
                    counter++;
                }
            }
            else{
            counter++;
            cursordownward(1);
           }
        } else
        if (c == KEY_UP) {
            //printf("%d counter",counter);
            if(counter==0)
            {
                counter=0;
                continue;
            }
            if(counter==lower_limit)
            {
                if(lower_limit!=0)
                {
                    clearsc();
                    lower_limit--;
                    upper_limit--;
                    printRecords();
                    printf("\033[2;1H");
                    counter--;
                }
            }
            else{
            //   printf("%d          ",counter);
            counter--;
            cursorupward(1);
            }
        } else if(c==KEY_HOME){
           clearsc();
           listd(c);
        }
        else
        {
            //Backspace
            if(BackdirNames.size()<=1)
                continue;
            clearsc();
            listd(127);
        }
    }

    //printf("\n");
    return 0;
}