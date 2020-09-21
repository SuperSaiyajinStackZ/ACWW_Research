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
#include "SaveUtils.hpp"
#include "Types.hpp"

#include <cstring>
#include <string>

/* Calculate AC:WW's Checksum. */
uint16_t Checksum::Calculate(const uint16_t *buffer, uint64_t size, uint16_t checksumOffset) {
	if ((checksumOffset & 1) == 1) return 0; // checksumOffset must be 16-bit aligned!

	uint16_t checksum = 0;

	for (uint i = 0; i < size; i++) {
		if (i == checksumOffset) continue;
		checksum += buffer[i];
	}

	return (uint16_t) -checksum;
}

/* Verify AC:WW's Checksum. */
bool Checksum::Verify(const uint16_t *buffer, uint64_t size, uint16_t currentChecksum, uint16_t checksumOffset) {
	if (Calculate(buffer, size, checksumOffset) == currentChecksum) return true;
	
	return false;
}

/* Update AC:WW's Checksum. */
void Checksum::UpdateChecksum(WWRegion region, uint8_t *saveBuffer, uint16_t *buffer, uint64_t size) {
	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			SaveUtils::Write<uint16_t>(saveBuffer, 0x15FDC, Calculate(buffer, size, 0xAFEE));
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			SaveUtils::Write<uint16_t>(saveBuffer, 0x12220, Calculate(buffer, size, 0x9110));
			break;

		case WWRegion::KOR_REV1:
			SaveUtils::Write<uint16_t>(saveBuffer, 0x173F8, Calculate(buffer, size, 0xB9FC));
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}