// BigLittleEndian.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

void Print(int l)
{
	unsigned char* p_c = (unsigned char*)(unsigned int*)&l;

	printf("|%10d  | ", l);

	for (int i = 0; i < sizeof(int); i++)
	{
		for (int i1 = 7; i1 >= 0; i1--)
		{
			printf("%u", (*p_c >> i1) & 1);
		}

		printf("  ");

		p_c++;
	}

	printf("|  ");

	for (int i = 0; i < sizeof(int); i++)
	{
		p_c--;

		for (int i1 = 7; i1 >= 0; i1--)
		{
			printf("%u", (*p_c >> i1) & 1);
		}

		printf("  ");
	}

	printf("|\n");
}

int main()
{
	printf("+------------+-----------------------------------------+------------------------------------------+\n");
	printf("|    Int     |  Little-endian storage type int memory  |    Big-endian storage type int memory    |\n");	
	printf("|     =      |  Adr + 0 | Adr + 1 | Adr + 2 | Adr + 3  |   Adr + 0 | Adr + 1 | Adr + 2 | Adr + 3  |\n");
	printf("+------------+-----------------------------------------+------------------------------------------+\n");

	for (int l = 250; l < 260; l++)
	{
		Print(l);
	}

	printf("+------------+-----------------------------------------+------------------------------------------+\n");

	getchar();

    return 0;
}