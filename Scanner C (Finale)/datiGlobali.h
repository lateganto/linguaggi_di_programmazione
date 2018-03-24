#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//--------Costanti---------
#define LUNG_MAX 40

//----------Tipi di dati---------
typedef char Stringa[LUNG_MAX+1];

typedef enum {
	IDENTIFICATORE,
	PAROLA_CHIAVE,
	PAR_D,
	PAR_S,
	GRAF_S,
	GRAF_D,
	PIU,
	MENO,
	ASTERISCO,
	UGUALE,
	PUNT_VIRG,
	PUNTO,
	VIRGOLA,
	MAGG,
	MAGG_UGUAL,
	MINOR,
	MINOR_UGUAL,
	DIVERSO,
	ASSEGNAZIONE,
	INTERO,
	REALE,
	REALE_ESP,
	ESADECIMALE,
	STRINGA,
	CARATTERE,
	ERROR
} TipoToken;
	
typedef struct {
	TipoToken tipo;
	Stringa valore;
} Token;

//----------Variabili Globali-----------
char *keywords[]={"int", "char", "float", "double", "while", "do"
					"if", "for", "else", "return", "void", "break", NULL};

//-------PROTOTIPI---------
int is_keyword(Stringa s);
int getToken (FILE* src, Token* tkn); 
