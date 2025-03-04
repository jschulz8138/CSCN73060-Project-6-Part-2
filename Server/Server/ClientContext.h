#pragma once
#include <WinSock2.h>
#include <vector>
#include "../../Shared/TelemetryData.h"

#define BUFFER_SIZE 512
#define MAXTHREADS 10

class ClientContext {
public:
    ClientContext(SOCKET clientSocket, int uniqueId);
    ~ClientContext();

    ClientContext& list_append(TelemetryData telemetryData);
    float calculateAverage();

    int getId();
    

    //Must be public for main & worker thread access.
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
private:
    SOCKET clientSocket;

    std::vector<char> buffer;
    DWORD bytesReceived;

    std::vector<TelemetryData> telemetryData;
    int uniqueId;
};