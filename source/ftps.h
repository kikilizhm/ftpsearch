
typedef struct tag_FTPSERVER_S
{
    unsigned char ip[20];
    unsigned char port[6];
    unsigned char user[20];
    unsigned char password[32];
    unsigned char desc[80];
}FTPSERVER_S;

//#define IP_SERV "202.38.97.230"
//#define PORT_SERV 21
//#define USERNAME "anonymous"
//#define PASSWORD "lzm@lzu.cn"

#define DATABASE_NAME "202.38.97.230"