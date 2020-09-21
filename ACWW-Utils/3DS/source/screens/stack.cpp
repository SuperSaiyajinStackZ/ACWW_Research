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

#include "acScreen.hpp"
#include "stack.hpp"

void Stack::Draw(void) const {
	GFX::DrawTop();
	GFX::DrawSprite(sprites_stackZ_idx, 2, 76);
	GFX::DrawSprite(sprites_universal_core_idx, 190, 105);
	Gui::DrawStringCentered(0, 25, 0.65, WHITE, "This app has been developed by SuperSaiyajinStackZ.\nThis app uses Universal-Core made by Universal-Team.\nThanks to all contributors. :)", 395, 30);

	Gui::DrawStringCentered(0, 215, 0.7, WHITE, "Press any key to continue.", 395, 30);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void Stack::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown) Gui::setScreen(std::make_unique<ACScreen>(), true, true);
}