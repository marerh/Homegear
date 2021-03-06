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

#ifndef CENTRAL_H_
#define CENTRAL_H_

#include "../RPC/Variable.h"
#include "../RPC/Device.h"

#include <set>

namespace BaseLib
{

namespace Systems
{

class LogicalDevice;

class Central
{
public:
	Central(BaseLib::Obj* baseLib, LogicalDevice* me);
	virtual ~Central();

	virtual LogicalDevice* logicalDevice() { return _me; }
	virtual int32_t physicalAddress();
	virtual uint64_t getPeerIDFromSerial(std::string serialNumber) { return 0; }

	virtual bool knowsDevice(std::string serialNumber) = 0;
	virtual bool knowsDevice(uint64_t id) = 0;

	virtual std::shared_ptr<RPC::Variable> activateLinkParamset(int32_t clientID, std::string serialNumber, int32_t channel, std::string remoteSerialNumber, int32_t remoteChannel, bool longPress) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> activateLinkParamset(int32_t clientID, uint64_t peerID, int32_t channel, uint64_t remoteID, int32_t remoteChannel, bool longPress) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> addDevice(int32_t clientID, std::string serialNumber) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> addLink(int32_t clientID, std::string senderSerialNumber, int32_t senderChannel, std::string receiverSerialNumber, int32_t receiverChannel, std::string name, std::string description) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> addLink(int32_t clientID, uint64_t senderID, int32_t senderChannel, uint64_t receiverID, int32_t receiverChannel, std::string name, std::string description) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> createDevice(int32_t clientID, int32_t deviceType, std::string serialNumber, int32_t address, int32_t firmwareVersion) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> deleteDevice(int32_t clientID, std::string serialNumber, int32_t flags) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> deleteDevice(int32_t clientID, uint64_t peerID, int32_t flags) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> getAllValues(int32_t clientID, uint64_t peerID, bool returnWriteOnly);
	virtual std::shared_ptr<RPC::Variable> getDeviceDescription(int32_t clientID, std::string serialNumber, int32_t channel);
	virtual std::shared_ptr<RPC::Variable> getDeviceDescription(int32_t clientID, uint64_t id, int32_t channel);
	virtual std::shared_ptr<RPC::Variable> getDeviceInfo(int32_t clientID, uint64_t id, std::map<std::string, bool> fields) = 0;
	virtual std::shared_ptr<RPC::Variable> getPeerID(int32_t clientID, int32_t filterType, std::string filterValue);
	virtual std::shared_ptr<RPC::Variable> getPeerID(int32_t clientID, int32_t address);
	virtual std::shared_ptr<RPC::Variable> getPeerID(int32_t clientID, std::string serialNumber);
	virtual std::shared_ptr<RPC::Variable> getInstallMode(int32_t clientID) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> getLinkInfo(int32_t clientID, std::string senderSerialNumber, int32_t senderChannel, std::string receiverSerialNumber, int32_t receiverChannel);
	virtual std::shared_ptr<RPC::Variable> getLinkInfo(int32_t clientID, uint64_t senderID, int32_t senderChannel, uint64_t receiverID, int32_t receiverChannel);
	virtual std::shared_ptr<RPC::Variable> getLinkPeers(int32_t clientID, std::string serialNumber, int32_t channel);
	virtual std::shared_ptr<RPC::Variable> getLinkPeers(int32_t clientID, uint64_t peerID, int32_t channel);
	virtual std::shared_ptr<RPC::Variable> getLinks(int32_t clientID, std::string serialNumber, int32_t channel, int32_t flags);
	virtual std::shared_ptr<RPC::Variable> getLinks(int32_t clientID, uint64_t peerID, int32_t channel, int32_t flags);
	virtual std::shared_ptr<RPC::Variable> getName(int32_t clientID, uint64_t id);
	virtual std::shared_ptr<RPC::Variable> getParamsetDescription(int32_t clientID, std::string serialNumber, int32_t channel, RPC::ParameterSet::Type::Enum type, std::string remoteSerialNumber, int32_t remoteChannel);
	virtual std::shared_ptr<RPC::Variable> getParamsetDescription(int32_t clientID, uint64_t peerID, int32_t channel, RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel);
	virtual std::shared_ptr<RPC::Variable> getParamsetId(int32_t clientID, std::string serialNumber, uint32_t channel, RPC::ParameterSet::Type::Enum type, std::string remoteSerialNumber, int32_t remoteChannel);
	virtual std::shared_ptr<RPC::Variable> getParamsetId(int32_t clientID, uint64_t peerID, uint32_t channel, RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel);
	virtual std::shared_ptr<RPC::Variable> getParamset(int32_t clientID, std::string serialNumber, int32_t channel, RPC::ParameterSet::Type::Enum type, std::string remoteSerialNumber, int32_t remoteChannel);
	virtual std::shared_ptr<RPC::Variable> getParamset(int32_t clientID, uint64_t peerID, int32_t channel, RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel);
	virtual std::shared_ptr<RPC::Variable> getServiceMessages(int32_t clientID, bool returnID);
	virtual std::shared_ptr<RPC::Variable> getValue(int32_t clientID, std::string serialNumber, uint32_t channel, std::string valueKey, bool requestFromDevice, bool asynchronous);
	virtual std::shared_ptr<RPC::Variable> getValue(int32_t clientID, uint64_t id, uint32_t channel, std::string valueKey, bool requestFromDevice, bool asynchronous);
	virtual std::shared_ptr<RPC::Variable> listDevices(int32_t clientID, bool channels, std::map<std::string, bool> fields);
	virtual std::shared_ptr<RPC::Variable> listDevices(int32_t clientID, bool channels, std::map<std::string, bool> fields, std::shared_ptr<std::set<uint64_t>> knownDevices);
	virtual std::shared_ptr<RPC::Variable> listTeams(int32_t clientID) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> putParamset(int32_t clientID, std::string serialNumber, int32_t channel, RPC::ParameterSet::Type::Enum type, std::string remoteSerialNumber, int32_t remoteChannel, std::shared_ptr<RPC::Variable> paramset) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> putParamset(int32_t clientID, uint64_t peerID, int32_t channel, RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, std::shared_ptr<RPC::Variable> paramset) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> reportValueUsage(int32_t clientID, std::string serialNumber);
	virtual std::shared_ptr<RPC::Variable> removeLink(int32_t clientID, std::string senderSerialNumber, int32_t senderChannel, std::string receiverSerialNumber, int32_t receiverChannel) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> removeLink(int32_t clientID, uint64_t senderID, int32_t senderChannel, uint64_t receiverID, int32_t receiverChannel) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> rssiInfo(int32_t clientID);
	virtual std::shared_ptr<RPC::Variable> searchDevices(int32_t clientID) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }

