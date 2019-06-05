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

#include "ftps.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
sqlite3 * open_database(char *name)
{
   char *zErrMsg = 0;
   int rc;
   int needcreattable = 0;
   char *sql;
   sqlite3 *db = NULL;

    if(NULL == name)
    {
        log_write("open database name null.");
        return NULL;
    }
    if(0 != access(name, F_OK|W_OK))
    {
        needcreattable = 1;
    }
    rc = sqlite3_open(name, &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
        fprintf(stderr, "Opened database [%s] successfully\n", name);
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
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            fprintf(stdout, "Table created successfully\n");
        }
    }
    return db;
}

sqlite3 * open_servdatabase(char *name)
{
   char *zErrMsg = 0;
   int rc;
   int needcreattable = 0;
   char *sql;
   sqlite3 *db = NULL;

    if(NULL == name)
    {
        log_write("open database name null.");
        return -1;
    }
    if(0 != access(name, F_OK|W_OK))
    {
        needcreattable = 1;
    }
    rc = sqlite3_open(name, &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    if(needcreattable)
    {
        /* Create SQL statement */
        //sql = "CREATE TABLE server (sid INTEGER PRIMARY KEY ASC AUTOINCREMENT, ip CHAR (20) NOT NULL, port INT (6) NOT NULL, sdisc CHAR);";
        sql = "CREATE TABLE server (sid INTEGER PRIMARY KEY ASC AUTOINCREMENT, ip CHAR (20) NOT NULL, port CHAR (6) NOT NULL, sdisc CHAR, user CHAR, password);";

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            fprintf(stdout, "Table created successfully\n");
        }
    }
   return db;
}

static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int insert_database(sqlite3 *db, unsigned char *dir,unsigned char *name)
{
   
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
   sprintf(sql,"INSERT INTO CFTPS (ID,DIR,NAME,SERVER) VALUES (NULL, 'Paul', 'namess', 'California'); " );
   if(db == NULL)
   {
      printf("fun %s db is null.\r\n", __FUNCTION__);
      return -1;
   }
   //sprintf(sql, "INSERT INTO CFTPS (ID,DIR,NAME,SERVER) VALUES (NULL, '%s', '%s', 'serv'); " ,dir, name);
   printf("sql:[%s]\r\n", sql);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, &sql[0], insert_callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }

   return rc;
}

int insertserv_database(sqlite3 *db, FTPSERVER_S*s)
{
   
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
   sprintf(sql, "INSERT INTO server (sid,ip,port,user,password,sdisc) VALUES (NULL, '%s', '%s','%s', '%s', '%s'); " \
    ,s->ip, s->port, s->user, s->password, s->desc);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, &sql[0], insert_callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }

   return rc;
}

int delserv_database(sqlite3 *db, int id)
{
   
   char *zErrMsg = 0;
   int rc;
   char sql[1024] = {0};

   
   /* Create SQL statement */
   //sql = "INSERT INTO CFTPS (ID,DIR,NAME,SERVER) VALUES (NULL, 'Paul', 'namess', 'California'); " ;
   sprintf(sql, "delete from server where sid = %d; " ,id);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, &sql[0], insert_callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }

   return rc;
}

int select_callback(void *data, int argc, char **argv, char **azColName){
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


int select_database(sqlite3* db, unsigned char* dir, unsigned char* name, void *callback)
{
   //sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char sql[1024] = {0};
   const char* data = "Callback function called";

   /* Create SQL statement */
   if(dir || name)
   {
      sprintf(&sql[0],"SELECT * from CFTPS where NAME like \'%%%s%%\' limit 20;", name );
   }
   else
   {
      /* code */
      sprintf(&sql[0],"SELECT * from CFTPS limit 20;");
   }
   
   printf("[sql=%s]",sql);
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, select_callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      //fprintf(stdout, "Operation done successfully\n");
   }
   return 0;
}

int listserv_callback(void *data, int argc, char **argv, char **azColName){
   int i;
  // fprintf(stderr, "%s: ", (const char*)data);
   //printf(" <tbody><tr>");
   for(i=0; i<argc; i++){
      printf("%s ", argv[i] ? argv[i] : "NULL");
    //printf("%s ",  "NULL");
   }
    printf(" \n");
   return 0;
}


int listser_database(sqlite3 *db, void *callback)
{
   char *zErrMsg = 0;
   int rc;
   char sql[1024] = {0};
   const char* data = "Callback function called";

   /* Create SQL statement */
   {
      /* code */
      sprintf(&sql[0],"select * from server;");
   }
   
   printf("[sql=%s]\r\n",sql);
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, data, &zErrMsg);

   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   return 0;
}

void close_database(sqlite3 *db)
{
    sqlite3_close(db);
}