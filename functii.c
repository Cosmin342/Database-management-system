/* Radu Cosmin - 313 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

//Se initilaizeaza baza de date
TDB initializare(char *nume_db)
{
	//Se aloca memorie pt structura bazei de date si se testeaza alocarea
	TDB baza = (TDB)malloc(sizeof(t_db));
	if (baza == NULL)
	{
		return NULL;
	}
	//Atribuirea unui nume bazei de date
	strcpy(baza->name, nume_db);
	//Setarea campului tables pe NULL
	baza->tables = NULL;
	return baza;
}

//Alocare unei celule pentru lista de coloane
TC aloc_string(char* info)
{
	//Alocare celula + testare alocare
	TC cel = (TC)malloc(sizeof(t_column));
	if (cel == NULL)
	{
		return NULL;
	}
	//Atribuirea numelui coloanei
	strcpy(cel->name, info);
	//Setarea campului next pe NULL
	cel->next = NULL;
	return cel;
}

//Crearea unui tabel
TT creare(char* name, char* type, int nr, char* coloane)
{
	TC p = NULL;
	char nume_col[MAX_COLUMN_NAME_LEN], *t;
	//Alocare tabel + testare alocare
	TT table = (TT)calloc(1, sizeof(t_table));
	if (table == NULL)
	{
		return NULL;
	}
	//Asocierea unui nume tabelului
	strcpy(table->name, name);
	//Asocierea tipului
	if (strcmp(type, "FLOAT") == 0)
	{
		table->type = FLOAT;
	}
	if (strcmp(type, "INT") == 0)
	{
		table->type = INT;
	}
	if (strcmp(type, "STRING") == 0)
	{
		table->type = STRING;
	}
	if (nr == 0)
	{
		return table;
	}
	//Determinarea si alocarea coloanelor folosind strtok pe sirul coloane
	t = strtok(coloane, ",");
	while (t != NULL)
	{
		strcpy(nume_col, t);
		TC cel = NULL;
		cel = aloc_string(nume_col);
		/*
		Daca nu exista nicio coloana legata la tabel, atunci se leaga una
		si se asociaza pointerului de parcurgere valoarea campului columns
		*/
		if (table->columns == NULL)
		{
			table->columns = cel;
			p = table->columns;
		}
		//Altfel se leaga la ultima coloana una noua
		else
		{
			p->next = cel;
			p = p->next;
		}
		t = strtok(NULL, ",");
	}
	return table;
}

//Stergerea celulelor unei linii de tip int
void del_int_line(TIC *cells)
{
	//Pointer ce retine adresa celulei curente de eliminat
	TIC aux_cel;
	//Cat timp exista celule se continua eliminarea
	while (*cells)
	{
		/*
		aux_cel ia valoarea celulei de eliminat, iar cells trece la
		urmatoarea celula
		*/
		aux_cel = (*cells);
		(*cells) = (*cells)->next;
		//Eliberare celula curenta
		free(aux_cel);
	}
	//Pentru a evita erorile, (*cells) ia la final valoarea NULL
	(*cells) = NULL;
}

//Analog pentru functiile del_float_line si del_string_line
void del_float_line(TFC *cells)
{
	TFC aux_cel;
	while (*cells)
	{
		aux_cel = (*cells);
		(*cells) = (*cells)->next;
		free(aux_cel);
	}
	(*cells) = NULL;
}

void del_string_line(TSC *cells)
{
	TSC aux_cel;
	while (*cells)
	{
		aux_cel = (*cells);
		(*cells) = (*cells)->next;
		/*
		La o linie de tip string, trebuie eliberat si campul value deoarece
		si acesta este alocat
		*/
		free(aux_cel->value);
		free(aux_cel);
	}
	(*cells) = NULL;
}

//Eliminarea tuturor liniilor unui tabel de tip INT
void del_int_lines(TT *table)
{
	//Pointer pentru parcurgerea liniilor
	TIL aux;
	while ((*table)->lines)
	{
		//aux ia valoarea liniei curente, iar campul lines trece mai departe
		aux = (TIL)(*table)->lines;
		(*table)->lines = (void*)aux->next;
		//Se apeleaza functia de eliminare a unei linii int
		del_int_line(&(aux->cells));
		//Se elibereaza si celula de linie
		free(aux);
	}
	(*table)->lines = NULL;
}

