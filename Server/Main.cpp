#include <iostream>
#include "Server.h"
#include "../Shared/Packet.h"
#include <vector>
#include <pqxx/pqxx> 

#define MAXTHREADS 10

int main(int argc, char* argv[], char* envp[]) {

	std::string connStr = "host=localhost port=5432 dbname=telemetry user=postgres password=yourpassword";

	Server s = Server(connStr, 8080);
	s.MainThread();
}