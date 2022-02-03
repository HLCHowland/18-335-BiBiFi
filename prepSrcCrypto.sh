#!/bin/bash          
# https://security.stackexchange.com/questions/183948/unix-command-to-generate-cryptographically-secure-random-string
salt=$(LC_ALL=C tr -dc '[:alnum:]' < /dev/urandom | head -c20)
PUID=$(LC_ALL=C tr -dc '[:alnum:]' < /dev/urandom | head -c20)

echo $salt
echo $PUID

sed -i "s/salt\[\] = \"[a-zA-Z0-9]*\";/salt\[\] = \"$salt\";/" "logappend.c" 
sed -i "s/PUID\[\] = \"[a-zA-Z0-9]*\";/PUID\[\] = \"$PUID\";/" "logappend.c" 

sed -i "s/salt\[\] = \"[a-zA-Z0-9]*\";/salt\[\] = \"$salt\";/" "logread.c" 
sed -i "s/PUID\[\] = \"[a-zA-Z0-9]*\";/PUID\[\] = \"$PUID\";/" "logread.c" 