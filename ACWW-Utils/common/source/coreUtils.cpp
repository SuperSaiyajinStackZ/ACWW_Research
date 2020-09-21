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

#include "coreUtils.hpp"
#include "definitions.hpp"
#include "msg.hpp"
#include <string>

/* Static function, for creating dump files. */
static void CreateDumpFile(const std::string path, WWRegion region, uint32_t offset, uint32_t size) {
	std::string regionString = "";

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
			regionString = "USA";
			break;

		case WWRegion::EUR_REV1:
			regionString = "EUR";
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			regionString = "JPN";
			break;

		case WWRegion::KOR_REV1:
			regionString = "KOR";
			break;

		case WWRegion::UNKNOWN:
			break;
	}

	FILE *file = fopen((std::string("sdmc:/3ds/ACWW-Utils/") + regionString + std::string("/") + path).c_str(), "w");

	std::unique_ptr<uint8_t[]> buffer = std::unique_ptr<uint8_t[]>(new uint8_t[size]);

	std::fill_n(buffer.get(), size, 0x0);

	for (int i = 0; i < (int)size; i++) {
		buffer.get()[i] = savefile->savePointer()[offset + i];
	}

	fwrite(buffer.get(), 1, size, file);
	fclose(file);
}

static void InjectDumpFile(const std::string path, WWRegion region, uint32_t offset, uint32_t size) {
	std::string regionString = "";

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
			regionString = "USA";
			break;

		case WWRegion::EUR_REV1:
			regionString = "EUR";
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			regionString = "JPN";
			break;

		case WWRegion::KOR_REV1:
			regionString = "KOR";
			break;

		case WWRegion::UNKNOWN:
			break;
	}

	FILE* in = fopen((std::string("sdmc:/3ds/ACWW-Utils/") + regionString + std::string("/") + path).c_str(), "rb");

	if (in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);

		std::unique_ptr<u8[]> bufferData = std::unique_ptr<u8[]>(new u8[size]);
		fread(bufferData.get(), 1, size, in);
		fclose(in);

		for (int i = 0; i < (int)size; i++) {
			savefile->savePointer()[offset + i] = bufferData.get()[i];
		}
	}
}


