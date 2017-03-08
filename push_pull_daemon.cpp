// Notice: All parts of this code is protected
// with following license:
// https://github.com/MishraShivendra/node_mcu_py_email/blob/master/LICENSE.md
// Corporate/commercial usage is restricted.
// 
// Author : Shivendra Mishra
// Env: gcc 5.4 on Linux Ubuntu 16.04 

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <curl/curl.h>
#include "mysql_connection.h"


using namespace std;

class pp_daemon
{
	CURL *curl;
	CURLcode curl_res;
	string data;
	string address;
	string port;
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	vector <string> temp_values;

	public:
		pp_daemon( const string& node_address, const string& node_port, 
			   const string& db_address, const string& db_user, 
			   const string& db_password );
		~pp_daemon( void );
		void conn_and_get( void );
		void get_temperature( string& sensor_number );
		string push_data_to_db( void );	
};

pp_daemon::pp_daemon( const string& node_address, const string& node_port, 
		      const string& db_address, const string& db_user, 
		      const string& db_password )
{
	// Initialize a tcp iostream
	address = node_address;
	port = node_port;
	curl = curl_easy_init();
	if( !curl ) {
		cerr<<"Failed to init curl"<<endl;
	}
	
	// Create a database connection 
	driver = get_driver_instance();
	con = driver->connect( db_address, db_user, db_password);
	// Connect to the MySQL temperature database
	con->setSchema("temp_data");
}

pp_daemon::~pp_daemon( void )
{
	delete res;
	delete stmt;
	delete con;
	curl_easy_cleanup(curl);
}

static size_t temp_value_write_callback( void *contents, size_t size, 
					     size_t nmemb, string* data )
{
    	size_t newLength = size*nmemb;
    	size_t oldLength = data->size();
    	cout<<"call back"<<endl;
	try {
        	data->resize(oldLength + newLength);
    	} catch(bad_alloc &e) {
        	//handle memory problem
		cerr<<"Failed to allocate memory"<<e.what()<<endl;
		return 0;
	}

	copy((char*)contents,(char*)contents+newLength,data->begin()+oldLength);
	return size*nmemb;
}


void pp_daemon::get_temperature( string& sensor_name )
{

	string url = "http://" + address + ":" + port + "/temp/sensor=" 
						      + sensor_name; 
	if(curl) {
		curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, 
				  temp_value_write_callback);
		curl_easy_setopt( curl, CURLOPT_WRITEDATA, 
				  (void *) &data);	
		curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
 		curl_res = curl_easy_perform(curl);
		if(curl_res != CURLE_OK) {
			cerr<<"curl_easy_perform() failed:"
			    <<curl_easy_strerror(curl_res);
		}
 	}
}

void pp_daemon::conn_and_get( void )
{
	// I have 1 to whatever number of values to get.
	for( int i = 0; i < 4; ++i ) {
		ostringstream ios;
		ios<<i+1;
		get_temperature( ios.str() );
		while( data.size() == 0 ) {
			cout<<"waiting"<<data<<endl;
		}
		cout<<"data"<<data<<endl;
		temp_values.push_back( data );
		data = "";
	}
}

string pp_daemon::push_data_to_db( void )
{
	string ID;
	try {
		
		stmt = con->createStatement();
		
		// We will get the last id
		res = stmt->executeQuery( "select ID from temperature order"\
					  " by ID desc limit 1;" );
		while( res->next() ) {
			ID = res->getString("ID");
		}
	} catch (sql::SQLException &e) {
		cout<<"Failed to get current ID:"<<e.what()<<endl;
	}
	{
		int id = atoi(ID.c_str()) + 1;
		ostringstream id_oss;
		id_oss<<id;
		ID = id_oss.str();
	}
	try{	
		// We will assume that a table already exists in the 
		// DB and we are just making entry to it .
		res = stmt->executeQuery(
			"INSERT INTO temperature ( time_stamp," \
						  "temp_1,"     \
						  "temp_2,"     \
						  "temp_3,"     \
						  "temp_4,"     \
						  "ID )"	\
					"values  ( CURRENT_TIMESTAMP," + 
						  temp_values.at(0)  + "," +
						  temp_values.at(1)  + "," +
						  temp_values.at(2)  + "," +
						  temp_values.at(3)  + "," +
						  ID + 
						");");
	} catch (sql::SQLException &e) {
		cout<<"Failed to push data to DB:"<<e.what()<<endl;
	}
}

void pp_daemon::read_set_conf( void )
{
	//in progress
	string dir = ".pp_app";
#ifdef __WINDOWS__
	CreateDirectory( dir, NULL );
#elif __LINUX__
	mkdir( "~/" + dir, 0777 );
	dir = "~/" + dir;
#endif
	ifstream conf_writer( dir + "pp_app.conf" );
	
	
	 		

}

int 
main( void )
{
	const string node_address = "localhost";
	const string node_port = "8080";
	const string sql_address = "localhost:3306";
	const string sql_user = "root";
	const string sql_password = "root";
	pp_daemon verify( node_address, node_port, sql_address,
			  sql_user, sql_password );
		cout<<"Getting data.."<<endl;
		verify.conn_and_get( );
		cout<<"Pushing data to DB.."<<endl;
		verify.push_data_to_db( ); 
	return 0;	
}
