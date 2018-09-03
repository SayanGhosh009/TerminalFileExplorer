//ROLL NUMBER : 2018202002
//NAME : SAYAN GHOSH

Format for Input to Obtain corresponding Output [SAME AS GIVEN IN ASSIGNMENT, JUST TO HIGHLIGHT] :

*SUMMARY*
1.Normal Mode everything works as given in assignment description, nothing extra added.
2.In Command Mode every input format is same as in assignment, but since in some places '~/' is added
and in some places its just 'filename.txt' , Which too will work in all cases but just to avoid ambiguity and to maintain uniformity I have ASSUMED '~/' will be added before every file/folder name even if its current directory so as to run the code clean and without any kind of errors,Though for special cases like in goto '/' or create_file '.' will work as intended in Project.

****IN SHORT PUTTING '~/' BEFORE EVERY FILE/FOLDERNAME(EXCEPT IN SEARCH) WILL WORK SMOOTHLY(EVEN IF ITS IN CURRENT DIRECTORY).****

*EXAMPLE FORMATS*

a)'create_file foo.txt ~/Videos/AnotherFolder' is correct format.
  'create_dir ~/snap/AnotherFolder ~/Videos/AnotherFolder' is correct format.
  'create_file /foo.txt ~/Videos' is correct format.
  '.' will work as intended in assignment question.


b)'delete_file ~/Videos/AnotherFolder/foo.txt' is correct format.
  'delete_dir  /Videos/AnotherFolder/ThirdFolder/foo.txt' is correct format.

c)'goto ~/snap/AnotherFolder' is correct format
  'goto /' will work as intended.

d)'search Filename.txt' is correct format. (No need of ~/ since its searching in current dir. recursively)
  'search Filename.<any other type>' is correct format[For any filenames].
  On pressing back key is LEFT arrow key here as intended in assignment.

e)To COPY/MOVE/RENAME it will be clean and error free to put '~/' before every file or foldername even if the file(s) are in current directory
  for COPY AND MOVE.
 'copy ~/foo.txt ~/snap/bar.txt ~/Videos/AnotherFolder' is correct. 
 'copy foo.txt bar.txt ~/Videos/AnotherFolder' is correct.
 'rename foo.txt bar.txt' is correct
 'rename ~/snap/CopyFolder/foo.txt ~/snap/CopyFolder/bar.txt' is correct
 'move ~/filename.txt ~/Folder1/Folder2' is correct

f)'snapshot ~/Foldername1/Foldername2 ~/Foldername/Dumpfile.txt' is correct format.
  'snapshot Foldername Dumpfile.txt' is correct format.
  Here, the Dumpfile.txt is assumed to not exist.Output is in ls -R format.

g)IMPORTANT:1. To Move from Command Mode to Normal Mode it is necessary to TYPE 'esc' (without quotes) In the Command Mode Terminal Instead of pressing escape Key.
2.The Terminal Screen Size should not be changed and kept at full screen throughout running the project

