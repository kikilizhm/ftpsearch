

/*************************************
文件名： client.c 
linux 下socket网络编程简例  - 客户端程序
服务器端口设为 0x8888   （端口和地址可根据实际情况更改，或者使用参数传入）
服务器地址设为 192.168.1.104
作者:kikilizhm#163.com (将#换为@)
*/
 
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>
#include <errno.h>
//#include <arpa/inet.h>

#include <sys/select.h>
#include <sys/time.h>

#include "sqlite.h"

#define IP_SERV "192.168.1.103"
#define PORT_SERV 1024
#define USERNAME "cx"
#define PASSWORD "cx"

int list_dir(char *abs, char *dir, int cmd_fd, int data_fd ,FILE* db_fd );
FILE *save_database(unsigned char *file);
FILE *open_tmpfile(unsigned char *file);

int connect_ser(unsigned char *ip, unsigned short port)
{
    int cfd = 0; /* 文件描述符 */
    struct sockaddr_in c_add; /* 存储服务端和本端的ip、端口等信息结构体 */

    /* 建立socket 使用因特网，TCP流传输 */
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd)
    {
        log_write("socket fail.(serverip %#x, port %#x)\r\n", ip, port);
        return -1;
    }
    /* 构造服务器端的ip和端口信息，具体结构体可以查资料 */
    bzero(&c_add,sizeof(struct sockaddr_in));
    c_add.sin_family=AF_INET;
    c_add.sin_addr.s_addr= inet_addr(ip); /* ip转换为4字节整形，使用时需要根据服务端ip进行更改 */
    c_add.sin_port=htons(port); /* 这里htons是将short型数据字节序由主机型转换为网络型，其实就是
        将2字节数据的前后两个字节倒换，和对应的ntohs效果、实质相同，只不过名字不同。htonl和ntohl是
        操作的4字节整形。将0x12345678变为0x78563412，名字不同，内容两两相同，一般情况下网络为大端，
        PPC的cpu为大端，x86的cpu为小端，arm的可以配置大小端，需要保证接收时字节序正确。
    */
  
    /* 客户端连接服务器，参数依次为socket文件描述符，地址信息，地址结构大小 */
    if(-1 == connect(cfd,(struct sockaddr *)(&c_add), sizeof(struct sockaddr)))
    {
        printf("connect fail to ip %s. port %#x.\r\n", ip,port);
        close(cfd);
        return -1;
    }
    log_write("connect to serverip = %#x, port = %#x\r\n",htonl(c_add.sin_addr.s_addr),htons(c_add.sin_port)); /* 这里打印出的是小端*/
    return cfd;
}

int ftp_pasvmode(int cmd_fd, int *pip, unsigned short*pport)
{
    unsigned char buffer[64] = {0};
    unsigned char dataname[6] = {0};
    int recbytes = 0;
    char *tmp = NULL;

    if(-1 == (recbytes = write(cmd_fd,"PASV\r\n",6)))
    {
        printf("list data fail !\r\n");
        return -1;
    }

    /*连接成功,从服务端接收字符*/
    if(-1 == (recbytes = read(cmd_fd,buffer,64)))
    {
        printf("read data fail !\r\n");
        return -1;
    }

    buffer[recbytes]='\0';
    printf("\r\n%s",buffer);

    tmp = (char *)strstr(buffer,"(");
    if(NULL == tmp) 
    {
        printf("pasv mode no reply.(%s)", buffer);
        return -1;
    }
    sscanf(tmp, "(%d,%d,%d,%d,%d,%d)*/", &dataname[0],&dataname[1],&dataname[2],&dataname[3],&dataname[4],&dataname[5]);
    if(pip) *pip = htonl(*(unsigned int *)&dataname[0]);
    if(pport) *pport = htons(*(unsigned short*)&dataname[4]);
    printf("data ip  %#x  %#x==",htonl(*(unsigned int *)&dataname[0]),htons(*(unsigned short*)&dataname[4]));
    return 0;
}
 
