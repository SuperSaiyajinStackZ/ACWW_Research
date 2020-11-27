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

#include "checksum.hpp"
#include "common.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>

static const bool Debug = false;


/*
	Initialisiere die Checksum Klasse.

	const std::string &FileName: Pfad zur Speicher-Datei.
*/
Checksum::Checksum(const std::string &FileName) : FileName(FileName) {
	if (access(this->FileName.c_str(), F_OK) != 0) {
		std::cout << "The file does not exist!!\n";
		return;
	}

	FILE *in = fopen(this->FileName.c_str(), "rb");

	if (in) {
		fseek(in, 0, SEEK_END);
		this->SaveSize = ftell(in); // Erhalte die Datei-Größe.
		fseek(in, 0, SEEK_SET);

		switch(this->SaveSize) {
			/* Gültige Speicher-Größen. */
			case 0x40000:
			case 0x4007A:
			case 0x80000:
			case 0x8007A:
				this->SaveData = std::unique_ptr<uint8_t[]>(new uint8_t[this->SaveSize]);
				fread(this->SaveData.get(), 1, this->SaveSize, in);

				for (uint8_t i = 0; i < 4; i++) {
					if (SaveData.get()[0] == Codes[i] && SaveData.get()[SavCopyOffsets[i]] == Codes[i]) {
						this->Region = (WWRegion)i;
						break;
					}
				}

			break;
		}

		fclose(in);
	}
}

/*
	Kalkuliere den Checksum und fixe ihn.. optional.

	bool fixIt: Ob es gefixt werden soll oder nicht.

	NOTE: (Alle Daten außer des Checksums) + (Der Checksum) müssen 65536 ergeben. Das wäre 1 über
		  2 byte's maximale Größe und resultiert dann zu 0. Das ist, was das Spiel überprüft.
*/
uint16_t Checksum::CalculateMain(bool fixIt) {
	if (!this->SaveData) return 0; // Speicherdaten müssen gültig sein.
	if (this->Region == WWRegion::UNKNOWN) return 0; // Region muss bekannt sein.

	uint16_t ChecksVar = 0;

	for (uint16_t index = 0; index < (SavCopySizes[(int)this->Region] / 2); index++) {
		/* Falls der index dem Checksum offset entspricht, überspringe es. */
		if (index == (ChecksumOffsets[(int)this->Region] / 2)) continue;

		/*
			Addiere die Speicherdaten des aktuellen indexes zur Variable.
			Natürlich mit sizeof(uint16_t) da es 16-bit ausgerichtet ist.
		*/
		ChecksVar += *reinterpret_cast<uint16_t *>(this->SaveData.get() + (index * sizeof(uint16_t)));
	}

	if (fixIt) {
		/*
			Schreibe ChecksVar in den Checksum Offset.
			Natürlich führe die Operation aus, damit der richtige checksum wert rauskommt.
		*/
		*reinterpret_cast<uint16_t *>(this->SaveData.get() + ChecksumOffsets[(int)this->Region]) = (uint16_t) - ChecksVar;

		/*
			Kopiere zur zweiten Speicher Kopie.
			Das ist notwendig, da das Spiel 2 mal den selben Speicherstand beinhaltet.
		*/
		memcpy(this->SaveData.get() + SavCopySizes[(int)this->Region], this->SaveData.get(), SavCopySizes[(int)this->Region]);
	}

	if (Debug) {
		std::cout << "ChecksVar: " << ChecksVar << ".\n";
		std::cout << "Checksum: " << (uint16_t) - ChecksVar << ".\n";
	}

	return (uint16_t) - ChecksVar; // return 0xFFFF - (ChecksVar - 1); würde auch gehen, falls uint16_t nicht existiert.
}

/*
	Das ist.. quasi das selbe wie oben, nur für die Post-Lagerung im Rathaus.
*/
uint16_t Checksum::CalculateLetter(bool fixIt) {
	if (!this->SaveData) return 0; // Speicherdaten müssen gültig sein.
	if (this->Region == WWRegion::UNKNOWN) return 0; // Region muss bekannt sein.

	uint16_t ChecksVar = 0;

	for (uint16_t index = 0; index < (ChecksumLetterSizes[(int)this->Region] / 2); index++) {
		/* Falls der index dem Checksum offset entspricht, überspringe es. */
		if (index == (ChecksumLetterOffsets[(int)this->Region] / 2)) continue;

		/*
			Addiere die Speicherdaten des aktuellen indexes zur Variable.
			Natürlich mit sizeof(uint16_t) da es 16-bit ausgerichtet ist.
		*/
		ChecksVar += *reinterpret_cast<uint16_t *>(this->SaveData.get() + LetterStart[(int)this->Region] + (index * sizeof(uint16_t)));
	}

	if (fixIt) {
		/*
			Schreibe ChecksVar in den Checksum Offset.
			Natürlich führe die Operation aus, damit der richtige checksum wert rauskommt.
		*/
		*reinterpret_cast<uint16_t *>(this->SaveData.get() + 0x3FFFE) = (uint16_t) - ChecksVar;
	}

	if (Debug) {
		std::cout << "ChecksVar: " << ChecksVar << ".\n";
		std::cout << "Checksum: " << (uint16_t) - ChecksVar << ".\n";
	}

	return (uint16_t) - ChecksVar; // return 0xFFFF - (ChecksVar - 1); würde auch gehen, falls uint16_t nicht existiert.
}

