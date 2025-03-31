#pragma once
#include <WinSock2.h>
#include "../Shared/TelemetryData.h"
#include <iostream>

struct ClientContext {
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    SOCKET clientSocket;
    std::vector<char> buffer;
    DWORD bytesReceived;

    TelemetryData prevTelemetryData;
    bool isPrevTelemetryDataInitialized = false;
    
    ~ClientContext() {
        std::cout << "destructor called" << std::endl;
        while (buffer.size() > 1) {
            buffer.pop_back();
        }
    }
};