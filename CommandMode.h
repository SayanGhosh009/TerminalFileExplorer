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
using namespace std;

#ifndef INC_OPENDIRECTORY_H
#define INC_OPENDIRECTORY_H

string GetCurrentWorkingDir(void);
void copyFilesDir(string source,string des,bool flag,int type);
string removePunct(string des);
vector<string> split_string(string input_string);
void makeDir(string source,string des);
void makeFile(string source,string des);
int removedirectoryrecursively(const char *dirname);
void removeFile(const char* des);
void searchFileName(const char* dirname,string searchWord);
void snapshot(const char* dirname,string d);
void renameFile(string o,string n);
// void copyDir(const char* dirname,const char* desname);
// void copyFilesDir(string source,string des,bool flag,int type);
#endif
