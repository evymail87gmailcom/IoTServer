#pragma once
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <WS2tcpip.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include<vector>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

class DeviceHandler
{
public:
	DeviceHandler();
	int sendToSql(string url, const string user, const string pass, const string database);
	~DeviceHandler();
private:
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	string deviceId, ipAdress, deviceType, deviceCategory, value, unit;
	void parseIncomingMsg(SOCKET clientSocket);
	void buildSQL(string url, const string user, const string pass, const string database);
};