int ftpser(void)
{
    int cfd = 0; /* 文件描述符 */
    int cfd_data = 0;

    int recbytes;
    int sin_size;
    char buffer[1024]={0};    /* 接受缓冲区 */
    struct sockaddr_in s_add,c_add; /* 存储服务端和本端的ip、端口等信息结构体 */
    unsigned short portnum=2222;  /* 服务端使用的通信端口，可以更改，需和服务端相同 */
    unsigned char dataname[24] = {0};
    struct sockaddr_in client ;
    int clientAddrLen = sizeof(client);
    char *tmp = NULL;
    FILE* fpp_tmp = NULL;
    FILE* ffp_database = open_tmpfile("databast.dat");
    int ip = 0;
    int dataport = 0;

    cfd = connect_ser(IP_SERV, PORT_SERV);

    if(-1 == cfd)
    {
        return -1;
    }

    //获取sockfd表示的连接上的本地地址
    if(getsockname(cfd, (struct sockaddr*)&client, &clientAddrLen) != 0 ){
        log_write("client get port error: %s(errno: %d))\n", strerror(errno), errno);
        return -1;
    }
    printf("self ip = %#x, port = %d \n",  htonl(client.sin_addr.s_addr),  htons(client.sin_port));


    /*连接成功,从服务端接收字符*/
    if(-1 == (recbytes = read(cfd,buffer,1024)))
    {
        printf("read data fail !\r\n");
        return -1;
    }
    printf("read welcome msg ok\r\nREC:\r\n");

    buffer[recbytes]='\0';
    printf("%s\r\n",buffer);


	if(0 != sendcmd(cfd, "USER " USERNAME))
	{
		return -1;
	}
	(void) recv_msg(cfd, NULL);

    if(0 != sendcmd(cfd, "PASS " PASSWORD))
	{
		return -1;
	}
	(void) recv_msg(cfd, NULL);



#if 0

ftp_pasvmode(cfd, &ip, &dataport);
  
    cfd_data = connect_ser("192.168.1.103", dataport) );

    if(-1 == cfd_data)
    {
        return -1;
    }

    if(-1 == (recbytes = write(cfd,"LIST\r\n",6)))
    {
        printf("list data fail !\r\n");
        return -1;
    }

    /*连接成功,从服务端接收字符*/
    if(-1 == (recbytes = read(cfd,buffer,1024)))
    {
        printf("read data fail !\r\n");
        return -1;
    }
    printf("read list ok\r\nREC:\r\n");

    buffer[recbytes]='\0';
    printf("%s\r\n",buffer);

    fpp_tmp = save_database("list_tmp.txt");
    if(NULL == fpp_tmp)
    {
        log_write("open tmp list file fail.");
        return -1;
    }


#endif
ftp_pasvmode(cfd, &ip, &dataport);

    open_database(IP_SERV ".db3");
    printf("database fd %d \r\n", ffp_database);
     list_dir("/", NULL, cfd, dataport, ffp_database);

    /*连接成功,从服务端接收字符*/


    getchar(); /* 此句为使程序暂停在此处，可以使用netstat查看当前的连接 */
    close(cfd); /* 关闭连接，本次通信完成 */
    return 0;

}




FILE *save_database(unsigned char *file)
{
    FILE *fp = NULL;
    fp = fopen(file, "w+");
    if(NULL == fp)
    {
        log_write("open database file %s fail.", file);
        return NULL;
    }
    return fp;
}

FILE *open_tmpfile(unsigned char *file)
{
    FILE *fp = NULL;
    fp = fopen(file, "w+");
    if(NULL == fp)
    {
        log_write("open database file %s fail.", file);
        return NULL;
    }
    return fp;
}

typedef struct tag_LIST_DATA_S
{
    char isdir; /*"D or -"*/
    unsigned char path[256];
    unsigned char name[256];
}LIST_DATA_S;

int recv_reply(int fp, FILE* ffp)
{
    fd_set rfds;
    struct timeval timeout = {3,0};
    char readbuff[10] = {0};
    char readline[1024] = {0};
    int ret;
    char *list_buff = NULL;
    int len = 0;
    int line = 0;
    LIST_DATA_S *plist = NULL;
    char *tmp = NULL;
    while(1)
    {
        FD_ZERO(&rfds);  /* 清空集合 */
        FD_SET(fp, &rfds);  /* 将fp添加到集合，后面的FD_ISSET和FD_SET没有必然关系，这里是添加检测 */
 
        ret=select(fp+1, &rfds, NULL, NULL, &timeout);
        //printf("\r\nselect ret = %d\r\n",ret);
        if(0 > ret)
        {
                return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            if(FD_ISSET(fp,&rfds))  /* 这里检测的是fp在集合中是否状态变化，即可以操作。 */
            {
                ret = read(fp, readbuff, 9, 0);
		        if(0 == ret) break;    /* 此处需要检测！否则ftp发送数据时，后面会循环接收到0字节数据 */
                printf("\r\n%s",readbuff);
                if(ffp)fputs(readbuff, ffp);
                memset (readbuff,0,10);
            }
        }
    }

    fseek(ffp, 0, SEEK_END);

    len = ftell(ffp);
    list_buff = malloc(sizeof(LIST_DATA_S)*5);
    plist = (struct LIST_DATA_S*)list_buff;
    if(NULL == plist)
    {
        log_write("malloc list buff fail %d.", sizeof(LIST_DATA_S));
        return -1;
    }
    log_write("malloc list buff %d.", len);
    memset(plist, 0, len);
    fseek(ffp, 0, SEEK_SET);

    while(NULL != fgets(&readline[0], 1024, ffp)) 
    {
        printf("line:%s|",readline);
        if((readline[0] != 'd') && (NULL == strstr("DIR", &readline[0])))
        {
            plist->isdir = 0;
        }
        else
        {
            plist->isdir = 1;
        }
        
        tmp = strrchr(readline, ' ');
        
        tmp += 1;
        
        len = 0;
        if(tmp)
        {
            while(('\r' != *(tmp+len)) && ('\n' != *(tmp+len)))
            {
                plist->name[len] = *(tmp+len);
                len += 1;
            }
            plist->name[len] = 0;
            printf("name :%s|\r\n", plist->name);
        }
        plist += 1;
    }
    return 0;
}


