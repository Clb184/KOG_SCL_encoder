#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../KOG_SCL_comp_asm/KOG_SCL_comp_asm/main_asm.h"

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
			readComent(buffer, lv);
			lv++;
		}
		lv++;
		int pos = lv, nmsize = 0;
		while (validChar(buffer[lv]))
		{
			readComent(buffer, lv);
			nmsize++;
			lv++;
		}
		lv = pos;
		char* subNm = new char[nmsize];
		for (int j = 0; j < nmsize; j++)
		{
			readComent(buffer, lv);
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
			

			readComent(buffer, lv);
			lv++;
		}
		readComent(buffer, lv);
		lv++;
		int pos = lv, nmsize = 0;
		while (validChar(buffer[lv]))
		{
			readComent(buffer, lv);
			nmsize++;
			lv++;
		}
		lv = pos;
		char* labNm = new char[nmsize];
		for (int j = 0; j < nmsize; j++)
		{
			readComent(buffer, lv);
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
			readComent(buffer, lv);
			lv++;
		}
		lv++;
		int pos = lv, nmsize = 0;
		while (validChar(buffer[lv]))
		{
			readComent(buffer, lv);
			nmsize++;
			lv++;
		}
		lv = pos;
		char* cmbNm = new char[nmsize];
		for (int j = 0; j < nmsize; j++)
		{
			readComent(buffer, lv);
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
			readComent(buffer, lv);
			lv++;
		}
		lv++;
		//std::cout << "Pos actual despues de leer un argumento: " << lv << "\n";
	}
}

