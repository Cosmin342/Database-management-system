/* Radu Cosmin - 313 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

/*
Functie pentru afisarea liniilor de tip INT ce indeplinesc o anumita conditie
*/
void print_int(char* relatie, int value, TT table, int nr_col)
{
	int indice_col = 0, gasit = 0;
	TIL aux = (TIL)table->lines;
	while (aux)
	{
		//Resetare variabile auxiliare
		indice_col = 0;
		gasit = 0;
		TIC aux_cel = aux->cells;
		while (aux_cel)
		{
			/*
			Daca se ajunge la coloana respectiva, se verifica daca este
			adevarata conditia
			*/
			if (indice_col == nr_col)
			{
				if (compara_int(aux_cel, relatie, value) == 1)
				{
					gasit = 1;
					break;
				}
			}
			indice_col++;
			aux_cel = aux_cel->next;
		}
		aux_cel = aux->cells;
		if (gasit == 1)
		{
			//Apelare functie de afisare a liniei
			afis_int_line(aux_cel);
		}
		aux = aux->next;
	}
	//Final de tabel
	printf("\n");
}

/*
Functie pentru afisarea liniilor de tip FLOAT ce indeplinesc o anumita conditie
*/
void print_float(char* relatie, float value, TT table, int nr_col)
{
	int indice_col = 0, gasit = 0;
	TFL aux = (TFL)table->lines;
	while (aux)
	{
		indice_col = 0;
		gasit = 0;
		TFC aux_cel = aux->cells;
		while (aux_cel)
		{
			/*
			Daca se ajunge la coloana respectiva, se verifica daca este
			adevarata conditia
			*/
			if (indice_col == nr_col)
			{
				if (compara_float(aux_cel, relatie, value) == 1)
				{
					gasit = 1;
					break;
				}
			}
			indice_col++;
			aux_cel = aux_cel->next;
		}
		aux_cel = aux->cells;
		if (gasit == 1)
		{
			//Apelare functie de afisare a liniei
			afis_float_line(aux_cel);
		}
		aux = aux->next;
	}
	//Final de tabel
	printf("\n");
}

/*
Functie pentru afisarea liniilor de tip STRING ce indeplinesc o anumita conditie
*/
void print_string(char* relatie, char* value, TT table, int nr_col)
{
	int indice_col = 0, gasit = 0;
	TSL aux = (TSL)table->lines;
	while (aux)
	{
		indice_col = 0;
		gasit = 0;
		TSC aux_cel = aux->cells;
		while (aux_cel)
		{
			/*
			Daca se ajunge la coloana respectiva, se verifica daca este
			adevarata conditia
			*/
			if (indice_col == nr_col)
			{
				if (compara_string(aux_cel, relatie, value) == 1)
				{
					gasit = 1;
					break;
				}
			}
			indice_col++;
			aux_cel = aux_cel->next;
		}
		aux_cel = aux->cells;
		if (gasit == 1)
		{
			//Apelare functie de afisare a liniei
			afis_string_line(aux_cel);
		}
		aux = aux->next;
	}
	//Final de tabel
	printf("\n");
}

/*
Functie de prelucrare a comenzii SEARCH pentru INT si afisare a
antetului tabelului
*/
void search_int(TT table, char* nume_col, char* relatie, int value)
{
	int indice_col = 0, padding, i, j, nr_col = 0;
	TC aux_col = table->columns;
	//Parcurgere coloane pentru gasirea celei ale carei valori se vor compara
	while (aux_col)
	{
		if (strcmp(aux_col->name, nume_col) == 0)
		{
			break;
		}
		//De asemenea se determina si inidcele ei
		indice_col++;
		aux_col = aux_col->next;
	}
	//Daca nu exista in tabel, atunci se intoarce mesajul de eroare
	if (aux_col == NULL)
	{
		printf("Table \"%s\" does not contain column \"%s\".\n",
			table->name, nume_col);
		return;
	}
	//Se printeaza antetul tabelului ca in functia de printare a tabelului
	printf("TABLE: %s\n", table->name);
	TC aux2 = table->columns;
	while (aux2)
	{
		printf("%s", aux2->name);
		padding = MAX_COLUMN_NAME_LEN - strlen(aux2->name);
		for (i = 0; i < padding; i++)
			printf(" ");
		printf(" ");
		nr_col++;
		aux2 = aux2->next;
	}
	printf("\n");
	for (i = 0; i < nr_col; i++)
	{
		for (j = 0; j < MAX_COLUMN_NAME_LEN; j++)
			printf("-");
		printf(" ");
	}
	printf("\n");
	//Apel functie de cautare
	print_int(relatie, value, table, indice_col);
}

/*
Analog functioneaza si urmatoarele doua functii
*/
void search_float(TT table, char* nume_col, char* relatie, float value)
{
	int indice_col = 0, padding, i, j, nr_col = 0;
	TC aux_col = table->columns;
	while (aux_col)
	{
		if (strcmp(aux_col->name, nume_col) == 0)
		{
			break;
		}
		indice_col++;
		aux_col = aux_col->next;
	}
	if (aux_col == NULL)
	{
		printf("Table \"%s\" does not contain column \"%s\".\n",
			table->name, nume_col);
		return;
	}
	printf("TABLE: %s\n", table->name);
	TC aux2 = table->columns;
	while (aux2)
	{
		printf("%s", aux2->name);
		padding = MAX_COLUMN_NAME_LEN - strlen(aux2->name);
		for (i = 0; i < padding; i++)
			printf(" ");
		printf(" ");
		nr_col++;
		aux2 = aux2->next;
	}
	printf("\n");
	for (i = 0; i < nr_col; i++)
	{
		for (j = 0; j < MAX_COLUMN_NAME_LEN; j++)
			printf("-");
		printf(" ");
	}
	printf("\n");
	print_float(relatie, value, table, indice_col);
}

void search_string(TT table, char* nume_col, char* relatie, char* value)
{
	int indice_col = 0, padding, i, j, nr_col = 0;
	TC aux_col = table->columns;
	while (aux_col)
	{
		if (strcmp(aux_col->name, nume_col) == 0)
		{
			break;
		}
		indice_col++;
		aux_col = aux_col->next;
	}
	if (aux_col == NULL)
	{
		printf("Table \"%s\" does not contain column \"%s\".\n",
			table->name, nume_col);
		return;
	}
	printf("TABLE: %s\n", table->name);
	TC aux2 = table->columns;
	while (aux2)
	{
		printf("%s", aux2->name);
		padding = MAX_COLUMN_NAME_LEN - strlen(aux2->name);
		for (i = 0; i < padding; i++)
			printf(" ");
		printf(" ");
		nr_col++;
		aux2 = aux2->next;
	}
	printf("\n");
	for (i = 0; i < nr_col; i++)
	{
		for (j = 0; j < MAX_COLUMN_NAME_LEN; j++)
			printf("-");
		printf(" ");
	}
	printf("\n");
	print_string(relatie, value, table, indice_col);
}