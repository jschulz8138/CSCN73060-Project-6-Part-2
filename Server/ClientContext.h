#pragma once
#include <WinSock2.h>

struct ClientContext {
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    SOCKET clientSocket;
    std::vector<char> buffer;
    DWORD bytesReceived;
};