int recv_msg(int fp, FILE* ffp)
{
    fd_set rfds;
    struct timeval timeout = {3,0};
    char readbuff[100] = {0};
    char readline[1024] = {0};
    int ret;
    char *list_buff = NULL;
    int len = 0;
    int line = 0;
    LIST_DATA_S *plist = NULL;
    char *tmp = NULL;
    while(1)
    {
        FD_ZERO(&rfds);  /* 清空集合 */
        FD_SET(fp, &rfds);  /* 将fp添加到集合，后面的FD_ISSET和FD_SET没有必然关系，这里是添加检测 */
 
        ret=select(fp+1, &rfds, NULL, NULL, &timeout);
        //printf("\r\nselect ret = %d\r\n",ret);
        if(0 > ret)
        {
                return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            if(FD_ISSET(fp,&rfds))  /* 这里检测的是fp在集合中是否状态变化，即可以操作。 */
            {
                ret = read(fp, readbuff, 99, 0);
		        if(0 == ret) break;    /* 此处需要检测！否则ftp发送数据时，后面会循环接收到0字节数据 */
                printf("\r\n%s",readbuff);
                if(ffp)
                {
                    fputs(readbuff, ffp);
                }
                
                memset (readbuff,0,100);
            }
        }
    }
    return 0;
}

int sendcmd_replybyfile(int cmd_fd, char *cmd, int *reply_ffd)
{
    char buff[32] = {0};
    int n = 0;
    FILE *fp = NULL;

    sprintf(buff, "%s_tmpreply.txt\r\n", cmd);
    fp = open_tmpfile(buff);
    {
        log_write("open file %s fail.", buff);
        return -1;
    }

    sprintf(buff, "%s\r\n", cmd);
    if(-1 == (n = write(cmd_fd,buff,strlen(buff))))
    {
        log_write("send cmd %s fail.\r\n", cmd);
        return -1;
    }
    if(0 != recv_msg(cmd_fd, fp))
    {
        log_write("recv msg fail for cmd %s.", cmd);
        return -1;
    }
    *reply_ffd = fp;
    return 0;
}

int sendcmd(int cmd_fd, char *cmd)
{
    char buff[512] = {0};
    int n = 0;

    sprintf(buff, "%s\r\n", cmd);
    if(-1 == (n = write(cmd_fd,buff,strlen(buff))))
    {
        log_write("send cmd [%s] fail.\r\n", cmd);
        return -1;
    }
	log_write("send cmd[%s] ok.", cmd);
    return 0;
}

/* 
    1. list 获取list数据
    2. 格式化list回显数据
    3. 从第一条数据开始解析，如果时dir则递归，文件则写数据库
 */
