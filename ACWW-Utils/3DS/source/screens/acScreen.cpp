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
#include "coreUtils.hpp"
#include "SaveUtils.hpp"

extern bool buttonTouch(touchPosition touch, ButtonStruct button);
extern bool exiting;

std::vector<std::string> regions = {
	"EUR", "USA", "JPN", "KOR"
};

bool ACScreen::loadSave(int index) {
	if (index == -1) return false;

	savefile = nullptr;

	FILE* in = fopen(saveLocs[index].c_str(), "rb");

	if (in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);

		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, in);

		fclose(in);

		savefile = SaveUtils::getSave(saveData, size);
	}

	if (!savefile) {
		return false;
	}
	
	return true;
}

ACScreen::ACScreen() { }

void ACScreen::SaveInitialize() {
	int index = GFX::SelectIndex("Select a Region.", regions);

	if (index != -1) {
		if (this->loadSave(index)) {
			this->loaded = true;
		}

	} else {
		exiting = true;
		return;
	}
}


std::string ACScreen::GetRegionName() const {
	switch(savefile->getRegion()) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return "USA / Europe";

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return "Japanese";

		case WWRegion::KOR_REV1:
			return "Korean";

		case WWRegion::UNKNOWN:
			return "Unknown Region.";
	}

	return "Unknown Region.";
}

void ACScreen::WriteSave() {
	if (this->injectsMade) {
		std::string fl;

		switch(savefile->getRegion()) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
				fl = saveLocs[0];
				break;

			case WWRegion::EUR_REV1:
				fl = saveLocs[1];
				break;

			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				fl = saveLocs[2];
				break;

			case WWRegion::KOR_REV1:
				fl = saveLocs[3];
				break;

			case WWRegion::UNKNOWN:
				return;
		}

		savefile->Finish();
		FILE *file = fopen(fl.c_str(), "wb");
		fwrite(savefile->rawData().get(), 1, savefile->getLength(), file);
		fclose(file);
	}
}

void ACScreen::Draw(void) const {
	if (this->loaded) {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2, 0.8f, WHITE, "AC:WW-Utils - Dump Section", 400);

		Gui::DrawStringCentered(0, 50, 0.6f, WHITE, "Detected Region: " + this->GetRegionName(), 400);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	
		GFX::DrawBottom();

		for (int i = 0, i2 = 0 + (page * 6); i < 6; i++, i2++) {
			GFX::Button(this->buttons[i2]);

			if (this->Selection == i) {
				Gui::drawAnimatedSelector(this->buttons[i].X, this->buttons[i].Y, this->buttons[i].xSize, this->buttons[i].ySize, .030, C2D_Color32(0, 220, 220, 255), C2D_Color32(0, 0, 0, 0));
			}
		}


		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}


void ACScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->loaded) {
		if (hDown & KEY_B) {
			this->WriteSave();
			this->loaded = false;
		}

		if (hDown & KEY_A) {
			std::vector<std::string> temp, temp2;
			int index = -1, index2 = -1;

			if (this->page == 0) {
				switch(this->Selection) {
					case 0:
						/* Letter select. */
						temp = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
						index = GFX::SelectIndex("Select a letter.", temp);

						if (index != -1) {
							temp2 = { "1", "2", "3", "4" };
							index2 = GFX::SelectIndex("Select a Player.", temp2);

							if (index2 != -1) {
								CoreUtils::DumpLetter(savefile->getRegion(), index, index2);
							}
						}
						break;

					case 1:
						/* Player select. */
						temp = { "1", "2", "3", "4" };
						index = GFX::SelectIndex("Select a Player.", temp);

						if (index != -1) {
							CoreUtils::DumpPlayer(savefile->getRegion(), index);
						}
						break;

					case 2:
						/* Villager select. */
						temp = { "1", "2", "3", "4", "5", "6", "7", "8" };
						index = GFX::SelectIndex("Select a Villager.", temp);

						if (index != -1) {
							CoreUtils::DumpVillager(savefile->getRegion(), index);
						}
						break;

					case 3:
						/* Letter select. */
						temp = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
						index = GFX::SelectIndex("Select a letter.", temp);

						if (index != -1) {
							temp2 = { "1", "2", "3", "4" };
							index2 = GFX::SelectIndex("Select a Player.", temp2);

							if (index2 != -1) {
								CoreUtils::InjectLetter(savefile->getRegion(), index, index2);
								this->injectsMade = true;
							}
						}
						break;

					case 4:
						/* Player select. */
						temp = { "1", "2", "3", "4" };
						index = GFX::SelectIndex("Select a Player.", temp);

						if (index != -1) {
							CoreUtils::InjectPlayer(savefile->getRegion(), index);
							this->injectsMade = true;
						}
						break;

					case 5:
						/* Villager select. */
						temp = { "1", "2", "3", "4", "5", "6", "7", "8" };
						index = GFX::SelectIndex("Select a Villager.", temp);

						if (index != -1) {
							CoreUtils::InjectVillager(savefile->getRegion(), index);
							this->injectsMade = true;
						}
						break;
				}
			}
		}

		if (hDown & KEY_TOUCH) {
			if (this->page == 0) {
				if (buttonTouch(touch, this->buttons[0])) {
					/* Letter select. */
					std::vector<std::string> temp = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
					int index = GFX::SelectIndex("Select a letter.", temp);

					if (index != -1) {
						std::vector<std::string> temp2 = { "1", "2", "3", "4" };
						int index2 = GFX::SelectIndex("Select a Player.", temp2);

						if (index2 != -1) {
							CoreUtils::DumpLetter(savefile->getRegion(), index, index2);
						}
					}

				} else if (buttonTouch(touch, this->buttons[1])) {
					/* Player select. */
					std::vector<std::string> temp = { "1", "2", "3", "4" };
					int index = GFX::SelectIndex("Select a Player.", temp);

					if (index != -1) {
						CoreUtils::DumpPlayer(savefile->getRegion(), index);
					}

				} else if (buttonTouch(touch, this->buttons[2])) {
					/* Villager select. */
					std::vector<std::string> temp = { "1", "2", "3", "4", "5", "6", "7", "8" };
					int index = GFX::SelectIndex("Select a Villager.", temp);

					if (index != -1) {
						CoreUtils::DumpVillager(savefile->getRegion(), index);
					}

				} else if (buttonTouch(touch, this->buttons[3])) {
					/* Letter select. */
					std::vector<std::string> temp = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
					int index = GFX::SelectIndex("Select a letter.", temp);

					if (index != -1) {
						std::vector<std::string> temp2 = { "1", "2", "3", "4" };
						int index2 = GFX::SelectIndex("Select a Player.", temp2);

						if (index2 != -1) {
							CoreUtils::InjectLetter(savefile->getRegion(), index, index2);
							this->injectsMade = true;
						}
					}

				} else if (buttonTouch(touch, this->buttons[4])) {
					/* Player select. */
					std::vector<std::string> temp = { "1", "2", "3", "4" };
					int index = GFX::SelectIndex("Select a Player.", temp);

					if (index != -1) {
						CoreUtils::InjectPlayer(savefile->getRegion(), index);
						this->injectsMade = true;
					}

				} else if (buttonTouch(touch, this->buttons[5])) {
					/* Villager select. */
					std::vector<std::string> temp = { "1", "2", "3", "4", "5", "6", "7", "8" };
					int index = GFX::SelectIndex("Select a Villager.", temp);

					if (index != -1) {
						CoreUtils::InjectVillager(savefile->getRegion(), index);
						this->injectsMade = true;
					}

				}
			}
		}


		if (hDown & KEY_DOWN) {
			if (this->Selection < 5) this->Selection++;
		}

		if (hDown & KEY_UP) {
			if (this->Selection > 0) this->Selection--;
		}
		
	} else {
		this->SaveInitialize();
	}
}