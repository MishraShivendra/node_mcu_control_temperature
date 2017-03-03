// Notice: All parts of this code is protected
// with following license:
// https://github.com/MishraShivendra/node_mcu_py_email/blob/master/LICENSE.md
// Corporate/commercial usage is restricted.
// 
// Author : Shivendra Mishra
// Env: gcc 5.4 on Linux Ubuntu 16.04 

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "mysql_connection.h"

using namespace std;

class pp_daemon
{
	string node_address;
	boost::asio::ip::tcp::iostream tcp_stream;
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	vector <string> temp_values;

	public:
		pp_daemon( string node, string db_address,
			   string db_user, string db_password );
		~pp_daemon( void );
		void conn_and_get( void );
		string get_temperature( string sensor_number );
		string push_data_to_db( string data );
};

pp_daemon::pp_daemon( string node, string db_address, 
		      string db_user, string db_password )
{
	// Initialize a tcp iostream
	node_address = node;
	tcp_stream.expires_from_now(boost::posix_time::seconds(2));
	tcp_stream.connect( node_address, "http" );

	// Create a database connection 
	driver = get_driver_instance();
	con = driver->connect( db_address, db_user, db_password);
	// Connect to the MySQL temperature database
	con->setSchema("temperature");
}

pp_daemon::~pp_daemon( void )
{
	tcp_stream.close();
	delete res;
	delete stmt;
	delete con;
}

string pp_daemon::get_temperature( string sensor_name )
{
	tcp_stream << "GET /temp?sensor=" + sensor_name + 
		      " HTTP/1.0\r\n"
		   << "Host:" + node_address + " \r\n"
		   << "Accept: */*\r\n"
		   << "Connection: close\r\n\r\n";
	tcp_stream.flush();
	ostringstream out;
	out<< tcp_stream.rdbuf();
	return out.str();
}

void pp_daemon::conn_and_get( void )
{
	// I have 1 to whatever number of values to get.
	for( int i = 0; i < 4; ++i ) {
		ostringstream ios;
		ios<<i;
		temp_values.push_back( get_temperature( ios.str() ) );
	}
}

string pp_daemon::push_data_to_db( string data )
{
	
	try {
		// We will assume that a table already exists in the 
		// DB and we are just making entry to it .
		stmt = con->createStatement();
		res = stmt->executeQuery(
			"INSERT INTO temperature ( time_stamp," \
						  "temp_1,"     \
						  "temp_2,"     \
						  "temp_3,"     \
						  "temp_4 )"    \
					"values  ( CURRENT_TIMESTAMP," + 
						  temp_values.at(0)  + "," +
						  temp_values.at(1)  + "," +
						  temp_values.at(2)  + "," +
						  temp_values.at(3)  + "," +
						");");
		//syslog( "Made entry to database." );

	} catch (sql::SQLException &e) {
		//syslog( "Running into SQL exception." );
	}
}
