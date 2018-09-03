// ROLL NUMBER : 2018202002
// NAME : SAYAN GHOSH

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
#include "CommandMode.h"
//#include "soufile.h"

using namespace std;

#define cursorupward(x) printf("\033[%dA", (x))
#define cursordownward(x) printf("\033[%dB", (x))
#define clearsc() printf("\033[H\033[J")
#define KEY_ESCAPE      0x001b
#define KEY_ENTER       0x0A
#define KEY_UP          0x0105
#define KEY_DOWN        0x0106
#define KEY_LEFT        0x0107
#define KEY_RIGHT       0x0108
#define KEY_BACKSPACE   127
#define KEY_HOME        104
#define KEY_COLON       58

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);

map<int,string> mp;
map<int,string> outputmp;
map<int,string> searchRes;
stack<string> BackdirNames;
stack<string> FwddirNames;

int counter=0;
int MaxRecords=0;
int lower_limit=0;
int upper_limit=10;
int LastCounter=0;
bool searchLeft = false;
bool searchEnter = false;
bool gotoBackspace = false;
bool noBackspace = false;
string mainsource_path;

void printRecords();
void printSearchRecords();
void goToPath(const char* dirname,bool flag);
bool is_dir(const char* path);
void copyDir(const char* dirname,const char* desname);

