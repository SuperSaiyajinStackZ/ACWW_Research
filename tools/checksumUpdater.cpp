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


/*
	Dies dient dazu, einen Checksum zu beheben, falls dieser Fehlerhaft ist.


	Was die CalculateChecksum(...) methode ausführt:

	1.) Stelle sicher, dass der Checksum Offset 16-Bit ausgerichtet ist.

	2.) Erstelle einen for loop, welcher ein uint16_t bis zum Ende der Speicherkopie Größe ist.
	NOTE: Weil dies uint16_t ist, muss der Checksum Offset & die Speicherkopie Größe durch 2 geteilt werden,
			ansonsten nimmt es die doppelte Größe an, da der for loop 2 byte groß ist.

	3.) Wir addieren alles vom Speicher-Buffer außer den Checksum Offset in eine uint16_t Checksum Variable.

	4.) Am Ende wiedergeben wir einen uint16_t, welcher dann 0 ist.. und subtrahieren den Checksum wert davon.
	NOTE: Weil ein uint16_t NIE kleiner als 0 wird, geht es stattdessen zurück zu 65535 / 0xFFFF
			und subtrahiert davon dann.. also: 0xFFFF - (Checksum - 1), das - 1, da wir von 0 schon eins
			subtrahiert haben.

	Dies wiedergibt den Checksum als uint16_t aus, welcher dann auf den Checksum Offset geschrieben wird.
*/

#include <cstring> // Benötigt für memcpy() und memcmp().
#include <iostream> // Benötigt für std::cout.
#include <memory> // Benötigt für std::unique_ptr.

/*
	Ein ganz einfacher enum class.. mit allen Regionen.

	Wird bei der Operation und der Region Erkennung verwendet.
*/
enum class Region {
	EUR_USA,
	JPN,
	KOR,
	UNKNOWN
};

/*
	Definiere ein paar Dinge.
*/

/* Europa | USA, Japan, Korea. */
static const uint32_t ChecksumOffsets[3] = { 0x15FDC, 0x12220, 0x173F8 }; // Checksum Offsets.
static const uint32_t SavCopySizes[3] = { 0x15FE0, 0x12224, 0x173FC }; // Speicherstand-Kopie Größen.

static const uint64_t ValidSizes[4] = { 0x40000, 0x4007A, 0x80000, 0x8007A }; // Gültige Größen.
const std::string startText = "Wild World Checksum Updater bei SuperSaiyajinStackZ.\n\n\n"; // Der Start-Text.

/*
	Variablen.
*/
static std::unique_ptr<uint8_t[]> SaveData = nullptr; // Die Speicherdaten.
static uint32_t SaveSize = 0; // Die Speicherstands-Größe.
static std::string Input = ""; // Für std::cin >>.
static Region SaveRegion = Region::UNKNOWN; // Die Region.

/*
	Kalkuliert den Checksum für Animal Crossing: Wild World.

	const uint16_t *Buffer: Die Speicherdaten.
	const uint64_t    Size: Die Speicherstands-Größe. (Muss durch 2 geteilt werden weil 2 byte).
	const uint16_t  Offset: Der Checksum Offset. (Muss durch 2 geteilt werden weil 2 byte).

	Was dies macht, ist im ersten Kommentar erklärt.
*/
static uint16_t CalculateChecksum(const uint16_t *Buffer, const uint64_t Size, const uint16_t Offset) {
	if ((Offset & 1) == 1) return 0; // Der Offset muss 16-Bit ausgerichtet sein!

	uint16_t Checksum = 0;
	for (uint16_t i = 0; i < Size; i++) {
		if (i == Offset) continue; // Falls der aktuelle index der Offset ist, wird dies übersprungen.
		Checksum += Buffer[i];
	}

	return (uint16_t) - Checksum; // Kann ebenfalls als: return 0xFFFF - (Checksum - 1); geschrieben werden.
}

/*
	Überprüft, ob der Checksum gültig ist.

	const uint16_t         *Buffer: Die Speicherdaten.
	const uint64_t            Size: Die Speicherstands-Größe. (Muss durch 2 geteilt werden weil 2 byte).
	const uint16_t          Offset: Der Checksum Offset. (Muss durch 2 geteilt werden weil 2 byte).
	const uint16_t CurrentChecksum: Der aktuelle Checksum's wert.

	Dies überprüft einfach nur.. ob der aktuelle Checksum korrekt ist.
*/
static bool IsChecksumValid(const uint16_t *Buffer, const uint64_t Size, const uint16_t Offset, const uint16_t CurrentChecksum) {
	return CalculateChecksum(Buffer, Size, Offset) == CurrentChecksum;
}

