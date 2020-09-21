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

#ifndef _ACWW_UTILS_DEFINITIONS_HPP
#define _ACWW_UTILS_DEFINITIONS_HPP

/* NOTE: EUR & USA are the same. */

/* Sizes. */
	/* Player. */
	#define EUR_PLR_SIZE			0x228C
	#define JPN_PLR_SIZE			0x1D10
	#define KOR_PLR_SIZE			0x249C
	#define PLAYER_SIZE				4

	/* Letters. */
	#define EUR_LTR_SIZE			0xF4
	#define JPN_LTR_SIZE			0x8C
	#define KOR_LTR_SIZE			0x100
	#define PLAYER_LETTER_SIZE		10

	/* Player specific sizes. */
	#define PLAYER_LETTER_SIZE		10
	#define PLAYER_PATTERN_SIZE		8

	/* Villagers. */
	#define EUR_VLG_SIZE			0x700
	#define JPN_VLG_SIZE			0x5C0
	#define KOR_VLG_SIZE			0x7EC
	#define VILLAGER_SIZE			8

	/* Patterns. */
	#define EUR_PTRN_SIZE			0x228
	#define JPN_PTRN_SIZE			0x220
	#define KOR_PTRN_SIZE			0x234

	/* Acres. */
	#define ACRE_SIZE				0x24

	/* Town Map Items. */
	#define TOWN_MAP_ITEM_SIZE		0x1000

	/* Able sister Pattern size. */
	#define ABLE_SISTER_PTRN_SIZE	8



/* Offsets. */
	/* Player. */
	#define EUR_PLR_START				0x000C
	#define JPN_PLR_START				0x000C
	#define KOR_PLR_START				0x0014

	/* Player Letters. */
	#define EUR_PLAYER_LETTER_START		EUR_PLR_START + 0x114C
	#define JPN_PLAYER_LETTER_START		JPN_PLR_START + 0x110C
	#define KOR_PLAYER_LETTER_START		KOR_PLR_START + 0x11AC

	/* Player Patterns. */
	#define EUR_PLAYER_PATTERN_START	EUR_PLR_START
	#define JPN_PLAYER_PATTERN_START	JPN_PLR_START
	#define KOR_PLAYER_PATTERN_START	KOR_PLR_START

	/* Villagers. */
	#define EUR_VLG_START				0x8A3C
	#define JPN_VLG_START				0x744C
	#define KOR_VLG_START				0x928C

	/* Acres. */
	#define EUR_ACRE_START				0xC330
	#define JPN_ACRE_START				0xA32C
	#define KOR_ACRE_START				0xD304

	/* Town Map Items. */
	#define EUR_MAP_ITEM_START			0xC354
	#define JPN_MAP_ITEM_START			0xA350
	#define KOR_MAP_ITEM_START			0xD328

	/* Able Sister Pattern. */
	#define EUR_ABLE_SISTER_PTRN		0xFAFC
	#define JPN_ABLE_SISTER_PTRN		0xDAF8
	#define KOR_ABLE_SISTER_PTRN		0x10AD0

#endif