//Analog functioneaza si cele pentru eliminarea la FLOAT si STRING
void del_float_lines(TT *table)
{
	TFL aux;
	while ((*table)->lines)
	{
		aux = (TFL)(*table)->lines;
		(*table)->lines = (void*)aux->next;
		del_float_line(&(aux->cells));
		free(aux);
	}
	(*table)->lines = NULL;
}

void del_string_lines(TT *table)
{
	TSL aux;
	while ((*table)->lines)
	{
		aux = (TSL)(*table)->lines;
		(*table)->lines = (void*)aux->next;
		del_string_line(&(aux->cells));
		free(aux);
	}
	(*table)->lines = NULL;
}

//Functia de stergere a unui tabel
void del_table (TT* table)
{
	//In functie de tip, se apeleaza functiile de eliminare a liniilor
	if ((*table)->type == INT)
	{
		del_int_lines(table);
	}
	if ((*table)->type == FLOAT)
	{
		del_float_lines(table);
	}
	if ((*table)->type == STRING)
	{
		del_string_lines(table);
	}
	//Pointer pentru parcurgerea coloanelor
	TC aux;
	while ((*table)->columns != NULL)
	{
		//aux va pointa la coloana curenta, iar campul columns trece mai departe
		aux = (*table)->columns;
		(*table)->columns = (*table)->columns->next;
		//Eliberarea coloanei curente
		free(aux);
	}
	//La final se elibereaza si tabelul
	free(*table);
}

//Functia de eliberare a bazei de date
void delete_db (TDB* base)
{
	//Se parcurge lista de tabele si se elibereaza memoria fiecarui tabel
	TT aux;
	while((*base)->tables != NULL)
	{
		aux = (*base)->tables;
		(*base)->tables = aux->next;
		del_table(&aux);
	}
	//Se elibereaza  si memoria ocupata de baza de date
	free(*base);
	//Fiind ultima functie, se opreste si executia programului
	exit(0);
}

//Functia de afisare a unei linii ce contine valori de tip int
void afis_int_line(TIC cells)
{
	int padding, i;
	TIC aux_cel = cells;
	while (aux_cel)
	{
		//Variabila pentru conversie
		char nr_conv[MAX_COLUMN_NAME_LEN];
		printf("%d", aux_cel->value);
		/*
		Se converteste un numar de tip int/float la char* folosind sprintf
		pentru a determina padding-ul
		*/
		sprintf(nr_conv, "%d", aux_cel->value);
		//Padding-ul reprezinta nr de spatii pentru alinierea coloanelor
		padding = MAX_COLUMN_NAME_LEN - strlen(nr_conv);
		for (i = 0; i < padding; i++)
		{
			printf(" ");
		}
		//Spatiu de separare a coloanelor
		printf(" ");
		aux_cel = aux_cel->next;
	}
	//Se trece la linia urmatoare
	printf("\n");
}

//Analog pentru functiile afis_float_line si afis_string_line
void afis_float_line(TFC cells)
{
	int padding, i;
	TFC aux_cel = cells;
	while (aux_cel)
	{
		char nr_conv[MAX_COLUMN_NAME_LEN];
		printf("%.6f", aux_cel->value);
		sprintf(nr_conv, "%.6f", aux_cel->value);
		padding = MAX_COLUMN_NAME_LEN - strlen(nr_conv);
		for (i = 0; i < padding; i++)
		{
			printf(" ");
		}
		printf(" ");
		aux_cel = aux_cel->next;
	}
	printf("\n");
}

void afis_string_line(TSC cells)
{
	int i, padding;
	TSC aux_cel = cells;
	while (aux_cel)
	{
		printf("%s", aux_cel->value);
		padding = MAX_COLUMN_NAME_LEN - strlen(aux_cel->value);
		for (i = 0; i < padding; i++)
		{
			printf(" ");
		}
		printf(" ");
		aux_cel = aux_cel->next;
	}
	printf("\n");
}

