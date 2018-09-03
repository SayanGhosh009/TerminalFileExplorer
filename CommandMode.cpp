// ROLL NUMBER : 2018202002
// NAME : SAYAN GHOSH


#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream>
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
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include <ctype.h>
#include <fstream>
#include "CommandMode.h"
using namespace std;


void copyFilesDir(string source,string des,bool flag,int type);

string GetCurrentWorkingDir(void) {
  char buff[FILENAME_MAX];
  getcwd( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

string removePunct(string des){
  string str=des;
  for (int i = 0, len = str.size(); i < len; i++)
    {
        // check whether parsing character is punctuation or not
        if (ispunct(str[i]))
        {
            str.erase(i--, 1);
            len = str.size();
        }
    }
    return str;
}
vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

void makeDir(string source,string des){
    string DirName;
    if(des!="")
    DirName = GetCurrentWorkingDir() + "/" + des + "/" + source ;
    else
    DirName = GetCurrentWorkingDir() + "/" + source ; 
    mkdir(DirName.c_str(),0777);
}

void makeFile(string source,string des){
    string fileName;
    if(des!="")
    fileName = GetCurrentWorkingDir() + "/" + des + "/" + source ;
    else
    fileName = GetCurrentWorkingDir() + "/" + source ;  
    std::ofstream file { fileName };
}

int removedirectoryrecursively(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) { 
        perror("Error opendir()");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                removedirectoryrecursively(path);
            }

            remove(path);
 
        }

    }
    closedir(dir);
    rmdir(dirname);

    return 1;
}

void removeFile(const char* des){
remove(des);
}

void snapshot(const char* dirname,string d){
  char* x= strdup(dirname);
  ofstream out( d );
  DIR *dir;
    struct dirent *entry;
    //dir = opendir(dirname);
    char path[PATH_MAX];
    stack<char*> s;
    s.push(x);
    //printf("%ld size is", s.size()); 
    while(!s.empty()){ 
        //printf("%ld size is", s.size()); 
      dir=opendir(s.top());
      if (dir == NULL) {
        perror("Error opendir()");
        return ;
        }
        x=s.top();
      //dir=opendir(rootDir().c_str());
      //printf("\n%s :\n",s.top());
      out << endl << x << " :" << endl;
      s.pop();
      //printf("%ld size is", s.size());
    while ((entry = readdir(dir)) != NULL) {
      //printf("i am in1\n");
      if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
          string currName = entry->d_name;
          //printf("%s\t",entry->d_name);
          out << currName << "\t";
          // printf("i am in2\n");
          if (entry->d_type == DT_DIR) {
            string dName = x;
            string fName = entry->d_name;
            string whole=dName+"/"+fName;
            snprintf(path, (size_t) PATH_MAX, "%s/%s", x, entry->d_name);
            const char* f=whole.c_str();
            char* newf = strdup(f);
            //printf("\n%s\n",newf);
            // printf("%ld",s.size());
            s.push(newf);
            // printf("%ld",s.size());
            // printf("\n%s :\n",s.top());
            }
          // printf("%s\n",path);
        }
       }
  }
   closedir(dir);
}

void renameFile(string o,string n){
  rename(o.c_str(),n.c_str());
}


