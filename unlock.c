#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <assert.h>

#include "data.h"
#include "crypto.h"

int main(void) {
    decrypt("./log1", "secret");
  return 0;
}

