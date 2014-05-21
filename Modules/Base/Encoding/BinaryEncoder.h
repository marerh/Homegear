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

#ifndef BINARYENCODER_H_
#define BINARYENCODER_H_

#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include <string>

namespace BaseLib
{
class BinaryEncoder
{
public:
	BinaryEncoder();
	virtual ~BinaryEncoder() {}

	virtual void encodeInteger(std::shared_ptr<std::vector<char>>& encodedData, int32_t integer);
	virtual void encodeInteger(std::vector<uint8_t>& encodedData, int32_t integer);
	virtual void encodeByte(std::shared_ptr<std::vector<char>>& encodedData, uint8_t byte);
	virtual void encodeByte(std::vector<uint8_t>& encodedData, uint8_t byte);
	virtual void encodeString(std::shared_ptr<std::vector<char>>& packet, std::string& string);
	virtual void encodeString(std::vector<uint8_t>& encodedData, std::string& string);
	virtual void encodeBoolean(std::shared_ptr<std::vector<char>>& encodedData, bool boolean);
	virtual void encodeBoolean(std::vector<uint8_t>& encodedData, bool boolean);
	virtual void encodeFloat(std::shared_ptr<std::vector<char>>& encodedData, double floatValue);
	virtual void encodeFloat(std::vector<uint8_t>& encodedData, double floatValue);
};
}
#endif /* BINARYENCODER_H_ */