	/**
     * RPC function to change the ID of the peer.
     *
     * @param oldPeerID The current ID of the peer.
     * @param newPeerID The new ID of the peer.
     * @return Returns "RPC void" on success or RPC error "-100" when the new peer ID is invalid and error "-101" when the new peer ID is already in use.
     */
    virtual std::shared_ptr<BaseLib::RPC::Variable> setId(int32_t clientID, uint64_t oldPeerID, uint64_t newPeerID);
	virtual std::shared_ptr<RPC::Variable> setInstallMode(int32_t clientID, bool on, uint32_t duration = 60, bool debugOutput = true) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> setInterface(int32_t clientID, uint64_t peerID, std::string interfaceID) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> setLinkInfo(int32_t clientID, std::string senderSerialNumber, int32_t senderChannel, std::string receiverSerialNumber, int32_t receiverChannel, std::string name, std::string description);
	virtual std::shared_ptr<RPC::Variable> setLinkInfo(int32_t clientID, uint64_t senderID, int32_t senderChannel, uint64_t receiverID, int32_t receiverChannel, std::string name, std::string description);
	virtual std::shared_ptr<RPC::Variable> setName(int32_t clientID, uint64_t id, std::string name);
	virtual std::shared_ptr<RPC::Variable> setTeam(int32_t clientID, std::string serialNumber, int32_t channel, std::string teamSerialNumber, int32_t teamChannel, bool force = false, bool burst = true) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> setTeam(int32_t clientID, uint64_t peerID, int32_t channel, uint64_t teamID, int32_t teamChannel, bool force = false, bool burst = true) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
	virtual std::shared_ptr<RPC::Variable> setValue(int32_t clientID, std::string serialNumber, uint32_t channel, std::string valueKey, std::shared_ptr<RPC::Variable> value);
	virtual std::shared_ptr<RPC::Variable> setValue(int32_t clientID, uint64_t id, uint32_t channel, std::string valueKey, std::shared_ptr<RPC::Variable> value);
	virtual std::shared_ptr<RPC::Variable> updateFirmware(int32_t clientID, std::vector<uint64_t> ids, bool manual) { return RPC::Variable::createError(-32601, "Method not implemented for this central."); }
protected:
	LogicalDevice* _me = nullptr;
private:
	BaseLib::Obj* _baseLib = nullptr;
	Central(const Central&);
	Central& operator=(const Central&);
};

}
}
#endif /* CENTRAL_H_ */
