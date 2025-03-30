#pragma once
#include <WinSock2.h>
#include "../Shared/TelemetryData.h"

struct ClientContext {
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    SOCKET clientSocket;
    std::vector<char> buffer;
    DWORD bytesReceived;

    TelemetryData prevTelemetryData;
    bool isPrevTelemetryDataInitialized = false;
};