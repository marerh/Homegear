#ifndef BASELIB_H_
#define BASELIB_H_

#include "Database/Database.h"
#include "Encoding/XMLRPCDecoder.h"
#include "Encoding/XMLRPCEncoder.h"
#include "FileDescriptorManager/FileDescriptorManager.h"
#include "HelperFunctions/HelperFunctions.h"
#include "Output/Output.h"
#include "RPC/Devices.h"
#include "Settings/Settings.h"
#include "Sockets/SocketOperations.h"
#include "Systems/DeviceFamily.h"
#include "Systems/Peer.h"
#include "Systems/SystemFactory.h"
#include "Systems/UpdateInfo.h"
#include "Threads/Threads.h"

namespace BaseLib
{

class Obj
{
public:
	static Systems::DeviceFamily* family;
	static Obj* ins;

	int32_t debugLevel;
	std::string executablePath;
	FileDescriptorManager fileDescriptorManager;
	Settings settings;
	Database db;
	RPC::Devices rpcDevices;
	Systems::UpdateInfo deviceUpdateInfo;

	Obj(std::string executablePath);
	virtual ~Obj();
private:
	//Non public constructor
};
}
#endif