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

#ifdef SCRIPTENGINE
#ifndef SCRIPTENGINE_H_
#define SCRIPTENGINE_H_

#include "../../Modules/Base/BaseLib.h"
#include "../RPC/ServerInfo.h"

#include <mutex>
#include <wordexp.h>

class ScriptEngine
{
public:
	ScriptEngine();
	virtual ~ScriptEngine();
	void dispose();

	std::vector<std::string> getArgs(const std::string& path, const std::string& args);
	void execute(const std::string path, const std::string arguments, std::shared_ptr<std::vector<char>> output = nullptr, int32_t* exitCode = nullptr, bool wait = true, int32_t threadId = -1);
	int32_t executeWebRequest(const std::string& path, BaseLib::HTTP& request, std::shared_ptr<RPC::ServerInfo::Info>& serverInfo, std::vector<char>& output);

	bool checkSessionId(const std::string& sessionId);

	std::shared_ptr<BaseLib::RPC::Variable> getAllScripts();
protected:
	bool _disposing = false;
	volatile int32_t _currentScriptThreadID = 0;
	std::map<int32_t, std::pair<std::thread, bool>> _scriptThreads;
	std::mutex _scriptThreadMutex;

	void collectGarbage();
	bool scriptThreadMaxReached();
	void setThreadNotRunning(int32_t threadId);
};
#endif
#endif
