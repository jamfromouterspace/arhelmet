/*
    kbhit() implementation retrieved from: https://www.linuxjournal.com/article/1138
    Written by Alan Cox
    Published 1 Sept 1995
*/

#include <sys/select.h>
#include <cstddef>
#include "Tools.h"

int kbhit(void)
{
  struct timeval tv;
  fd_set read_fd;

  // Do not wait at all, not even a microsecond
  tv.tv_sec=0;
  tv.tv_usec=0;

  // Must be done first to initialize read_fd
  FD_ZERO(&read_fd);

  // Makes select() ask if input is ready:
  // 0 is the file descriptor for stdin
  FD_SET(0,&read_fd);

  // The first parameter is the number of the
  // largest file descriptor to check + 1
  if(select(1, &read_fd, NULL, NULL, &tv) == -1)
    return 0;  // An error occured

  //  read_fd now holds a bit map of files that are
  // readable. We test the entry for the standard
  // input (file 0)
  if(FD_ISSET(0,&read_fd))
    // Character pending on stdin
    return 1;

  // no characters were pending
  return 0;
}
