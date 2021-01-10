#include "deviceHandler.h"

int main(int argc, const char** argv) {
	
	//Credentials for MySql	
	string url(argc >= 2 ? argv[1] : "127.0.0.1");
	const string user(argc >= 3 ? argv[2] : "root");
	const string pass(argc >= 4 ? argv[3] : "kaLLebANaN1967");
	const string database(argc >= 5 ? argv[4] : "environmentdata");

	//Send the incoming devicevalues to a database in mySql
	DeviceHandler* myHandler = new DeviceHandler;
	myHandler->sendToSql(url, user, pass, database);
	delete myHandler;
}