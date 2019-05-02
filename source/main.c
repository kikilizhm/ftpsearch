/* add main.c for test ftp 2018.10.1 国庆节 */


#include "stdio.h"

#define NAME "FTPSearch"

void helpmsg(void)
{
  printf("Usage: %s xxx\r\n", NAME);
  return ;
}

int main(int argc, char *argv[])
{
  if(1 == argc)
  {
    helpmsg();
    return -1;
  }
  ftpser();
  printf("success exec %s .", NAME);
  return 0;
}
