/* Radu Cosmin - 313 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

//Functie pentru stergerea unei linii de tip INT si refacerea legaturilor
void distr_int_line(TIL *aux, TIL *ant, TT *table, TIC *aux_cel)
{
	//Daca nu exista anterior, liniile ramase se leaga la campul lines
	if (*ant == NULL) 
	{
		(*table)->lines = (void*)(*aux)->next;
		//Apel functie de eliberare a unei linii de tip int
		del_int_line(aux_cel);
	}
	//Altfel restul de linii se leaga dupa anterior
	else 
	{
		(*ant)->next = (*aux)->next;
		del_int_line(aux_cel);
	}
	//Aux avanseaza la urmatoarea linie
	(*aux) = (*aux)->next;
}

/*
Functie pentru eliminarea liniilor de tip INT ce indeplinesc o anumita
conditie
*/
void distr_int(char* relatie, int value, TT *table, int nr_col)
{
	int indice_col = 0, gasit = 0;
	TIL aux = (TIL)(*table)->lines, ant = NULL, p;
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
					//Atunci gasit este 1 si se intrerupe bucla
					gasit = 1;
					break;
				}
			}
			indice_col++;
			aux_cel = aux_cel->next;
		}
		aux_cel = aux->cells;
		//Se retine zona unde pointeaza aux pentru a fi ulterior eliberata
		p = aux;
		if (gasit == 1)
		{
			//Apel functie de stergere
			distr_int_line(&aux, &ant, table, &aux_cel);
			free(p);
			continue;
		}
		//Daca gasit este 0 atunci aux si ant avanseaza
		ant = aux;
		aux = aux->next;
	}
}

//Functie pentru stergerea unei linii de tip FLOAT si refacerea legaturilor
void distr_float_line(TFL *aux, TFL *ant, TT *table, TFC *aux_cel)
{
	//Daca nu exista anterior, liniile ramase se leaga la campul lines
	if (*ant == NULL)
	{
		(*table)->lines = (void*)(*aux)->next;
		//Apel functie de eliberare a unei linii de tip float
		del_float_line(aux_cel);
	}
	//Altfel restul de linii se leaga dupa anterior
	else
	{
		(*ant)->next = (*aux)->next;
		del_float_line(aux_cel);
	}
	//Aux avanseaza la urmatoarea linie
	(*aux) = (*aux)->next;
}

/*
Functie pentru eliminarea liniilor de tip FLOAT ce indeplinesc o anumita
conditie
*/
void distr_float(char* relatie, float value, TT *table, int nr_col)
{
	int indice_col = 0, gasit = 0;
	TFL aux = (TFL)(*table)->lines, ant = NULL, p;
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
					//Atunci gasit este 1 si se intrerupe bucla
					gasit = 1;
					break;
				}
			}
			indice_col++;
			aux_cel = aux_cel->next;
		}
		aux_cel = aux->cells;
		//Se retine zona unde pointeaza aux pentru a fi ulterior eliberata
		p = aux;
		if (gasit == 1)
		{
			//Apel functie de stergere
			distr_float_line(&aux, &ant, table, &aux_cel);
			free(p);
			continue;
		}
		//Daca gasit este 0 atunci aux si ant avanseaza
		ant = aux;
		aux = aux->next;
	}
}

//Functie pentru stergerea unei linii de tip STRING si refacerea legaturilor
void distr_string_line(TSL *aux, TSL *ant, TT *table, TSC *aux_cel)
{
	//Daca nu exista anterior, liniile ramase se leaga la campul lines
	if (*ant == NULL)
	{
		(*table)->lines = (void*)(*aux)->next;
		//Apel functie de eliberare a unei linii de tip string
		del_string_line(aux_cel);
	}
	//Altfel restul de linii se leaga dupa anterior
	else
	{
		(*ant)->next = (*aux)->next;
		del_string_line(aux_cel);
	}
	//Aux avanseaza la urmatoarea linie
	(*aux) = (*aux)->next;
}

/*
Functie pentru eliminarea liniilor de tip STRING ce indeplinesc o anumita
conditie
*/
void distr_string(char* relatie, char* value, TT *table, int nr_col)
{
	int indice_col = 0, gasit = 0;
	TSL aux = (TSL)(*table)->lines, ant = NULL, p;
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
					//Atunci gasit este 1 si se intrerupe bucla
					gasit = 1;
					break;
				}
			}
			indice_col++;
			aux_cel = aux_cel->next;
		}
		aux_cel = aux->cells;
		//Se retine zona unde pointeaza aux pentru a fi ulterior eliberata
		p = aux;
		if (gasit == 1)
		{
			//Apel functie de stergere
			distr_string_line(&aux, &ant, table, &aux_cel);
			free(p);
			continue;
		}
		//Daca gasit este 0 atunci aux si ant avanseaza
		ant = aux;
		aux = aux->next;
	}
}

//Functie ce prelucreaza comanda de stergere pentru o valoare de tip int
void del_int(TT table, char* nume_col, char* relatie, int value)
{
	//Variabila ce retine a cata coloana este cea de comparat
	int indice_col = 0;
	TC aux_col = table->columns;
	if (aux_col == NULL) return;
	while (aux_col)
	{
		if (strcmp(aux_col->name, nume_col) == 0)
		{
			break;
		}
		indice_col++;
		aux_col = aux_col->next;
	}
	//Daca nu exista respectiva coloana, se afiseaza un mesaj de eroare
	if (aux_col == NULL)
	{
		printf("Table \"%s\" does not contain column \"%s\".\n", table->name,
			nume_col);
		return;
	}
	//Apel functie de stergere pentru linii de tip INT
	distr_int(relatie, value, &table, indice_col);
}

//In mod analog se comporta si celelalte doua functii
void del_float(TT table, char* nume_col, char* relatie, float value)
{
	int indice_col = 0;
	TC aux_col = table->columns;
	if (aux_col == NULL) return;
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
		printf("Table \"%s\" does not contain column \"%s\".\n", table->name,
			nume_col);
		return;
	}
	distr_float(relatie, value, &table, indice_col);
}

void del_string(TT table, char* nume_col, char* relatie, char* value)
{
	int indice_col = 0;
	TC aux_col = table->columns;
	if (aux_col == NULL) return;
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
		printf("Table \"%s\" does not contain column \"%s\".\n", table->name,
			nume_col);
		return;
	}
	distr_string(relatie, value, &table, indice_col);
}