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

#ifndef _ACWW_UTILS_COMMON_HPP
#define _ACWW_UTILS_COMMON_HPP

#include "gfx.hpp"
#include "gui.hpp"
#include "msg.hpp"
#include "screenCommon.hpp"
#include <array>
#include <string>

/* Change these strings to your saves. */
const std::array<std::string, 4> saveLocs = {
    "sdmc:/3ds/ACWW-Utils/EUR/saves/EUR.sav",
    "sdmc:/3ds/ACWW-Utils/USA/saves/USA.sav",
    "sdmc:/3ds/ACWW-Utils/JPN/saves/JPN.sav",
    "sdmc:/3ds/ACWW-Utils/KOR/saves/KOR.sav"
};

#endif