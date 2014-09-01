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

#include "QueueManager.h"
#include "../Base/BaseLib.h"
#include "GD.h"

namespace Insteon
{
QueueData::QueueData(std::shared_ptr<IPhysicalInterface> physicalInterface)
{
	if(!physicalInterface) physicalInterface = GD::defaultPhysicalInterface;
	queue = std::shared_ptr<PacketQueue>(new PacketQueue(physicalInterface));
	lastAction.reset(new int64_t);
	*lastAction = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

QueueManager::QueueManager()
{
}

QueueManager::~QueueManager()
{
	try
	{
		if(!_disposing) dispose();
		_workerThreadMutex.lock();
		if(_workerThread.joinable()) _workerThread.join();
		_workerThreadMutex.unlock();
		_resetQueueThreadMutex.lock();
		if(_resetQueueThread.joinable()) _resetQueueThread.join(); //After waiting for worker thread!
		_resetQueueThreadMutex.unlock();
	}
    catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
}

void QueueManager::dispose(bool wait)
{
	_disposing = true;
	_stopWorkerThread = true;
}

void QueueManager::worker()
{
	try
	{
		std::chrono::milliseconds sleepingTime(500);
		int32_t lastQueueOuter;
		lastQueueOuter = 0;
		std::string lastQueueInner;
		while(!_stopWorkerThread)
		{
			try
			{
				std::this_thread::sleep_for(sleepingTime);
				if(_stopWorkerThread) return;
				_queueMutex.lock();
				if(!_queues.empty())
				{
					std::unordered_map<int32_t, std::map<std::string, std::shared_ptr<QueueData>>>::iterator nextQueueOuter = _queues.find(lastQueueOuter);
					if(nextQueueOuter != _queues.end())
					{
						std::map<std::string, std::shared_ptr<QueueData>>::iterator nextQueueInner = nextQueueOuter->second.find(lastQueueInner);
						nextQueueInner++;
						if(nextQueueInner == nextQueueOuter->second.end())
						{
							nextQueueOuter++;
							if(nextQueueOuter == _queues.end()) nextQueueOuter = _queues.begin();
							if(!nextQueueOuter->second.empty()) lastQueueInner = nextQueueOuter->second.begin()->first;
							else lastQueueInner = "";
						}
						else lastQueueInner = nextQueueInner->first;
					}
					else
					{
						nextQueueOuter = _queues.begin();
						if(!nextQueueOuter->second.empty()) lastQueueInner = nextQueueOuter->second.begin()->first;
					}
					lastQueueOuter = nextQueueOuter->first;
				}
				std::shared_ptr<QueueData> queue;
				if(_queues.find(lastQueueOuter) != _queues.end() && _queues.at(lastQueueOuter).find(lastQueueInner) != _queues.at(lastQueueOuter).end()) queue = _queues.at(lastQueueOuter).at(lastQueueInner);
				_queueMutex.unlock();
				if(queue)
				{
					try
					{
						_resetQueueThreadMutex.lock();
						if(_disposing)
						{
							_resetQueueThreadMutex.unlock();
							return;
						}
						if(_resetQueueThread.joinable()) _resetQueueThread.join();
						//Has to be called in a thread as resetQueue might cause queuing (retrying in setUnreach) and therefore a deadlock
						_resetQueueThread = std::thread(&QueueManager::resetQueue, this, lastQueueOuter, lastQueueInner, queue->id);
					}
					catch(const std::exception& ex)
					{
						GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
					}
					catch(BaseLib::Exception& ex)
					{
						GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
					}
					catch(...)
					{
						GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					}
					_resetQueueThreadMutex.unlock();
				}
			}
			catch(const std::exception& ex)
			{
				_queueMutex.unlock();
				GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
			}
			catch(BaseLib::Exception& ex)
			{
				_queueMutex.unlock();
				GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
			}
			catch(...)
			{
				_queueMutex.unlock();
				GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
			}
		}
	}
    catch(const std::exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
}

std::shared_ptr<PacketQueue> QueueManager::createQueue(InsteonDevice* device, std::shared_ptr<IPhysicalInterface> physicalInterface, PacketQueueType queueType, int32_t address)
{
	try
	{
		if(_disposing) return std::shared_ptr<PacketQueue>();
		if(!physicalInterface) physicalInterface = GD::defaultPhysicalInterface;
		_queueMutex.lock();
		if(_stopWorkerThread)
		{
			_queueMutex.unlock();
			_workerThreadMutex.lock();
			if(_stopWorkerThread)
			{
				if(_disposing)
				{
					_workerThreadMutex.unlock();
					return std::shared_ptr<PacketQueue>();
				}
				try //Catch "Resource deadlock avoided". Error should be fixed, but just in case.
				{
					if(_workerThread.joinable()) _workerThread.join();
					_stopWorkerThread = false;
					_workerThread = std::thread(&QueueManager::worker, this);
					BaseLib::Threads::setThreadPriority(GD::bl, _workerThread.native_handle(), GD::bl->settings.workerThreadPriority(), GD::bl->settings.workerThreadPolicy());
				}
				catch(const std::exception& ex)
				{
					GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
				}
				catch(...)
				{
					GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
			}
			_workerThreadMutex.unlock();
		}
		else if(_queues.find(address) != _queues.end())
		{
			if(_queues.at(address).find(physicalInterface->getID()) != _queues.at(address).end())
			{
				_queues.at(address).erase(physicalInterface->getID());
				_queueMutex.unlock();
			}
			else _queueMutex.unlock();
		}
		else _queueMutex.unlock();

		_queueMutex.lock();
		std::shared_ptr<QueueData> queueData(new QueueData(physicalInterface));
		queueData->queue->setQueueType(queueType);
		queueData->queue->device = device;
		queueData->queue->lastAction = queueData->lastAction;
		queueData->queue->id = _id++;
		queueData->id = queueData->queue->id;
		if(_queues.find(address) == _queues.end())
		{
			_queues.insert(std::pair<int32_t, std::map<std::string, std::shared_ptr<QueueData>>>(address, std::map<std::string, std::shared_ptr<QueueData>>()));
		}
		_queues.at(address).insert(std::pair<std::string, std::shared_ptr<QueueData>>(physicalInterface->getID(), queueData));
		_queueMutex.unlock();
		GD::out.printDebug("Creating SAVEPOINT PacketQueue" + std::to_string(address) + "_" + std::to_string(queueData->id));
		raiseCreateSavepoint("PacketQueue" + std::to_string(address) + "_" + std::to_string(queueData->id));
		return queueData->queue;
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    _queueMutex.unlock();
    _workerThreadMutex.unlock();
    return std::shared_ptr<PacketQueue>();
}

void QueueManager::resetQueue(int32_t address, std::string interfaceID, uint32_t id)
{
	try
	{
		if(_disposing) return;
		_queueMutex.lock();
		if(_queues.empty())
		{
			_stopWorkerThread = true;
			_queueMutex.unlock();
			return;
		}
		std::shared_ptr<QueueData> queueData;
		if(_queues.find(address) != _queues.end() && _queues.at(address).find(interfaceID) != _queues.at(address).end())
		{
			queueData = _queues.at(address).at(interfaceID);
			if(!queueData || !queueData->queue)
			{
				_queueMutex.unlock();
				return;
			}
			if(!queueData->queue->isEmpty() && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() <= *queueData->lastAction + 15000)
			{
				_queueMutex.unlock();
				return;
			}
		}
		else
		{
			_queueMutex.unlock();
			return;
		}

		std::shared_ptr<InsteonPeer> peer;
		bool setUnreach = false;

		if(queueData && queueData->id == id)
		{
			if(queueData->queue.use_count() > 1 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() <= *queueData->lastAction + 20000)
			{
				_queueMutex.unlock();
				GD::out.printDebug("Debug: Postponing deletion of queue " + std::to_string(id) + " for interface \"" + interfaceID + "\" and peer with address 0x" + BaseLib::HelperFunctions::getHexString(address) + ", because it is still in use (" + std::to_string(queueData->queue.use_count()) + " referring objects).");
				return;
			}
			GD::out.printDebug("Debug: Deleting queue " + std::to_string(id) + " for interface \"" + interfaceID + "\" and peer with address 0x" + BaseLib::HelperFunctions::getHexString(address));
			_queues.at(address).erase(interfaceID);
			if(_queues.at(address).empty()) _queues.erase(address);
			if(!queueData->queue->isEmpty() && queueData->queue->getQueueType() != PacketQueueType::PAIRING)
			{
				peer = queueData->queue->peer;
				if(peer && peer->rpcDevice && ((peer->getRXModes() & BaseLib::RPC::Device::RXModes::Enum::always) || (peer->getRXModes() & BaseLib::RPC::Device::RXModes::Enum::burst)))
				{
					setUnreach = true;
				}
			}
			queueData->queue->dispose();
		}
		//setUnreach calls enqueuePendingQueues, which calls PacketQueueManger::get => deadlock,
		//so we need to unlock first
		if(_queues.empty()) _stopWorkerThread = true;
		_queueMutex.unlock();
		if(setUnreach) peer->serviceMessages->setUnreach(true, true);
	}
	catch(const std::exception& ex)
    {
		_queueMutex.unlock();
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
    	_queueMutex.unlock();
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	_queueMutex.unlock();
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    GD::out.printDebug("Releasing SAVEPOINT PacketQueue" + std::to_string(address) + "_" + std::to_string(id));
    raiseReleaseSavepoint("PacketQueue" + std::to_string(address) + "_" + std::to_string(id));
}

std::shared_ptr<PacketQueue> QueueManager::get(int32_t address, std::string interfaceID)
{
	try
	{
		if(_disposing) return std::shared_ptr<PacketQueue>();
		_queueMutex.lock();
		//Make a copy to make sure, the element exists
		std::shared_ptr<PacketQueue> queue((_queues.find(address) != _queues.end() && _queues.at(address).find(interfaceID) != _queues.at(address).end()) ? _queues.at(address).at(interfaceID)->queue : std::shared_ptr<PacketQueue>());
		if(queue) queue->keepAlive(); //Don't delete queue in the next second
		_queueMutex.unlock();
		return queue;
	}
	catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    _queueMutex.unlock();
    return std::shared_ptr<PacketQueue>();
}

//Event handling
void QueueManager::raiseCreateSavepoint(std::string name)
{
	if(_eventHandler) ((IQueueManagerEventSink*)_eventHandler)->onQueueCreateSavepoint(name);
}

void QueueManager::raiseReleaseSavepoint(std::string name)
{
	if(_eventHandler) ((IQueueManagerEventSink*)_eventHandler)->onQueueReleaseSavepoint(name);
}
//End event handling
}
