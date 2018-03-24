#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Costanti -----------------------------------------------------------------*/

#define MAX_STRING_LENGTH 40

/* Tipi di dati -------------------------------------------------------------*/

typedef char String[MAX_STRING_LENGTH+1]; //"String" rappresenta una stringa di lunghezza prefissata

typedef enum {
 	IDENTIFIER,  		//identificatore
 	KEYWORD,  			//parola chiave
 	COMMENT,  			//commento (ignorati)
 	LEFT_PAR,  			//parentesi sinistra '('
 	RIGHT_PAR, 			//parentesi sinistra ')'
 	PLUS,  				//più '+'
 	MINUS,  			//meno '-'
 	ASTERIX,  			//asterisco '*'
 	SLASH,  			//slash '/'
 	EQUAL,  			//uguale
 	SEMICOLON,  		//punto e virgola ';'
 	DOT,  				//punto '.'
 	COMMA,  			//virgola ','
 	GREATER,  			//maggiore stretto '>'
 	GREATER_OR_EQUAL,  	//maggiore o uguale '>='
 	LESS,  				//minore '<'
 	LESS_OR_EQUAL,  	//minore o uguale '<='
 	DIFFERENT,  		//diverso '<>'
 	COLON,  			//
 	ASSIGNMENT,  		//assegnazione ':='
 	INTEGER,  			//numero intero
 	REAL,  				//numero reale
 	ESADECIMAL,			//numero esadecimale
 	REAL_ESP,			//numero reale in notazione esponenziale
 	STRING,  			//stringa
 	CHARACTER,			//carattere
 	ERROR  				//indica eventuali errori lessicali
} TokenType; //questa struttura elenca le varie classi che possono assumere i token

typedef struct
{
 	TokenType type; //definisce le "classi", cioè il tipo di token
 	String value;  //array che contiene il "valore" del token
} Token;

/* Variabili globali --------------------------------------------------------*/

char *keywords[]={"begin","boolean","char", "do", "else","end",  //indica le varie parole chiave del linguaggio
                    "false","for","if","integer","program","real", "repeat",
                    "then", "to", "true","until","var","while",NULL};
					
//-------PROTOTIPI---------
int is_keyword(String s);
int getToken (FILE* src, Token* tkn);                 
