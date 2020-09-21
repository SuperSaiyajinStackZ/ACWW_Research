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

#include "common.hpp"

extern C2D_SpriteSheet sprites; // Include the spritesheet extern.

void GFX::DrawTop(void) {
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 25, C2D_Color32(0, 160, 160, 255));
	Gui::Draw_Rect(0, 25, 400, 190, C2D_Color32(0, 120, 120, 255));
	Gui::Draw_Rect(0, 215, 400, 25, C2D_Color32(0, 160, 160, 255));
}

void GFX::DrawBottom(void) {
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 25, C2D_Color32(0, 160, 160, 255));
	Gui::Draw_Rect(0, 25, 320, 190, C2D_Color32(0, 120, 120, 255));
	Gui::Draw_Rect(0, 215, 320, 25, C2D_Color32(0, 160, 160, 255));
}

/* Select an index. */
int GFX::SelectIndex(const std::string Text, std::vector<std::string> &contents) {
	s32 Selection = 0;
	
	while(1) {
		std::string indexes;
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.8f, C2D_Color32(255, 255, 255, 255), Text, 390);

		for (int i = (Selection < 5) ? 0 : Selection - 5; i < (int)contents.size() && i < ((Selection < 5) ? 6 : Selection + 1); i++) {
			if (i == Selection) {
				indexes += "> " + contents[i] + "\n\n";

			} else {
				indexes += contents[i] + "\n\n";
			}
		}

		for (uint i = 0; i < ((contents.size() < 6) ? 6 - contents.size() : 0); i++) {
			indexes += "\n\n";
		}

		Gui::DrawString(26, 32, 0.53f, C2D_Color32(255, 255, 255, 255), indexes, 360);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		GFX::DrawBottom();
		C3D_FrameEnd(0);

		/* Input part. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();

		if (hDown & KEY_A) {
			return Selection;
		}

		if (hRepeat & KEY_UP) {
			if (Selection > 0) {
				Selection--;
			}
		}
		
		if (hRepeat & KEY_DOWN) {
			if ((uint)Selection < contents.size() - 1) {
				Selection++;
			}
		}
		
		if (hDown & KEY_B) {
			return -1;
		}
	}
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, img, x, y, ScaleX, ScaleY);
}

void GFX::Button(const ButtonStruct btn) {
	Gui::Draw_Rect(btn.X, btn.Y, btn.xSize, btn.ySize, C2D_Color32(0, 170, 170, 255));
	Gui::DrawStringCentered(btn.X - 160 + (btn.xSize / 2), btn.Y + (btn.ySize / 2) - 10, 0.6f, C2D_Color32(255, 255, 255, 255), btn.Text, btn.X - 10, btn.Y - 5);
}