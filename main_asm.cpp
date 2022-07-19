#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

#define MAXCHAR_KOG 10
#define SCL_BUFFER 50

#define DWORD_S 2
#define CHAR_S 1

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

void firstCopy(char* buffer);

inline bool validChar(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')|| (ch >= '0' && ch <= '9') || ch == '_';
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

bool checkIns(std:: string ins)
{
	return ins == "NOP" || 
		ins == "SET" ||
		ins == "LOAD" ||
		ins == "RECT" ||
		ins == "ANIME" ||
		ins == "LOAD2" ||
		ins == "LOADEX" ||
		ins == "STOP" ||
		ins == "CALL" ||
		ins == "ATK" ||
		ins == "ESET" ||
		ins == "RET" ||
		ins == "ANM" ||
		ins == "FATK" ||
		ins == "ATKNP" ||
		ins == "MOV" ||
		ins == "ACC" ||
		ins == "ROL" ||
		ins == "WAITATOBJ" ||
		ins == "PSE" ||
		ins == "KILL" ||
		ins == "MDMG" ||
		ins == "CHILD" ||
		ins == "CHGTASK" ||
		ins == "PARENT" ||
		ins == "PMOV" ||
		ins == "PACC" ||
		ins == "PROL" ||
		ins == "PNOP" ||
		ins == "ATK2" ||
		ins == "EFC" ||
		ins == "TAMA" ||
		ins == "LASER" ||
		ins == "DEGE" ||
		ins == "DEGS" ||
		ins == "LLCHARGE" ||
		ins == "LLOPEN" ||
		ins == "LLCLOSE" ||
		ins == "HLASER" ||
		ins == "LSPHERE" ||
		ins == "RLASER" ||
		ins == "CROSS" ||
		ins == "FLOWER" ||
		ins == "GFIRE" ||
		ins == "IONRING" ||
		ins == "TALKMSG" ||
		ins == "TALKKEY" ||
		ins == "TALKNEWL" ||
		ins == "TALKWAIT" ||
		ins == "TASK" ||
		ins == "TEXMODE" ||
		ins == "PUSHR" ||
		ins == "POPR" ||
		ins == "MOVC" ||
		ins == "PUSHC" ||
		ins == "TJMP" ||
		ins == "FJMP" ||
		ins == "JMP" ||
		ins == "OJMP" ||
		ins == "AJMP" ||
		ins == "EXIT" ||
		ins == "LPOP" ||
		ins == "LJMP" ||
		ins == "ADD" ||
		ins == "SUB" ||
		ins == "MUL" ||
		ins == "DIV" ||
		ins == "MOD" ||
		ins == "NEG" ||
		ins == "SINL" ||
		ins == "COSL" ||
		ins == "RND" ||
		ins == "ATAN" ||
		ins == "EQUAL" ||
		ins == "NOTEQ" ||
		ins == "GREAT" ||
		ins == "LESS" ||
		ins == "GREATEQ" ||
		ins == "LESSEQ" ||
		ins == "MAX" ||
		ins == "MIN";
}

void getSubNames(char* buffer, std::string* sub_names, int sub_cnt, int& lv)
{

	for (int i = 0; i < sub_cnt; i++)
	{
		while (buffer[lv] != '@')
		{
			lv++;
		}
		lv++;
		int pos = lv, nmsize = 0;
		while (validChar(buffer[lv]))
		{
			nmsize++;
			lv++;
		}
		lv = pos;
		char* subNm = new char[nmsize];
		for (int j = 0; j < nmsize; j++)
		{
			subNm[j] = buffer[lv];
			subNm[j + 1] = 0x00;
			lv++;
		}
		sub_names[i] = subNm;
	}
	lv = 0;
}

void getLabNames(char* buffer, std::string* lab_names, int lab_cnt, int& lv)
{

	for (int i = 0; i < lab_cnt; i++)
	{
		while (buffer[lv] != '.')
		{
			if (buffer[lv] == '"')
			{
				lv++;
				while (buffer[lv] != '"')
				{
					lv++;
				}
			}

			lv++;
		}
		lv++;
		int pos = lv, nmsize = 0;
		while (validChar(buffer[lv]))
		{
			nmsize++;
			lv++;
		}
		lv = pos;
		char* labNm = new char[nmsize];
		for (int j = 0; j < nmsize; j++)
		{
			labNm[j] = buffer[lv];
			labNm[j + 1] = 0x00;
			lv++;
		}
		lab_names[i] = labNm;
	}
	lv = 0;
}

void getCmbNames(char* buffer, std::string* cmb_names, int cmb_cnt, int& lv)
{

	for (int i = 0; i < cmb_cnt; i++)
	{
		while (buffer[lv] != '[')
		{
			lv++;
		}
		lv++;
		int pos = lv, nmsize = 0;
		while (validChar(buffer[lv]))
		{
			nmsize++;
			lv++;
		}
		lv = pos;
		char* cmbNm = new char[nmsize];
		for (int j = 0; j < nmsize; j++)
		{
			cmbNm[j] = buffer[lv];
			cmbNm[j + 1] = 0x00;
			lv++;
		}
		cmb_names[i] = cmbNm;
	}
	lv = 0;
}

inline int ECStrSize(char* buffer, int& lv)
{
	int size = 0;
	while (buffer[lv] != '"')
	{
		lv++;
	}
	lv++;
	while (buffer[lv] != '"')
	{
		size++;
		lv++;
	}
	buffer[lv] = ' ';
	//std::cout << "Instruccion con string, tam: " << size << "\n";
	return size + 1;
}

inline void ofParam(int tms, char* buffer, int& lv)
{
	for (int i = 0; i < tms; i++)
	{
		//std::cout << "Pos actual antes de leer un argumento: " << lv << "\n";
		while (buffer[lv] != ',')
		{
			lv++;
		}
		lv++;
		//std::cout << "Pos actual despues de leer un argumento: " << lv << "\n";
	}
}

std::string getStr(char* buffer, int& lv)
{
	while (buffer[lv] != '"')
	{
		if (buffer[lv] != ' ')
		{
			printf("Error: Caracter invalido.\n");
		}
		lv++;
	}
	lv++;
	int pos = lv, size = 0;
	while (buffer[lv] != '"')
	{
		size++;
		lv++;
	}
	lv = pos;
	char* string = new char[size];
	for (int i = 0; i < size; i++)
	{
		string[i] = buffer[lv];
		string[i + 1] = 0x00;
		lv++;
	}
	return string;
}

int getNumber(char* buffer, int& lv)
{
	while (!(buffer[lv] >= '0' && buffer[lv] <= '9' || buffer[lv] == '-'))
	{
		if(buffer[lv] != ' ')
		{
			printf("Error: Caracter invalido.\n");
		}
		lv++;
	}
	int pos = lv, i = 0;
	while (buffer[lv] >= '0' && buffer[lv] <= '9' || buffer[lv] == '-')
	{
		i++;
		lv++;
	}
	lv = pos;
	char* num = new char[i];
	for(int j = 0; j < i; j++)
	{
		num[j] = buffer[lv];
		num[j + 1] = 0;
		lv++;
	}
	std::string conv = num;
	//std::cout << num << "\n";
	return	std::stoi(conv);;
}

int getVar(char* buffer, int& lv)
{
	while (buffer[lv] != 'v')
	{
		if (buffer[lv] != ' ')
		{

		}
		lv++;
	}
	if (buffer[lv + 1] == 'a' && buffer[lv + 2] == 'r')
	{
		if (buffer[lv + 3] == ' ')
		{

		}
		lv += 3;
	}
	else
	{
		printf("Error: No es una variable.\n");
	}
	return getNumber(buffer, lv);
}


void instSize(std::string ins, char* buffer, int& lv, FILE* file)
{
	byte cmd, p1 = 0x0, a_char[4];
	dword p2;
	int p3, fill =  0x00000000;
	std::string p4;

#define CM(x) cmd = x;	fputc(cmd, file);
#define ZE		fputc(0x00, file);
#define VAR		p1 = getVar(buffer, lv);	fputc(p1, file);
#define BY		p1 = getNumber(buffer, lv);	fputc(p1, file);
#define DW		p2 = getNumber(buffer, lv); convDwChar(p2, a_char);	fputc(a_char[0], file);fputc(a_char[1], file);
#define IN		p3 = getNumber(buffer, lv);	convIntChar(p3, a_char); fputc(a_char[0], file);fputc(a_char[1], file);fputc(a_char[2], file);fputc(a_char[3], file);
#define STR		p4 = getStr(buffer, lv);	fputs(p4.c_str(), file);
#define FILLER	for(int i = 0; i < 4; i++) {fputc(0x00, file);}

#define NEXT ofParam(1, buffer, lv);

	if (ins == "NOP")
	{
		CM(0x30) NEXT DW
	}
	else if (ins == "SET")
	{
		CM(0x31) NEXT DW NEXT DW NEXT FILLER
	}
	else if (ins == "LOAD")
	{
		BY 
		NEXT 
		CM(0x40) 
		BY
		NEXT
		STR
		ZE
	}
	else if (ins == "RECT")
	{
		CM(0x41) BY NEXT DW NEXT DW NEXT DW NEXT DW
	}
	else if (ins == "ANIME")
	{
		CM(0x42) BY
			char ar = p1;
		for(int i = 0; i < ar; i++)
		{
			NEXT DW
		}
	}
	else if (ins == "LOAD2")
	{
		BY NEXT CM(0x43) BY NEXT STR ZE
	}
	else if (ins == "LOADEX")
	{
		CM(0x44) STR ZE
	}
	else if (ins == "STOP")
	{
		CM(0x45)
	}
	else if (ins == "CALL")
	{
		CM(0x50) FILLER
	}
	else if (ins == "ATK")
	{
		CM(0x51) DW NEXT DW NEXT FILLER
	}
	else if (ins == "ESET")
	{
		CM(0x52) FILLER
	}
	else if (ins == "RET")
	{
		CM(0x53) VAR
	}
	else if (ins == "ANM")
	{
		CM(0x54) BY NEXT BY
	}
	else if (ins == "FATK")
	{
		CM(0x55) FILLER
	}
	else if (ins == "ATKNP")
	{
		CM(0x56) FILLER
	}
	else if (ins == "MOV")
	{
		CM(0x57) DW NEXT DW
	}
	else if (ins == "ACC")
	{
		CM(0x58) DW NEXT DW NEXT DW
	}
	else if (ins == "ROL")
	{
		CM(0x59) DW NEXT BY NEXT DW
	}
	else if (ins == "WAITATOBJ")
	{
		CM(0x5A)
	}
	else if (ins == "PSE")
	{
		CM(0x5B) BY
	}
	else if (ins == "KILL")
	{
		CM(0x5C)
	}
	else if (ins == "MDMG")
	{
		CM(0x5D) DW
	}
	else if (ins == "CHILD")
	{
		CM(0x5E) FILLER
	}
	else if (ins == "CHGTASK")
	{
		CM(0x5F) BY NEXT FILLER
	}
	else if (ins == "PARENT")
	{
		CM(0x60) BY
	}
	else if (ins == "PMOV")
	{
		CM(0x61) DW NEXT DW
	}
	else if (ins == "PACC")
	{
		CM(0x62) DW NEXT DW NEXT DW
	}
	else if (ins == "PROL")
	{
		CM(0x63) DW NEXT BY NEXT DW
	}
	else if (ins == "PNOP")
	{
		CM(0x64) DW
	}
	else if (ins == "ATK2")
	{
		CM(0x65) DW NEXT DW NEXT IN NEXT FILLER
	}
	else if (ins == "EFC")
	{
		CM(0x66) DW NEXT DW NEXT BY
	}
	else if (ins == "TAMA")
	{
		CM(0x70)
	}
	else if (ins == "LASER")
	{
		CM(0x71)
	}
	else if (ins == "DEGE")
	{
		CM(0x72)
	}
	else if (ins == "DEGS")
	{
		CM(0x73)
	}
	else if (ins == "LLCHARGE")
	{
		CM(0x74)
	}
	else if (ins == "LLOPEN")
	{
		CM(0x75) IN
	}
	else if (ins == "LLCLOSE")
	{
		CM(0x76)
	}
	else if (ins == "HLASER")
	{
		CM(0x77)
	}
	else if (ins == "LSPHERE")
	{
		CM(0x78)
	}
	else if (ins == "RLASER")
	{
		CM(0x79)
	}
	else if (ins == "CROSS")
	{
		CM(0x7A)
	}
	else if (ins == "FLOWER")
	{
		CM(0x7B)
	}
	else if (ins == "GFIRE")
	{
		CM(0x7C)
	}
	else if (ins == "IONRING")
	{
		CM(0x7D)
	}
	else if (ins == "TALKMSG")
	{
		CM(0x90) STR ZE
	}
	else if (ins == "TALKKEY")
	{
		CM(0x91)
	}
	else if (ins == "TALKNEW")
	{
		CM(0x92)
	}
	else if (ins == "TALKWAIT")
	{
		CM(0x93) DW
	}
	else if (ins == "TASK")
	{
		CM(0xA0) FILLER
	}
	else if (ins == "TEXMODE")
	{
		CM(0xA1) BY
	}
	else if (ins == "PUSHR")
	{
		VAR NEXT CM(0xC0)
	}
	else if (ins == "POPR")
	{
		VAR NEXT CM(0xC1)
	}
	else if (ins == "MOVC")
	{
		CM(0xC2) BY NEXT IN
	}
	else if (ins == "PUSHC")
	{
		CM(0xC4) IN
	}
	else if (ins == "TJMP")
	{
		CM(0xC5) FILLER
	}
	else if (ins == "FJMP")
	{
		CM(0xC6) FILLER
	}
	else if (ins == "JMP")
	{
		CM(0xC7) FILLER
	}
	else if (ins == "OJMP")
	{
		CM(0xCA) FILLER
	}
	else if (ins == "AJMP")
	{
		CM(0xCB) FILLER
	}
	else if (ins == "EXIT")
	{
		CM(0xCC)
	}
	else if (ins == "LPOP")
	{
		CM(0xCD)
	}
	else if (ins == "LJMP")
	{
		CM(0xCE) NEXT FILLER
	}
	else if (ins == "ADD")
	{
		CM(0xD0)
	}
	else if (ins == "SUB")
	{
		CM(0xD1)
	}
	else if (ins == "MUL")
	{
		CM(0xD2)
	}
	else if (ins == "DIV")
	{
		CM(0xD3)
	}
	else if (ins == "MOD")
	{
		CM(0xD4)
	}
	else if (ins == "NEG")
	{
		CM(0xD5)
	}
	else if (ins == "SINL")
	{
		CM(0xD6)
	}
	else if (ins == "COSL")
	{
		CM(0xD7)
	}
	else if (ins == "RND")
	{
		CM(0xD8)
	}
	else if (ins == "ATAN")
	{
		CM(0xD9)
	}
	else if (ins == "EQUAL")
	{
		CM(0xDA)
	}
	else if (ins == "NOTEQ")
	{
		CM(0xDB)
	}
	else if (ins == "GREAT")
	{
		CM(0xDC)
	}
	else if (ins == "LESS")
	{
		CM(0xDD)
	}
	else if (ins == "GREATEQ")
	{
		CM(0xDE)
	}
	else if (ins == "LESSEQ")
	{
		CM(0xDF)
	}
	else if (ins == "MAX")
	{
		CM(0xE0)
	}
	else if (ins == "MIN")
	{
		CM(0xE1)
	}
}

void determineSize(char* buffer, int lv, FILE* file)
{
	std::string instName;
	while (buffer[lv])
	{
		if (buffer[lv] == '@')
		{
			lv++;
			while (buffer[lv] != ':')
			{
				if (!validChar(buffer[lv]))
				{
					printf("Error: Carácter incorrecto para nombre de Subrutina.\n");
					exit(2);
				}
				lv++;
			}
		}
		if (buffer[lv] == '.')
		{
			lv++;
			while (buffer[lv] != ':')
			{
				if (!validChar(buffer[lv]))
				{
					printf("Error: Carácter incorrecto para nombre de etiqueta.\n");
					exit(2);
				}
				lv++;
			}
		}
		if (buffer[lv] == '[')
		{
			lv++;
			while (buffer[lv] != ']')
			{
				if (!validChar(buffer[lv]))
				{
					printf("Error: Carácter incorrecto para nombre de etiqueta de Combo.\n");
					exit(2);
				}
				lv++;
			}
		}
		if ((buffer[lv] >= 'A' && buffer[lv] <= 'Z')
			|| (buffer[lv] >= '0' && buffer[lv] <= '9')
			|| (buffer[lv] >= 'a' && buffer[lv] <= 'z'))
		{
			int pos = lv, nm_len = 0;
			while ((buffer[lv] >= 'A' && buffer[lv] <= 'Z')
				|| (buffer[lv] >= '0' && buffer[lv] <= '9')
				|| (buffer[lv] >= 'a' && buffer[lv] <= 'z'))
			{
				nm_len++;
				lv++;
			}
			lv = pos;
			char* insName = new char[nm_len];

			int i = 0;
			while ((buffer[lv] >= 'A' && buffer[lv] <= 'Z')
				|| (buffer[lv] >= '0' && buffer[lv] <= '9')
				|| (buffer[lv] >= 'a' && buffer[lv] <= 'z'))
			{
				insName[i] = buffer[lv];
				insName[i + 1] = 0x00;
				lv++;
				i++;
			}
			instName = insName;



			if (instName == "SCLlv1" || instName == "SCLlv2" || instName == "SCLlv3" || instName == "SCLlv4"
				|| instName == "AttackLv1" || instName == "AttackLv2" || instName == "BossAttack"
				|| instName == "Lv1Portrait" || instName == "Lv2Portrait" || instName == "BossPortrait" || instName == "WinPortrait"
				|| instName == "TexPortrait0" || instName == "TexPortrait1" || instName == "TexPortrait2" || instName == "TexPortrait3"
				|| instName == "TexPortrait4" || instName == "TexPortrait5" || instName == "TexPortrait6" || instName == "TexPortrait7"
				|| instName == "TexPortrait8")
			{

				while (buffer[lv] != '}')
				{
					lv++;
				}
				int d = 0;
			}
			else if (checkIns(instName))
			{
				std::cout << instName << "\n";
				if (instName == "EXIT" || instName == "RET")
					printf("\n");
				instSize(instName, buffer, lv, file);
				while (buffer[lv] != 0x0A)
				{
					lv++;
				}
			}
			else
			{
				printf("Error en posición %d, mnemonico desconocido.\n", lv);
				exit(2);
			}
		}
		else
		{
			lv++;
		}
	}
}

void createFile(std::string* sub, std::string* lab, std::string* cmb, char* buffer, int sub_cnt, int lab_cnt, int cmb_cnt, int sSize, SCLHeader&);

void ReadFile(char* buffer, long lSize)
{
	SCLHeader Header;
	bool onSub = false;
	int lv = 0, cnt_ins = 0, sub_cnt = 0, lab_cnt = 0, cmb_cnt = 0;
	std::string instName;
	while (lv < lSize)
	{
		if (buffer[lv] == '@')
		{
			if (!onSub)
			{
				onSub = true;
			}
			lv++;
			while (buffer[lv] != ':')
			{
				if (!validChar(buffer[lv]))
				{
					printf("Error: Carácter incorrecto para nombre de Subrutina.\n");
					exit(2);
				}
				lv++;
			}
			sub_cnt++;
#ifdef CLB_DEBUG
			printf("Nombre de Subrutina leida correctamente.\n");
#endif
		}
		if (buffer[lv] == '.')
		{
			lv++;
			while (buffer[lv] != ':')
			{
				if (!validChar(buffer[lv]))
				{
					printf("Error: Carácter incorrecto para nombre de etiqueta.\n");
					exit(2);
				}
				lv++;
			}
			lab_cnt++;
#ifdef CLB_DEBUG
			printf("Nombre de etiqueta leida correctamente.\n");
#endif
		}
		if (buffer[lv] == '[')
		{
			lv++;
			while (buffer[lv] != ']')
			{
				if (!validChar(buffer[lv]))
				{
					printf("Error: Carácter incorrecto para nombre de etiqueta de Combo.\n");
					exit(2);
				}
				lv++;
			}
			cmb_cnt++;
#ifdef CLB_DEBUG
			printf("Etiqueta de Combo Attack leida correctamente.\n");
#endif
		}
		char chara = buffer[lv];
		if ((buffer[lv] >= 'A' && buffer[lv] <= 'Z') 
			|| (buffer[lv] >= '0' && buffer[lv] <= '9')
			|| (buffer[lv] >= 'a' && buffer[lv] <= 'z'))
		{
			int pos = lv, nm_len = 0;
			while ((buffer[lv] >= 'A' && buffer[lv] <= 'Z') 
				|| (buffer[lv] >= '0' && buffer[lv] <= '9')
				|| (buffer[lv] >= 'a' && buffer[lv] <= 'z'))
			{
				nm_len++;
				lv++;
			}
			lv = pos;
			char* insName = new char[nm_len];

			int i = 0;
			while ((buffer[lv] >= 'A' && buffer[lv] <= 'Z') 
				|| (buffer[lv] >= '0' && buffer[lv] <= '9') 
				|| (buffer[lv] >= 'a' && buffer[lv] <= 'z'))
			{
				insName[i] = buffer[lv];
				insName[i + 1] = 0x00;
				lv++;
				i++;
			}
			instName = insName;



				if (instName == "SCLlv1" || instName == "SCLlv2" || instName == "SCLlv3" || instName == "SCLlv4" 
					|| instName == "AttackLv1" || instName == "AttackLv2" || instName == "BossAttack"
					|| instName == "Lv1Portrait" || instName == "Lv2Portrait" || instName == "BossPortrait" || instName == "WinPortrait"
					|| instName == "TexPortrait0" || instName == "TexPortrait1" || instName == "TexPortrait2" || instName == "TexPortrait3"
					|| instName == "TexPortrait4" || instName == "TexPortrait5" || instName == "TexPortrait6" || instName == "TexPortrait7"
					|| instName == "TexPortrait8")
				{
					if (buffer[lv] != '{')
					{
						printf("Error: Caracter no admitido para variable del SCL Header.\n");
					}
					else
					{
						while (buffer[lv] != 0x0A)
						{
							lv++;
						}
						lv++;
					}
					while (buffer[lv] != '}')
					{
						if (buffer[lv] == 0x0a)
						{
							if (instName == "SCLlv1")
							{
								Header.Lvl1SCL++;
							}
							else if (instName == "SCLlv2")
							{

								Header.Lvl2SCL++;
							}
							else if (instName == "SCLlv3")
							{
								Header.Lvl3SCL++;
							}
							else if (instName == "SCLlv4")
							{
								Header.Lvl4SCL++;
							}
						}
						lv++;
					}
					int d = 0;
				}
				else if (checkIns(instName))
				{
					onSub = !(instName == "EXIT" || instName == "RET");
#ifdef CLB_DEBUG
					if(onSub)
					{
					//	printf("Instruccion leida correctamente.\n");
					}
					else
					{
					//	printf("Fin de la Subrutina.\n");
					}
#endif // CLB_DEBUG

					cnt_ins++;
					while (buffer[lv] != 0x0A)
					{
						lv++;
					}
				}
				else
				{
					printf("Error en posición %d, mnemonico desconocido.\n", lv);
					exit(2);
				}
		}
		else
		{
			lv++;
		}
	}
	lv = 0;
	std::string* sub_names = new std::string [sub_cnt];
	std::string* lab_names = new std::string [lab_cnt];
	std::string* cmb_names = new std::string [cmb_cnt];

	getSubNames(buffer, sub_names, sub_cnt, lv);
	getLabNames(buffer, lab_names, lab_cnt, lv);
	getCmbNames(buffer, cmb_names, cmb_cnt, lv);
/*
#ifdef CLB_DEBUG
	for (int i = 0; i < sub_cnt; i++)
	{
		std::cout << sub_names[i] << "\n";
	}
	for (int i = 0; i < lab_cnt; i++)
	{
		std::cout << lab_names[i] << "\n";
	}
	for (int i = 0; i < cmb_cnt; i++)
	{
		std::cout << cmb_names[i] << "\n";
	}
#endif // CLB_DEBUG
*/

	int* sub_add = new int[sub_cnt];
	int* lab_add = new int[lab_cnt];
	int* cmb_add = new int[cmb_cnt];

	firstCopy(buffer);
	//createFile(sub_names, lab_names, cmb_names, buffer, sub_cnt, lab_cnt, cmb_cnt, Header);
}

void writeOnHeader(SCLHeader& Header)
{

}

void firstCopy(char* buffer)
{
	FILE* pFile;
	int lv = 0;
	pFile = fopen("output.scl", "wb");
	if (pFile != NULL)
	{
		//fputs("Me encanta ser basado", pFile);
		//fputc(0x30, pFile);
		//int a = 0xffffffff;
		//char b;
		//fputc(a, pFile);
		determineSize(buffer, lv, pFile);
	}
	fclose(pFile);
}

void createFile(std::string* sub, std::string* lab, std::string* cmb, char* buffer, int sub_cnt, int lab_cnt, int cmb_cnt, SCLHeader& Header)
{


	FILE* pFile;
	char* SCLFile = (char*)malloc(sizeof(SCLHeader));
	pFile = fopen("output.scl", "wb");



	memcpy(SCLFile, &Header, sizeof(Header));
}

/*
void determineSize()
{

	for (int j = 0 ; j < 0xff ; j++)
	{
		bool isIns = (j == 0x30 || j == 0x31 || (j >= 0x40 && j <= 0x45) ||
			(j >= 0x50 && j <= 0x66) || (j >= 0x70 && j <= 0x7d) ||
			(j >= 0x90 && j <= 0x93) || j == 0xa0 || j == 0xa1 ||
			(j >= 0xc0 && j <= 0xc7) || (j >= 0xca && j <= 0xce) ||
			(j >= 0xd0 && j <= 0xe1));
		if (!isIns)
		{
			finalfilesize[j] *= 0;
		}
	}
	finalfilesize[0x30] *= 3;
	finalfilesize[0x31] *= 9;
	//finalfilesize[0x40] *= 4;
	finalfilesize[0x41] *= 10;
	//finalfilesize[0x44] *= 2;
	finalfilesize[0x45] *= 1;
	finalfilesize[0x50] *= 5;
	finalfilesize[0x51] *= 9;
	finalfilesize[0x52] *= 5;
	finalfilesize[0x53] *= 2;
	finalfilesize[0x54] *= 3;
	finalfilesize[0x55] *= 5;
	finalfilesize[0x56] *= 5;
	finalfilesize[0x57] *= 5;
	finalfilesize[0x58] *= 7;
	finalfilesize[0x59] *= 6;
	finalfilesize[0x5a] *= 1;
	finalfilesize[0x5b] *= 2;
	finalfilesize[0x5d] *= 3;
	finalfilesize[0x5e] *= 6;
	finalfilesize[0x5f] *= 6;
	finalfilesize[0x60] *= 2;
	finalfilesize[0x61] *= 5;
	finalfilesize[0x62] *= 7;
	finalfilesize[0x63] *= 6;
	finalfilesize[0x64] *= 3;
	finalfilesize[0x65] *= 13;
	finalfilesize[0x66] *= 6;
	finalfilesize[0x75] *= 5;
	finalfilesize[0x93] *= 3;
	finalfilesize[0xa0] *= 5;
	finalfilesize[0xa1] *= 2;
	finalfilesize[0xc0] *= 2;
	finalfilesize[0xc1] *= 2;
	finalfilesize[0xc2] *= 6;
	finalfilesize[0xc4] *= 5;
	finalfilesize[0xc5] *= 5;
	finalfilesize[0xc6] *= 5;
	finalfilesize[0xc7] *= 5;
	finalfilesize[0xca] *= 5;
	finalfilesize[0xcb] *= 5;
//	finalfilesize[0xcd] *= 5;
	finalfilesize[0xce] *= 5;
	int resultSize = 0;
	for (int i = 0; i < 0xff; i++)
	{
		resultSize += finalfilesize[i];
	}
	resultSize += sizeof(SCLHeader);
}
*/

char* openFile(const char* fileName, long& lSize)
{

	char* buffer;

	//This code has been taken from https://cplusplus.com/reference/cstdio/fread/
	FILE* pFile;
	size_t result;

	pFile = fopen(fileName, "rb");
	if (pFile == NULL) { fputs("Couldnt open the file", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize) { fputs("Reading error", stderr); exit(3); }
	//Until here

	return buffer;
}

int main()
{
	std::string filename;
	std::cout << "Nombre del archivo: "; std::cin >> filename;
	const char* fName = filename.c_str();
	long fileSize;
	char* buffer = openFile(fName, fileSize);

	ReadFile(buffer, fileSize);
	free(buffer);
}
