/* Copyright 2013-2014 Sathya Laufer
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

#ifndef PEER_H_
#define PEER_H_

#include "../RPC/Device.h"
#include "../Database/DatabaseTypes.h"
#include "ServiceMessages.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace BaseLib
{
namespace Systems
{
class LogicalDevice;
class Central;

class RPCConfigurationParameter
{
public:
	RPCConfigurationParameter() {}
	virtual ~RPCConfigurationParameter() {}

	uint64_t databaseID = 0;
	std::shared_ptr<RPC::Parameter> rpcParameter;
	std::vector<uint8_t> data;
	std::vector<uint8_t> partialData;
};

class ConfigDataBlock
{
public:
	ConfigDataBlock() {}
	virtual ~ConfigDataBlock() {}

	uint32_t databaseID = 0;
	std::vector<uint8_t> data;
};

class BasicPeer
{
public:
	BasicPeer() {}
	BasicPeer(int32_t addr, std::string serial, bool hid) { address = addr; serialNumber = serial; hidden = hid; }
	virtual ~BasicPeer() {}

	bool isSender = false;
	uint64_t id = 0;
	int32_t address = 0;
	std::string serialNumber;
	int32_t channel = 0;
	int32_t physicalIndexOffset = 0;
	bool hidden = false;
	std::string linkName;
	std::string linkDescription;
	std::shared_ptr<LogicalDevice> device;
	std::vector<uint8_t> data;
	int32_t configEEPROMAddress = -1;
};

class Peer : public ServiceMessages::IServiceEventSink, public IEvents
{
public:
	//Event handling
	class IPeerEventSink : public IEventSinkBase
	{
	public:
		//Database
			//General
			virtual void onCreateSavepoint(std::string name) = 0;
			virtual void onReleaseSavepoint(std::string name) = 0;

			//Metadata
			virtual void onDeleteMetadata(uint64_t peerID, std::string serialNumber, std::string dataID = "") = 0;

			//Peer
			virtual void onDeletePeer(uint64_t id) = 0;
			virtual uint64_t onSavePeer(uint64_t id, uint32_t parentID, int32_t address, std::string serialNumber) = 0;
			virtual uint64_t onSavePeerParameter(uint64_t peerID, Database::DataRow data) = 0;
			virtual uint64_t onSavePeerVariable(uint64_t peerID, Database::DataRow data) = 0;
			virtual std::shared_ptr<Database::DataTable> onGetPeerParameters(uint64_t peerID) = 0;
			virtual std::shared_ptr<Database::DataTable> onGetPeerVariables(uint64_t peerID) = 0;
			virtual void onDeletePeerParameter(uint64_t peerID, Database::DataRow data) = 0;
			virtual bool onSetPeerID(uint64_t oldPeerID, uint64_t newPeerID) = 0;

			//Service messages
			virtual std::shared_ptr<Database::DataTable> onGetServiceMessages(uint64_t peerID) = 0;
			virtual uint64_t onSaveServiceMessage(uint64_t peerID, Database::DataRow data) = 0;
			virtual void onDeleteServiceMessage(uint64_t databaseID) = 0;
		//End database

		virtual void onRPCEvent(uint64_t id, int32_t channel, std::string deviceAddress, std::shared_ptr<std::vector<std::string>> valueKeys, std::shared_ptr<std::vector<std::shared_ptr<RPC::RPCVariable>>> values) = 0;
		virtual void onRPCUpdateDevice(uint64_t id, int32_t channel, std::string address, int32_t hint) = 0;
		virtual void onEvent(uint64_t peerID, int32_t channel, std::shared_ptr<std::vector<std::string>> variables, std::shared_ptr<std::vector<std::shared_ptr<BaseLib::RPC::RPCVariable>>> values) = 0;
	};
	//End event handling

	bool deleting = false; //Needed, so the peer gets not saved in central's worker thread while being deleted

	std::unordered_map<uint32_t, ConfigDataBlock> binaryConfig;
	std::unordered_map<uint32_t, std::unordered_map<std::string, RPCConfigurationParameter>> configCentral;
	std::unordered_map<uint32_t, std::unordered_map<std::string, RPCConfigurationParameter>> valuesCentral;
	std::unordered_map<uint32_t, std::unordered_map<int32_t, std::unordered_map<uint32_t, std::unordered_map<std::string, RPCConfigurationParameter>>>> linksCentral;
	std::shared_ptr<RPC::Device> rpcDevice;
	std::shared_ptr<ServiceMessages> serviceMessages;

	Peer(BaseLib::Obj* baseLib, uint32_t parentID, bool centralFeatures, IPeerEventSink* eventHandler);
	Peer(BaseLib::Obj* baseLib, int32_t id, int32_t address, std::string serialNumber, uint32_t parentID, bool centralFeatures, IPeerEventSink* eventHandler);
	virtual ~Peer();
	virtual void dispose();

	//Features
	virtual bool wireless() = 0;
	//End features

	//In table peers:
	virtual uint32_t getParentID() { return _parentID; }
	virtual int32_t getAddress() { return _address; }
	virtual uint64_t getID() { return _peerID; }
	virtual void setID(uint64_t id);
	virtual void setAddress(int32_t value) { _address = value; if(_peerID > 0) save(true, false, false); }
	virtual std::string getSerialNumber() { return _serialNumber; }
	virtual void setSerialNumber(std::string serialNumber);
	//End

	//In table variables:
	virtual int32_t getFirmwareVersion() { return _firmwareVersion; }
	virtual void setFirmwareVersion(int32_t value) { _firmwareVersion = value; saveVariable(1001, value); }
	virtual LogicalDeviceType getDeviceType() { return _deviceType; }
	virtual void setDeviceType(LogicalDeviceType value) { _deviceType = value; saveVariable(1002, (int32_t)_deviceType.type()); }
    virtual std::string getName() { return _name; }
	virtual void setName(std::string value) { _name = value; saveVariable(1000, _name); }
    //End

	virtual std::string handleCLICommand(std::string command) = 0;
	virtual RPC::Device::RXModes::Enum getRXModes();
	virtual bool isTeam() { return false; }
	virtual void setLastPacketReceived();
	virtual uint32_t getLastPacketReceived() { return _lastPacketReceived; }
	virtual bool pendingQueuesEmpty() { return true; }
	virtual void enqueuePendingQueues() {}
	virtual int32_t getChannelGroupedWith(int32_t channel) = 0;
	virtual int32_t getNewFirmwareVersion() = 0;
	virtual std::string getFirmwareVersionString(int32_t firmwareVersion) = 0;
    virtual bool firmwareUpdateAvailable() = 0;

	virtual bool load(LogicalDevice* device) { return false; }
	virtual void save(bool savePeer, bool saveVariables, bool saveCentralConfig);
	virtual void loadConfig();
    virtual void saveConfig();
	virtual void saveParameter(uint32_t parameterID, RPC::ParameterSet::Type::Enum parameterSetType, uint32_t channel, std::string parameterName, std::vector<uint8_t>& value, int32_t remoteAddress = 0, uint32_t remoteChannel = 0);
	virtual void saveParameter(uint32_t parameterID, uint32_t address, std::vector<uint8_t>& value);
	virtual void saveParameter(uint32_t parameterID, std::vector<uint8_t>& value);
	virtual void loadVariables(LogicalDevice* device = nullptr, std::shared_ptr<BaseLib::Database::DataTable> rows = std::shared_ptr<BaseLib::Database::DataTable>());
	virtual void saveVariables();
	virtual void saveVariable(uint32_t index, int32_t intValue);
    virtual void saveVariable(uint32_t index, int64_t intValue);
    virtual void saveVariable(uint32_t index, std::string& stringValue);
    virtual void saveVariable(uint32_t index, std::vector<uint8_t>& binaryValue);
    virtual void deleteFromDatabase();
    virtual void savePeers() = 0;

    virtual std::shared_ptr<BasicPeer> getPeer(int32_t channel, int32_t address, int32_t remoteChannel = -1);
	virtual std::shared_ptr<BasicPeer> getPeer(int32_t channel, uint64_t id, int32_t remoteChannel = -1);
	virtual std::shared_ptr<BasicPeer> getPeer(int32_t channel, std::string serialNumber, int32_t remoteChannel = -1);

    virtual std::shared_ptr<Central> getCentral() = 0;
	virtual std::shared_ptr<LogicalDevice> getDevice(int32_t address) = 0;

    //RPC methods
	virtual std::shared_ptr<RPC::RPCVariable> getAllValues(bool returnWriteOnly);
	virtual std::shared_ptr<std::vector<std::shared_ptr<RPC::RPCVariable>>> getDeviceDescriptions(bool channels, std::map<std::string, bool> fields);
    virtual std::shared_ptr<RPC::RPCVariable> getDeviceDescription(int32_t channel, std::map<std::string, bool> fields);
    virtual std::shared_ptr<RPC::RPCVariable> getDeviceInfo(std::map<std::string, bool> fields);
    virtual std::shared_ptr<RPC::RPCVariable> getLink(int32_t channel, int32_t flags, bool avoidDuplicates);
    virtual std::shared_ptr<RPC::RPCVariable> getLinkInfo(int32_t senderChannel, uint64_t receiverID, int32_t receiverChannel);
	virtual std::shared_ptr<RPC::RPCVariable> setLinkInfo(int32_t senderChannel, uint64_t receiverID, int32_t receiverChannel, std::string name, std::string description);
	virtual std::shared_ptr<RPC::RPCVariable> getLinkPeers(int32_t channel, bool returnID);
    virtual std::shared_ptr<RPC::RPCVariable> getParamset(int32_t channel, RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel) = 0;
    virtual std::shared_ptr<RPC::RPCVariable> getParamsetDescription(std::shared_ptr<BaseLib::RPC::ParameterSet> parameterSet);
    virtual std::shared_ptr<RPC::RPCVariable> getParamsetDescription(int32_t channel, BaseLib::RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel) = 0;
    virtual std::shared_ptr<RPC::RPCVariable> getParamsetId(uint32_t channel, RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel);
    virtual std::shared_ptr<RPC::RPCVariable> getServiceMessages(bool returnID);
    virtual std::shared_ptr<BaseLib::RPC::RPCVariable> getValue(uint32_t channel, std::string valueKey);
    virtual std::shared_ptr<BaseLib::RPC::RPCVariable> putParamset(int32_t channel, BaseLib::RPC::ParameterSet::Type::Enum type, uint64_t remoteID, int32_t remoteChannel, std::shared_ptr<BaseLib::RPC::RPCVariable> variables, bool onlyPushing = false) = 0;

    /**
     * RPC function to change the ID of the peer.
     *
     * @param newPeerID The new ID of the peer.
     * @return Returns "RPC void" on success or RPC error "-100" when the new peer ID is invalid and error "-101" when the new peer ID is already in use.
     */
    virtual std::shared_ptr<BaseLib::RPC::RPCVariable> setId(uint64_t newPeerID);
	virtual std::shared_ptr<BaseLib::RPC::RPCVariable> setValue(uint32_t channel, std::string valueKey, std::shared_ptr<BaseLib::RPC::RPCVariable> value);
    //End RPC methods
