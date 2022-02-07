#!/bin/bash          
#This file overwrites the keys placed into sourcecode
sed -i "s/char salt\[\] = \"[a-zA-Z0-9]*\";/char salt\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logappend.c" 
sed -i "s/char CTUID\[\] = \"[a-zA-Z0-9]*\";/char CTUID\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logappend.c" 
sed -i "s/char salt\[\] = \"[a-zA-Z0-9]*\";/char salt\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logread.c" 
sed -i "s/char CTUID\[\] = \"[a-zA-Z0-9]*\";/char CTUID\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logread.c" 