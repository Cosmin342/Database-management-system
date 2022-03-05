/* Radu Cosmin - 313 CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"


//Returneaza numarul de cuvinte al unei comenzi folosind repetat strtok
int nr_cuvinte(char* comanda)
{
	int nr = 0;
	//Pointer pentru strtok
	char* p;
	p = strtok(comanda, " ");
	while (p != NULL)
	{
		nr++;
		p = strtok(NULL, " ");
	}
	return nr;

}

/*
Extrage numele tabelului/bazei de date, daca acesta este al doilea
parametru al unei comenzi
*/
char* nume(char* comanda)
{
	char* p;
	p = strtok(comanda, " ");
	p = strtok(NULL, " ");
	return p;
}

//Functie pentru prelucrarea comenzii de creare a unui nou tabel
void prel_tabel(char* comanda, TDB base)
{
	TT table = NULL, aux_t, ant;
	char* p;
	/*
	Datorita anumitor bug-uri ale functiei strtok, am folosit, atat in
	aceasta functie, cat si in urmatoarele, mai multe variabile auxiliare
	*/
	char nume_tab[MAX_TABLE_NAME_LEN], rez[MAX_CMD_LEN], type[MAX_TYPE_LEN],
		col[MAX_CMD_LEN] = {0}, aux[MAX_COLUMN_NAME_LEN];
	strcpy(rez, comanda);
	/*
	Se extrage nr de parametrii pentru a se determina cate coloane va avea
	tabelul
	*/
	int nr = nr_cuvinte(rez), i = 0, eroare_tip = 0;
	p = strtok(comanda, " ");
	//In functie de i se extrag numele tabelului, tipul sau numele coloanelor
	while (p != NULL)
	{
		if (i == 1)
		{
			strcpy(nume_tab, p);
		}
		if (i == TWO)
		{
			strcpy(type, p);
		}
		if (i >= THREE)
		{
			strcpy(aux, p);
			strcat(col, aux);
			strcat(col, ",");
		}
		i++;
		p = strtok(NULL, " ");
	}
	if (strcmp(type, "INT") != 0)
	{
		eroare_tip++;
	}
	if (strcmp(type, "FLOAT") != 0)
	{
		eroare_tip++;
	}
	if (strcmp(type, "STRING") != 0)
	{
		eroare_tip++;
	}
	//Daca eroare_tip e 3, tipul nu este unul cunoscut
	if (eroare_tip == THREE)
	{
		printf("Unknown data type: \"%s\".\n", type);
		return;
	}
	/*
	Daca numarul de cuvinte al comenzii este 3, se creaza un tabel fara coloane.
	Altfel, se creaza un tabel ce are coloanele specificate
	*/
	if (nr == THREE)
	{
		table = creare(nume_tab, type, 0, NULL);
	}
	else
	{
		table = creare(nume_tab, type, 1, col);
	}
	//Se leaga tabelul la baza de date
	if (base->tables == NULL)
	{
		//Daca baza nu are tabele, noul tabel va fi primul din baza
		base->tables = table;
	}
	else
	{
		/*
		Altfel se determina ultimul tabel si se leaga la campul next al
		acestuia
		*/
		for (aux_t = base->tables; aux_t != NULL; aux_t = aux_t->next)
		{
			ant = aux_t;
		}
		ant->next = table;
	}
}

//Functie ce cauta un tabel in baza de date, iar daca il gaseste, il afiseaza
void afis_tab(char* comanda, TDB base)
{
	TT aux = base->tables;
	char nume_tab[MAX_TABLE_NAME_LEN];
	strcpy(nume_tab, nume(comanda));
	while (aux != NULL)
	{
		//Daca tabelul este gasit, se opreste cautarea
		if (strcmp(aux->name, nume_tab) == 0)
		{
			break;
		}
		aux = aux->next;
	}
	//Daca aux e NULL, tabelul nu exista in baza de date
	if (aux == NULL)
	{
		printf("Table \"%s\" not found in database.\n", nume_tab);
	}
	else
	{
		print_table(aux);
	}
}

//Functie de stergere a liniilor unui tabel
void clear(char* comanda, TDB base)
{
	TT aux = base->tables;
	char nume_tab[MAX_TABLE_NAME_LEN];
	strcpy(nume_tab, nume(comanda));
	while (aux != NULL)
	{
		if (strcmp(aux->name, nume_tab) == 0)
		{
			break;
		}
		aux = aux->next;
	}
	if (aux == NULL)
	{
		printf("Table \"%s\" not found in database.\n", nume_tab);
	}
	//In functie de tipul datelor, se apeleaza functiile de stergere
	else
	{
		if (aux->type == INT)
		{
			del_int_lines(&aux);
		}
		if (aux->type == FLOAT)
		{
			del_float_lines(&aux);
		}
		if (aux->type == STRING)
		{
			del_string_lines(&aux);
		}
	}
}

