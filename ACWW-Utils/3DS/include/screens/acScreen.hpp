/*
*   This file is part of ACWW-Utils
*   Copyright (C) 2020 SuperSaiyajinStackZ
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

#ifndef _ACWW_UTILS_AC_SCREEN_HPP
#define _ACWW_UTILS_AC_SCREEN_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class ACScreen : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	ACScreen();
private:
	std::string GetRegionName() const;
	bool loadSave(int index);
	void SaveInitialize();
	void WriteSave();
	
	const std::vector<ButtonStruct> buttons = {
		{10, 40, 140, 35, "Letter Dump"},
		{10, 100, 140, 35, "Player Dump"},
		{10, 160, 140, 35, "Villager Dump"},

		{170, 40, 140, 35, "Letter Inject"},
		{170, 100, 140, 35, "Player Inject"},
		{170, 160, 140, 35, "Villager Inject"}
	};

	int Selection = 0, page = 0;
	bool loaded = false, injectsMade = false;
};

#endif