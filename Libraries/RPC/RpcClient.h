/* Copyright 2013-2015 Sathya Laufer
 *
 * Homegear is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Homegear is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Homegear.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#ifndef RPCCLIENT_H_
#define RPCCLIENT_H_

#include "ClientSettings.h"
#include "../../Modules/Base/BaseLib.h"
#include "Auth.h"
#include "RemoteRpcServer.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <list>
#include <mutex>
#include <map>

#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>

namespace RPC
{
class RpcClient {
public:
	RpcClient();
	virtual ~RpcClient();

	void invokeBroadcast(RemoteRpcServer* server, std::string methodName, std::shared_ptr<std::list<std::shared_ptr<BaseLib::RPC::Variable>>> parameters);
	std::shared_ptr<BaseLib::RPC::Variable> invoke(std::shared_ptr<RemoteRpcServer> server, std::string methodName, std::shared_ptr<std::list<std::shared_ptr<BaseLib::RPC::Variable>>> parameters);

	void reset();
protected:
	std::unique_ptr<BaseLib::RPC::RPCDecoder> _rpcDecoder;
	std::unique_ptr<BaseLib::RPC::RPCEncoder> _rpcEncoder;
	std::unique_ptr<BaseLib::RPC::XMLRPCDecoder> _xmlRpcDecoder;
	std::unique_ptr<BaseLib::RPC::XMLRPCEncoder> _xmlRpcEncoder;
	std::unique_ptr<BaseLib::RPC::JsonDecoder> _jsonDecoder;
	std::unique_ptr<BaseLib::RPC::JsonEncoder> _jsonEncoder;
	int32_t _sendCounter = 0;

	void sendRequest(RemoteRpcServer* server, std::vector<char>& data, std::vector<char>& responseData, bool insertHeader, bool& retry);
	std::string getIPAddress(std::string address);
};

}
#endif
