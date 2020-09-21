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

#ifndef _WILDEDIT_CORE_SAV_HPP
#define _WILDEDIT_CORE_SAV_HPP

#include "Sav.hpp"
#include "Types.hpp"

#include <string>

class Sav {
protected:
	std::shared_ptr<uint8_t[]> saveData;
	WWRegion region;
	uint32_t size;
public:
	Sav(std::shared_ptr<uint8_t[]> dt, WWRegion Region, uint32_t ssize) : saveData(dt), region(Region), size(ssize) { }
	Sav(const Sav& sav) = delete;
	Sav& operator=(const Sav& sav) = delete;

	void Finish(void);

	WWRegion getRegion() { return region; }

	/* return Sav stuff. */
	uint32_t getLength() const { return this->size; }
	std::shared_ptr<uint8_t[]> rawData() const { return this->saveData; }

	uint8_t *savePointer() const {
		return this->saveData.get();
	}
private:

};

#endif