/* Copyright 2013 Sathya Laufer
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

#include "BidCoSQueueManager.h"
#include "GD.h"
#include "HelperFunctions.h"

BidCoSQueueData::BidCoSQueueData()
{
	queue = std::shared_ptr<BidCoSQueue>(new BidCoSQueue());
	lastAction.reset(new int64_t);
	*lastAction = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

BidCoSQueueManager::BidCoSQueueManager()
{
}

BidCoSQueueManager::~BidCoSQueueManager()
{
	try
	{
		if(!_disposing) dispose();
		_workerThreadMutex.lock();
		if(_workerThread.joinable()) _workerThread.join();
	}
    catch(const std::exception& ex)
    {
    	HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(Exception& ex)
    {
    	HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
	_workerThreadMutex.unlock();
}

void BidCoSQueueManager::dispose(bool wait)
{
	_disposing = true;
	_stopWorkerThread = true;
}

void BidCoSQueueManager::worker()
{
	try
	{
		std::chrono::milliseconds sleepingTime(100);
		int32_t lastQueue;
		lastQueue = 0;
		while(!_stopWorkerThread)
		{
			try
			{
				std::this_thread::sleep_for(sleepingTime);
				if(_stopWorkerThread) return;
				_queueMutex.lock();
				if(!_queues.empty())
				{
					if(!_queues.empty())
					{
						std::unordered_map<int32_t, std::shared_ptr<BidCoSQueueData>>::iterator nextQueue = _queues.find(lastQueue);
						if(nextQueue != _queues.end())
						{
							nextQueue++;
							if(nextQueue == _queues.end()) nextQueue = _queues.begin();
						}
						else nextQueue = _queues.begin();
						lastQueue = nextQueue->first;
					}
				}
				std::shared_ptr<BidCoSQueueData> queue;
				if(_queues.find(lastQueue) != _queues.end()) queue = _queues.at(lastQueue);
				_queueMutex.unlock();
				if(queue) resetQueue(lastQueue, queue->id);
			}
			catch(const std::exception& ex)
			{
				_queueMutex.unlock();
				HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
			}
			catch(Exception& ex)
			{
				_queueMutex.unlock();
				HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
			}
			catch(...)
			{
				_queueMutex.unlock();
				HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
			}
		}
	}
    catch(const std::exception& ex)
    {
    	HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(Exception& ex)
    {
    	HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
}

std::shared_ptr<BidCoSQueue> BidCoSQueueManager::createQueue(HomeMaticDevice* device, BidCoSQueueType queueType, int32_t address)
{
	try
	{
		if(_disposing) return std::shared_ptr<BidCoSQueue>();
		_queueMutex.lock();
		if(_stopWorkerThread)
		{
			_queueMutex.unlock();
			_workerThreadMutex.lock();
			if(_workerThread.joinable())
			{
				//_workerThread.join might very rarely cause the exception "Resource deadlock avoided".
				//That's the reason for the try...catch block here.
				try
				{
					_workerThread.join();
				}
				catch(const std::exception& ex)
				{
					HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
				}
				catch(...)
				{
					HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
			}
			_stopWorkerThread = false;
			_workerThread = std::thread(&BidCoSQueueManager::worker, this);
			HelperFunctions::setThreadPriority(_workerThread.native_handle(), 19);
			_workerThreadMutex.unlock();
			_queueMutex.lock();
		}
		else if(_queues.find(address) != _queues.end()) _queues.erase(_queues.find(address));
		_queueMutex.unlock();

		std::shared_ptr<BidCoSQueueData> queueData(new BidCoSQueueData());
		queueData->queue->setQueueType(queueType);
		queueData->queue->device = device;
		queueData->queue->lastAction = queueData->lastAction;
		queueData->queue->id = _id++;
		queueData->id = queueData->queue->id;
		_queueMutex.lock();
		_queues.insert(std::pair<int32_t, std::shared_ptr<BidCoSQueueData>>(address, queueData));
		_queueMutex.unlock();
		HelperFunctions::printDebug("Creating SAVEPOINT queue" + std::to_string(address) + "_" + std::to_string(queueData->id));
		GD::db.executeCommand("SAVEPOINT queue" + std::to_string(address) + "_" + std::to_string(queueData->id));
		return queueData->queue;
	}
	catch(const std::exception& ex)
    {
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(Exception& ex)
    {
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    _queueMutex.unlock();
    _workerThreadMutex.unlock();
    return std::shared_ptr<BidCoSQueue>();
}

void BidCoSQueueManager::resetQueue(int32_t address, uint32_t id)
{
	try
	{
		if(_disposing) return;
		_queueMutex.lock();
		if(_queues.find(address) != _queues.end() && _queues.at(address) && _queues.at(address)->queue && !_queues.at(address)->queue->isEmpty() && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() <= *_queues.at(address)->lastAction + 1000)
		{
			if(_queues.empty()) _stopWorkerThread = true;
			_queueMutex.unlock();
			return;
		}

		std::shared_ptr<BidCoSQueueData> queue;
		std::shared_ptr<Peer> peer;
		bool setUnreach = false;
		if(_queues.find(address) != _queues.end() && _queues.at(address) && _queues.at(address)->id == id)
		{
			HelperFunctions::printDebug("Debug: Deleting queue " + std::to_string(id) + " for 0x" + HelperFunctions::getHexString(address));
			queue = _queues.at(address);
			_queues.erase(address);
			if(!queue->queue->isEmpty() && queue->queue->getQueueType() != BidCoSQueueType::PAIRING)
			{
				peer = queue->queue->peer;
				if(peer && peer->rpcDevice && ((peer->rpcDevice->rxModes & RPC::Device::RXModes::Enum::always) || (peer->rpcDevice->rxModes & RPC::Device::RXModes::Enum::burst)))
				{
					setUnreach = true;
				}
			}
			queue->queue->dispose();
		}
		//setUnreach calls enqueuePendingQueues, which calls BidCoSQueueManger::get => deadlock,
		//so we need to unlock first
		if(_queues.empty()) _stopWorkerThread = true;
		_queueMutex.unlock();
		if(setUnreach) peer->serviceMessages->setUnreach(true);
	}
	catch(const std::exception& ex)
    {
		_queueMutex.unlock();
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(Exception& ex)
    {
    	_queueMutex.unlock();
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	_queueMutex.unlock();
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    HelperFunctions::printDebug("Releasing SAVEPOINT queue" + std::to_string(address) + "_" + std::to_string(id));
    GD::db.executeCommand("RELEASE queue" + std::to_string(address) + "_" + std::to_string(id));
}

std::shared_ptr<BidCoSQueue> BidCoSQueueManager::get(int32_t address)
{
	try
	{
		if(_disposing) return std::shared_ptr<BidCoSQueue>();
		_queueMutex.lock();
		//Make a copy to make sure, the element exists
		std::shared_ptr<BidCoSQueue> queue((_queues.find(address) != _queues.end()) ? _queues[address]->queue : std::shared_ptr<BidCoSQueue>());
		_queueMutex.unlock();
		return queue;
	}
	catch(const std::exception& ex)
    {
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(Exception& ex)
    {
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        HelperFunctions::printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    _queueMutex.unlock();
    return std::shared_ptr<BidCoSQueue>();
}
