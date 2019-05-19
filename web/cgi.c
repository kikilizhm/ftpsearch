#include <stdio.h>
 #include <stdlib.h>

#include <string.h>  

 void html_body(void);
 void html_foot(void);

int main(void)
{
    printf("Content-Type: text/html\n\n");
    printf("<html><head>");
    printf("<meta charset=\"gbk\">");
    printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    printf("<title>FTP Search</title>");
    printf("<link rel=\"stylesheet\" href=\"/main.css\"></head>");

    html_body();

    html_foot();
    
}
#define IP_SERV "202.38.97.230"


int open_database(char *name);
int insert_database(unsigned char *dir,unsigned char *name);
int select_database(unsigned char *dir, unsigned char* name);
void close_database(void);
void dis_rest(void)
{
    int len;
    char *r_method;
    char *http_buf=NULL;
    char *str = NULL;

    r_method=getenv("REQUEST_METHOD");
    if(!strcmp(r_method,"POST")){

        len=atoi(getenv("CONTENT_LENGTH"));
        http_buf=(char *)malloc(len+1);
        memset((char *)http_buf,0x0,len+1);
        fgets((char *)http_buf,len+1,stdin);
        printf("[%s]\r\n",http_buf);

        str = strstr(http_buf,"=");
        if(str)
        {
            str += 1;
            if(0 == memcmp(str,"",1))
            {
                printf("no msg");
                return ;
            }

            (void)open_database(IP_SERV ".db3");
        printf("      <table   width=\"90%%\" id=\"mytab\"  border=\"1\" class=\"t1\"><thead><tr  class=\"a1\"><td>ID</td><td>DIR</td><td>NAME</td><td></td></tr></thead>");

        (void)select_database(str, str);
        close_database();
        printf("</table>");
        }        
    }
    else
    {
        printf("no post");
    }

}

void html_body(void)
{
    printf("<body>");
    printf("<div id=\"container\">");
    printf("<div class=\"search bar6\">");
        printf("<form method=\"POST\" action=\"\">");
        printf("<input name=str type=\"text\" placeholder=\"请输入您要搜索的内容...\">");
        printf("<button type=\"submit\"></button>");
        printf("    </form>  </div>  </div> ");

        dis_rest();
}

void html_foot(void)
{

    printf("      </body></html>");
}