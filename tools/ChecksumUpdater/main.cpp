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
		std::cout << "Die Datei existiert nicht!!\n";
		return;
	}

	FILE *in = fopen(this->FileName.c_str(), "rb");

	if (in) {
		fseek(in, 0, SEEK_END);
		this->SaveSize = ftell(in); // Erhalte die Datei-Größe.
		fseek(in, 0, SEEK_SET);

		/* Überprüfe für korrekte Größen. */
		for (uint8_t i = 0; i < 4; i++) {
			if (this->SaveSize == ValidSizes[i]) {
				this->SaveValid = true;
				break;
			}
		}

		if (this->SaveValid) {
			this->SaveData = std::unique_ptr<uint8_t[]>(new uint8_t[this->SaveSize]);
			fread(this->SaveData.get(), 1, this->SaveSize, in);

			/*
				Überprüfe nun für die Region.

				Dies überprüft aktuell die ersten 2 byte mit dem Start der zweiten Speicher-Kopie.
				NOTE: Unsicher ob das immer klappt.
			*/

			/* Japan. */
			if (memcmp(this->SaveData.get() + SavCopySizes[1], this->SaveData.get(), sizeof(uint16_t)) == 0) {
				this->Region = WWRegion::JPN;

			/* Europa und USA. */
			} else if (memcmp(this->SaveData.get() + SavCopySizes[0], this->SaveData.get(), sizeof(uint16_t)) == 0) {
				this->Region = WWRegion::EUR_USA;

			/* Korea. */
			} else if (memcmp(this->SaveData.get() + SavCopySizes[2], this->SaveData.get(), sizeof(uint16_t)) == 0) {
				this->Region = WWRegion::KOR;
			}
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
	std::cout << "Erkannte Region: ";

	switch(this->Region) {
		case WWRegion::EUR_USA:
			std::cout << "Europe | USA.\n\n\n";
			break;

		case WWRegion::JPN:
			std::cout << "Japan.\n\n\n";
			break;

		case WWRegion::KOR:
			std::cout << "Korea.\n\n\n";
			break;

		case WWRegion::UNKNOWN:
			std::cout << "Unbekannt (Ist eventuell kein Animal Crossing: Wild World Speicherstand).\n\n\n";
			break;
	}
}

int main(int argc, char *argv[]) {
	bool needWrite = false;

	std::cout << StartText;

	if (argc > 1) {
		const std::string fileName = argv[1];

		std::cout << "Folgender Parameter wurde erkannt: " << fileName << ".\n";

		std::unique_ptr<Checksum> checksum = std::make_unique<Checksum>(fileName);

		checksum->PrintRegion();
		if (checksum->RegionValid()) {
			std::cout << "Checke Haupt-Checksum...\n";

			if (!checksum->ChecksumMainValid()) {
				std::cout << "Der Checksum ist ungueltig! Dieser wird nun behoben...\n";
				checksum->CalculateMain(true);
				std::cout << "Der Haupt-Checksum wurde behoben!\n\n\n";
				needWrite = true;

			} else {
				std::cout << "Der Haupt-Checksum ist gut!\n\n\n";
			}

			std::cout << "Checke Post-Lagerungs-Checksum...\n";
			if (!checksum->ChecksumLetterValid()) {
				std::cout << "Der Checksum ist ungueltig! Dieser wird nun behoben...\n";
				checksum->CalculateLetter(true);
				std::cout << "Der Post-Lagerungs-Checksum wurde behoben!\n\n\n";
				needWrite = true;

			} else {
				std::cout << "Der Post-Lagerungs-Checksum ist gut!\n\n\n";
			}

			if (needWrite) {
				std::cout << "Schreibe nun zur Datei...\n";
				checksum->WriteBack();
				std::cout << "Fertig!\n";

			} else {
				std::cout << "Da alles gut war, muss nichts zur Datei geschrieben werden.\nFertig!\n";
			}

		} else {
			std::cout << "Die Region wurde nicht erkannt.\nIst das wirklich ein Animal Crossing: Wild World Speicherstand?\n";
		}

	} else {
		std::cout << "Du hast keinen Parameter uebergeben!! Bitte ziehe deinen Speicherstand auf die Anwendung.\n";
	}

	std::string End;
	std::cin >> End;
	return 0;
}