//Functie ce apeleaza functiile de afisare a liniilor in functie de tip
void afis_linie(t_cellType type, void* lines)
{
	if (type == INT)
	{
		TIL aux = (TIL)lines;
		while (aux)
		{
			afis_int_line(aux->cells);
			aux = aux->next;
		}
	}
	if (type == FLOAT)
	{
		TFL aux = (TFL)lines;
		while (aux)
		{
			afis_float_line(aux->cells);
			aux = aux->next;
		}
	}
	if (type == STRING)
	{
		TSL aux = (TSL)lines;
		while (aux)
		{
			afis_string_line(aux->cells);
			aux = aux->next;
		}
	}
}

//Functie de afisare a tabelului conform cerintei
void print_table(TT table)
{
	printf("TABLE: %s\n", table->name);
	int padding, i, j, nr_col = 0;
	TC aux = table->columns;
	while (aux)
	{
		printf("%s", aux->name);
		//Si pentru coloane se determina un padding
		padding = MAX_COLUMN_NAME_LEN - strlen(aux->name);
		for (i = 0; i < padding; i++)
		{
			printf(" ");
		}
		printf(" ");
		/*
		Numar folosit pentru a stii cat de multe linii deimitatoare vor
		fi afisate
		*/
		nr_col++;
		aux = aux->next;
	}
	printf("\n");
	for (i = 0; i < nr_col; i++)
	{
		for (j = 0; j < MAX_COLUMN_NAME_LEN; j++)
		{
			printf("-");
		}
		printf(" ");
	}
	printf("\n");
	if (table->type == INT)
	{
		afis_linie(INT, table->lines);
	}
	if (table->type == FLOAT)
	{
		afis_linie(FLOAT, table->lines);
	}
	if (table->type == STRING)
	{
		afis_linie(STRING, table->lines);
	}
	//Dupa fiecare tabel se lasa un rand liber
	printf("\n");
}

//Functia de afisare a intregii baze de date
void print_db (TDB base)
{
	printf("DATABASE: %s\n\n", base->name);
	//Se parcurg tabelele acesteia si se afiseaza folosind functia anterioara
	TT aux = base->tables;
	while(aux != NULL)
	{
		print_table(aux);
		aux = aux->next;
	}
}

//Functie de adaugare a unei linii cu elemente de tip int
void add_int_line(TT table, int nr, int* val)
{
	int i, elem;
	TIC act;
	//Alocarea unei noi linii + testare
	TIL line = (TIL)calloc(1, sizeof(t_intLine)), ant, p;
	if (line == NULL)
	{
		return;
	}
	//Se parcurge vectorul de valori si se creeaza cate o celula pentru fiecare
	for (i = 0; i < nr; i++)
	{
		TIC cel;
		elem = val[i];
		cel = (TIC)calloc(1, sizeof(t_intCell));
		/*
		Daca alocarea uneia esueaza, se elibereaza mai intai ce a fost alocat
		inainte
		*/
		if (cel == NULL)
		{
			del_int_line(&line->cells);
			free(line);
			return;
		}
		cel->value = elem;
		//Celule cu valori se leaga la linie precum coloanele
		if (line->cells == NULL)
		{
			line->cells = cel;
			act = line->cells;
		}
		else
		{
			act->next = cel;
			act = act->next;
		}
	}
	act = NULL;
	//Daca tabelul nu are linii, noua linie se leaga la campul lines
	if (table->lines == NULL)
	{
		table->lines = (void*)line;
	}
	//Altfel se determina ultima linie iar cea noua se leaga dupa aceasta
	else
	{
		for (p = (TIL)table->lines; p != NULL; p = p->next)
		{
			ant = p;
		}
		ant->next = line;
	}
}

