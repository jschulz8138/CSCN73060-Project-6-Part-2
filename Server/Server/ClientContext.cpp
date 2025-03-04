#pragma comment(lib, "Ws2_32.lib")
#include "ClientContext.h"

ClientContext::ClientContext(SOCKET clientSocket, int uniqueId)
{
	this->clientSocket = clientSocket;
	
	//Make sure we clear all memory at this address. Otherwise something might go wrong.
	ZeroMemory(&this->overlapped, sizeof(OVERLAPPED));
	
	this->buffer.resize(BUFFER_SIZE);
	this->wsaBuf.buf = this->buffer.data();
	this->wsaBuf.len = BUFFER_SIZE;

	this->uniqueId = uniqueId;
	this->bytesReceived = 0;
}

ClientContext::~ClientContext()
{
	if (closesocket(this->clientSocket) != 0)
	{
		//Implement Logging
	}
}

ClientContext& ClientContext::list_append(TelemetryData telemetryData)
{
	this->telemetryData.push_back(telemetryData);
	return *this;
}

float ClientContext::calculateAverage()
{
	//Placeholder Function
	return 0.0f;
}

int ClientContext::getId()
{
	return this->uniqueId;
}
