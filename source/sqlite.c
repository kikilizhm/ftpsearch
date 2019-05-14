#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "sqlite3.h"

sqlite3 *g_db = NULL;
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
int open_database(char *name)
{
   char *zErrMsg = 0;
   int rc;
   int needcreattable = 0;
   char *sql;

    if(NULL == name)
    {
        log_write("open database name null.");
        return -1;
    }
    if(0 != access(name, F_OK|W_OK))
    {
        needcreattable = 1;
    }
    rc = sqlite3_open(name, &g_db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(g_db));
      exit(0);
   }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    if(needcreattable)
    {
	
	/*
	CREATE TABLE CFTPS (ID INTEGER PRIMARY KEY AUTOINCREMENT, DIR NCHAR NOT NULL, NAME NCHAR NOT NULL, SERVER NCHAR);
	*/
	
        /* Create SQL statement */
        sql = "CREATE TABLE CFTPS("  \
                "ID INTEGER PRIMARY KEY  AUTOINCREMENT," \
                "DIR           NCHAR    NOT NULL," \
                "NAME            NCHAR     NOT NULL," \
                "SERVER         NCHAR );";

        /* Execute SQL statement */
        rc = sqlite3_exec(g_db, sql, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            fprintf(stdout, "Table created successfully\n");
        }
    }
    return rc;
}

static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int insert_database(unsigned char *dir,unsigned char *name)
{
    sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char sql[1024] = {0};

   /* Open database
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   } */

   /* Create SQL statement */
   //sql = "INSERT INTO CFTPS (ID,DIR,NAME,SERVER) VALUES (NULL, 'Paul', 'namess', 'California'); " ;
   sprintf(sql, "INSERT INTO CFTPS (ID,DIR,NAME,SERVER) VALUES (NULL, '%s', '%s', 'serv'); " ,dir, name);

   /* Execute SQL statement */
   rc = sqlite3_exec(g_db, &sql[0], insert_callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }

   return rc;
}

static int select_callback(void *data, int argc, char **argv, char **azColName){
   int i;
  // fprintf(stderr, "%s: ", (const char*)data);
   printf(" <tbody><tr>");
   for(i=0; i<argc; i++){
     // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");


    printf(" <td>%s</td>", argv[i] ? argv[i] : "NULL");


   }
    printf(" </tr></tbody>");
   return 0;
}

int select_database(unsigned char* dir, unsigned char* name)
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char sql[1024] = {0};
   const char* data = "Callback function called";

   /* Open database
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   } */

   /* Create SQL statement */
   sprintf(&sql[0],"SELECT * from CFTPS where \'NAME\' like \'%%%s%%\' limit 20;", name );

   /* Execute SQL statement */
   rc = sqlite3_exec(g_db, sql, select_callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      //fprintf(stdout, "Operation done successfully\n");
   }
   return 0;
}
void close_database(void)
{
    sqlite3_close(g_db);
}