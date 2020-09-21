/*
*   This file is part of WildEdit-Core
*   Copyright (C) 2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "Checksum.hpp"
#include "Sav.hpp"

#include <cstring>

/* Last call before writing to file. Update Checksum. */
void Sav::Finish(void) {
	switch (this->region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Checksum::UpdateChecksum(this->region, this->savePointer(), reinterpret_cast<uint16_t*>(this->savePointer()), 0x15FE0 / sizeof(uint16_t));
			memcpy(this->savePointer() + 0x15FE0, this->savePointer(), 0x15FE0); // Copy SaveData to the second save copy.
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Checksum::UpdateChecksum(this->region, this->savePointer(), reinterpret_cast<uint16_t*>(this->savePointer()), 0x12224 / sizeof(uint16_t));
			memcpy(this->savePointer() + 0x12224, this->savePointer(), 0x12224); // Copy SaveData to the second save copy.
			break;

		case WWRegion::KOR_REV1:
			Checksum::UpdateChecksum(this->region, this->savePointer(), reinterpret_cast<uint16_t*>(this->savePointer()), 0x173FC / sizeof(uint16_t));
			memcpy(this->savePointer() + 0x173FC, this->savePointer(), 0x173FC); // Copy SaveData to the second save copy.
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}