string CommandModeDisplay(){
    //gotostmt:
    printf("\e[30;1H");
    printf("\e[2K");
    vector<string> arr;
    bool x;
    while(1){
      arr.clear();
      string s;
      getline (cin, s);
      vector<string> arr_temp = split_string(s);
      int n=arr_temp.size();
      int outer_flag=1;
      for (int i = 0; i < n; i++) {
          string arr_item = arr_temp[i];
          arr.push_back(arr_item);
      }

      if(arr[0]=="create_dir")
      {
        string des;
        string source;
        char x  = arr[2][0];
        if(arr[2]==".")
        des="";
        else if(x!='~')
        {
            des=arr[2];
        }
        else
           des=arr[2].substr(2);
        x = arr[1][0];
        if(x!='~')
        {
            source=arr[1];
        }
        else
           source=arr[1].substr(2); 
        makeDir(source,des);
        goToPath(BackdirNames.top().c_str(),false);  
        return arr[0]; 
      }
      else if(arr[0]=="create_file"){//Just filename.txt or ~/foldername1/foldername2/filename.txt
        string des;
        string source;
        char x  = arr[2][0];
        if(arr[2]==".")
        des="";
        else if(x!='~')
        {
            des=arr[2];
        }
        else
           des=arr[2].substr(2);
        x = arr[1][0];
        if(x!='~')
        {
            source=arr[1];
        }
        else
           source=arr[1].substr(2); 
        makeFile(source,des);
        goToPath(BackdirNames.top().c_str(),false); 
        return arr[0]; 
      }
      else if(arr[0]=="delete_dir"){
            string des=arr[1];  
            char x = arr[1][0];
            if(x=='~')
            {
                des=GetCurrentWorkingDir() + "/" + arr[1].substr(2);
            }
            else if(x=='/' && des.length()==1) //CAREFUL
            {
                des=GetCurrentWorkingDir();
            }
            else
                des=arr[1];
            removedirectoryrecursively(des.c_str());
            goToPath(BackdirNames.top().c_str(),false);  
            return arr[0]; 
      }
      else if(arr[0]=="delete_file"){
            string des;  
            char x = arr[1][0];
            if(x=='~')
            {
                des=GetCurrentWorkingDir() + "/" + arr[1].substr(2);
            }
            else
                des=arr[1];
            removeFile(des.c_str());
            goToPath(BackdirNames.top().c_str(),false);  
            return arr[0]; 
      }
      else if(arr[0]=="search"){ // search with only filename.txt and press Left for back
            clearsc();
            mp.clear();
            outputmp.clear();
            string searchString;
            searchString=arr[1];
            counter=0;
            searchFileName(BackdirNames.top().c_str() ,searchString);
            searchLeft=true;
            searchEnter=true;
            MaxRecords=counter;
            upper_limit=MaxRecords-1;
            lower_limit=upper_limit-10;//Total 11 records
            if(lower_limit<0)
                lower_limit=0;
            printSearchRecords();
            return arr[0];
      }
      else if(arr[0]=="goto"){
            noBackspace=false;
            string des;
            des=arr[1];
            char x = arr[1][0];
            if(x=='~')
            {
                des=GetCurrentWorkingDir() + "/" + arr[1].substr(2);
            }
            else if(x=='/' && des.length()==1)
            {
                des=GetCurrentWorkingDir();
                noBackspace=true;
            }
            else
                des=arr[1];
            goToPath(des.c_str(),true);
            gotoBackspace=true;  
            return arr[0];
      }
      else if(arr[0]=="snapshot"){
        string sourceDir=arr[1];
        char x = arr[1][0];
        if(x=='.')
            sourceDir=GetCurrentWorkingDir();
        else if(x=='~')
        {
            sourceDir=GetCurrentWorkingDir() + "/" + arr[1].substr(2);
        }
        else if(x=='/' && sourceDir.length()==1)
        {
            sourceDir=GetCurrentWorkingDir();
        }
        else
            sourceDir=GetCurrentWorkingDir()+"/"+arr[1];
        string dumpFile;
        dumpFile=arr[2];
        x = arr[2][0];
        if(x=='~')
        {
            dumpFile=GetCurrentWorkingDir() + "/" + arr[2].substr(2);
        }
        else
            dumpFile=GetCurrentWorkingDir()+"/"+arr[2];
        std::ofstream file { dumpFile };
        snapshot(sourceDir.c_str(),dumpFile); 
      }
      else if(arr[0]=="rename"){//Give full path for renaming to 
        string oldF=arr[1];
        char x = oldF[0];
        if(x=='~'){
            oldF=GetCurrentWorkingDir() + "/" + oldF.substr(2);
        }
        string newF=arr[2];
        char y = newF[0];
        if(y=='~'){
            newF=GetCurrentWorkingDir() + "/" + newF.substr(2);
        }
        renameFile(oldF,newF);
        goToPath(BackdirNames.top().c_str(),false);  
        return arr[0]; 
      }
      else if(arr[0]=="move"){ 
        int len=arr.size();
        bool flag=true;
        for(int i=1;i<len-1;i++){ 
            bool flag=true;
            s = arr[i];
            char x=arr[i][0];
            //char x =arr[len-i][0];
            string mainsource=arr[i].substr(2);
            mainsource_path = GetCurrentWorkingDir() + "/" + mainsource;
            string source=arr[i].substr(arr[i].rfind("/")+1);
            string full_path = GetCurrentWorkingDir() + "/" +source;
            if (!is_dir(full_path.c_str()))
                flag=true;//File
            else
                flag=false;//Dir
            string des=arr[len-1].substr(2);
            x=arr[len-1][0];
            if(x!='~')
                des=arr[len-1];
            string newdes = GetCurrentWorkingDir () + "/" + des + "/" + source;
            int type=0;
            if(is_dir(full_path.c_str())){
            copyDir(mainsource_path.c_str(),newdes.c_str());
            removedirectoryrecursively(full_path.c_str());
            }
            else
            {
                copyFilesDir(source,des,flag,type);
                remove(full_path.c_str());
            }
            printf("\e[1;1H");
            goToPath(BackdirNames.top().c_str(),false); 
        }
        return arr[0];
      }
      else if(arr[0]=="copy"){ //Works on ~/ only
        int len=arr.size();
        for(int i=1;i<arr.size()-1;i++){
            bool flag=true;
            s = arr[i];
            char x=arr[i][0];
            //char x =arr[len-i][0];
            string mainsource=arr[i].substr(2);
            mainsource_path = GetCurrentWorkingDir() + "/" + mainsource;
            string source=arr[i].substr(arr[i].find_last_of("/\\")+1);
            string full_path = GetCurrentWorkingDir() + "/" +source;
            if (source.find('.')!=std::string::npos)
                flag=true;//File
            else
                flag=false;//Dir
            string des=arr[len-1].substr(2);
            x=arr[len-1][0];
            if(x!='~')
                des=arr[len-1];
            int type=0;
            copyFilesDir(source,des,flag,type);
            printf("\e[1;1H");
            goToPath(BackdirNames.top().c_str(),false);    
         }
         return arr[0]; 
      }
      else if(arr[0]=="esc"){
        printf("\e[1;1H");
        return arr[0];
        break;
      }
      else if(arr[0]=="exit"){
        return arr[0];
        break;
      }
   }
}
void copyDir(const char* dirname,const char* desname){

    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    char despath[PATH_MAX];
    // char* dirnameR=(dirname);
    // char* 
    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
    }
    //Get the Permissions here
    if(mkdir(desname,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1){
      std::cout << "cannot create sessionnamefolder error:" << strerror(errno) << std::endl;
    }
     dir = opendir(dirname);
    if (dir == NULL) { 
        perror("Error opendir()");
        return ;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(despath, (size_t)PATH_MAX, "%s/%s" , desname,entry->d_name);
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            printf("%s/%s\n",dirname,entry->d_name);
            printf("%s/%s\n",desname,entry->d_name);
            if (entry->d_type == DT_DIR) {
                copyDir(path,despath);
            }
            else
            {
              string source = path;
              string des = despath;
              bool flag=true;
              int type=1;
              copyFilesDir(source,des,flag,type);
          }

      }
    }
    closedir(dir);
}

