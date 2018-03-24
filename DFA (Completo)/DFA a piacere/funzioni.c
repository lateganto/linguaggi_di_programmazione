#include <stdio.h>
#include <stdlib.h>

int scan(char* s)
{
	enum {S,A,B,C,POZZA,NON_RICONOSCIUTO} current_state = S; //variabile che rappresenta lo stato corrente inizializzata allo stato di partenza S
 	int i = 0; //indice per la scansione della stringa in input
 
	while ((s[i]!='\0' && s[i]!='\n') && current_state != POZZA) { //finchè non si è raggiunta la fine della riga e lo stato non è quello POZZA
		switch (current_state) {
   			case S:
        		if (s[i] == 'a')
           			current_state = S;
        		else if (s[i] == 'b')
             		current_state = A;
             	else if(!(s[i] == 'a' || s[i] == 'b'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;
   			case A:
        		if (s[i] == 'b')
             		current_state = B;
             	else if (s[i] == 'a')
             		current_state = S;
             	else if(!(s[i] == 'a' || s[i] == 'b'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;        
   			case B:
   				if (s[i] == 'b')
             		current_state = B;
             	else if (s[i] == 'a')
             		current_state = C;
             	else if(!(s[i] == 'a' || s[i] == 'b'))
             		current_state = NON_RICONOSCIUTO;
        		else
					current_state = POZZA;
				break;
   			case C:
        		if (s[i] == 'a')
           			current_state = C;
        		else if (s[i] == 'b')
             		current_state = C;
             	else if(!(s[i] == 'a' || s[i] == 'b'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;               
		}
	  	i++;
 	}
 	if (current_state == S || current_state == A || current_state == B) //se lo stato corrente è A, B o S (stati finali) torna 1 altrimenti torna 0
		return 0;
	else if (current_state == C)
		return 1;
	else if (current_state == NON_RICONOSCIUTO)
		return 2;
	else
		return 3;
}
