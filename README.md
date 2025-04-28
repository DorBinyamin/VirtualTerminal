# VirtualTerminal
VirtualTerminal-Example of running in a readme file
[input]    pwd 
[output]   V/ 
[input]    touch V/test.txt 
[input]    write V/test.txt 0 d 
[input]    read V/test.txt 0  
[output]   d 
[input]    write V/test.txt 1 e 
[input]    write V/test.txt 2 J 
[input]    write V/test.txt 3 o 
[input]    write V/test.txt 4 n 
[input]    write V/test.txt 5 g 
[input]    cat V/text.txt 
[output]   ERROR: no such filename 
[input]    cat V/test.txt  
[output]   deJong 
[input]    mkdir V/tmp/ 
[input]    chdir V/tmp/ 
[input]    pwd 
[output]   V/tmp/ 
[input]    ls V/tmp/ 
[output]   V/tmp/: 
[input]    copy V/test.txt V/tmp/t.dat 
[input]    copy FS.cpp V/tmp/f.cc 
[input]    ln V/tmp/f.cc V/tmp/FS.cpp 
[input]    lproot 
[output]   V/: 
[output]   test.txt 1 
[output]   V/tmp/: 
[output]   f.cc 2 
[output]   t.dat 1 
[output]   FS.cpp 1 
[input]    rmdir V/tmp/ 
[input]    lproot 
[output]   V/: 
[output]   test.txt 1 
[input]    exit 
