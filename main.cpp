#include <iostream>
#include <stdio.h>
#include <fstream>

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
char* mapbuffer;
long lSize;
long mapSize;

void determineSize();

void ReadFile()
{
	int lv = 0, cnt_ins = 0, pos, sub_cnt = 0, lab_cnt = 0, strsize = 0; 
	std::string instName;
	while (lv < lSize)
	{
		if (instName == "Sub")
		{
			sub_cnt++;
			while (buffer[lv] != ':')
			{
				lv++;
			}
			instName = "a";
		}
		if (buffer[lv - 1] == '(')
		{
			while (buffer[lv] != ')')
			{
				lv++;
			}
		}
		char chara = buffer[lv];
		if (buffer[lv] >= 'A' && buffer[lv] <= 'z')
		{
			pos = lv;
			while (buffer[lv] >= 'A' && buffer[lv] <= 'z')
			{
				cnt_ins++;
				lv++;
			}
				int j = 0;
				lv = pos;
				char* insname = new char[cnt_ins];
				for (int i = 0; i < cnt_ins; i++)
				{
					insname[i] = buffer[lv];
					insname[i + 1] = 0;
					lv++;
				}
				instName = insname;

				if (instName == "SCLlv1")
				{
					//while()
					int d = 0;
				}

				else if(instName == "Combo_")
				{
				}
				else
				{
					while (instName != ins_name[j] && j < 0xff && instName != "Sub" && instName != "Combo")
					{
						j++;
					}
					if (!(j >= 0xff))
					finalfilesize[j]++;
					//if (!isIns || instName != "Sub" || instName != "Combo")
					//{
					//	printf("Error 2.\n");
					//	exit(1);
					//}
					if (j == 0x40 || j == 0x43 || j == 0x44 || j == 0x90)
					{
						pos = lv;
						while (!(buffer[lv] >= 'A' && buffer[lv] <= 'z'))
						{
							lv++;
						}
						while ((buffer[lv] >= 'A' && buffer[lv] <= 'z') || buffer[lv] == '.' || buffer[lv] == '_' || (buffer[lv] >= '0' && buffer[lv] <= '9'))
						{
							strsize++;
							lv++;
						}

						switch (j)
						{
						case 0x40:
						case 0x43:
							finalfilesize[j] += strsize + 2; break;
						case 0x44:
						case 0x90:
							finalfilesize[j] += strsize + 1; break;
						}
						strsize = 0;
						lv = pos;
					}
					else if (j == 0x42)
					{
						finalfilesize[j];
					}
				}
				if (j >= 0xff)
				{
					while ((buffer[lv] >= 'A' && buffer[lv] <= 'z') || buffer[lv] == '.' || buffer[lv] == '_' || (buffer[lv] >= '0' && buffer[lv] <= '9'))
					{
						lv++;
						chara = buffer[lv];
					}
					if (buffer[lv] == ':')
						lab_cnt++;
					else
					{
						printf("Error 1.\n");
						exit(1);
					}
				}
				printf("Instruccion leida correctamente.\n");
				cnt_ins = 0;
				lv++;
		}
		
		else
		{
			lv++;
		}
	}
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
	finalfilesize[0x41] *= 10;
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
	finalfilesize[0x5b] *= 2;
	finalfilesize[0x5e] *= 5;
	finalfilesize[0x5f] *= 5;
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
	finalfilesize[0xcd] *= 5;
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


void openSCLmap(const char* fileName)
{


	//std::string ins_name[82];

	//This code has been taken from https://cplusplus.com/reference/cstdio/fread/
	FILE* pFile;
	size_t result;

	pFile = fopen(fileName, "rb");
	if (pFile == NULL) { fputs("Couldn't open the sclmap", stderr); exit(1); }

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	mapSize = ftell(pFile);
	rewind(pFile);

	// allocate memory to contain the whole file:
	mapbuffer = (char*)malloc(sizeof(char) * mapSize);
	if (mapbuffer == NULL) { fputs("Memory error", stderr); exit(2); }

	// copy the file into the buffer:
	result = fread(mapbuffer, 1, mapSize, pFile);
	if (result != mapSize) { fputs("Reading error", stderr); exit(3); }
	//Until here

	int i = 0;
	while (mapbuffer[i] != '!')
	{
		i++;
		if (i > mapSize)
		{
			printf("No se encuentra la etiqueta \"!kog_ins\"\n");
			exit(4);
		}
	}
	char chkmap[7];
	chkmap[0] = 'k';
	chkmap[1] = 'o';
	chkmap[2] = 'g';
	chkmap[3] = '_';
	chkmap[4] = 'i';
	chkmap[5] = 'n';
	chkmap[6] = 's';
	i++;
	int j = 0;
	while (j < 7 && chkmap[j] == mapbuffer[i + j])
	{
		j++;
	}
	if (j < 7)
	{
		printf("Etiqueta desconocida.\n");
		exit(5);
	}
	i += j;


#ifdef CLB_DEBUG
	printf("Archivo abierto correctamente.\n");
#endif



	int ins_num = 0;
	int getins_n[4];
	getins_n[3] = 0;
	int k = 0;
	char insname[256];
	int l = 0;
	while (mapbuffer[i] != ';')
	{
		while (mapbuffer[i] >= '0' && mapbuffer[i] <= '9')
		{
			getins_n[k] = mapbuffer[i] - 0x30;
			if (getins_n[3] != 0)
			{
				printf("Numero de instruccion invalido.\n");
				exit(6);
			}
			k++;
			i++;
		}
		if (mapbuffer[i - 1] >= '0' && mapbuffer[i - 1] <= '9')
		{
			if (k == 3)
			{
				ins_num = (getins_n[0] * 100) + (getins_n[1] * 10) + getins_n[2];
			}
			else if (k == 2)
			{
				ins_num = (getins_n[0] * 10) + getins_n[1];
			}
			else if (k == 0)
			{
			}
			else
			{
				printf("Numero de instruccion invalido.\n");
				exit(6);
			}

			while (ins_num >= 0x30 && !(mapbuffer[i] >= 'A' && mapbuffer[i] <= 'z') && mapbuffer[i] != ';')
			{
				i++;
			}
			k = 0;
			int size = 0;
			int init_pos = i;
			while (mapbuffer[i] >= 'A' && mapbuffer[i] <= 'z')
			{
				insname[size] = mapbuffer[i];
				insname[size + 1] = 0;
				l++;
				i++;
				size++;
			}
			l = 0;
			if (ins_num == 0x30 || ins_num == 0x31 || (ins_num >= 0x40 && ins_num <= 0x45) ||
				(ins_num >= 0x50 && ins_num <= 0x66) || (ins_num >= 0x70 && ins_num <= 0x7d) ||
				(ins_num >= 0x90 && ins_num <= 0x93) || ins_num == 0xa0 || ins_num == 0xa1 ||
				(ins_num >= 0xc0 && ins_num <= 0xc7) || (ins_num >= 0xca && ins_num <= 0xce) ||
				(ins_num >= 0xd0 && ins_num <= 0xe1))
			{
				ins_namePos[ins_num] = init_pos;
				ins_nameLength[ins_num] = size;
				ins_name[ins_num] = insname;
			}

		}


		i++;
	}
}


int main()
{
	for (int i = 0; i < 0xff; i++)
	{
		ins_name[i] = "ins_" + i;
	}
	for (int i = 0; i < 0xff; i++)
	{
		finalfilesize[i] = 0;
	}
	int number_1 = 3;
	std::string filename;
	std::string mapname;
	std::cout << "Nombre del archivo: "; std::cin >> filename;
	std::cout << "Nombre del mapa: "; std::cin >> mapname;
	const char* fName = filename.c_str();
	const char* mName = mapname.c_str();
	openSCLmap(mName);
	openFile(fName);
	ReadFile();
	free(buffer);
	free(mapbuffer);
}