/*
Functie ce prelucreaza comanda de adaugare pentru valori de tip int.
Urmatoarele doua functii merg pe acelasi principiu, singurul lucru
modificat fiind tipul de linie adaugat.
*/
void prel_int(TT aux, char* comanda)
{
	int col[MAX_COLUMN_NAME_LEN] = {0}, i = 0, nr_arg = 0;
	char* p;
	p = strtok(comanda, " ");
	while (p != NULL)
	{
		/*
		Daca se trece de numele comenzii si de cel al tabelului, se extrag
		argumentele si se convertesc folosind atoi.
		*/
		if (i >= TWO)
		{
			col[nr_arg] = atoi(p);
			//Se creste nr de argumente extrase
			nr_arg++;
		}
		i++;
		p = strtok(NULL, " ");
	}
	//Apelare functie de adaugare
	add_int_line(aux, nr_arg, col);
}

/*
La float se convertesc argumentele folosind atof ce intoarce double,
rezultatul fiind castat la float
*/
void prel_float(TT aux, char* comanda)
{
	float col[30] = {0};
	int i = 0, nr_arg = 0;
	char* p;
	p = strtok(comanda, " ");
	while (p != NULL)
	{
		if (i >= TWO)
		{
			col[nr_arg] = (float)atof(p);
			nr_arg++;
		}
		i++;
		p = strtok(NULL, " ");
	}
	add_float_line(aux, nr_arg, col);
}

/*
La string se creeaza un nou sir cu argumentele separate prin virgula
*/
void prel_string(TT aux, char* comanda)
{
	char col[MAX_CMD_LEN] = {0}, *p;
	int i = 0, nr_arg = 0;
	p = strtok(comanda, " ");
	while (p != NULL)
	{
		if (i >= TWO)
		{
			strcat(col, p);
			strcat(col, ",");
		}
		i++;
		p = strtok(NULL, " ");
	}
	add_string_line(aux, nr_arg, col);
}

/*
Functia principala ce cauta in baza de date tabelul pentru a-i determina
tipul si pentru a-l trimite ca paramentru functiilor de adaugare
*/
void add(char* comanda, TDB base)
{
	TT aux = base->tables;
	char nume_tab[MAX_TABLE_NAME_LEN], rez[MAX_CMD_LEN];
	strcpy(rez, comanda);
	strcpy(nume_tab, nume(rez));
	//Localizeaza tabelul in baza de date
	while (aux != NULL)
	{
		if (strcmp(aux->name, nume_tab) == 0)
		{
			break;
		}
		aux = aux->next;
	}
	//Daca aux este NULL, tabelul nu a fost gasit
	if (aux == NULL) 
	{
		printf("Table \"%s\" not found in database.\n", nume_tab);
		return;
	}
	//In functie de tip, se apeleaza una dintre functiile de mai sus
	if (aux->type == INT)
	{
		prel_int(aux, comanda);
	}
	if (aux->type == FLOAT)
	{
		prel_float(aux, comanda);
	}
	if (aux->type == STRING)
	{
		prel_string(aux, comanda);
	}
}

//Functie ce prelucreaza comanda de search
void cauta_tab(char* comanda, TDB base)
{
	TT aux = base->tables;
	int i = 0;
	char rez[MAX_CMD_LEN];
	char *p;
	char aux_tab[MAX_TABLE_NAME_LEN] = {0}, col[MAX_COLUMN_NAME_LEN],
		relatie[TWO], valoare[MAX_COLUMN_NAME_LEN],
		nume_tab[MAX_TABLE_NAME_LEN];
	//Se determina numele tabelului in care se face cautarea
	strcpy(rez, comanda);
	strcpy(aux_tab, nume(rez));
	strcpy(nume_tab, aux_tab);
	p = strtok(comanda, " ");
	//Se extrag numele coloanei, relatia si valoarea dupa care se cauta
	while (p != NULL)
	{
		if (i == TWO)
		{
			strcpy(col, p);
		}
		if (i == THREE)
		{
			strcpy(relatie, p);
		}
		if (i == FOUR)
		{
			strcpy(valoare, p);
		}
		i++;
		p = strtok(NULL, " ");
	}
	while (aux != NULL)
	{
		if (strcmp(aux->name, nume_tab) == 0) break;
		aux = aux->next;
	}
	if (aux == NULL)
	{
		printf("Table \"%s\" not found in database.\n", nume_tab);
		return;
	}
	/*
	In functie de tipul tabelului, se apeleaza functiile de search
	corespunzatoare si eventual se realizeaza conversii
	*/
	if (aux->type == INT)
	{
		int val = atoi(valoare);
		search_int(aux, col, relatie, val);
	}
	if (aux->type == FLOAT)
	{
		float val = (float)atof(valoare);
		search_float(aux, col, relatie, val);
	}
	if (aux->type == STRING)
	{
		search_string(aux, col, relatie, valoare);
	}
}

