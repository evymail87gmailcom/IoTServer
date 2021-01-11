#include "deviceHandler.h"

int main(int argc, const char** argv) {
	
	//Credentials for MySql	
	string url(argc >= 2 ? argv[1] : "insert_sql_url");
	const string user(argc >= 3 ? argv[2] : "insert_sql_username");
	const string pass(argc >= 4 ? argv[3] : "insert_password_to_sql_database_here");
	const string database(argc >= 5 ? argv[4] : "environmentdata");

	//Send the incoming devicevalues to a database in mySql
	DeviceHandler* myHandler = new DeviceHandler;
	myHandler->sendToSql(url, user, pass, database);
	delete myHandler;
}
