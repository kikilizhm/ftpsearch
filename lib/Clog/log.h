#define LOG_ITEM  0x100

#define LOG_LENGTH  256

#define TRUE 1
#define FALSE 0

/* 使用 */
typedef struct tagLOG_BUFF_S
{
    int bufftail;
    unsigned char szBuff[LOG_ITEM][LOG_LENGTH];
    int magicword;
    int isout;

}LOG_BUFF_S;

/* API接口 */
void log_init(void); /* log模块的总初始化函数 */
void log_display(void); /* 打印日志内容 */
void log_write(unsigned char* fmt, ...); /* 记录一条日志信息 */