void CoreUtils::DumpLetter(WWRegion region, int slot, int player) {
	if (slot > PLAYER_LETTER_SIZE - 1) return;
	if (player > PLAYER_SIZE - 1) return;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Msg::DisplayMsg("Dumping Europe / USA Letter from Player " + std::to_string(player + 1) + "\nWith the index of slot " + std::to_string(slot + 1) + "...");
			CreateDumpFile("Letters/Player" + std::to_string(player + 1) + "_Letter" + std::to_string(slot + 1) + ".dat", region, EUR_PLAYER_LETTER_START + (slot * EUR_LTR_SIZE) + (player * EUR_PLR_SIZE), EUR_LTR_SIZE);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Msg::DisplayMsg("Dumping Japanese Letter from Player " + std::to_string(player + 1) + "\nWith the index of slot " + std::to_string(slot + 1) + "...");
			CreateDumpFile("Letters/Player" + std::to_string(player + 1) + "_Letter" + std::to_string(slot + 1) + ".dat", region, JPN_PLAYER_LETTER_START + (slot * JPN_LTR_SIZE) + (player * JPN_PLR_SIZE), JPN_LTR_SIZE);
			break;

		case WWRegion::KOR_REV1:
			Msg::DisplayMsg("Dumping Korean Letter from Player " + std::to_string(player + 1) + "\nWith the index of slot " + std::to_string(slot + 1) + "...");
			CreateDumpFile("Letters/Player" + std::to_string(player + 1) + "_Letter" + std::to_string(slot + 1) + ".dat", region, KOR_PLAYER_LETTER_START + (slot * KOR_LTR_SIZE) + (player * KOR_PLR_SIZE), KOR_LTR_SIZE);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

void CoreUtils::InjectLetter(WWRegion region, int slot, int player) {
	if (slot > PLAYER_LETTER_SIZE - 1) return;
	if (player > PLAYER_SIZE - 1) return;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Msg::DisplayMsg("Inject Europe / USA Letter from Player " + std::to_string(player + 1) + "\nWith the index of slot " + std::to_string(slot + 1) + "...");
			InjectDumpFile("Letters/Player" + std::to_string(player + 1) + "_Letter" + std::to_string(slot + 1) + ".dat", region, EUR_PLAYER_LETTER_START + (slot * EUR_LTR_SIZE) + (player * EUR_PLR_SIZE), EUR_LTR_SIZE);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Msg::DisplayMsg("Inject Japanese Letter from Player " + std::to_string(player + 1) + "\nWith the index of slot " + std::to_string(slot + 1) + "...");
			InjectDumpFile("Letters/Player" + std::to_string(player + 1) + "_Letter" + std::to_string(slot + 1) + ".dat", region, JPN_PLAYER_LETTER_START + (slot * JPN_LTR_SIZE) + (player * JPN_PLR_SIZE), JPN_LTR_SIZE);
			break;

		case WWRegion::KOR_REV1:
			Msg::DisplayMsg("Inject Korean Letter from Player " + std::to_string(player + 1) + "\nWith the index of slot " + std::to_string(slot + 1) + "...");
			InjectDumpFile("Letters/Player" + std::to_string(player + 1) + "_Letter" + std::to_string(slot + 1) + ".dat", region, KOR_PLAYER_LETTER_START + (slot * KOR_LTR_SIZE) + (player * KOR_PLR_SIZE), KOR_LTR_SIZE);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

void CoreUtils::DumpPlayer(WWRegion region, int player) {
	if (player > PLAYER_SIZE - 1) return;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Msg::DisplayMsg("Dumping Europe / USA Player from slot " + std::to_string(player + 1) + "...");
			CreateDumpFile("Players/Player" + std::to_string(player + 1) + ".dat", region, EUR_PLR_START + (player * EUR_PLR_SIZE), EUR_PLR_SIZE);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Msg::DisplayMsg("Dumping Japanese Player from slot " + std::to_string(player + 1) + "...");
			CreateDumpFile("Players/Player" + std::to_string(player + 1) + ".dat", region, JPN_PLR_START + (player * JPN_PLR_SIZE), JPN_PLR_SIZE);
			break;

		case WWRegion::KOR_REV1:
			Msg::DisplayMsg("Dumping Korean Player from slot " + std::to_string(player + 1) + "...");
			CreateDumpFile("Players/Player" + std::to_string(player + 1) + ".dat", region, KOR_PLR_START + (player * KOR_PLR_SIZE), KOR_PLR_SIZE);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

void CoreUtils::InjectPlayer(WWRegion region, int player) {
	if (player > PLAYER_SIZE - 1) return;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Msg::DisplayMsg("Inject Europe / USA Player from slot " + std::to_string(player + 1) + "...");
			InjectDumpFile("Players/Player" + std::to_string(player + 1) + ".dat", region, EUR_PLR_START + (player * EUR_PLR_SIZE), EUR_PLR_SIZE);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Msg::DisplayMsg("Inject Japanese Player from slot " + std::to_string(player + 1) + "...");
			InjectDumpFile("Players/Player" + std::to_string(player + 1) + ".dat", region, JPN_PLR_START + (player * JPN_PLR_SIZE), JPN_PLR_SIZE);
			break;

		case WWRegion::KOR_REV1:
			Msg::DisplayMsg("Inject Korean Player from slot " + std::to_string(player + 1) + "...");
			InjectDumpFile("Players/Player" + std::to_string(player + 1) + ".dat", region, KOR_PLR_START + (player * KOR_PLR_SIZE), KOR_PLR_SIZE);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

void CoreUtils::DumpVillager(WWRegion region, int villager) {
	if (villager > VILLAGER_SIZE - 1) return;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Msg::DisplayMsg("Dumping Europe / USA Villager from slot " + std::to_string(villager + 1) + "...");
			CreateDumpFile("Villagers/Villager" + std::to_string(villager + 1) + ".dat", region, EUR_VLG_START + (villager * EUR_VLG_SIZE), EUR_VLG_SIZE);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Msg::DisplayMsg("Dumping Japanese Villager from slot " + std::to_string(villager + 1) + "...");
			CreateDumpFile("Villagers/Villager" + std::to_string(villager + 1) + ".dat", region, JPN_VLG_START + (villager * JPN_VLG_SIZE), JPN_VLG_SIZE);
			break;

		case WWRegion::KOR_REV1:
			Msg::DisplayMsg("Dumping Korean Villager from slot " + std::to_string(villager + 1) + "...");
			CreateDumpFile("Villagers/Villager" + std::to_string(villager + 1) + ".dat", region, KOR_VLG_START + (villager * KOR_VLG_SIZE), KOR_VLG_SIZE);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}

void CoreUtils::InjectVillager(WWRegion region, int villager) {
	if (villager > VILLAGER_SIZE - 1) return;

	switch(region) {
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			Msg::DisplayMsg("Inject Europe / USA Villager from slot " + std::to_string(villager + 1) + "...");
			InjectDumpFile("Villagers/Villager" + std::to_string(villager + 1) + ".dat", region, EUR_VLG_START + (villager * EUR_VLG_SIZE), EUR_VLG_SIZE);
			break;

		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			Msg::DisplayMsg("Inject Japanese Villager from slot " + std::to_string(villager + 1) + "...");
			InjectDumpFile("Villagers/Villager" + std::to_string(villager + 1) + ".dat", region, JPN_VLG_START + (villager * JPN_VLG_SIZE), JPN_VLG_SIZE);
			break;

		case WWRegion::KOR_REV1:
			Msg::DisplayMsg("Inject Korean Villager from slot " + std::to_string(villager + 1) + "...");
			InjectDumpFile("Villagers/Villager" + std::to_string(villager + 1) + ".dat", region, KOR_VLG_START + (villager * KOR_VLG_SIZE), KOR_VLG_SIZE);
			break;

		case WWRegion::UNKNOWN:
			break;
	}
}