protected:
    BaseLib::Obj* _bl = nullptr;
    std::map<uint32_t, uint32_t> _variableDatabaseIDs;
    std::shared_ptr<Central> _central;

	//In table peers:
	uint64_t _peerID = 0;
	uint32_t _parentID = 0;
	int32_t _address = 0;
	std::string _serialNumber;
	//End

	//In table variables:
	int32_t _firmwareVersion = 0;
	LogicalDeviceType _deviceType;
	std::unordered_map<int32_t, std::vector<std::shared_ptr<BasicPeer>>> _peers;
	std::string _name;
	//End

	RPC::Device::RXModes::Enum _rxModes = RPC::Device::RXModes::Enum::none;

	bool _disposing = false;
	bool _centralFeatures = false;
	uint32_t _lastPacketReceived = 0;
	std::mutex _databaseMutex;

	//Event handling
	//Database
		//General
		virtual void raiseCreateSavepoint(std::string name);
		virtual void raiseReleaseSavepoint(std::string name);

		//Metadata
		virtual void raiseDeleteMetadata(uint64_t peerID, std::string serialNumber, std::string dataID = "");

		//Peer
		virtual void raiseDeletePeer();
		virtual uint64_t raiseSavePeer();
		virtual uint64_t raiseSavePeerParameter(Database::DataRow data);
		virtual uint64_t raiseSavePeerVariable(Database::DataRow data);
		virtual std::shared_ptr<Database::DataTable> raiseGetPeerParameters();
		virtual std::shared_ptr<Database::DataTable> raiseGetPeerVariables();
		virtual void raiseDeletePeerParameter(Database::DataRow data);
		virtual bool raiseSetPeerID(uint64_t newPeerID);

		//Service messages
		virtual std::shared_ptr<Database::DataTable> raiseGetServiceMessages();
		virtual uint64_t raiseSaveServiceMessage(Database::DataRow data);
		virtual void raiseDeleteServiceMessage(uint64_t id);
	//End database

	virtual void raiseRPCEvent(uint64_t id, int32_t channel, std::string deviceAddress, std::shared_ptr<std::vector<std::string>> valueKeys, std::shared_ptr<std::vector<std::shared_ptr<RPC::RPCVariable>>> values);
	virtual void raiseRPCUpdateDevice(uint64_t id, int32_t channel, std::string address, int32_t hint);
	virtual void raiseEvent(uint64_t peerID, int32_t channel, std::shared_ptr<std::vector<std::string>> variables, std::shared_ptr<std::vector<std::shared_ptr<BaseLib::RPC::RPCVariable>>> values);
	//End event handling

	//ServiceMessages event handling
	virtual void onConfigPending(bool configPending);

	virtual void onRPCEvent(uint64_t id, int32_t channel, std::string deviceAddress, std::shared_ptr<std::vector<std::string>> valueKeys, std::shared_ptr<std::vector<std::shared_ptr<RPC::RPCVariable>>> values);
	virtual void onSaveParameter(std::string name, uint32_t channel, std::vector<uint8_t>& data);
	virtual std::shared_ptr<Database::DataTable> onGetServiceMessages();
	virtual uint64_t onSaveServiceMessage(Database::DataRow data);
	virtual void onDeleteServiceMessage(uint64_t databaseID);
	virtual void onEnqueuePendingQueues();
	//End ServiceMessages event handling

	virtual std::shared_ptr<RPC::ParameterSet> getParameterSet(int32_t channel, BaseLib::RPC::ParameterSet::Type::Enum type) = 0;
};

}
}
#endif
