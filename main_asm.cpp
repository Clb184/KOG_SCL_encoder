#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>

#define MAXCHAR_KOG 10
#define SCL_BUFFER 50

struct texEntry
{
	int entryPoint[12];
	int num_tex;
};

struct SCLHeader
{
	//Cantidad de enemigos en niveles del SCL, no de los ataques de los jugadores.
	int Lvl1SCL;
	int Lvl2SCL;
	int Lvl3SCL;
	int Lvl4SCL;

	//Dirección del Inicializador de texturas.
	int TexInit;

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
};



unsigned int ins_namePos[0xff];
unsigned int ins_nameLength[0xff];
unsigned int finalfilesize[0xff];
std::string ins_name[0xff];

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

char* buffer;
long lSize;

void determineSize();

inline bool validChar(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')|| (ch >= '0' && ch <= '9') || ch == '_';
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


void ReadFile()
{
	//unsigned int finalfilesize[0xff];
	for (int i = 0; i < 0xff; i++)
	{
		finalfilesize[i] = 0;
	}
	bool onSub = false;
	int lv = 0, cnt_ins = 0, sub_cnt = 0, lab_cnt = 0, cmb_cnt = 0, strsize = 0;
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
					while (buffer[lv] != '}')
					{
						lv++;
					}
					int d = 0;
				}
				else if (checkIns(instName))
				{

					printf("Instruccion leida correctamente.\n");
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

	for (int j = 0; j < 0xff; j++)
	{
		if (finalfilesize[j])
		{
			std::cout << "Cantidad " << ins_name[j] << ": " << finalfilesize[j] << "\n";
		}
	}
	lv = 0;
	std::string* sub_names = new std::string [sub_cnt];
	std::string* lab_names = new std::string [lab_cnt];
	std::string* cmb_names = new std::string [cmb_cnt];
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
		std::cout << sub_names[i] << "\n";
	}
	lv = 0;
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
		std::cout << lab_names[i] << "\n";
	}
	lv = 0;
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
		std::cout << cmb_names[i] << "\n";
	}
	lv = 0;




	determineSize();
}

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

void openFile(const char* fileName)
{


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

}



int main()
{
	std::string filename;
	std::cout << "Nombre del archivo: "; std::cin >> filename;
	const char* fName = filename.c_str();
	openFile(fName);
	ReadFile();
	free(buffer);
}
