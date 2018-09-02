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
//#include "soufile.h"
#include "CommandMode.h"
using namespace std;

// vector<string> tokens;
// map<string,string> searchRes;

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
// std::string rootDir(void){
//     return "/home/sayan";
// }
// std::string GetCurrentWorkingDir(void) {
//   char buff[FILENAME_MAX];
//   getcwd( buff, FILENAME_MAX );
//   std::string current_working_dir(buff);
//   return current_working_dir;
// }

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

            /*
             * Here, the actual deletion must be done.  Beacuse this is
             * quite a dangerous thing to do, and this program is not very
             * well tested, we are just printing as if we are deleting.
             */
            remove(path);
            printf("Deleting: %s\n", path);
            /*
             * When you are finished testing this and feel you are ready to do the real
             * deleting, use this: remove*STUB*(path);
             * (see "man 3 remove")
             * Please note that I DONT TAKE RESPONSIBILITY for data you delete with this!
             */
        }

    }
    closedir(dir);
    rmdir(dirname);
    /*
     * Now the directory is emtpy, finally delete the directory itself. (Just
     * printing here, see above) 
     */
    printf("Deleting: %s\n", dirname);

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
    	printf("\n%s :\n",s.top());
    	out << endl << x << " :" << endl;
     	s.pop();
     	//printf("%ld size is", s.size());
    while ((entry = readdir(dir)) != NULL) {
    	//printf("i am in1\n");
    	if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
	        string currName = entry->d_name;
	        printf("%s\t",entry->d_name);
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

void moveFilesDir(const char* source,const char* des){
struct stat statbuf_src, statbuf_dest;
  char *src, *dest, *new_src, *new_dest;
  const char *current_directory;

  // if (argc != 3) {
  //   fprintf(stderr, "usage: %s src dest\n", argv[0]);
  //   exit(EXIT_FAILURE);
  // }
  // work on copy
  src = (char *)malloc(strlen(source) + 1);
  dest =(char *)malloc(strlen(des) + 1);
  strcpy(src, source);
  strcpy(dest, des);

  stat(src, &statbuf_src);
  stat(dest, &statbuf_dest);

  // there are many more, of course
  // printf("\"%s\" is a ", src);
  // if (S_ISREG(statbuf_src.st_mode)) {
  //   puts("a regular file");
  // }
  // if (S_ISDIR(statbuf_src.st_mode)) {
  //   puts("a directory");
  // }

  // printf("\"%s\" is a ", dest);
  // if (S_ISREG(statbuf_dest.st_mode)) {
  //   puts("a regular file");
  // }
  // if (S_ISDIR(statbuf_dest.st_mode)) {
  //   puts("a directory");
  // }

  // if (SAME_INODE(statbuf_dest, statbuf_src)) {
  //   printf("%s and %s are the identical\n", src, dest);
  // }
  // if that is not set you have to do it by hand:
  // climb up the tree, concatenating names until the inodes are the same
  current_directory = GetCurrentWorkingDir().c_str();
  //printf("current directory is \"%s\"\n", current_directory);

  // I'm pretty sure it can be done in a much more elegant way
  new_src = (char *)malloc(strlen(src) + 1 + strlen(current_directory) + 1);
  strcpy(new_src,current_directory);
  strcat(new_src,"/");
  strcat(new_src,src);
  //printf("new_src = %s\n",new_src);

  new_dest = (char *)malloc(strlen(dest) + 1 + strlen(current_directory) + 1 + strlen(src) + 1);
  strcpy(new_dest,current_directory);
  strcat(new_dest,"/");
  strcat(new_dest,dest);
  strcat(new_dest,"/");
  strcat(new_dest,src);
  //printf("new_dest = %s\n",new_dest);

  if(rename(new_src,new_dest) != 0){
    fprintf(stderr,"rename failed with error %s\n",strerror(errno));
  }

  free(new_src);
  free(new_dest);
  free(src);
  free(dest);
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
    dir = opendir(dirname);
    if (dir == NULL) { 
        perror("Error opendir()");
        return ;
    }
    //Get the Permissions here
    if(mkdir(desname,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1){
    	std::cout << "cannot create sessionnamefolder error:" << strerror(errno) << std::endl;
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
void copyFilesDir(string source,string des,bool flag,int type){
	DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];
    struct stat* sb;
    string s_path,dst_path;
    if(type==0){
    s_path = GetCurrentWorkingDir()+ "/" + source;
    dst_path = GetCurrentWorkingDir()+ "/" + des + "/" + source;
	}
	else
	{
	  s_path =  source;
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
     	copyDir(s_path.c_str(),dst_path.c_str());
     }
}

// int main(){
//       string s;
//       getline (cin, s);
//       vector<string> arr_temp = split_string(s);
//       int n=arr_temp.size();
//       int outer_flag=1;
//       vector<string> arr;
//       for (int i = 0; i < n; i++) {
//           string arr_item = arr_temp[i];

//           arr.push_back(arr_item);
//       }

//       if(arr[0]=="create_dir")
//       {
//         string des;
//         if(arr[2]==".")
//             des="";
//         else
//         des=arr[2].substr(2);
//         makeDir(arr[1],des);
//       }
//       else if(arr[0]=="create_file"){
//         string des;
//         if(arr[2]==".")
//             des="";
//         else
//         des=arr[2].substr(2);
//         makeFile(arr[1],des);
//       }
//       else if(arr[0]=="delete_dir"){
//             string des;
//             if(arr[1]==".")
//                 des="";
//             else
//                 des=arr[1].substr(2);
//             removedirectoryrecursively(des.c_str());
//       }
//       else if(arr[0]=="delete_file"){
//             string des;
//             if(arr[1]==".")
//                 des="";
//             else
//                 des=arr[1].substr(2);
//             removeFile(des.c_str());
//       }
//       else if(arr[0]=="search"){
//             string searchString;
//             searchString=arr[1];
//             searchFileName( GetCurrentWorkingDir().c_str() ,searchString);
//       }
//       else if(arr[0]=="goto"){
//             string des;
//             des=arr[1];
//             // if(arr[1][0]=="~".c_str())
//             // {
//             //  des=GetCurrentWorkingDir() + "/" + arr[1].substr(2);
//             // }
//             //goToPath(des);
//       }
//       else if(arr[0]=="snapshot"){
//         string sourceDir=arr[1];
//         string dumpFile;
//         dumpFile=arr[2];
//         std::ofstream file { dumpFile };
//         snapshot(sourceDir.c_str(),dumpFile);
//       }
//       else if(arr[0]=="rename"){
//         string oldF=arr[1];
//         char x = oldF[0];
//         if(x=='~'){
//             oldF=GetCurrentWorkingDir() + "/" + oldF.substr(2);
//         }
//         string newF=arr[2];
//         char y = newF[0];
//         if(y=='~'){
//             newF=GetCurrentWorkingDir() + "/" + newF.substr(2);
//         }
//         renameFile(oldF,newF);
//       }
//       else if(arr[0]=="move"){
//         int len=arr.size();
//         for(int i=1;i<len-1;i++){
//             s = arr[i];
//             string source=arr[i].substr(2);
//             string des=arr[len-1].substr(2);
//             moveFilesDir(source.c_str(),des.c_str());
//         }
//       }
//       else if(arr[0]=="copy"){
//         int len=arr.size();
//         for(int i=1;i<arr.size()-1;i++){
//             bool flag=true;
//             s = arr[i];
//             if (s.find('.') != std::string::npos)
//                 flag=true;//File
//             else
//                 flag=false;//Dir
//             string source=arr[i].substr(2);
//             string des=arr[len-1].substr(2);
//             int type=0;
//             copyFilesDir(source,des,flag,type);
//          }
//       }
//       else if(arr[0]=="esc"){
//       }
//       else if(arr[0]=="exit"){
//       }
//     }
   
