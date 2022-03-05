/* Radu Cosmin - 313 CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __STRUCTURES__
#define __STRUCTURES__

#define TWO					2
#define THREE				3
#define FOUR				4
#define PRINT				5
#define SEARCH				6
#define CLEAR				7
#define ADD					8
#define DEL_TAB				9
#define DEL_COND			10
#define MAX_TYPE_LEN		20
#define MAX_COLUMN_NAME_LEN	30
#define MAX_TABLE_NAME_LEN	30
#define MAX_DB_NAME_LEN		30
#define MAX_CMD_LEN			300

typedef void (*TLAfis) (void*);

// int
typedef struct intCell {
	int    value;
	struct intCell* next;
} t_intCell, *TIC;

typedef struct intLine {
	t_intCell*      cells;
	struct intLine* next;
} t_intLine, *TIL;

// float
typedef struct floatCell {
	float  value;
	struct floatCell* next;
} t_floatCell, *TFC;

typedef struct floatLine {
	t_floatCell*      cells;
	struct floatLine* next;
} t_floatLine, *TFL;

// string
typedef struct stringCell {
	char*  value;
	struct stringCell* next;
} t_stringCell, *TSC;

typedef struct stringLine {
	t_stringCell*       cells;
	struct stringLine* next;
} t_stringLine, *TSL;

// cell type
typedef enum {
	INT,
	FLOAT,
	STRING
} t_cellType;

// column
typedef struct column {
	char name[MAX_COLUMN_NAME_LEN];
	struct column* next;
} t_column, *TC;

// table
typedef struct table {
	char name[MAX_TABLE_NAME_LEN];
	t_cellType    type;
	t_column*     columns;
	void*         lines;  // t_intLine* | t_floatLine* | t_stringLine*
	struct table* next;
} t_table, *TT;

// database
typedef struct db {
	char name[MAX_DB_NAME_LEN];
	t_table* tables;
} t_db, *TDB;

//Initializare baza de date
TDB initializare(char *nume_db);

//Creare nou tabel
TT creare(char* name, char* type, int nr, char* coloane);

//Elimina linie de int-uri
void del_int_line(TIC *cells);

//Elimina linie de float-uri
void del_float_line(TFC *cells);

//Elimina linie de string-uri
void del_string_line(TSC *cells);

//Elimina toate liniile ale unui tabel de tip INT
void del_int_lines(TT *table);

//Elimina toate liniile ale unui tabel de tip FLOAT
void del_float_lines(TT *table);

//Elimina toate liniile ale unui tabel de tip STRING
void del_string_lines(TT *table);

//Elibereaza memoria ocupata de un tabel
void del_table (TT* table);

//Sterge intreaga baza de date
void delete_db (TDB* base);

//Afisarea unei linii de tip INT
void afis_int_line(TIC cells);

//Afisarea unei linii de tip FLOAT
void afis_float_line(TFC cells);

//Afisarea unei linii de tip STRING
void afis_string_line(TSC cells);

//Afisarea intregii baze de date
void print_db (TDB base);

//Afisarea unui tabel
void print_table(TT table);

//Functie ce apeleaza in functie de tip o functie de afisare a unei linii
void afis_linie(t_cellType type, void* lines);

//Adaugarea unei linii de tip INT
void add_int_line(TT table, int nr, int* val);

//Adaugarea unei linii de tip FLOAT
void add_float_line(TT table, int nr, float* val);

//Adaugarea unei linii de tip STRING
void add_string_line(TT table, int nr, char* val);

//Cautarea intr-un tabel de tip INT
void search_int(TT table, char* nume_col, char* relatie, int value);

//Cautarea intr-un tabel de tip FLOAT
void search_float(TT table, char* nume_col, char* relatie, float value);

//Cautarea intr-un tabel de tip STRING
void search_string(TT table, char* nume_col, char* relatie,	char* value);

//Steregerea cu conditie dintr-un tabel de tip INT
void del_int(TT table, char* nume_col, char* relatie, int value);

//Steregerea cu conditie dintr-un tabel de tip FLOAT
void del_float(TT table, char* nume_col, char* relatie,	float value);

//Steregerea cu conditie dintr-un tabel de tip STRING
void del_string(TT table, char* nume_col, char* relatie, char* value);

//Functie de comparare pentru valori de tip int
int compara_int(TIC cel, char* relatie, int value);

//Functie de comparare pentru valori de tip float
int compara_float(TFC cel, char* relatie, float value);

//Functie de comparare pentru valori de tip char*
int compara_string(TSC cel, char* relatie, char* value);

#endif
