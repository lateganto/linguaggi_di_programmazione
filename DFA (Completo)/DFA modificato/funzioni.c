#include <stdio.h>
#include <stdlib.h>

int scan(char* s)
{
	enum {S,A,B,C,D,E,G,H,POZZA,NON_RICONOSCIUTO} current_state = S; //variabile che rappresenta lo stato corrente inizializzata allo stato di partenza S
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
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;
   			case A:
        		if (s[i] >= '1' && s[i] <= '9')
             		current_state = C;
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;        
   			case B:
   				if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
					current_state = POZZA;
   			case C:
        		if (s[i]=='.')
           			current_state = D;
        		else if (s[i]=='e' || s[i]=='E')
             		current_state = E;
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;
   			case D:
        		if (s[i]=='e' || s[i]=='E')
           			current_state = E;
        		else if (s[i] >= '0' && s[i] <= '9')
             		current_state = D;
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;
   			case E:
        		if (s[i]=='+' || s[i]=='-')
           			current_state = H;
        		else if (s[i] >= '0' && s[i] <= '9')
             		current_state = G;
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;
   			case H:
        		if (s[i] >= '0' && s[i] <= '9')
             		current_state = G;
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;   
   			case G:
        		if (s[i] >= '0' && s[i] <= '9')
             		current_state = G;
             	else if(!(s[i]=='.' || s[i]=='e' || s[i]=='E' || (s[i] >= '1' && s[i] <= '9') || s[i]=='+' || s[i]=='-'))
             		current_state = NON_RICONOSCIUTO;
        		else
            		current_state = POZZA;
        		break;                
		}
	  	i++;
 	}
 	if (current_state == B || current_state == G) //se lo stato corrente è B o G (stati finali) torna 1 altrimenti torna 0
		return 0;
	else if (current_state == S || current_state == A || current_state == C || current_state == D || current_state == E || current_state == H)
		return 1;
	else if (current_state == NON_RICONOSCIUTO)
		return 2;
	else
		return 3;
}
