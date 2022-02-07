#!/bin/bash          
# https://security.stackexchange.com/questions/183948/unix-command-to-generate-cryptographically-secure-random-string
salt=$(LC_ALL=C tr -dc '[:alnum:]' < /dev/urandom | head -c20)
CTUID=$(LC_ALL=C tr -dc '[:alnum:]' < /dev/urandom | head -c20)

sed -i "s/char salt\[\] = \"[a-zA-Z0-9]*\";/char salt\[\] = \"$salt\";/" "logappend.c" 
sed -i "s/char CTUID\[\] = \"[a-zA-Z0-9]*\";/char CTUID\[\] = \"$CTUID\";/" "logappend.c" 

sed -i "s/char salt\[\] = \"[a-zA-Z0-9]*\";/char salt\[\] = \"$salt\";/" "logread.c" 
sed -i "s/char CTUID\[\] = \"[a-zA-Z0-9]*\";/char CTUID\[\] = \"$CTUID\";/" "logread.c" 