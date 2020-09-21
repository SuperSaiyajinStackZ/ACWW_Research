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
#include "init.hpp"
#include "stack.hpp"
#include "structs.hpp"

#include <dirent.h>

bool exiting = false;

C2D_SpriteSheet sprites;

bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h)) return true;
	return false;
}

/* If button Position pressed -> Do something. */
bool buttonTouch(touchPosition touch, ButtonStruct button) {
	if (touch.px >= button.X && touch.px <= (button.X + button.xSize) && touch.py >= button.Y && touch.py <= (button.Y + button.ySize)) return true;
	return false;
}

static void CreateDirs() {
	mkdir("sdmc:/3ds", 0x777);

	mkdir("sdmc:/3ds/ACWW-Utils", 0x777);

	/* Create Region dirs. */
	mkdir("sdmc:/3ds/ACWW-Utils/EUR", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/USA", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/JPN", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/KOR", 0x777);

	/* Letter dirs. */
	mkdir("sdmc:/3ds/ACWW-Utils/EUR/Letters", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/USA/Letters", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/JPN/Letters", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/KOR/Letters", 0x777);

	/* Player dirs. */
	mkdir("sdmc:/3ds/ACWW-Utils/EUR/Players", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/USA/Players", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/JPN/Players", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/KOR/Players", 0x777);

	/* Villager dirs. */
	mkdir("sdmc:/3ds/ACWW-Utils/EUR/Villagers", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/USA/Villagers", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/JPN/Villagers", 0x777);
	mkdir("sdmc:/3ds/ACWW-Utils/KOR/Villagers", 0x777);
}


Result Init::Initialize() {
	/* Here we set the initial fade effect for fadein. */
	fadealpha = 255;
	fadein = true;

	gfxInitDefault();
	romfsInit();

	Gui::init();
	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);
	osSetSpeedupEnable(true); // Enable speed-up for New 3DS users.

	CreateDirs();

	Gui::setScreen(std::make_unique<Stack>(), false, true);
	hidSetRepeatParameters(10, 10);
	return 0;
}

Result Init::MainLoop() {
	Init::Initialize();

	while (aptMainLoop()) {
		/* Scan the input. */
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hHeld = hidKeysHeld();
		touchPosition touch;
		hidTouchRead(&touch);

		Gui::clearTextBufs();
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

		/* Screen Logic & Draw. */
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::DrawScreen(true);
		Gui::ScreenLogic(hDown, hHeld, touch, true, true);
		C3D_FrameEnd(0);

		if (exiting) {
			if (!fadeout) break;
		}

		Gui::fadeEffects(16, 16, true);
	}

	return Init::Exit();
}

Result Init::Exit() {
	Gui::exit();
	Gui::unloadSheet(sprites);
	gfxExit();
	cfguExit();
	romfsExit();
	return 0;
}