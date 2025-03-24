#pragma once
enum ProtocolFlag {
	GENERATEID = 0,
	SENDDATA = 1,
	ENDCOMMUNICATION = 2,
	ACK = 3,
	ERR = -1
	//maybe more flags
};