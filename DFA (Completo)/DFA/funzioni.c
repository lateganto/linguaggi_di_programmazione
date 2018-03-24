#include <stdio.h>
#include <stdlib.h>

int scan(char* s)
{
	enum {S,A,B,C,D,E,G,H,POZZA} current_state = S; //variabile che rappresenta lo stato corrente inizializzata allo stato di partenza S
 	int i = 0; //indice per la scansione della stringa in input
 
	while ((s[i]!='\0' && s[i]!='\n') && current_state != POZZA) { //finchè non si è raggiunta la fine della riga e lo stato non è quello POZZA
		switch (current_state) {
   			case S:
        		if (s[i]=='+' || s[i]=='-')
           			current_state = A;
        		else if (s[i]=='0')
             		current_state = B;
        		else if (s[i] >= '1' && s[i] <= '9')
             		current_state = C;
        		else
            		current_state = POZZA;
        		break;
   			case A:
        		if (s[i] >= '1' && s[i] <= '9')
             		current_state = C;
        		else
            		current_state = POZZA;
        		break;        
   			case B:
        		current_state = POZZA;
   			case C:
        		if (s[i]=='.')
           			current_state = D;
        		else if (s[i]=='e' || s[i]=='E')
             		current_state = E;
        		else
            		current_state = POZZA;
        		break;
   			case D:
        		if (s[i]=='e' || s[i]=='E')
           			current_state = E;
        		else if (s[i] >= '0' && s[i] <= '9')
             		current_state = D;
        		else
            		current_state = POZZA;
        		break;
   			case E:
        		if (s[i]=='+' || s[i]=='-')
           			current_state = H;
        		else if (s[i] >= '0' && s[i] <= '9')
             		current_state = G;
        		else
            		current_state = POZZA;
        		break;
   			case H:
        		if (s[i] >= '0' && s[i] <= '9')
             		current_state = G;
        		else
            		current_state = POZZA;
        		break;   
   			case G:
        		if (s[i] >= '0' && s[i] <= '9')
             		current_state = G;
        		else
            		current_state = POZZA;
        		break;                
		}
	  	i++;
 	}
 	return (current_state == B || current_state == G); //se lo stato corrente è B o G (stati finali) torna 1 altrimenti torna 0
}