/*
	Überprüfe ob der Haupt-Checksum gültig ist.
*/
bool Checksum::ChecksumMainValid() {
	if (!this->SaveData) return false; // Speicherdaten müssen gültig sein.
	if (this->Region == WWRegion::UNKNOWN) return false; // Region muss bekannt sein.

	/* Falls der aktuelle Checksum der Speicherdaten dem Kalkuliertem entsprechen, ist es gültig. */
	return (this->CalculateMain(false) ==
		*reinterpret_cast<uint16_t *>(this->SaveData.get() + ChecksumOffsets[(int)this->Region]));
}

/*
	Überprüfe ob der Checksum der Post-Lagerung gültig ist.
*/
bool Checksum::ChecksumLetterValid() {
	if (!this->SaveData) return false; // Speicherdaten müssen gültig sein.
	if (this->Region == WWRegion::UNKNOWN) return false; // Region muss bekannt sein.

	/* Falls der aktuelle Checksum der Speicherdaten dem Kalkuliertem entsprechen, ist es gültig. */
	return (this->CalculateLetter(false) ==
		*reinterpret_cast<uint16_t *>(this->SaveData.get() + 0x3FFFE));
}

/*
	Schreibe zurück zur Datei.
*/
void Checksum::WriteBack() {
	if (!this->SaveData) return; // Speicherdaten müssen gültig sein.

	if (this->Region != WWRegion::UNKNOWN) {
		FILE *out = fopen(this->FileName.c_str(), "wb");
		fwrite(this->SaveData.get(), 1, this->SaveSize, out);
		fclose(out);
	}
}

/*
	Gebe die erkannte Region aus.
*/
void Checksum::PrintRegion() {
	std::cout << "Detected region: ";

	switch(this->Region) {
		case WWRegion::EUR:
			std::cout << "Europe.\n\n\n";
			break;

		case WWRegion::USA:
			std::cout << "USA.\n\n\n";
			break;

		case WWRegion::JPN:
			std::cout << "Japanese.\n\n\n";
			break;

		case WWRegion::KOR:
			std::cout << "Korean.\n\n\n";
			break;

		case WWRegion::UNKNOWN:
			std::cout << "Unknown (Is probably not an Animal Crossing: Wild World save).\n\n\n";
			break;
	}
}

int main(int argc, char *argv[]) {
	bool needWrite = false;

	std::cout << StartText;

	if (argc > 1) {
		const std::string fileName = argv[1];

		std::cout << "The following parameter got detected: " << fileName << ".\n";

		std::unique_ptr<Checksum> checksum = std::make_unique<Checksum>(fileName);

		checksum->PrintRegion();
		if (checksum->RegionValid()) {
			std::cout << "Check main checksum...\n";

			if (!checksum->ChecksumMainValid()) {
				std::cout << "The main checksum is invalid! This will be fixed now...\n";
				checksum->CalculateMain(true);
				std::cout << "The main checksum got fixed!\n\n\n";
				needWrite = true;

			} else {
				std::cout << "The main checksum is good!\n\n\n";
			}

			std::cout << "check letter storage checksum...\n";
			if (!checksum->ChecksumLetterValid()) {
				std::cout << "The letter storage checksum is invalid! This will be fixed now...\n";
				checksum->CalculateLetter(true);
				std::cout << "The letter storage checksum got fixed!\n\n\n";
				needWrite = true;

			} else {
				std::cout << "The letter storage checksum is good!\n\n\n";
			}

			if (needWrite) {
				std::cout << "Writing to file...\n";
				checksum->WriteBack();
				std::cout << "Done!\n";

			} else {
				std::cout << "Because everything is good, nothing needs to be written to file.\nDone!\n";
			}

		} else {
			std::cout << "The region was not able to be detected.\nIs this really an Animal Crossing: Wild World Save?\n";
		}

	} else {
		std::cout << "You didn't provided a parameter!! Please drag and drop your savefile to the executable.\n";
	}

	std::string End;
	std::cin >> End;

	return 0;
}