/*
	Lädt einen Speicherstand.

	const std::string &FILENAME: Der Pfad zur Datei.
*/
static bool LoadSave(const std::string &FILENAME) {
	bool result = false;

	FILE *in = fopen(FILENAME.c_str(), "rb");

	if (in) { // Stelle sicher der FILE zeiger ist gültig.
		fseek(in, 0, SEEK_END);
		SaveSize = ftell(in);
		fseek(in, 0, SEEK_SET);

		/* Überprüfe für Korrekte Größen. */
		for (uint8_t i = 0; i < 4; i++) {
			if (SaveSize == ValidSizes[i]) {
				result = true;
				break;
			}
		}

		if (result) { // Nur wenn die Größe passt.
			SaveData = std::unique_ptr<uint8_t[]>(new uint8_t[SaveSize]);
			fread(SaveData.get(), 1, SaveSize, in);

			/*
				Überprüfe für Regionen.

				Als erstes Japan, weil die kleinste Größe.
				Dann Europa / USA (beide haben die gleiche Struktur).
				Dann Korea.

				Dies vergleicht, ob die Speicherdaten beider Speicher-Kopien gleich sind.
			*/
			if (memcmp(SaveData.get(), SaveData.get() + SavCopySizes[1], SavCopySizes[1]) == 0) {
				SaveRegion = Region::JPN;

			} else if (memcmp(SaveData.get(), SaveData.get() + SavCopySizes[0], SavCopySizes[0]) == 0) {
				SaveRegion = Region::EUR_USA;

			} else if (memcmp(SaveData.get(), SaveData.get() + SavCopySizes[2], SavCopySizes[2]) == 0) {
				SaveRegion = Region::KOR;
			}
		}

		fclose(in);
	}

	return result;
}

/*
	Führe die Checksum Operation aus.
*/
static uint8_t DoOperation() {
	if (SaveData) { // Stelle sicher, Die Speicherdaten ist gültig.
		if (SaveRegion != Region::UNKNOWN) { // Nur, falls die Region erkannt wurde.

			const uint16_t checksum = *reinterpret_cast<uint16_t *>(SaveData.get() + ChecksumOffsets[(int)SaveRegion]);

			/* Überprüfe ob der Checksum gültig ist. */
			if (IsChecksumValid(reinterpret_cast<uint16_t *>(SaveData.get()), SavCopySizes[(int)SaveRegion] / sizeof(uint16_t), (ChecksumOffsets[(int)SaveRegion] / 2), checksum)) {
				std::cout << "Der Checksum ist gueltig! Es muss nichts behoben werden.\n";
				return 0; // Checksum gültig.

			} else {
				std::cout << "Der Checksum ist Fehlerhaft! Moechtest du den Checksum beheben?\nBestaetige mit 'Ja'.\n";
				std::cin >> Input;

				if (Input == "Ja") {
					std::cout << "Alter Checksum wert: " << checksum << ".\n";
					*reinterpret_cast<uint16_t *>(SaveData.get() + ChecksumOffsets[(int)SaveRegion]) = CalculateChecksum(reinterpret_cast<uint16_t *>(SaveData.get()), SavCopySizes[(int)SaveRegion] / sizeof(uint16_t), (ChecksumOffsets[(int)SaveRegion] / 2));
					std::cout << "Neuer Checksum wert: " << *reinterpret_cast<uint16_t *>(SaveData.get() + ChecksumOffsets[(int)SaveRegion]) << ".\n";

					/* Kopiere die erste Speicherstand Kopie zur Zweiten. */
					memcpy(SaveData.get() + SavCopySizes[(int)SaveRegion], SaveData.get(), SavCopySizes[(int)SaveRegion]);
					std::cout << "Checksum behoben!\n";
					return 1; // Checksum wurde behoben.
				}
			}
		}

	} else {
		return 2; // Region ist Unbekannt.
	}

	return 3; // Die Speicherdaten sind ungültig.
}

/*
	Schreibe zum Speicherstand.

	const std::string &FILENAME: Der Pfad zur Datei.
*/
static void WriteSave(const std::string &FILENAME) {
	if (SaveData) { // Stelle sicher, Die Speicherdaten ist gültig.
		FILE *out = fopen(FILENAME.c_str(), "wb");
		fwrite(SaveData.get(), 1, SaveSize, out);
		fclose(out);
	}
}

/*
	Gebe die erkannte Region aus.
*/
static void printDetected() {
	switch(SaveRegion) {
		case Region::EUR_USA:
			std::cout << "Ein EUR / USA Speicherstand wurde erkannt.\n";
			break;

		case Region::JPN:
			std::cout << "Ein JPN Speicherstand wurde erkannt.\n";
			break;

		case Region::KOR:
			std::cout << "Ein KOR Speicherstand wurde erkannt.\n";
			break;

		case Region::UNKNOWN:
			std::cout << "Erkannte Region ist Ungueltig! Ist dies wirklich ein Animal Crossing: Wild World Speicherstand?\n";
			break;
	}
}

int main(int argc, char *argv[]) {
	std::cout << startText;

	if (argc > 1) { // Die Argument anzahl muss 2 oder größer sein, damit dies funktioniert.
		const std::string fileName = argv[1]; // Argument 2 (1 weil array) ist das erste Argument, welches den Pfad enthält.
		std::cout << "Lade " << fileName << "...\n";

		LoadSave(fileName);
		printDetected();

		const uint8_t res = DoOperation();

		if (res == 1) { // res == 1 bedeutet: Der Checksum wurde behoben.
			std::cout << "Schreibe zum Speicherstand...\n";
			WriteSave(fileName);
		}

		std::cout << "Fertig!\n";
		std::cin >> Input;
		return 0;

	} else {
		std::cout << "Bitte ziehe einen Animal Crossing: Wild World Speicherstand auf die exe Datei!\n";
		std::cin >> Input;
		return 0;
	}
}