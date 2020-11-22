/*
	Diese Datei ist Teil von ACWW_Research Tools.
	Copyright (C) 2020 bei SuperSaiyajinStackZ.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Additional Terms 7.b and 7.c of GPLv3 apply to this file:
		* Requiring preservation of specified reasonable legal notices or
		  author attributions in that material or in the Appropriate Legal
		  Notices displayed by works containing it.
		* Prohibiting misrepresentation of the origin of that material,
		  or requiring that modified versions of such material be marked in
		  reasonable ways as different from the original version.
*/

#ifndef _CHECKSUM_UPDATER_CHECKSUM_HPP
#define _CHECKSUM_UPDATER_CHECKSUM_HPP

#include "common.hpp"
#include <memory>
#include <string>

class Checksum {
public:
	Checksum(const std::string &FileName);

	void PrintRegion();

	uint16_t CalculateMain(bool fixIt = true);
	uint16_t CalculateLetter(bool fixIt = true);

	bool ChecksumMainValid();
	bool ChecksumLetterValid();

	bool RegionValid() const { return this->Region != WWRegion::UNKNOWN; };

	void WriteBack();
private:
	std::string FileName = "";
	WWRegion Region = WWRegion::UNKNOWN;
	std::unique_ptr<uint8_t[]> SaveData = nullptr;
	uint32_t SaveSize = 0;
	bool SaveValid = false;
};

#endif