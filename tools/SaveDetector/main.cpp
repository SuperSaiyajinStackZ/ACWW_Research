/*
	Diese Datei ist Teil von ACWW_Research Tools.
	Copyright (C) 2020 bei SuperSaiyajinStackZ.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Additional Terms 7.b and 7.c of GPLv3 apply to this file:
		* Requiring preservation of specified reasonable legal notices or
		  author attributions in that material or in the Appropriate Legal
		  Notices displayed by works containing it.
		* Prohibiting misrepresentation of the origin of that material,
		  or requiring that modified versions of such material be marked in
		  reasonable ways as different from the original version.
*/

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

static const bool Debug = false;

enum class Regions : uint8_t {
	EUR,
	USA, // Jedoch ist EUR und USA das selbe von der Speicher-Struktur, aber byte 0x0 ist anders.
	JPN,
	KOR,
	UNKNOWN
};

Regions Region = Regions::UNKNOWN;
const uint8_t Codes[4] = { 0xC5, 0x8A, 0x32, 0x32 }; // Die Spielcodes.
const int SavCopyOffsets[4] = { 0x15FE0, 0x15FE0, 0x12224, 0x173FC }; // Die Speicher-Kopie Offsets.
std::unique_ptr<uint8_t[]> SaveData = nullptr;
const std::string StartText = "Animal Crossing: Wild World Save Detector by SuperSaiyajinStackZ.\n\n\n"; // Der Start-Text.

void LoadSave(const std::string &FileName) {
	if (access(FileName.c_str(), F_OK) != 0) {
		std::cout << "The file does not exist!!\n";
		return;
	}

	FILE *in = fopen(FileName.c_str(), "rb");

	if (in) {
		fseek(in, 0, SEEK_END);
		const uint32_t size = ftell(in);
		fseek(in, 0, SEEK_SET);

		switch(size) {
			/* Gültige Speicher-Größen. */
			case 0x40000:
			case 0x4007A:
			case 0x80000:
			case 0x8007A:
				SaveData = std::unique_ptr<uint8_t[]>(new uint8_t[size]);
				fread(SaveData.get(), 1, size, in);

				for (uint8_t i = 0; i < 4; i++) {
					if (SaveData.get()[0] == Codes[i] && SaveData.get()[SavCopyOffsets[i]] == Codes[i]) {
						Region = (Regions)i;
						break;
					}
				}

				break;
		}

		fclose(in);
	}
}

void printRegion() {
	std::cout << "Detected Region: ";

	switch(Region) {
		case Regions::EUR:
			std::cout << "Europe.\n";
			break;

		case Regions::USA:
			std::cout << "USA.\n";
			break;

		case Regions::JPN:
			std::cout << "Japanese.\n";
			break;

		case Regions::KOR:
			std::cout << "Korean.\n";
			break;

		case Regions::UNKNOWN:
			std::cout << "Unknown.\n";
			break;
	}
}

int main(int argc, char *argv[]) {
	std::cout << StartText;

	if (argc > 1) {
		const std::string fileName = argv[1];

		std::cout << "The following parameter got detected: " << fileName << ".\n";

		LoadSave(fileName);
		printRegion();

	} else {
		std::cout << "You didn't provided a parameter!! Please drag and drop your savefile to the executable.\n";
	}

	std::string End;
	std::cin >> End;
	return 0;
}