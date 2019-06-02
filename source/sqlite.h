int open_database(char *name);
sqlite3 * open_servdatabase(char *name);
int insert_database(sqlite3 *db, unsigned char *dir,unsigned char *name);
int select_database(sqlite3* db, unsigned char* dir, unsigned char* name, void *callback);
void close_database(sqlite3 *db);
int listserv_callback(void *data, int argc, char **argv, char **azColName);
int listser_database(sqlite3* db, void *callback);
int insertserv_database(sqlite3 *db, FTPSERVER_S*s);