std::string getStr(char* buffer, int& lv, int& strSize)
{
	while (buffer[lv] != '"')
	{
		if (buffer[lv] != ' ')
		{
			printf("Error en pos 0x%x: Caracter invalido.\n", lv);
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
	strSize = size + 1;
	return string;
}

int getNumber(char* buffer, int& lv)
{
	while (!(buffer[lv] >= '0' && buffer[lv] <= '9' || buffer[lv] == '-'))
	{
		readComent(buffer, lv);
		if(buffer[lv] != ' ')
		{
			printf("Error en pos 0x%x: Caracter invalido.\n", lv);
			exit(2);
		}
		lv++;
	}
	int pos = lv, i = 0;
	while (buffer[lv] >= '0' && buffer[lv] <= '9' || buffer[lv] == '-')
	{
		readComent(buffer, lv);
		i++;
		lv++;
	}
	lv = pos;
	char* num = new char[i];
	for(int j = 0; j < i; j++)
	{
		readComent(buffer, lv);
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
		readComent(buffer, lv);
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
		readComent(buffer, lv);
	}
	else
	{
		printf("Error en pos 0x%x: No es una variable.\n", lv);
		exit(2);
	}
	return getNumber(buffer, lv);
}

std::string readSub(char* buffer, int& lv)
{
	while (!((buffer[lv] >= 'A' && buffer[lv] <= 'Z') ||
		(buffer[lv] >= 'a' && buffer[lv] <= 'z') ||
		(buffer[lv] >= '0' && buffer[lv] <= '9') ||
		buffer[lv] == '_'))
	{
		readComent(buffer, lv);
		if (buffer[lv] != ' ')
		{
			printf("Error en pos 0x%x: Caracter invalido", lv);
			exit(2);
		}
		lv++;
	}
	int pos = lv, size = 0;
	while ((buffer[lv] >= 'A' && buffer[lv] <= 'Z') ||
		(buffer[lv] >= 'a' && buffer[lv] <= 'z') ||
		(buffer[lv] >= '0' && buffer[lv] <= '9') ||
		buffer[lv] == '_')
	{
		readComent(buffer, lv);
		size++;
		lv++;
	}
	lv = pos;
	char* nm = new char[size];
	for (int i = 0; i < size; i++)
	{
		readComent(buffer, lv);
		nm[i] = buffer[lv];
		nm[i + 1] = 0;
		lv++;
	}
	//std::string strname = name;
	return nm;
}

int getAdd(char* buffer, int& lv, int* typ, std::string* typ_names)
{
	std::string typName = readSub(buffer, lv);;
	int i = 0;
	while (typName != typ_names[i])
	{
		i++;
	}
	return typ[i];
}

void instSize(std::string ins, char* buffer, int& lv, FILE* file, int& sclPos, bool wrtAdd, int* sub, int* lab, int* cmb,
	std::string* sub_names, std::string* lab_names, std::string* cmb_names)
{
	byte cmd, p1 = 0x0, a_char[4];
	dword p2;
	int p3, fill =  0x00000000, str_size = 0;
	std::string p4, sub_nm, lab_nm;

#define CM(x) cmd = x;	fputc(cmd, file);
#define ZE		fputc(0x00, file);
#define VAR		p1 = getVar(buffer, lv);	fputc(p1, file);
#define BY		p1 = getNumber(buffer, lv);	fputc(p1, file);
#define DW		p2 = getNumber(buffer, lv); convDwChar(p2, a_char);	fputc(a_char[0], file);fputc(a_char[1], file);
#define IN		p3 = getNumber(buffer, lv);	convIntChar(p3, a_char); fputc(a_char[0], file);fputc(a_char[1], file);fputc(a_char[2], file);fputc(a_char[3], file);
#define SUB		if(wrtAdd) { p3 = getAdd(buffer, lv, sub, sub_names); convIntChar(p3, a_char); for(int i = 0; i < 4; i++) {fputc(a_char[i], file);} } else { readSub(buffer, lv); }
#define LAB		if(wrtAdd) { p3 = getAdd(buffer, lv, lab, lab_names); convIntChar(p3, a_char); for(int i = 0; i < 4; i++) {fputc(a_char[i], file);} } else { readSub(buffer, lv); }
//#define CMB		if(wrtAdd) { p3 = getAdd(buffer, lv, cmb, cmb_names); convIntChar(p3, a_char); for(int i = 0; i < 4; i++) {fputc(a_char[i], file);} } else {  }
#define STR		int pos = lv; lv = pos; p4 = getStr(buffer, lv, str_size);	fputs(p4.c_str(), file);
#define FILLER	if(!wrtAdd) {for(int i = 0; i < 4; i++) {fputc(0x00, file);} }

#define NEXT ofParam(1, buffer, lv);
#define SCLPOS(x) sclPos += x + 1 + str_size;

	if (ins == "NOP")
	{
		CM(0x30) DW SCLPOS(DWORD_S)
	}
	else if (ins == "SET")
	{
		CM(0x31) DW NEXT DW NEXT SUB FILLER SCLPOS(DWORD_S + DWORD_S + INT_S)
	}
	else if (ins == "LOAD")
	{
		BY NEXT CM(0x40) BY	NEXT
		STR
		ZE SCLPOS(CHAR_S * 2)
	}
	else if (ins == "RECT")
	{
		CM(0x41) BY NEXT DW NEXT DW NEXT DW NEXT DW SCLPOS(CHAR_S + (DWORD_S * 4))
	}
	else if (ins == "ANIME")
	{
		CM(0x42) 
			BY NEXT BY
			char ar = p1;
		for(int i = 0; i < ar; i++)
		{
			NEXT 
				BY
		} SCLPOS( (2 * CHAR_S) + ar)
	}
	else if (ins == "LOAD2")
	{
		BY NEXT CM(0x43) BY NEXT STR ZE SCLPOS(CHAR_S * 2)
	}
	else if (ins == "LOADEX")
	{
		CM(0x44) STR ZE SCLPOS()
	}
	else if (ins == "STOP")
	{
		CM(0x45) SCLPOS(0)
	}
	else if (ins == "CALL")
	{
		CM(0x50) SUB FILLER SCLPOS(INT_S)
	}
	else if (ins == "ATK")
	{
		CM(0x51) DW NEXT DW NEXT SUB FILLER SCLPOS((2 * DWORD_S) + INT_S)
	}
	else if (ins == "ESET")
	{
		CM(0x52) SUB FILLER SCLPOS(INT_S)
	}
	else if (ins == "RET")
	{
		CM(0x53) VAR SCLPOS(CHAR_S)
	}
	else if (ins == "ANM")
	{
		CM(0x54) BY NEXT BY SCLPOS(2 * CHAR_S)
	}
	else if (ins == "FATK")
	{
		CM(0x55) 
			SUB 
			FILLER 
			SCLPOS(INT_S)
	}
	else if (ins == "ATKNP")
	{
		CM(0x56) SUB FILLER SCLPOS(INT_S)
	}
	else if (ins == "MOV")
	{
		CM(0x57) DW NEXT DW SCLPOS(2 * DWORD_S)
	}
	else if (ins == "ACC")
	{
		CM(0x58) DW NEXT DW NEXT DW SCLPOS(3 * DWORD_S)
	}
	else if (ins == "ROL")
	{
		CM(0x59) DW NEXT BY NEXT DW SCLPOS((2 * DWORD_S) + CHAR_S)
	}
	else if (ins == "WAITATOBJ")
	{
		CM(0x5A) SCLPOS(0)
	}
	else if (ins == "PSE")
	{
		CM(0x5B) BY SCLPOS(CHAR_S)
	}
	else if (ins == "KILL")
	{
		CM(0x5C) SCLPOS(0)
	}
	else if (ins == "MDMG")
	{
		CM(0x5D) DW SCLPOS(DWORD_S)
	}
	else if (ins == "CHILD")
	{
		CM(0x5E)  BY NEXT SUB FILLER SCLPOS(CHAR_S + INT_S)
	}
	else if (ins == "CHGTASK")
	{
		CM(0x5F) 
			BY 
			NEXT 
			SUB FILLER SCLPOS(CHAR_S + INT_S)
	}
	else if (ins == "PARENT")
	{
		BY CM(0x60) SCLPOS(CHAR_S)
	}
	else if (ins == "PMOV")
	{
		CM(0x61) DW NEXT DW SCLPOS(2 * DWORD_S)
	}
	else if (ins == "PACC")
	{
		CM(0x62) DW NEXT DW NEXT DW SCLPOS(3 * DWORD_S)
	}
	else if (ins == "PROL")
	{
		CM(0x63) DW NEXT BY NEXT DW SCLPOS((2 * DWORD_S) + CHAR_S)
	}
	else if (ins == "PNOP")
	{
		CM(0x64) DW SCLPOS(DWORD_S)
	}
	else if (ins == "ATK2")
	{ 
		CM(0x65) DW NEXT DW NEXT IN NEXT  SUB FILLER SCLPOS((2 * DWORD_S) + (2 * INT_S))
	}
	else if (ins == "EFC")
	{
		CM(0x66) DW NEXT DW NEXT BY SCLPOS((2 * DWORD_S) + CHAR_S)
	}
	else if (ins == "TAMA")
	{
		CM(0x70) SCLPOS(0)
	} 
	else if (ins == "LASER")
	{
		CM(0x71) SCLPOS(0)
	}
	else if (ins == "DEGE")
	{
		CM(0x72) SCLPOS(0)
	}
	else if (ins == "DEGS")
	{
		CM(0x73) SCLPOS(0)
	}
	else if (ins == "LLCHARGE")
	{
		CM(0x74) SCLPOS(0)
	}
	else if (ins == "LLOPEN")
	{
		CM(0x75) IN SCLPOS(INT_S)
	}
	else if (ins == "LLCLOSE")
	{
		CM(0x76) SCLPOS(0)
	}
	else if (ins == "HLASER")
	{
		CM(0x77) SCLPOS(0)
	}
	else if (ins == "LSPHERE")
	{
		CM(0x78) SCLPOS(0)
	}
	else if (ins == "RLASER")
	{
		CM(0x79) SCLPOS(0)
	}
	else if (ins == "CROSS")
	{
		CM(0x7A) SCLPOS(0)
	}
	else if (ins == "FLOWER")
	{
		CM(0x7B) SCLPOS(0)
	}
	else if (ins == "GFIRE")
	{
		CM(0x7C) SCLPOS(0)
	}
	else if (ins == "IONRING")
	{
		CM(0x7D) SCLPOS(0)
	}
	else if (ins == "TALKMSG")
	{
		CM(0x90) STR ZE SCLPOS()
	}
	else if (ins == "TALKKEY")
	{
		CM(0x91) SCLPOS(0)
	}
	else if (ins == "TALKNEW")
	{
		CM(0x92) SCLPOS(0)
	}
	else if (ins == "TALKWAIT")
	{
		CM(0x93) DW SCLPOS(DWORD_S)
	}
	else if (ins == "TASK")
	{
		CM(0xA0)  SUB FILLER SCLPOS(INT_S)
	}
	else if (ins == "TEXMODE")
	{
		CM(0xA1) BY SCLPOS(CHAR_S)
	}
	else if (ins == "PUSHR")
	{
		VAR CM(0xC0) SCLPOS(CHAR_S)
	}
	else if (ins == "POPR")
	{
		VAR	CM(0xC1) SCLPOS(CHAR_S)
	}
	else if (ins == "MOVC")
	{
		CM(0xC2) VAR NEXT IN SCLPOS(CHAR_S + INT_S)
	}
	else if (ins == "PUSHC")
	{
		CM(0xC4) IN SCLPOS(INT_S)
	}
	else if (ins == "TJMP")
	{
		CM(0xC5)  LAB FILLER SCLPOS(INT_S)
	}
	else if (ins == "FJMP")
	{
		CM(0xC6)  LAB FILLER SCLPOS(INT_S)
	}
	else if (ins == "JMP")
	{
		CM(0xC7)  LAB FILLER SCLPOS(INT_S)
	}
	else if (ins == "OJMP")
	{
		CM(0xCA)  LAB FILLER SCLPOS(INT_S)
	}
	else if (ins == "AJMP")
	{
		CM(0xCB)  LAB FILLER SCLPOS(INT_S)
	}
	else if (ins == "EXIT")
	{
		CM(0xCC) SCLPOS(0)
	}
	else if (ins == "LPOP")
	{
		CM(0xCD) SCLPOS(0)
	}
	else if (ins == "LJMP")
	{
		CM(0xCE)  LAB FILLER SCLPOS(INT_S)
	}
	else if (ins == "ADD")
	{
		CM(0xD0) SCLPOS(0)
	}
	else if (ins == "SUB")
	{
		CM(0xD1) SCLPOS(0)
	}
	else if (ins == "MUL")
	{
		CM(0xD2) SCLPOS(0)
	}
	else if (ins == "DIV")
	{
		CM(0xD3) SCLPOS(0)
	}
	else if (ins == "MOD")
	{
		CM(0xD4) SCLPOS(0)
	}
	else if (ins == "NEG")
	{
		CM(0xD5) SCLPOS(0)
	}
	else if (ins == "SINL")
	{
		CM(0xD6) SCLPOS(0)
	}
	else if (ins == "COSL")
	{
		CM(0xD7) SCLPOS(0)
	}
	else if (ins == "RND")
	{
		CM(0xD8) SCLPOS(0)
	}
	else if (ins == "ATAN")
	{
		CM(0xD9) SCLPOS(0)
	}
	else if (ins == "EQUAL")
	{
		CM(0xDA) SCLPOS(0)
	}
	else if (ins == "NOTEQ")
	{
		CM(0xDB) SCLPOS(0)
	}
	else if (ins == "GREAT")
	{
		CM(0xDC) SCLPOS(0)
	}
	else if (ins == "LESS")
	{
		CM(0xDD) SCLPOS(0)
	}
	else if (ins == "GREATEQ")
	{
		CM(0xDE) SCLPOS(0)
	}
	else if (ins == "LESSEQ")
	{
		CM(0xDF) SCLPOS(0)
	}
	else if (ins == "MAX")
	{
		CM(0xE0) SCLPOS(0)
	}
	else if (ins == "MIN")
	{
		CM(0xE1) SCLPOS(0)
	}
}

void determineSize(char* buffer, int lv, FILE* file, int* sub, int* lab, int* cmb, std::string* sub_names, std::string* lab_names, std::string* cmb_names, bool wrtAdd)
{
	int sclPos = 0x0 + sizeof(SCLHeader), s_pos = 0, l_pos = 0, c_pos = 0, inst = 0;
	std::string instName;
	while (buffer[lv])
	{
		readComent(buffer, lv);
		if (buffer[lv] == '@')
		{
			sub[s_pos] = sclPos;
			s_pos++;
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
			lab[l_pos] = sclPos;
			l_pos++;
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
			cmb[c_pos] = sclPos;
			c_pos++;
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
					readComent(buffer, lv);
					lv++;
				}
				int d = 0;
			}
			else if (checkIns(instName))
			{

#ifdef CLB_DEBUG
				std::cout << instName << "\n";
				if(wrtAdd)
				printf("Ins numero %d desde el segundo acceso.\n", inst);
				if (instName == "EXIT" || instName == "RET")
					printf("\n");
#endif
				readComent(buffer, lv);
				instSize(instName, buffer, lv, file, sclPos, wrtAdd, sub, lab, cmb, sub_names, lab_names, cmb_names);
				while (buffer[lv] != 0x0A)
				{
					readComent(buffer, lv);
					lv++;
				}
				inst++;
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


void ReadFile(char* buffer, long lSize, const char* oName)
{
	SCLHeader Header;
	bool onSub = false;
	int lv = 0, cnt_ins = 0, sub_cnt = 0, lab_cnt = 0, cmb_cnt = 0;
	std::string instName;
	while (lv < lSize)
	{
		readComent(buffer, lv);
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
					printf("Error en pos 0x%x: Carácter incorrecto para nombre de Subrutina.\n", lv);
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
					printf("Error en pos 0x%x: Carácter incorrecto para nombre de etiqueta.\n", lv);
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
					printf("Error en pos 0x%x: Carácter incorrecto para nombre de Subrutina de Combo.\n", lv);
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
						printf("Error en pos 0x%x: Caracter no admitido para variable del SCL Header.\n", lv);
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
							else if (instName == "TexPortrait0")
							{
								Header.LTEntry[0].num_tex++;
							}
							else if (instName == "TexPortrait1")
							{
								Header.LTEntry[1].num_tex++;
							}
							else if (instName == "TexPortrait2")
							{
								Header.LTEntry[2].num_tex++;
							}
							else if (instName == "TexPortrait3")
							{
								Header.LTEntry[3].num_tex++;
							}
							else if (instName == "TexPortrait4")
							{
								Header.LTEntry[4].num_tex++;
							}
							else if (instName == "TexPortrait5")
							{
								Header.LTEntry[5].num_tex++;
							}
							else if (instName == "TexPortrait6")
							{
								Header.LTEntry[6].num_tex++;
							}
							else if (instName == "TexPortrait7")
							{
								Header.LTEntry[7].num_tex++;
							}
							else if (instName == "TexPortrait8")
							{
								Header.LTEntry[8].num_tex++;
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
					printf("Error en posición 0x%x, mnemonico desconocido.\n", lv);
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

	//int* sub_ins_cnt = new int[sub_cnt];
	//int* lab_ins_cnt = new int[lab_cnt];
	//int* cmb_ins_cnt = new int[cmb_cnt];

	createFile(buffer, Header, sub_add, lab_add, cmb_add, sub_names, lab_names, cmb_names, false, oName);
	//createFile(sub_names, lab_names, cmb_names, buffer, sub_cnt, lab_cnt, cmb_cnt, Header);

	lv = 0;

	while (lv < lSize)
	{
		readComent(buffer, lv);
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
				if (buffer[lv] != '}')
				{
						if (instName == "SCLlv1")
						{
							for (int i = 0; i < Header.Lvl1SCL ; i++)
							{
								Header.SCL_lv1[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
								chara = buffer[lv];
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "SCLlv2")
						{
							for (int i = 0; i < Header.Lvl2SCL; i++)
							{
								Header.SCL_lv2[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "SCLlv3")
						{
							for (int i = 0; i < Header.Lvl3SCL; i++)
							{
								Header.SCL_lv3[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "SCLlv4")
						{

							for (int i = 0; i < Header.Lvl4SCL; i++)
							{
								Header.SCL_lv4[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "AttackLv1")
						{

							for (int i = 0; i < MAXCHAR_KOG - 1; i++)
							{
								Header.Lv1Attack[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "AttackLv2")
						{

							for (int i = 0; i < MAXCHAR_KOG - 1; i++)
							{
								Header.Lv2Attack[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "BossAttack")
						{

						for (int i = 0; i < MAXCHAR_KOG - 1; i++)
						{
							Header.BossAttack[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "Lv1Portrait")
						{
	
							for (int i = 0; i < MAXCHAR_KOG - 1; i++)
							{
								Header.AnmLv1[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "Lv2Portrait")
						{

						for (int i = 0; i < MAXCHAR_KOG - 1; i++)
						{
							Header.AnmLv2[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "BossPortrait")
						{

						for (int i = 0; i < MAXCHAR_KOG - 1; i++)
						{
							Header.AnmBoss[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "WinPortrait")
						{

							for (int i = 0; i < MAXCHAR_KOG - 1; i++)
							{
								Header.AnmWin[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "TexPortrait0")
						{
	
							for (int i = 0; i < Header.LTEntry[0].num_tex; i++)
							{
								Header.LTEntry[0].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
								while (buffer[lv] != 0x0a)
								{
									lv++;
								}
								lv++;
							}
							while (buffer[lv] != '}')
							{
								lv++;
							}
							lv++;
						}
						else if (instName == "TexPortrait1")
						{

						for (int i = 0; i < Header.LTEntry[1].num_tex; i++)
						{
							Header.LTEntry[1].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait2")
						{

						for (int i = 0; i < Header.LTEntry[2].num_tex; i++)
						{
							Header.LTEntry[2].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait3")
						{

						for (int i = 0; i < Header.LTEntry[3].num_tex; i++)
						{
							Header.LTEntry[3].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait4")
						{

						for (int i = 0; i < Header.LTEntry[4].num_tex; i++)
						{
							Header.LTEntry[4].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait5")
						{

						for (int i = 0; i < Header.LTEntry[5].num_tex; i++)
						{
							Header.LTEntry[5].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait6")
						{

						for (int i = 0; i < Header.LTEntry[6].num_tex; i++)
						{
							Header.LTEntry[6].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait7")
						{

						for (int i = 0; i < Header.LTEntry[7].num_tex; i++)
						{
							Header.LTEntry[7].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
						else if (instName == "TexPortrait8")
						{

						for (int i = 0; i < Header.LTEntry[8].num_tex; i++)
						{
							Header.LTEntry[8].entryPoint[i] = getAdd(buffer, lv, sub_add, sub_names);
							while (buffer[lv] != 0x0a)
							{
								lv++;
							}
							lv++;
						}
						while (buffer[lv] != '}')
						{
							lv++;
						}
						lv++;
						}
					lv++;
				}
				int d = 0;
			}
			else if (checkIns(instName))
			{
				onSub = !(instName == "EXIT" || instName == "RET");
#ifdef CLB_DEBUG
				if (onSub)
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
	for (int i = 0 ; i < MAXCHAR_KOG - 1; i++)
	{
		Header.ComboAttack[i] = cmb_add[i];
	}

	Header.TexInit = sub_add[0];

	createFile(buffer, Header, sub_add, lab_add, cmb_add, sub_names, lab_names, cmb_names, true, oName);
#ifdef CLB_DEBUG
	printf("Leidas %d instrucciones desde el primer acceso.\n", cnt_ins);
#endif // CLB_DEBUG

}

void createFile(char* buffer, SCLHeader& Header, int* sub, int* lab, int* cmb, std::string* sub_names, std::string* lab_names, std::string* cmb_names, bool ready, const char* oName)
{
	FILE* pFile;
	int lv = 0;
	pFile = fopen(oName, "wb");
	char* SCLFile = (char*)malloc(sizeof(SCLHeader));
	memcpy(SCLFile, &Header, sizeof(Header));
	if (pFile != NULL)
	{
		for (int i = 0; i < sizeof(SCLHeader); i++)
		{
			fputc(SCLFile[i], pFile);
		}
		determineSize(buffer, lv, pFile, sub, lab, cmb, sub_names, lab_names, cmb_names, ready);
	}
	fclose(pFile);
	free(SCLFile);
}

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

#ifdef CLB_DEBUG

int main()
{
	std::string filename;
	std::cout << "Nombre del archivo: "; std::cin >> filename;
	const char* fName = filename.c_str();
	long fileSize;
	char* buffer = openFile(fName, fileSize);

	ReadFile(buffer, fileSize, "output.scl");
	free(buffer);
}
#endif // CLB_DEBUG

#ifndef CLB_DEBUG

int main(int argc, char* argv[])
{
	if (argv[1] == NULL)
	{
		printf("Uso: KOG_c.exe input output\n");
		return 1;
	}
	else
	{
		long fileSize;
		const char* fName = argv[1];
		const char* oName = argv[2];
		char* buffer = openFile(fName, fileSize);

		ReadFile(buffer, fileSize, oName);
		free(buffer);
	}
}
#endif 
