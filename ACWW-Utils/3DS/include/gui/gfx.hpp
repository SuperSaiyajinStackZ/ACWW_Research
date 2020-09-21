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

#ifndef _ACWW_UTILS_GFX_HPP
#define _ACWW_UTILS_GFX_HPP

#include "gui.hpp"
#include "sprites.h"

#include <citro2d.h>
#include <string>
#include <vector>

struct ButtonStruct {
	int X;
	int Y;
	float xSize;
	float ySize;
	std::string Text;
};

#define WHITE C2D_Color32(255, 255, 255, 255)

namespace GFX {
	void DrawTop(void);
	void DrawBottom(void);
	void DrawSprite(int img, int x, int y, float ScaleX = 1, float ScaleY = 1);

	int SelectIndex(const std::string Text, std::vector<std::string> &contents);
	void Button(const ButtonStruct btn);
}

#endif