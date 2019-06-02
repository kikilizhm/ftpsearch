/* add main.c for test ftp 2018.10.1 国庆节 */


#include "stdio.h"
#include <string.h>
#include "sqlite3.h"
#include "sqlite.h"

#define NAME "FTPSearch"

void helpmsg(void)
{
  printf("Usage: %s <operation> [options]\r\n", NAME);
	printf("Operations:\r\n \
	ls -- list ftp server info\r\n  \
	add -- add ftp server\r\n \
	del -- del ftp server\r\n \
	help -- echo help info\r\n");
  return ;
}

void server_list(char *ops)
{
	sqlite3 *db = NULL;
	db = open_servdatabase("ftpserver.db3");
	if(!db)
	{
		printf("open serdatabase fail.\r\n");
		return ;
	}
	listser_database(db, listserv_callback); 
	close_database(db);
}

void server_add(char *ops)
{
	sqlite3 *db = NULL;
	char *ip;
	char *port;
	char *desc;
	if(ops == NULL)
	{
		printf("no ip info.\r\n");
		return;
	}
	db = open_servdatabase("ftpserver.db3");
	if(!db)
	{
		printf("open serdatabase fail.\r\n");
		return ;
	}


	ip = strtok(ops,":");
	if(ip == NULL)
	{
		printf("ip null.\r\n");
		return ;
	}
	
	port = strtok(NULL, ":");
	if(port == NULL)
	{
		printf("port null.\r\n");
		return ;
	}

	desc = strtok(NULL, ":");
	if(desc == NULL)
	{
		printf("desc null.\r\n");
		return ;
	}
	printf("add ip[%s] port[%s] desc[%s]\r\n", ip, port, desc);
	insertserv_database(db, ip, port);
	close_database(db);
}

void server_del(char *ops)
{
	sqlite3 *db = NULL;
	int id = 0;
	char *ip;
	char *port;
	char *desc;
	if(ops == NULL)
	{
		printf("no ip info(cmd del server_id).\r\n");
		return;
	}
	db = open_servdatabase("ftpserver.db3");
	if(!db)
	{
		printf("open serdatabase fail.\r\n");
		return ;
	}

	id = atoi(ops);

	if(0 != delserv_database(db, id))
	{
		printf("del server id %d fail.\r\n", id);
	}
	close_database(db);
}
typedef void (ARGCPRO)( char *);

typedef struct tag_SLIST_S
{
	char cmd[64];
	ARGCPRO *pfunc;
}SLIST_S;

SLIST_S g_cmd_list[] = 
{
	{"ls", server_list},
	{"add", server_add},
	{"help", NULL},
	{"del", server_del},
	{NULL, NULL}
};

int prasearg(char *cmd, char *ops)
{
	int i = 0;
	int size = sizeof(g_cmd_list)/sizeof( SLIST_S) ;
	SLIST_S*p = &g_cmd_list[0];
	for(i = 0; i< size; i++)
	{
		if(!p->cmd) break;

		if(memcmp(p->cmd, cmd, strlen(cmd)+1) == 0)
		{
			printf("match cmd %s.\n", p->cmd);
			if(p->pfunc) 
			{
				p->pfunc(ops);
				printf("cmd proc[%p]\r\n", p->pfunc);
			}
			return 0;
		}
		p++;
	}
	return -1;
}

int main(int argc, char *argv[])
{
	char *opr = NULL;
	char *ops = NULL;
	if(1 == argc)
	{
		helpmsg();
		return -1;
	}
	log_init();
	log_write("init log compelte.");

	if(argc >= 2) opr= argv[1];
	if(argc >=3) ops = argv[2];
	if(0 != prasearg(opr, ops))
	{

		printf("parse arg fail.\r\n");
		return 0;
	}
	else
	{
		/* code */
		printf("parse arg ok.\r\n");
		return 0;
	}
	

	(void) open_database("ftpdata");
  ftpser();
  log_display();
  printf("success exec %s .", NAME);
  return 0;
}
