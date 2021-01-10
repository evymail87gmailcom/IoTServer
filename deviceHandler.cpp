#include "deviceHandler.h"
int DeviceHandler::sendToSql(string url, const string user, const string pass, const string database) {

	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0)
	{
		cerr << "Can't initialize winsock! Quitting" << endl;
		return 2;
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		WSACleanup();
		return 2;
	}

	//Bind the an ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	::bind(listening, (sockaddr*)&hint, sizeof(hint));

	//Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//Wait for a connection
	sockaddr_in client;

	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Not Accepted!" << endl;
		cout << WSAGetLastError() << endl;
		//cout << getA << endl;

		WSACleanup();
		return 2;
	}

	//Clients remote name
	char host[NI_MAXHOST];
	//Service (port) the client is connected on
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "Connected on port: " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "Connected on port: " << ntohs(client.sin_port) << endl;
	}
	//Close listening socket
	closesocket(listening);
	
	while (true)
	{	//Listen and parse incoming message
		parseIncomingMsg(clientSocket);
		//Build the string to send to SQL
		buildSQL(url, user, pass, database);
	}

	//Close the socket
	closesocket(clientSocket);

	//Cleanup winsock
	WSACleanup();
};
void DeviceHandler::parseIncomingMsg(SOCKET clientSocket) {

	//Collect incoming message
	char buf[4096];
	int bytesRecieved;
	ZeroMemory(buf, 4096);
	//Wait for client to send data
	bytesRecieved = recv(clientSocket, buf, 4096, 0);
	if (bytesRecieved == SOCKET_ERROR)
	{
		cerr << "Error in recv(). Quitting" << endl;
	}

	cout << string(buf, 0, bytesRecieved) << endl;

	//Parsing incoming message: 
	string incomingMsg = string(buf, 0, bytesRecieved);
	vector<string> result;

	//Create string stream from the string
	stringstream s_stream(incomingMsg);
	while (s_stream.good()) {
		string substr;

		//Get first string delimited by comma
		getline(s_stream, substr, ',');
		result.push_back(substr);
	}
	//Print all splitted strings
	for (int i = 0; i < result.size(); i++) {
		cout << result.at(i) << endl;
	}
	//Give each variable in the substringvector the characteristics needed for the sql statement
	for (int i = 0; i <= result.size(); i++)
	{
		switch (i)
		{
		case 0: {
			deviceId = "'" + result.at(i) + "', ";
			cout << "deviceId: " << deviceId << endl;
			break;
		}
		case 1: {
			ipAdress = "'" + result.at(i) + "', ";
			cout << "ipAdress: " << ipAdress << endl;
			break;
		}
		case 2: {
			deviceType = "'" + result.at(i) + "', ";
			cout << "deviceType: " << deviceType << endl;
			break;
		}
		case 3: {
			deviceCategory = "'" + result.at(i) + "', ";
			cout << "deviceCategory: " << deviceCategory << endl;
			break;
		}
		case 4: {
			value = result.at(i) + ", ";
			cout << "value: " << value << endl;
			break;
		}
		case 5: {
			unit = "'" + result.at(i) + "'";
			cout << "unit: " << unit << endl;
			break;
		}
		default:
			break;
		}
	}
}
void DeviceHandler::buildSQL(string url, const string user, const string pass, const string database) {
	sql::Driver* driver = get_driver_instance();
	std::auto_ptr<sql::Connection> con(driver->connect(url, user, pass));
	con->setSchema(database);
	std::auto_ptr<sql::Statement> stmt(con->createStatement());

	try {
		//Build string to send values
		string insert = "INSERT INTO environmentdata.tempvalues (DeviceId, IpAdress, DeviceType, DeviceCategory, Value, Unit) VALUES(";
		string addValues = insert + deviceId + ipAdress + deviceType + deviceCategory + value + unit + ");";
		cout << addValues << endl;
		stmt->execute(addValues);
	}
	catch (sql::SQLException& e) {

		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		cout << EXIT_FAILURE << endl;
	}
};
DeviceHandler::DeviceHandler()
{
}
DeviceHandler::~DeviceHandler()
{
}