// Notice: All parts of this code is protected
// with following license:
// https://github.com/MishraShivendra/node_mcu_py_email/blob/master/LICENSE.md
// Corporate/commercial usage is restricted.
// 
// Author : Shivendra Mishra
// Env: gcc 5.4 on Linux Ubuntu 16.04 
#ifndef INCLUDE_PULL_PUSH_APP_H
#define INCLUDE_PULL_PUSH_APP_H __file__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <new>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <curl/curl.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <err.h>
#include "mysql_connection.h"

#define SQL_ADDRESS "sql_address"
#define SQL_USER "sql_user"
#define SQL_PASSWORD "sql_password"
#define NODE_ADDRESS "node_address"
#define NODE_PORT "node_port"

using namespace std;

class pp_daemon
{
	CURL *curl;
	CURLcode curl_res;
	string data;
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	vector <string> temp_values;
	map<string, string> valid_attrs;

	public:
		pp_daemon( int argc, char** argv );
		~pp_daemon( void );
		void conn_and_get( void );
		void get_temperature( const string& sensor_number );
		void push_data_to_db( void );
		void read_set_conf( int argc, char** argv );
		void parse_cli_load_conf( string file_name, int argc,
					  char** cli_argv );
		void show_help ( void );
		void load_conf_attr( string &file_name );
		bool test_if_file_exists( string &file_name );
		string get_home_dir( void );
		bool test_if_dir_exists( string& path );
		void show_args();
		void cleanup();		
};

#endif // End of .h file