//Analog functioneaza si add_float_line si add_string_line
void add_float_line(TT table, int nr, float* val)
{
	int i;
	float elem;
	TFC act;
	TFL line = (TFL)calloc(1, sizeof(t_floatLine)), ant, p;
	if (line == NULL)
	{
		return;
	}
	for (i = 0; i < nr; i++)
	{
		TFC cel;
		elem = val[i];
		cel = (TFC)calloc(1, sizeof(t_floatCell));
		if (cel == NULL)
		{
			del_float_line(&line->cells);
			free(line);
			return;
		}
		cel->value = elem;
		if (line->cells == NULL)
		{
			line->cells = cel;
			act = line->cells;
		}
		else
		{
			act->next = cel;
			act = act->next;
		}
	}
	act = NULL;
	if (table->lines == NULL)
	{
		table->lines = (void*)line;
	}
	else
	{
		for (p = (TFL)table->lines; p != NULL; p = p->next)
		{
			ant = p;
		}
		ant->next = line;
	}
}

void add_string_line(TT table, int nr, char* val)
{
	int i;
	char elem[MAX_COLUMN_NAME_LEN][MAX_COLUMN_NAME_LEN], *t;
	TSC act;
	TSL line = (TSL)calloc(1, sizeof(t_stringLine)), ant, p;
	if (line == NULL) return;
	nr = 0;
	t = strtok(val, ",");
	//Pentru string se face strtok pentru a se crea cun vector de string-uri
	while(t != NULL)
	{
		strcpy(elem[nr], t);
		nr++;
		t = strtok(NULL, ",");
	}
	for (i = 0; i < nr; i++)
	{
		TSC cel;		
		cel = (TSC)calloc(1, sizeof(t_stringCell));
		if (cel == NULL)
		{
			del_string_line(&line->cells);
			free(line);
			return;
		}
		//Copiere si alocare directa a numelui
		cel->value = strdup(elem[i]);
		if (line->cells == NULL)
		{
			line->cells = cel;
			act = line->cells;
		}
		else
		{
			act->next = cel;
			act = act->next;
		}
	}
	act = NULL;
	if (table->lines == NULL)
	{
		table->lines = (void*)line;
	}
	else
	{
		for (p = (TSL)table->lines; p != NULL; p = p->next)
		{
			ant = p;
		}
		ant->next = line;
	}
}

/*
Functie ce compara valoarea unei celule cu valoarea data ca parametru in
functie de sirul de caractere relatie. Intoarce 1 daca este adevarata relatie
si 0 in caz negativ
*/
int compara_int(TIC cel, char* relatie, int value)
{
	if (strcmp(relatie, "==") == 0)
	{
		if (cel->value == value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, ">=") == 0)
	{
		if (cel->value >= value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "<=") == 0)
	{
		if (cel->value <= value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, ">") == 0)
	{
		if (cel->value > value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "<") == 0)
	{
		if (cel->value < value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "!=") == 0)
	{
		if (cel->value != value)
		{
			return 1;
		}
	}
	return 0;
}

//Acelasi mod de functionare il au si urmatoarele doua functii
int compara_float(TFC cel, char* relatie, float value)
{
	if (strcmp(relatie, "==") == 0)
	{
		if (cel->value == value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, ">=") == 0)
	{
		if (cel->value >= value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "<=") == 0)
	{
		if (cel->value <= value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, ">") == 0)
	{
		if (cel->value > value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "<") == 0)
	{
		if (cel->value < value)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "!=") == 0)
	{
		if (cel->value != value)
		{
			return 1;
		}
	}
	return 0;
}

int compara_string(TSC cel, char* relatie, char* value)
{
	if (strcmp(relatie, "==") == 0)
	{
		if (strcmp(cel->value, value) == 0)
		{
			return 1;
		}
	}
	if (strcmp(relatie, ">=") == 0)
	{
		if (strcmp(cel->value, value) >= 0)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "<=") == 0)
	{
		if (strcmp(cel->value, value) <= 0)
		{
			return 1;
		}
	}
	if (strcmp(relatie, ">") == 0)
	{
		if (strcmp(cel->value, value) > 0)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "<") == 0)
	{
		if (strcmp(cel->value, value) < 0)
		{
			return 1;
		}
	}
	if (strcmp(relatie, "!=") == 0)
	{
		if (strcmp(cel->value, value) != 0)
		{
			return 1;
		}
	}
	return 0;
}