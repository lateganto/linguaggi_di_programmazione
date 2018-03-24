#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "datiGlobali.h"

int is_keyword(Stringa s) //funzione che verifica se un identificatore è in realtà una parola chiave
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
	static char c;  //contiene il carattere letto da file
	static int lookahead = 0;  //indica se il carattere contenuto in "c" è stato consumato (vero) o deve essere ripreso in considerazione per il riconoscimento (falso)
	
	enum {
		S,				/* Stato Iniziale */
		ID_PC,			/* Identificatore o Keyword */
		SLASH,			/* Slash */
		EQUAL,			/* Segno uguale */
		G_GE,			/* Maggiore o maggiore-uguale */
		L_LE,			/* Minore, minore-uguale */
		ESCL,			/* Punto esclamativo */
		IN_CAR,			/* Verifica carattere */
		CAR,			/* Carattere */
		APICE2,			/* Apice in carattere */
		IN_STR,		 	/* Stringa */
		NUM,			/* Numero intero o reale */
		NUM_R,			/* Numero reale */
		ES_ESP,			/* Verifica numero esadecimale o in notazione esponenziale */
		NUM_R_ESP,		/* Numero reale esponenziale */
		ESP,			/* Esponente numero reale in notazione esponenziale */
		NUM_ES,			/* Numero esadecimale */
		START_COM,		/* Inizio commento */
		AST_IN_COM,		/* Asterisco in commento */
		APICE			/* Apice in stringa */
	} current_state = S;   //lo stato parte da "S"
	
	int i = 0;  //contatore per inserire i caratteri in "token->valore"
	int eot = 0;  /* "end of token" (se assume "vero" indica che il token è terminato) */
	tkn->tipo = ERROR;
	
	while (!eot && i<LUNG_MAX) {
		if (!lookahead)
			c = fgetc(src);
		else 
			lookahead = 0;
	
		if (c == EOF)
			break;
		else 
			switch (current_state) {
				case S:
					if (isalpha(c) || c == '_') {
						current_state = ID_PC;
						tkn->valore[i++] = c;
						tkn->tipo = IDENTIFICATORE;
					}
					else if (isspace(c))
						current_state = S;
					else if (isdigit(c)) {
						current_state = NUM;
						tkn->valore[i++] = c;
						tkn->tipo = INTERO;
					}
					else switch (c) {
						case '+':
							tkn->tipo = PIU;
							tkn->valore[i++] = c;
							eot = !0;
							break;
						case '-':
							tkn->tipo = MENO;
							tkn->valore[i++] = c;
							eot = !0;
							break;
						case '*':
              				tkn->tipo = ASTERISCO;
              				tkn->valore[i++] = c;
              				eot = !0;
	              			break;
    	          		case '/':
        	      			current_state = SLASH;
            	  			tkn->valore[i++] = c;
              				break;
              			case '=':
	              			current_state = EQUAL;
    	          			tkn->valore[i++] = c;
        	      			break;
            	  		case ';':
              				tkn->tipo = PUNT_VIRG;
              				tkn->valore[i++] = c;
              				eot = !0;
	              			break;
    	          		case '.':
        	      			tkn->tipo = PUNTO;
            	  			tkn->valore[i++] = c;
              				eot = !0;
              				break;
	              		case ',':
    	          			tkn->tipo = VIRGOLA;
        	      			tkn->valore[i++] = c;
            	  			eot = !0;
              				break;
	              		case '(':
    	          			tkn->tipo = PAR_S;
        	      			tkn->valore[i++] = c;
            	  			eot = !0;
              				break;
              			case ')':
	              			tkn->tipo = PAR_D;
    	          			tkn->valore[i++] = c;
        	      			eot = !0;
            	  			break;
              			case '>':
              				current_state = G_GE;
              				tkn->valore[i++] = c;
	              			break;
    	          		case '<':
        	      			current_state = L_LE;
            	  			tkn->valore[i++] = c;
              				break;
						case '!':
							current_state = ESCL;
							tkn->valore[i++] = c;
							break;	
        	      		case '\'':
            	  			current_state = IN_CAR;
              				break;
              			case '{':
              				tkn->tipo = GRAF_S;
	              			tkn->valore[i++] = c;
    	          			eot = !0;
        	      			break;
            	  		case '}':
              				tkn->tipo = GRAF_D;
              				tkn->valore[i++] = c;
              				eot = !0;
	              			break;
    	          		default: 
        	          		eot = !0;
            	      		tkn->valore[i++] = c;	
					}
					break;
				case ID_PC:
					if (isalnum(c) || c=='_') {
						current_state = ID_PC;
						tkn->valore[i++] = c;
					}
					else 
						lookahead = eot = !0;
					break;
				case NUM:
					if (isdigit(c)) {
						tkn->valore[i++] = c;
					} 
					else if (c == 'E') {
						tkn->valore[i++] = c;
						current_state = ES_ESP;
					}	
					else if  (isxdigit(c)) {
						current_state = NUM_ES;
						tkn->valore[i++] = c;
					}
					else if (c=='.') {
						current_state = NUM_R;
						tkn->valore[i++] = c;
					}
					else {
						tkn->tipo = INTERO;
						eot = lookahead = !0;
					}
					break;
				case ES_ESP:
					if (isxdigit(c)) {
						tkn->valore[i++] = c;
						current_state = NUM_ES;
					}
					else {
						tkn->valore[i++] = c;
						current_state = NUM_R_ESP;
					}
					break;		
				case NUM_R:
					if (isdigit(c)) {
						tkn->valore[i++] = c;
					}
					else if (c == 'e' || c == 'E') {
           				current_state = NUM_R_ESP;
           				tkn->valore[i++] = c;
	           		}	
					else {
						tkn->tipo = REALE;
						eot = lookahead = !0;
					}
					break;
				case NUM_R_ESP: //aggiunta "numero reale in notazione esponenziale"
        			if (c == '+' || c == '-') {
        				current_state = ESP;
  		      			tkn->valore[i++] = c;						
        			}        			
					else if (isdigit(c))
        				tkn->valore[i++] = c;
        			else {
						tkn->tipo = REALE_ESP;
						eot = lookahead = !0;
    	    		}
        			break;
				case ESP: //aggiunta esponente
					if (isdigit(c))
						tkn->valore[i++] = c;
					else {
						tkn->tipo = REALE_ESP;
						eot = lookahead = !0;
					}
					break;
				case NUM_ES:
					if (isxdigit(c))
						tkn->valore[i++] = c;
					else {
						tkn->tipo = ESADECIMALE;
						eot = lookahead = !0;
					}
					break;
				case SLASH:
					if (c == '*')
						current_state = START_COM;
						i--;
					break;
				case START_COM:
					if (c == '*')
						current_state = AST_IN_COM;
					break;
				case AST_IN_COM:
					if (c == '/')
						current_state = S;
					else
						current_state = START_COM;
					break;
				case G_GE: 
    	    		if (c == '=') {
        	   			tkn->tipo = MAGG_UGUAL;
           				tkn->valore[i++] = c;
           				eot = !0; //hai raggiunto la fine del token ">="
        			}
	        		else {
    	        		tkn->tipo = MAGG;
        	    		eot = lookahead = !0;
        			}
        			break;
	   			case L_LE:
    	    		if (c == '=') {
        	   			tkn->tipo = MINOR_UGUAL;
           				tkn->valore[i++] = c;
           				eot = !0;
	        		}
    	    		else {
        	    		tkn->tipo = MINOR;
            			eot = lookahead = !0;
        			}
	        		break;
    	    	case EQUAL:
        			if (c == '=') {
        				tkn->tipo = UGUALE;
        				tkn->valore[i++] = c;
        				eot = !0;
	        		}
    	    		else {
        				tkn->tipo = ASSEGNAZIONE;
        				eot = lookahead = !0;
        			}	
	        		break;
    	    	case ESCL:
        			if (c == '=') {
        				tkn->tipo = DIVERSO;
        				tkn->valore[i++] = c;
        				eot = !0;
        			}
	        		break;
	        	case IN_CAR:
	        		if (isgraph(c)) {
        				tkn->valore[i++] = c;
        				current_state = CAR;
        			}        		
        			break; 
    	    	case CAR:  //aggiunta per riconoscere caratteri
        			if (c == '\'') {
						current_state = APICE2;
						tkn->valore[i++] = c;
 	    	   		}
    	    		else {
        				current_state = IN_STR;  
						tkn->valore[i++] = c;      			
        			}
        			break;
        		case APICE2:
					if (c == ';'){
						tkn->tipo = CARATTERE;
						tkn->valore[--i] = ' ';
						eot = lookahead = !0;
					}
					else if (isgraph(c)) {
						current_state = IN_STR;
						tkn->valore[i++] = c;
					}
					else {
						tkn->valore[--i] = ' ';
						tkn->tipo = CARATTERE;
						eot = lookahead = !0;
					}
					break;
				case IN_STR: //Stringa
        			if (c == '\'') 
           				current_state = APICE;
        			else {
            			tkn->valore[i++] = c;
        			}
	        		break;
   				case APICE: //apice in stringa
        			if (c == '\'') {
           				current_state = IN_STR;
           				tkn->valore[i++] = c;
        			}
	        		else {
    	        		eot = lookahead = !0;
        	    		tkn->tipo = STRINGA;
        			}
        			break;	 	
			}
	}
	
	tkn->valore[i++] = '\0'; //poni il carattere nullo di terminazione per la stringa che contiene il valore
 
 	if (is_keyword(tkn->valore)) //verifica se l'identificatore è una parola-chiave
		tkn->tipo = PAROLA_CHIAVE;  
      
 	return (c!=EOF); //torna il valore 1 se "c" è diverso da EOF
}

//---------------MAIN--------------
int main (int argc, char* argv[])
{
 	char* filename = argv[1];
 	FILE* source;
 	Token token;
 
// 	source = fopen(filename,"r");
	source = fopen("test.pas","r");
 
	while (getToken(source, &token)) {
  		if (token.tipo == ERROR)
     		printf ("Lexical Error (%s)\n", token.valore);
  		else
        	printf ("Tipo = %d, Valore = '%s'\n", token.tipo,token.valore);
 	}
 
 	fclose(source);
 
 	system("PAUSE");
}