int list_dir(char *abs, char *dir, int cmd_fd, int /*data_fd*/nused ,FILE* database_fd)
{
    FILE* tmp_datafd = open_tmpfile("list_tmp.txt");
    char buff[256] = {0};
    char readline[512] = {0};
    char nextdir[512] = {0};
	char absdir[512] = {0};
    int line = 0;
    char *list_buff = NULL;
    LIST_DATA_S* plist = NULL;

    
    char *tmp = NULL;
    int len = 0;
    int i = 0;
    int ip = 0;
    unsigned short dataport = 0;

    int data_fd = 0;


    /* send cmd */
	if(0 == dir)
	{
		sprintf(absdir, "%s", abs);
	}
	else
	{
		if(0 == memcmp(abs, "/", 2))
		{
			sprintf(absdir, "%s%s", abs, dir);
		}
		else
		{
			sprintf(absdir, "%s/%s", abs, dir);
		}	
	}	

	if(NULL != dir)
	{
	    sprintf(buff,"CWD %s", dir);
	    if(0 != sendcmd(cmd_fd, buff))
	    {
	        log_write("cd %s fail.", buff);
	        return -1;
	    }
	    (void)recv_msg(cmd_fd, NULL);
	}

    ftp_pasvmode(cmd_fd, &ip, &dataport);

    if(0 != sendcmd(cmd_fd, "LIST"))
    {
        log_write("list %s fail.", buff);
        return -1;
    }
    (void)recv_msg(cmd_fd, NULL);

    #if 1
    printf("\r\n list / connect data");
    data_fd = connect_ser(IP_SERV, dataport);

    if(-1 == data_fd)
    {
        log_write("connect data channel fail.");
        return -1;
    }
    #endif
printf("\r\n list / rev data");
    recv_msg(data_fd, tmp_datafd);

    close(data_fd);



    /* 格式化 */
    fseek(tmp_datafd, 0, SEEK_SET);

    while(NULL != fgets(&readline[0], 1024, tmp_datafd)) 
    {
        line += 1;
    }
#if 1
	printf("\r\n list / get line %d", line);
    if(!line) 
	{
		printf("\r\n dir[%s] has no file.\r\n", absdir);
		sprintf(buff,"CWD ..");
	    if(0 != sendcmd(cmd_fd, buff))
	    {
	        log_write("cd %s fail.", buff);
	        return -1;
	    }
	    (void)recv_msg(cmd_fd, NULL);
		return 0;
	}
#endif
    list_buff = malloc(sizeof(LIST_DATA_S)*line);
    plist = (struct LIST_DATA_S*)list_buff;
    if(NULL == plist)
    {
        log_write("malloc list buff fail %d.", sizeof(LIST_DATA_S));
        return -1;
    }

    memset(plist, 0, len);
    fseek(tmp_datafd, 0, SEEK_SET);
    line = 0;

    while(NULL != fgets(&readline[0], 1024, tmp_datafd)) 
    {
        //printf("line:%s|",readline);
        if((readline[0] != 'd') && (NULL == strstr("DIR", &readline[0])))
        {
            plist->isdir = 0;
        }
        else
        {
            plist->isdir = 1;
        }
        
        tmp = strrchr(readline, ' ');
        
        tmp += 1;
        
        len = 0;
        if(tmp)
        {
            while(('\r' != *(tmp+len)) && ('\n' != *(tmp+len)))
            {
                plist->name[len] = *(tmp+len);
                len += 1;
            }
            plist->name[len] = 0;
            printf("name :%s|\r\n", plist->name);
        }
        plist += 1;
        line += 1;
    }
    fclose(tmp_datafd);


    plist = (struct LIST_DATA_S*)list_buff;
    for(i=0;i<line;i++)
    {
        
        if(plist->isdir == 0)
        {
#if 0
            printf("\r\n list / write file database \r\n");
            if(0 >  fputs(dir, database_fd))
            {
                log_write("write database dir %s fail.(%s %d)",dir, strerror(errno), errno);
                printf("write database dir %s fail.(%s %d)\r\n",dir, strerror(errno), errno);                
            }
            if(0 >fputs(",", database_fd))
            {
                log_write("write database dir %s fail.",dir);
                
            }
            if(0 > fputs(plist->name, database_fd ))
            {
                log_write("write database dir %s fail.",dir);            
            }
            if(0 > fputs("\r\n", database_fd))
            {
                log_write("write database dir %s fail.",dir);            
            }
#endif
            if(0 != insert_database(absdir, plist->name))
            {
                log_write("insert data[%s][%s] fail.", absdir, plist->name);
                return -1;
            }
        }
        else
        {
			if((0 == memcmp(plist->name, ".", 2)) || (0 == memcmp(plist->name, "..", 3)))
			{
				;
			}
			else
			{
            //sprintf(nextdir, "%s/%s", dir,plist->name);
            	printf("\r\n enter next dir[%s][%s]", absdir, plist->name);
            	(void)list_dir(absdir, plist->name, cmd_fd, data_fd ,database_fd);            
			}
        }
        plist += 1;

    }
	sprintf(buff,"CWD ..");
    if(0 != sendcmd(cmd_fd, buff))
    {
        log_write("cd %s fail.", buff);
        return -1;
    }
    (void)recv_msg(cmd_fd, NULL);

    return 0;
}