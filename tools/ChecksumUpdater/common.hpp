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

#ifndef _CHECKSUM_UPDATER_COMMON_HPP
#define _CHECKSUM_UPDATER_COMMON_HPP

#include <string>

/*
	Ein ganz einfacher enum class.. mit allen Regionen.

	Wird bei der Operation und der Region Erkennung verwendet.
*/
enum class WWRegion {
	EUR,
	USA,
	JPN,
	KOR,
	UNKNOWN
};

/*
	Definiere ein paar Dinge.

	Europa & USA, Japan, Korea.
*/

/* Haupt Zeug. */
static const uint32_t ChecksumOffsets[4] = { 0x15FDC, 0x15FDC, 0x12220, 0x173F8 }; // Checksum Offsets.
static const uint32_t SavCopySizes[4] = { 0x15FE0, 0x15FE0, 0x12224, 0x173FC }; // Speicherstand-Kopie Größen.
static const uint32_t SavCopyOffsets[4] = { 0x15FE0, 0x15FE0, 0x12224, 0x173FC }; // Zweite Speicher-Kopie Offsets.
static const uint8_t Codes[4] = { 0xC5, 0x8A, 0x32, 0x32 }; // Die Spielcodes.

/* Post-Lagerungs Zeug. */
static const uint32_t ChecksumLetterOffsets[4] = { 0x11DF2, 0x11DF2, 0xA412, 0xC802 }; // Post-Lagerung Offsets.
static const uint32_t LetterStart[4] = { 0x2E20C, 0x2E20C, 0x35BEC, 0x337FC }; // Post-Lagerung Start Offsets.
static const uint32_t ChecksumLetterSizes[4] = { 0x11DF4, 0x11DF4,  0xA414, 0xC804 }; // Post-Lagerung Größen.

static const std::string StartText = "Animal Crossing: Wild World Checksum Updater by SuperSaiyajinStackZ.\n\n\n"; // Der Start-Text.

#endif