#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "datiGlobali.h"

/* Funzioni -----------------------------------------------------------------*/
                    
int is_keyword(String s) //funzione che verifica se un identificatore è in realtà una parola chiave
{
	char* keyw = keywords[0];
    int i = 0;
    
    while (keywords[i]) 
		if (strcmpi(keywords[i++],s)==0) //se keywords[i] == s allora restituisci 1
			return !0;
    
    return 0;       
}

int getToken (FILE* src, Token* tkn)
{
	//"c" e "lookahead" vengono dichiarate con 'static' per evitare che vengano distrutte al termine della chiamata di funzione
 	static char c;  //contiene il carattere letto da file
 	static int lookahead = 0;  //indica se il carattere contenuto in "c" è stato consumato (vero) o deve essere ripreso in considerazione per il riconoscimento (falso)
 
	enum {
      	S,              /* Stato Iniziale */
      	ID_KW,          /* Identificatore o Keyword */
      	BR_COM,         /* Parentesi o commento */
      	START_COM,      /* All'interno di un commento */
      	AST_IN_COM,     /* Asterisco in commento */ 
      	G_GE,           /* Maggiore o maggiore-uguale */ 
      	L_LE_D,         /* Minore, minore-uguale, o diverso */ 
      	COL_ASS,        /* Due punti o assegnamento */
      	NUM,            /* Numero intero o reale */
      	NUM_R,          /* Numero reale */
      	NUM_EX,			/* Numero esadecimale */
      	NUM_R_ESP,		/* Costante reale in notazione esponenziale */ 
      	ESP,			/* Esponente per numero in notazione esponenziale */
      	IN_STR,         /* Stringa */
      	APEX,           /* Apice in stringa */
      	APEX2,			/* Apice in carattere */
      	CHAR, 			/* Carattere */
      	IN_CHAR,		/* Verifica carattere */
      	COMM,			/* Commento con { } */
 	} current_state = S;  //lo stato parte da "S"
 
 	int i = 0;  //contatore per inserire i caratteri in "token->value"
 	int eot = 0;    /* "end of token" (se assume "vero" indica che il token è terminato) */

 	tkn->type = ERROR;
 
	while (!eot && i<MAX_STRING_LENGTH) { 
  		//questa parte permette di realizzare il meccanismo del lookahead
		if (!lookahead)  //se "lookahead" assume valore "falso (0)" si deve leggere il carattere dal file
     		c = fgetc(src);
  		else  //altrimenti è già stato letto nella scansione del token precedente quindi è già presente in "c"
     		lookahead = 0;  //lookahead viene azzerato
  
  		if (c==EOF) //se è raggiunta la fine del file "esci dal ciclo"
     		break;
  		else 
		  switch (current_state) {
   			case S:
        		if (isalpha(c)||c=='_') { //se "c" è una lettera
           			current_state = ID_KW;
           			tkn->value[i++] = c;
           			tkn->type = IDENTIFIER;
        		}
        		else if (isspace(c)) //se "c" è '\n' o ' '
             		current_state = S;
        		else if (isdigit(c)) { //se "c" è un numero
             		current_state = NUM;
             		tkn->value[i++] = c;
             		tkn->type = INTEGER;             
        		}
        		else switch (c) {
         				case '+':
              				tkn->type = PLUS;
              				tkn->value[i++] = c;
              				eot = !0;  //fine token
              				break;
        				case '-':
             				tkn->type = MINUS;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case '*':
              				tkn->type = ASTERIX;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case '/':
              				tkn->type = SLASH;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case '=':
              				tkn->type = EQUAL;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case ';':
              				tkn->type = SEMICOLON;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case '.':
              				tkn->type = DOT;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case ',':
              				tkn->type = COMMA;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case '(':
              				current_state = BR_COM;
              				tkn->value[i++]=c;
              				break;             
         				case ')':
              				tkn->type = RIGHT_PAR;
              				tkn->value[i++] = c;
              				eot = !0;
              				break;
         				case '>':
              				current_state = G_GE;
              				tkn->value[i++] = c;
              				break;
         				case '<':
              				current_state = L_LE_D;
              				tkn->value[i++] = c;
              				break;
         				case ':':
              				current_state = COL_ASS;
              				tkn->value[i++] = c;
              				break;
         				case '\'':
              				current_state = IN_CHAR;  //modifica per riconoscere caratteri
              				break;
              			case '{':  //aggiunta per riconoscere commenti delimitati da '{'
              				current_state = COMM;
              				break;
              			case '$':  //aggiunta per riconoscere numeri esadecimali
              				current_state = NUM_EX;
              				break;
         				default: 
                  			eot = !0;
                  			tkn->value[i++] = c;
        		}
        		break;
   			case ID_KW:
        		if (isalnum(c) || c=='_') { //se "c" è una lettera, una cifra o '_'
           			current_state = ID_KW;
           			tkn->value[i++] = c;
        		}
        		else {
            		lookahead = !0;  //"lookahead" assume il valore "vero"
            		eot = !0;  //"eot" assume il valore "vero" 
        		}
        		break;
   			case BR_COM: //parentesi o commento
        		if (c == '*') { //si tratta dell'inizio di un commento
           			current_state = START_COM;
           			i--;  //perchè i commenti devono essere ignorati
        		}
        		else {  //si tratta di una parentesi sinistra
            		lookahead = !0;
            		eot = !0;
            		tkn->type = LEFT_PAR;
        		}
        		break;
        	case COMM:  //aggiunta commento tipo '{ }'
        		if (c == '}')
        			current_state = S;
        		break;
   			case START_COM:
        		if (c == '*') { //asterisco finale di un commento
           			current_state = AST_IN_COM;
        		}
        		break;
   			case AST_IN_COM:
        		if (c == ')') { //commento terminato
           			current_state = S;  //torna allo stato iniziale
        		}
        		else
            		current_state = START_COM; //legge un altro simbolo (interno commento) finchè non arriva all'asterisco finale
        		break;
   			case G_GE: 
        		if (c == '=') {
           			tkn->type = GREATER_OR_EQUAL;
           			tkn->value[i++] = c;
           			eot = !0; //hai raggiunto la fine del token ">="
        		}
        		else {
            		tkn->type = GREATER;
            		eot = lookahead = !0;
        		}
        		break;
   			case L_LE_D:
        		if (c == '=') {
           			tkn->type = LESS_OR_EQUAL;
           			tkn->value[i++] = c;
           			eot = !0;
        		}
        		else if (c == '>') {
           			tkn->type = DIFFERENT;
           			tkn->value[i++] = c;
           			eot = !0;         
        		}
        		else {
            		tkn->type = LESS;
            		eot = lookahead = !0;
        		}
        		break;
   			case COL_ASS:
        		if (c == '=') {
           			tkn->type = ASSIGNMENT;
           			tkn->value[i++] = c;
           			eot = !0;
        		}
        		else {
            		tkn->type = COLON;
            		eot = lookahead = !0;
        		}
        		break;
   			case NUM:
        		if (isdigit(c))
           			tkn->value[i++] = c;
        		else if (c == '.') {
             		current_state = NUM_R;
             		tkn->value[i++] = c;
        		}
        		else {
            		tkn->type = INTEGER;
            		eot = lookahead = !0;
        		}
        		break;
        	case NUM_EX: //aggiunta "numero esadecimale"
        		if (isxdigit(c))
        			tkn->value[i++] = c;
        		else {
        			tkn->type = ESADECIMAL;
        			eot = lookahead = !0;
        		}
        		break;
   			case NUM_R:
        		if (isdigit(c))
           			tkn->value[i++] = c;
           		else if (c == 'e' || c == 'E') {
           			current_state = NUM_R_ESP;
           			tkn->value[i++] = c;
           		}	
        		else {
            		tkn->type = REAL;
            		eot = lookahead = !0;  
        		}
        		break;      		
        	case NUM_R_ESP: //aggiunta "numero reale in notazione esponenziale"
        		if (c == '+' || c == '-') {
        			current_state = ESP;
        			tkn->value[i++] = c;						
        		}        			
				else if (isdigit(c))
        			tkn->value[i++] = c;
        		else {
					tkn->type = REAL_ESP;
					eot = lookahead = !0;
        		}
        		break;
			case ESP: //aggiunta esponente
				if (isdigit(c))
					tkn->value[i++] = c;
				else {
					tkn->type = REAL_ESP;
					eot = lookahead = !0;
				}
				break;		       		
   			case IN_STR:
        		if (c == '\'') 
           			current_state = APEX;
        		else {
            		tkn->value[i++] = c;
        		}
        		break;
   			case APEX:
        		if (c == '\'') {
           			current_state = IN_STR;
           			tkn->value[i++] = c;
        		}
        		else {
            		eot = lookahead = !0;
            		tkn->type = STRING;
        		}
        		break;
        	case CHAR:  //aggiunta per riconoscere caratteri
        		if (c == '\'') {
					current_state = APEX2;
					tkn->value[i++] = c;
        		}
        		else {
        			current_state = IN_STR;  
					tkn->value[i++] = c;      			
        		}
        		break;	
			case APEX2:
				if (c == ';'){
					tkn->type = CHARACTER;
					tkn->value[--i] = ' ';
					eot = lookahead = !0;
				}
				else if (isgraph(c)) {
					current_state = IN_STR;
					tkn->value[i++] = c;
				}
				else {
					tkn->value[--i] = ' ';
					tkn->type = CHARACTER;
					eot = lookahead = !0;
				}
				break;
        	case IN_CHAR:  //aggiunta per riconoscere caratteri
        		if (isgraph(c)) {
        			tkn->value[i++] = c;
        			current_state = CHAR;
        		}        		
        		break;        			
  		  }  
	}
 
 	tkn->value[i++]='\0'; //poni il carattere nullo di terminazione per la stringa che contiene il valore
 
 	if (is_keyword(tkn->value)) 
		tkn->type = KEYWORD;
      
 	return (c!=EOF); //torna il valore 1 se "c" è diverso da EOF  
}


/* Main Program --------------------------------------------------------------*/
int main (int argc, char* argv[])
{
 	char* filename = argv[1];
 	FILE* source;
 	Token token;
 
// 	source = fopen(filename,"r");
	source = fopen("test.pas","r");
 
	while (getToken(source, &token)) {
  		if (token.type == ERROR)
     		printf ("Lexical Error (%s)\n", token.value);
  		else
        	printf ("Tipo = %d, Valore = '%s'\n", token.type,token.value);
 	}
 
 	fclose(source);
 
 	system("PAUSE");
}
