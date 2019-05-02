

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
        printf("connect fail !\r\n");
        close(cfd);
        return -1;
    }
    log_write("connect to serverip = %#x, port = %d\r\n",htonl(c_add.sin_addr.s_addr),htons(c_add.sin_port)); /* 这里打印出的是小端*/
    return cfd;
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

    cfd = connect_ser("192.168.1.103", 2222);

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
    printf("read ok\r\nREC:\r\n");

    buffer[recbytes]='\0';
    printf("%s\r\n",buffer);

    if(-1 == (recbytes = write(cfd,"USER cx\r\n",9)))
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
    

    if(-1 == (recbytes = write(cfd,"PASS cx\r\n",9)))
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

    if(-1 == (recbytes = write(cfd,"PASV\r\n",6)))
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

    tmp = (char *)strstr(buffer,"(");
    printf("%s\r\n",tmp);

    sscanf(tmp, "(%d,%d,%d,%d,%d,%d)*/", &dataname[0],&dataname[1],&dataname[2],&dataname[3],&dataname[4],&dataname[5]);
    printf("data ip  %#x  %#x==",htonl(*(unsigned int *)&dataname[0]),htons(*(unsigned short*)&dataname[4]));

    cfd_data = connect_ser("192.168.1.103", htons(*(unsigned short*)&dataname[4]) );

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

    /*连接成功,从服务端接收字符*/
    if(-1 == (recbytes = read(cfd_data,buffer,1024)))
    {
        printf("read data fail !\r\n");
        return -1;
    }
    printf("read list ok\r\nREC:\r\n");

    buffer[recbytes]='\0';
    printf("%s\r\n",buffer);

    getchar(); /* 此句为使程序暂停在此处，可以使用netstat查看当前的连接 */
    close(cfd); /* 关闭连接，本次通信完成 */
    return 0;

}



FILE *save_database(unsigned char *file)
{
    FILE *fp = NULL;
    fp = fopen(file, "w");
    if(NULL == fp)
    {
        log_write("open database file %s fail.", file);
        return NULL;
    }
    return fp;
}



