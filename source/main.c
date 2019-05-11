/* add main.c for test ftp 2018.10.1 国庆节 */


#include "stdio.h"

#include "sqlite.h"

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
	// return -1;
	}
	log_init();
	log_write("init log compelte.");

	(void) open_database("ftpdata");
  ftpser();
  log_display();
  printf("success exec %s .", NAME);
  return 0;
}
