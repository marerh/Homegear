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

#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include "delegate.hpp"
#include "../Base/BaseLib.h"
#include "MAXPacket.h"

#include <iostream>
#include <string>
#include <list>
#include <queue>
#include <thread>
#include <mutex>

namespace MAX
{
class MAXPeer;
class MAXMessage;
class MAXDevice;
class PendingQueues;

enum class QueueEntryType { UNDEFINED, MESSAGE, PACKET };

class CallbackFunctionParameter
{
public:
	std::vector<int64_t> integers;
	std::vector<std::string> strings;

	CallbackFunctionParameter() {}
	virtual ~CallbackFunctionParameter() {}
};

class PacketQueueEntry {
protected:
	QueueEntryType _type = QueueEntryType::UNDEFINED;
	std::shared_ptr<MAXMessage> _message;
	std::shared_ptr<MAXPacket> _packet;
public:
	bool stealthy = false;
	bool forceResend= false;

	PacketQueueEntry() {}
	virtual ~PacketQueueEntry() {}
	QueueEntryType getType() { return _type; }
	void setType(QueueEntryType type) { _type = type; }
	std::shared_ptr<MAXPacket> getPacket() { return _packet; }
	void setPacket(std::shared_ptr<MAXPacket> packet, bool setQueueEntryType) { _packet = packet; if(setQueueEntryType) _type = QueueEntryType::PACKET; }
	std::shared_ptr<MAXMessage> getMessage() { return _message; }
	void setMessage(std::shared_ptr<MAXMessage> message, bool setQueueEntryType) { _message = message; if(setQueueEntryType) _type = QueueEntryType::MESSAGE; }
};

enum class PacketQueueType { EMPTY, DEFAULT, CONFIG, PAIRING, PAIRINGCENTRAL, UNPAIRING, PEER };

class PacketQueue
{
    protected:
		bool _disposing = false;
		//I'm using list, so iterators are not invalidated
        std::list<PacketQueueEntry> _queue;
        std::shared_ptr<BaseLib::Systems::IPhysicalInterface> _physicalInterface;
        std::shared_ptr<PendingQueues> _pendingQueues;
        std::mutex _queueMutex;
        PacketQueueType _queueType;
        bool _stopResendThread = false;
        std::mutex _resendThreadMutex;
        std::thread _resendThread;
        int32_t _resendCounter = 0;
        uint32_t _resendThreadId = 0;
        bool _stopPopWaitThread = false;
        uint32_t _popWaitThreadId = 0;
        std::thread _popWaitThread;
        std::thread _sendThread;
        std::mutex _sendThreadMutex;
        std::thread _startResendThread;
        std::mutex _startResendThreadMutex;
        std::thread _pushPendingQueueThread;
        std::mutex _pushPendingQueueThreadMutex;
        bool _workingOnPendingQueue = false;
        int64_t _lastPop = 0;
        void (MAXDevice::*_queueProcessed)() = nullptr;
        void pushPendingQueue();
        void sleepAndPushPendingQueue();
        void resend(uint32_t threadId, bool burst);
        void startResendThread(bool force);
        void stopResendThread();
        void popWaitThread(uint32_t threadId, uint32_t waitingTime);
        void stopPopWaitThread();
        void nextQueueEntry();
    public:
        uint32_t retries = 3;
        uint32_t id = 0;
        uint32_t pendingQueueID = 0;
        std::shared_ptr<int64_t> lastAction;
        bool noSending = false;
        MAXDevice* device = nullptr;
        std::shared_ptr<MAXPeer> peer;
        PacketQueueType getQueueType() { return _queueType; }
        std::list<PacketQueueEntry>* getQueue() { return &_queue; }
        void setQueueType(PacketQueueType queueType) {  _queueType = queueType; }
        std::shared_ptr<BaseLib::Systems::IPhysicalInterface> getPhysicalInterface() { return _physicalInterface; }
        std::string parameterName;
        int32_t channel = -1;

        void push(std::shared_ptr<MAXMessage> message, bool forceResend = false);
        void push(std::shared_ptr<MAXMessage> message, std::shared_ptr<MAXPacket> packet, bool forceResend = false);
        void pushFront(std::shared_ptr<MAXPacket> packet, bool stealthy = false, bool popBeforePushing = false, bool forceResend = false);
        void push(std::shared_ptr<MAXPacket> packet, bool forceResend = false, bool stealthy = false);
        void push(std::shared_ptr<PendingQueues>& pendingQueues);
        void push(std::shared_ptr<PacketQueue> pendingQueue, bool popImmediately, bool clearPendingQueues);
        PacketQueueEntry* front() { return &_queue.front(); }
        void pop();
        void popWait(uint32_t waitingTime);
        bool isEmpty();
        bool pendingQueuesEmpty();
        void clear();
        void setWakeOnRadio(bool value);
        void send(std::shared_ptr<MAXPacket> packet, bool stealthy);
        void keepAlive();
        void longKeepAlive();
        void dispose();
        void serialize(std::vector<uint8_t>& encodedData);
        void unserialize(std::shared_ptr<std::vector<char>> serializedData, MAXDevice* device, uint32_t position = 0);

        PacketQueue();
        PacketQueue(std::shared_ptr<BaseLib::Systems::IPhysicalInterface> physicalInterface);
        PacketQueue(std::shared_ptr<BaseLib::Systems::IPhysicalInterface> physicalInterface, PacketQueueType queueType);
        virtual ~PacketQueue();
};
}
#endif
