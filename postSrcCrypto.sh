#!/bin/bash          
#This file overwrites the keys placed into sourcecode
sed -i "s/salt\[\] = \"[a-zA-Z0-9]*\";/salt\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logappend.c" 
sed -i "s/PUID\[\] = \"[a-zA-Z0-9]*\";/PUID\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logappend.c" 
sed -i "s/salt\[\] = \"[a-zA-Z0-9]*\";/salt\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logread.c" 
sed -i "s/PUID\[\] = \"[a-zA-Z0-9]*\";/PUID\[\] = \"XXXXXXXXXXXXXXXXXXXX\";/" "logread.c" 