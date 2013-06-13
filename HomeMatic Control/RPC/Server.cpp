#include "Server.h"
#include "RPCMethods.h"

namespace RPC {

void Server::registerMethods()
{
	_server->registerMethod("system.getCapabilities", std::shared_ptr<RPCMethod>(new RPCSystemGetCapabilities()));
	_server->registerMethod("system.listMethods", std::shared_ptr<RPCMethod>(new RPCSystemListMethods(_server)));
	_server->registerMethod("system.methodSignature", std::shared_ptr<RPCMethod>(new RPCSystemMethodSignature(_server)));
	_server->registerMethod("system.multicall", std::shared_ptr<RPCMethod>(new RPCSystemMulticall(_server)));
	_server->registerMethod("getParamsetDescription", std::shared_ptr<RPCMethod>(new RPCGetParamsetDescription()));
	_server->registerMethod("getValue", std::shared_ptr<RPCMethod>(new RPCGetValue()));
	_server->registerMethod("init", std::shared_ptr<RPCMethod>(new RPCInit()));
	_server->registerMethod("listDevices", std::shared_ptr<RPCMethod>(new RPCListDevices()));
	_server->registerMethod("logLevel", std::shared_ptr<RPCMethod>(new RPCLogLevel()));
	_server->registerMethod("setValue", std::shared_ptr<RPCMethod>(new RPCSetValue()));
}

void Server::start()
{
	registerMethods();
	_server->start();
}

} /* namespace RPC */
