#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h> // used to find window size - unix-based specific
#include <signal.h> // used to catch SIGWINCH
#include <assert.h>