void moveFilesDir(const char* source,const char* des,bool flag){
  if(flag==true){
    copyDir(des,source);
    removedirectoryrecursively(source);
   }
   else
   {
    string o=source;
    string y=des;
    copyFilesDir(o,y,1,true);
    removeFile(source);
   }
 }

void copyFilesDir(string source,string des,bool flag,int type){
  DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    struct stat* sb;
    string s_path,dst_path;
    if(type==0){
    s_path = mainsource_path;
    dst_path = GetCurrentWorkingDir()+ "/" + des + "/" + source;
    }
    else
    {
      s_path =  mainsource_path;
      dst_path =  des;
    }
    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return ;
    }
    if(flag==true){
      makeFile(source,des);
      const char* src_path = s_path.c_str();
      ifstream inFile(s_path,ios::binary);
      ofstream outFile(dst_path, ios::binary);
      outFile << inFile.rdbuf();
      return;
     }
     else
     {
      copyDir(mainsource_path.c_str(),dst_path.c_str());
     }
}
void printRecords(){

    int curr_row=lower_limit;
    printf("\033[1;31m%-30s %9s %12s %27s %32s\n" , "file name", "file size(B)",  "permission","User-Group ID" , "last_modified") ;
    //curr_row=last_row_no - upper_limit + 1;
     while(curr_row<=upper_limit){
        printf("\033[0;33m %s",outputmp[curr_row].c_str());
        curr_row++;
      }
    printf("\e[29;1H");
    printf("-----------------------------------------------------------COMMAND MODE-----------------------------------------------------------------------");
    printf("\033[%d;1H",upper_limit-lower_limit+3);
}

void printSearchRecords(){

    int curr_row=lower_limit;
    printf(" %-50s \n" , "File OR Folder Path(s)") ;
     while(curr_row<=upper_limit){
        printf("\033[0;33m %s\n",searchRes[curr_row].c_str());
        curr_row++;
      }
    printf("\e[29;1H");
    printf("-----------------------------------------------------------SEARCH RECORDS----------------------------------------------------------------------");
    printf("\033[%d;1H",upper_limit-lower_limit+3);
}

void searchFileName(const char* dirname,string searchWord){
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            string currName = entry->d_name;
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            //printf("%s\n",path);
            if(currName==searchWord)
            {
                //printf("%s\n",path);
                string res=path;
                searchRes[counter++] = res;
            }
            if (entry->d_type == DT_DIR) {
            searchFileName(path,searchWord);
            }
        }
    }
    closedir(dir);
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

string UserAndGroup()
{
    register struct passwd *pw;
    register uid_t uid;
    int c;

    uid = geteuid ();
    pw = getpwuid (uid);
    register gid_t gid = getgid();
    register struct group *grp;
    grp = getgrgid(gid);
    string s;
    if (pw)
    {
        s = pw->pw_name; 
        s = s + " "+ grp->gr_name;
    }
    return s;
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
            case 'D':
                c = KEY_LEFT;
                break;
            case 'C':
                c = KEY_RIGHT;
                break;
            case 'h':
                c = KEY_HOME;
                break;
            case ':':
                c = KEY_COLON;
                break;
            default:
                c = KEY_BACKSPACE;
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

void goToPath(const char* dirname,bool flag){ //For GOTO MODE
        string y=dirname;
        if(flag==true)
        BackdirNames.push(y);
        struct stat stn_buf ;
        struct dirent *dir;
        DIR *d;
        d=opendir(dirname);
        clearsc();
        mp.clear();
        outputmp.clear();
        counter=0;
        char path[PATH_MAX];
        //printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &stn_buf) ;
            string d_name=dir->d_name;
            string z = y+ "/" +dir->d_name;
            mp[counter]= z;
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
            int time= stn_buf.st_mtime;
            if(time > 0)
            {
            snprintf(path, (size_t) PATH_MAX, "%-30s", d_name.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%10s", sz_s.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%15s", x.c_str());
            res=res+path;
            res= res+ "\t\t" + UserAndGroup();
            res=res + "\t\t\t" + ctime(&stn_buf.st_mtime);
            }
            else //If time is somehow null, but will get updated above if u visit the folder again.
            {
            snprintf(path, (size_t) PATH_MAX, "%-30s", d_name.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%10s", sz_s.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%15s", x.c_str());
            res=res+path ;
            res=res+ "\t\t" + UserAndGroup() +"\n";
            }
            outputmp[counter]=res;
            counter++;
        }
        MaxRecords=counter;
        upper_limit=MaxRecords-1;
        lower_limit=upper_limit-10;//Total 11 records
        if(lower_limit<0)
            lower_limit=0;
        printRecords();
        closedir(d);
 }

