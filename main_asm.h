#pragma once

#include <stdio.h>

#define MAXCHAR_KOG 10
#define SCL_BUFFER 50

#define CMD 1
#define CHAR_S 1
#define DWORD_S 2
#define INT_S 4

typedef char byte;
typedef short dword;


struct texEntry
{
	int entryPoint[12];
	int num_tex = 0;
	texEntry()
	{
		std::memset(entryPoint, 0xffffffff, sizeof(entryPoint));
	}
};

struct SCLHeader
{
	//Cantidad de enemigos en niveles del SCL, no de los ataques de los jugadores.
	int Lvl1SCL = 0;
	int Lvl2SCL = 0;
	int Lvl3SCL = 0;
	int Lvl4SCL = 0;

	//Dirección del Inicializador de texturas.
	int TexInit = 0;

	//Direcciones de los ataques de los jugadores.
	int Lv1Attack[MAXCHAR_KOG];
	int Lv2Attack[MAXCHAR_KOG];
	int BossAttack[MAXCHAR_KOG];
	int ComboAttack[MAXCHAR_KOG];

	//Direcciones de los retratos y sus scripts.
	int AnmLv1[MAXCHAR_KOG];
	int AnmLv2[MAXCHAR_KOG];
	int AnmBoss[MAXCHAR_KOG];
	int AnmWin[MAXCHAR_KOG];

	//Timelines
	int SCL_lv1[SCL_BUFFER];
	int SCL_lv2[SCL_BUFFER];
	int SCL_lv3[SCL_BUFFER];
	int SCL_lv4[SCL_BUFFER];

	//Direcciones de los retratos (?
	texEntry LTEntry[MAXCHAR_KOG];

	SCLHeader()
	{
		std::memset(Lv1Attack, 0xffffffff, sizeof(Lv1Attack));
		std::memset(Lv2Attack, 0xffffffff, sizeof(Lv2Attack));
		std::memset(BossAttack, 0xffffffff, sizeof(BossAttack));
		std::memset(ComboAttack, 0xffffffff, sizeof(ComboAttack));

		std::memset(AnmLv1, 0xffffffff, sizeof(AnmLv1));
		std::memset(AnmLv2, 0xffffffff, sizeof(AnmLv2));
		std::memset(AnmBoss, 0xffffffff, sizeof(AnmBoss));
		std::memset(AnmWin, 0xffffffff, sizeof(AnmWin));

		std::memset(SCL_lv1, 0xffffffff, sizeof(SCL_lv1));
		std::memset(SCL_lv2, 0xffffffff, sizeof(SCL_lv2));
		std::memset(SCL_lv3, 0xffffffff, sizeof(SCL_lv3));
		std::memset(SCL_lv4, 0xffffffff, sizeof(SCL_lv4));
	}
};

void createFile(char* buffer, SCLHeader& Header, int* sub, int* lab, int* cmd, std::string* sub_names, std::string* lab_names, std::string* cmb_names, bool, const char*);

inline bool validChar(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_';
}

inline int32_t convCharInt(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

inline int16_t convCharInt(uint8_t c1, uint8_t c2) {
	return c2 << 8 | c1;
}

inline uint32_t convCharUInt(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) {
	return c4 << 24 | c3 << 16 | c2 << 8 | c1;
}

inline uint16_t convCharUInt(uint8_t c1, uint8_t c2) {
	return c2 << 8 | c1;
}

inline uint8_t uChar(uint8_t ch)
{
	return ch;
}

inline void convIntChar(int32_t num, char* c_num)
{
	for (int i = 0; i < 4; i++)
	{
		c_num[i] = num;
		num >>= 8;
	}
}

inline void convDwChar(int32_t num, char* c_num)
{
	for (int i = 0; i < 2; i++)
	{
		c_num[i] = num;
		num >>= 8;
	}
}