//Functie pentru stergerea unui tabel si refacerea legaturilor
void delete_tab(char* comanda, TDB base)
{
	char nume_tab[MAX_TABLE_NAME_LEN] = {0}, rez[MAX_CMD_LEN] = {0};
	strcpy(rez, comanda);
	strcpy(nume_tab, nume(rez));
	TT aux = base->tables, ant = NULL;
	//Se localizeaza tabelul in baza de date, dar si anteriorul
	while (aux != NULL)
	{
		if (strcmp(aux->name, nume_tab) == 0)
		{
			break;
		}
		ant = aux;
		aux = aux->next;
	}
	if (aux == NULL)
	{
		printf("Table \"%s\" not found in database.\n", nume_tab);
		return;
	}
	//Daca anteriorul este NULL, restul tabelelor se leaga la base->tables
	if (ant == NULL)
	{
		base->tables = aux->next;
		//Apel functie de eliberare tabel
		del_table(&aux);
	}
	//Altfel, in continuarea lui anterior se leaga tabelele de dupa aux
	else
	{
		ant->next = aux->next;
		del_table(&aux);
	}
}

/*
Functia ce prelucreaza input-ul pentru DELETE cu o conditie.
Modul de implementare este analog cu cel al functiei cauta_tab,
singurul lucru modificat fiind inlocuirea functiilor de search
cu cele de delete
*/
void delete_cond(char* comanda, TDB base)
{
	TT aux = base->tables;
	int i = 0;
	char rez[MAX_CMD_LEN];
	char *p;
	char aux_tab[MAX_TABLE_NAME_LEN] = {0}, col[MAX_COLUMN_NAME_LEN],
		relatie[TWO], valoare[MAX_COLUMN_NAME_LEN],
		nume_tab[MAX_TABLE_NAME_LEN];
	strcpy(rez, comanda);
	strcpy(aux_tab, nume(rez));
	strcpy(nume_tab, aux_tab);
	p = strtok(comanda, " ");
	while (p != NULL)
	{
		if (i == 2)
		{
			strcpy(col, p);
		}
		if (i == 3)
		{
			strcpy(relatie, p);
		}
		if (i == 4)
		{
			strcpy(valoare, p);
		}
		i++;
		p = strtok(NULL, " ");
	}
	while (aux != NULL)
	{
		if (strcmp(aux->name, nume_tab) == 0)
		{
			break;
		}
		aux = aux->next;
	}
	if (aux == NULL) 
	{
		printf("Table \"%s\" not found in database.\n", nume_tab);
		return;
	}
	if (aux->type == INT)
	{
		int val = atoi(valoare);
		del_int(aux, col, relatie, val);
	}
	if (aux->type == FLOAT)
	{
		float val = (float)atof(valoare);
		del_float(aux, col, relatie, val);
	}
	if (aux->type == STRING)
		del_string(aux, col, relatie, valoare);
}

/*
Functie ce returneaza in functie de primul cuvant un numar pentru a se
executa functiile corspunzatoare folosind un switch
*/
int consola(char *functia)
{
	char command1[] = "INIT_DB";
	char command2[] = "DELETE_DB";
	char command3[] = "CREATE";
	char command4[] = "PRINT_DB";
	char command5[] = "PRINT";
	char command6[] = "SEARCH";
	char command7[] = "CLEAR";
	char command8[] = "ADD";
	char command9_10[] = "DELETE";
	char rezerva[MAX_CMD_LEN];
	strcpy(rezerva, functia);
	if (strstr(functia, command1) != NULL) return 1;
	if (strstr(functia, command2) != NULL) return TWO;
	if (strstr(functia, command3) != NULL) return THREE;
	if (strstr(functia, command4) != NULL) return FOUR;
	if (strstr(functia, command5) != NULL) return PRINT;
	if (strstr(functia, command6) != NULL) return SEARCH;
	if (strstr(functia, command7) != NULL) return CLEAR;
	if (strstr(functia, command8) != NULL) return ADD;
	if (strstr(functia, command9_10) != NULL)
	{
		if (nr_cuvinte(rezerva) == 2) return DEL_TAB;
		else return DEL_COND;
	}
	return 0;
}

int main()
{
	int loop = 0;
	char comanda[MAX_CMD_LEN];
	TDB base;
	while (!loop)
	{
		fgets(comanda, MAX_CMD_LEN, stdin);
		//ELiminare \n citit de fgets
		comanda[strlen(comanda) - 1] = '\0';
		//Apelare functii pentru valoarea intoarsa de consola
		switch (consola(comanda))
		{
			case 0:
				printf("Unknown command: \"%s\".\n", comanda);
				break;
			case 1:
				base = initializare(nume(comanda));
				break;
			case TWO:
				delete_db(&base);
				break;
			case THREE:
				prel_tabel(comanda, base);
				break;
			case FOUR:
				print_db(base);
				break;
			case PRINT:
				afis_tab(comanda, base);
				break;
			case SEARCH:
				cauta_tab(comanda, base);
				break;
			case CLEAR:
				clear(comanda, base);
				break;
			case ADD:
				add(comanda, base);
				break;
			case DEL_TAB:
				delete_tab(comanda, base);
				break;
			case DEL_COND:
				delete_cond(comanda, base);
				break;
		}
	}
	return 0;
}