void ListFilesAndDirectories(int c){
    struct stat stn_buf ;
    int status ;
    struct dirent *dir;
    DIR *d;
    string y;
    bool flag=true;
    if(c==KEY_ENTER && searchEnter==false){
        if(BackdirNames.size()==1 && mp[counter]==GetCurrentWorkingDir()+"/..")
        //printf("%s",mp[counter].c_str());
        return;
        y=mp[counter];
        //printf("%s",y.c_str());
        if(is_dir(y.c_str())){
        d = opendir(y.c_str());
        BackdirNames.push(y);
        }
        else{
            flag=false;
            // //fopen(y.c_str(),"r");
            // string total = "xdg-open "+y+" &";
            // system(total.c_str());
            int pid=fork();
            if(pid==0){
                execl("/usr/bin/xdg-open","xdg-open",y.c_str(),NULL);
                exit(1);
            }
        }
    }
    else if(c==KEY_ENTER && searchEnter==true){
        struct stat s;
        y=searchRes[counter];
        if(!is_dir(y.c_str())){
        flag=false;
        // string total = "xdg-open "+y+" &";
        // system(total.c_str());
        int pid=fork();
            if(pid==0){
                execl("/usr/bin/xdg-open","xdg-open",y.c_str(),NULL);
                exit(1);
            }
       }
       else{
        d=opendir(y.c_str());
        BackdirNames.push(y);
        searchEnter=false;
        searchLeft=true;
       }
    }
    else if(c==KEY_RIGHT){ 
        string x=FwddirNames.top();
        FwddirNames.pop();
        BackdirNames.push(x);
        d=opendir(x.c_str());
        y=x;
    }
    else if(c==KEY_LEFT && !searchLeft){
        string x=BackdirNames.top();
        FwddirNames.push(x);
        BackdirNames.pop();
        x=BackdirNames.top();
        d=opendir(x.c_str());
        y=x;
    }
    else if(c==KEY_LEFT && searchLeft){
        string x=BackdirNames.top();
        FwddirNames.push(x);
        BackdirNames.pop();
        d=opendir(x.c_str());
        y=x;
        searchEnter=false;
    }
    else if(c==KEY_BACKSPACE){
        BackdirNames.pop();
        string x=BackdirNames.top();
        d=opendir(x.c_str());
        y=x;
        
    }
    else if(c==KEY_HOME){
        while(!BackdirNames.empty()){
            BackdirNames.pop();
        }
        while(!FwddirNames.empty()){
            FwddirNames.pop();
        }
        d=opendir(GetCurrentWorkingDir().c_str()); // KEY h
        y=GetCurrentWorkingDir();
    }    
    if (d && flag)
    {
        //printf("i am in");
        clearsc();
        mp.clear();
        outputmp.clear();
        counter=0;
        char path[PATH_MAX];
        //printf(" %-30s %10s %15s %30s\n" , "file name", "file_size",  "permission", "last_modified") ;
        while ((dir = readdir(d)) != NULL)
        {

            stat(dir->d_name , &stn_buf) ;
            string d_name=dir->d_name;
            string z = y+ "/" +dir->d_name;
            mp[counter]= z;
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
            int time= stn_buf.st_mtime;
            if(time > 0)
            {
            snprintf(path, (size_t) PATH_MAX, "%-30s", d_name.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%10s", sz_s.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%15s", x.c_str());
            res=res+path;
            res= res+ "\t\t" + UserAndGroup();
            res=res + "\t\t\t" + ctime(&stn_buf.st_mtime);
            }
            else //If time is somehow null, but will get updated above if u visit the folder again.
            {
            snprintf(path, (size_t) PATH_MAX, "%-30s", d_name.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%10s", sz_s.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%15s", x.c_str());
            res=res+path ;
            res=res+ "\t\t" + UserAndGroup() +"\n";
            }
            outputmp[counter]=res;
            counter++;
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
        char path[PATH_MAX];
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
            snprintf(path, (size_t) PATH_MAX, "%-30s", d_name.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%10s", sz_s.c_str());
            res=res+path;
            snprintf(path, (size_t) PATH_MAX, "%15s", x.c_str());
            res= res+ path;
            res= res+ "\t\t" + UserAndGroup();
            res=res + "\t\t\t" + ctime(&st_buf.st_mtime);
            outputmp[counter]=res;
            counter++;
        }
        MaxRecords=counter;
        upper_limit=MaxRecords-1;
        lower_limit=upper_limit-10;//Total 11 records
        if(lower_limit<0)
            lower_limit=0;
        printRecords();
        closedir(d);
    }
    else printf("there seems some error with your path?") ;
    escstmt:
    struct termios oldt,newt;//oldt = Command newt = Normal
    tcgetattr(STDIN_FILENO,&oldt);
    newt=oldt;
    newt.c_lflag = newt.c_lflag & ~(ECHO | ICANON);
    newt.c_lflag = 1;
    int c;
    while (1) {
        //gotostmt:
        c = kbget();
        if (c == KEY_ESCAPE) {
            break;
        } 
        else if(c== KEY_LEFT && searchLeft == false){ 
            if(BackdirNames.size()<=1)// && !searchLeft)
                continue;
            clearsc();
            ListFilesAndDirectories(c);
            
        }
        else if(c== KEY_LEFT && searchLeft == true && searchEnter == false){ 
            if(BackdirNames.size()<=1 && !searchLeft)
                continue;
            //counter=0;
            int pos=0;
            // for(auto it=mp.begin();it!=mp.end();it++)
            // {
            //     string substr=it->second;
            //     if(substr.find("..")!=std::string::npos){
            //         pos=counter;
            //         break;
            //     }
            //     counter++;
            // }
            string y = BackdirNames.top();
            string z = y.substr(0,y.rfind("/"));
            clearsc();
            //searchLeft=false;
            goToPath(z.c_str(),true);
        }
        else if(c== KEY_LEFT && searchLeft == true && searchEnter == true){ 
            string val=BackdirNames.top();
            //BackdirNames.pop();
            //goToPath(val.c_str(),true);
            //searchEnter=false;
            //printRecods();
            ListFilesAndDirectories(c);
        }
        else if(c== KEY_RIGHT){
            if(FwddirNames.size()==0)
                continue;
            clearsc();
            ListFilesAndDirectories(c);
        }
        else if(c== KEY_ENTER){
            if(counter==MaxRecords)
                continue;
            gotoBackspace=false;
            ListFilesAndDirectories(c);
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
            counter--;
            cursorupward(1);
            }
        } else if(c==KEY_HOME){
           clearsc();
           ListFilesAndDirectories(c);
        }
        else if(c==KEY_BACKSPACE && gotoBackspace==false){
            if(BackdirNames.size()<=1)
                continue;
            clearsc();
            ListFilesAndDirectories(127);
        }
        else if(c==KEY_BACKSPACE && gotoBackspace==true){
            if(noBackspace==true)
                continue;
            counter=0;
            int pos=0;
            for(auto it=mp.begin();it!=mp.end();it++)
            {
                string substr=it->second;
                if(substr.find("..")!=std::string::npos){
                    pos=counter;
                    break;
                }
                counter++;
            }
            clearsc();
            BackdirNames.pop();
            goToPath(mp[counter].c_str(),true);//Check Dir name completely
        }
        else if(c==KEY_COLON)
        {
             searchLeft=false;
             printf("\e[30;1H");
             printf("\e[2K");
             break;
             
        }
    }
    tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
    gotostmt:
    string CmdCheck = CommandModeDisplay();
    if(CmdCheck=="esc")
    {
         upper_limit=MaxRecords-1;
         lower_limit=upper_limit-10;
         if(lower_limit<0)
            lower_limit=0;
         counter=MaxRecords;
         if(searchEnter==false)
         printRecords();
         else
         printSearchRecords();
         goto escstmt;
    }
    else if(CmdCheck=="goto" || CmdCheck == "copy" || CmdCheck =="move" || CmdCheck=="search" || CmdCheck=="create_file" 
                || CmdCheck == "create_dir" || CmdCheck == "delete_file" || CmdCheck == "delete_dir" || CmdCheck == "rename" ){
        goto gotostmt;
    }
    else
    return 0;
}
    
