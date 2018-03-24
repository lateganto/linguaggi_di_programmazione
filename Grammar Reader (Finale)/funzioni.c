#include <stdio.h>
#include "datiGlobali.h"

// Procedure di riconoscimento dei simboli -----------------------------------*/

int is_terminal(Symbol s)
{
	return (islower(s));
    //return (s >= 'a') && (s <= 'z');
}

int is_nonterminal(Symbol s)
{
    return (isupper(s));
    //return (s >= 'A') && (s <= 'Z');
}

int is_prodsym(Symbol s)
{
    return (s == '>');
}

int is_prodsep(Symbol s)
{
    return (s == '\n');
}

/* Lettura di simboli da file ------------------------------------------------*/

Symbol read_sym(FILE* file)
{
    Symbol s;

    // fscanf(file,"%c",&s);

    //questo ciclo permette di saltare la lettura di spazi tra i simboli
    do {
    	s = getc(file);
    } while (s==' ');

    return s;
}

Production* add_new_production(Grammar *g)
{
  	Production* p;

  	p = &(g->productions[g->numprod++]);
  	p->left.length = 0;
   //p->right.length = 0;

  	return p;
}


void add_symbol(Word *w, Symbol s)
{
  	w->word[w->length++] = s;
}


// Procedura di acquisizione di una grammatica da un file --------------------*/

Grammar* load_grammar(FILE* file, Grammar* g)
{
    enum States {START,LEFT,RIGHT};
    /*   START  = Scansione di una nuova produzione [F]
         LEFT   = Scansione della parte sinistra
         RIGHT  = Scansione della parte destra [F]
    */
    enum States current_state = START;  // Stato iniziale
    Symbol s;
    Production* p;

    g->numprod = 0; // Inizializza la grammatica
	g->error = F; //si suppone che la grammatica non presenta alcun errore
	
    while (!feof(file)) {
    	s = read_sym(file);
        if (feof(file)) break;
        switch(current_state) {
            case START:
                if (is_terminal(s) || is_nonterminal(s)) {
                	current_state = LEFT;
                    p = add_new_production(g);
                    //L'istruzione precedente corrisponde a p = &(g->productions[g->numprod++]);
                    //										 p->left.length = 0;
                    add_symbol(&p->left,s);
                    //L'istruzione precedente corrisponde a p->left.word[p->left.length++] = s;
                }
                else if (is_prodsep(s)) { //nuova produzione perchè si è incontrato /n
                    current_state = START;
                }
                else { 
                	p->prodErr[3] = T; //errore assenza simboli nella parte destra.
                	g->error = T; //errore nella grammatica
                	current_state = LEFT;
                }
                break;

			case LEFT:
                if (is_terminal(s) || is_nonterminal(s)) {
                    current_state = LEFT;
                    add_symbol(&p->left,s);
                }
                else if (is_prodsym(s) && contieneNonterminale(p)) { //Aggiunta ESERCIZIO 1
                    current_state = RIGHT;
                    p->right.length = 0;
                }
				else if (is_prodsym(s) && !(contieneNonterminale(p) )) {
					p->prodErr[0] = T; //la produzione in questione presenta un errore cioè assenza non terminale
					current_state = RIGHT;
					g->error = T;
				}
                else{
                	p->prodErr[2] = T; //errore mancanza simbolo di produzione
                	current_state = START;
                	g->error = T;
                }
                break;

			case RIGHT:
                if (is_terminal(s) || is_nonterminal(s)) {
                    current_state = RIGHT;
                    add_symbol(&p->right,s);
                }
                else if (is_prodsep(s)) {
                    current_state = START;
                }
                else {
                    p->prodErr[1] = T; //errore simbolo non riconosciuto
                    g->error = T;
                	current_state = START;
                }
                break;
        }
    }
    
    return g; //viene restituita l'intera grammatica
}

// Procedure di stampa

void print_sym (Symbol s)
{
    printf("%c ",s);
}

void print_word (Word* w)
{
    int i;

	for (i=0; i<w->length; i++)
        print_sym(w->word[i]);
}

void print_production (Production* p)
{
	print_word(&p->left);
    printf (" --> ");
    print_word(&p->right);
}

void print_grammar(Grammar* g)
{
    int i;

	if (!(contieneSimboloIniziale(g)))
		printf("La grammatica non presenta alcuna produzione con il simbolo di partenza S!\n");
	if (g->error==T)
		printf ("Errore! Grammatica non valida! \n");

	printf ("Numero di produzioni: %d\n", g->numprod);
    for (i=0; i<g->numprod; i++) { //permette di stampare la produzione e il o i tipi di errore affianco
        print_production(&g->productions[i]);

		if (g->productions[i].prodErr[0]==T)
			printf(" Assenza non terminale. ");
		if (g->productions[i].prodErr[1]==T)
			printf(" Simbolo non corretto. ");
		if (g->productions[i].prodErr[2]==T)
			printf(" Assenza simbolo di produzione. ");
		if (g->productions[i].prodErr[3]==T)
			printf(" Assenza simboli nella parte destra. ");

    	printf ("\n");
	}
    
	if (g->error==F) { //se non si presenta nessun errore allora controlliamo che tipo di grammatica è
		if(contextFree(g))  //aggiunta ESERCIZIO 3
			puts("La grammatica e' CONTEXT FREE!");
		if(isMonotonic(g))  //aggiunta ESERCIZIO 4
			puts("La grammatica e' MONOTONA!");
		if(lineareDestra(g))  //aggiunta ESERCIZIO 5
			puts("La grammatica e' LINEARE DESTRA!");
    }
}

//ESERCIZIO 1
int contieneNonterminale(Production* p)
{
	int i;
	Word* w = &p->left; //dichiarazione di un puntatore di tipo Word che punta alla parte sinistra della produzione

	for(i=0; i < w->length; i++) {
		if(is_nonterminal(w->word[i])) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

//ESERCIZIO 2
int contieneSimboloIniziale(Grammar* g)
{
	int i;
	Production* p;
	Word* w;
	Symbol s;  //variabile di tipo carattere che contiene il simbolo da verificare

	for(i=0; i < g->numprod; i++) {
		p = &g->productions[i];
		w = &p->left;
		s = w->word[0];

		if(w->length==1 && s=='S') {
			return 1;
		}
	}
	return 0;
}

//ESERCIZIO 3
int contextFree(Grammar* g)
{
	int i;
	Production* p;
	Word* w;
	Symbol s;

	for(i=0; i < g->numprod; i++) {
		p = &g->productions[i];
		w = &p->left;
		s = w->word[0];

		if (!(w->length==1 && is_nonterminal(s))) {
			return 0;
		}
	}
	return 1;
}

//ESERCIZIO 4
int isMonotonic(Grammar* g)
{
	int i;
	Production* p;

	for(i=0; i < g->numprod; i++) {
		p = &g->productions[i];

		if(p->right.length == 0)  //bisogna considerare il caso in cui la parte destra sia la parola vuota
			p->right.length = 1;
		if(p->left.length > p->right.length) {
			return 0;
		}
	}
	return 1;
}

//ESERCIZIO 5
int lineareDestra(Grammar* g)
{
	int i;
	Production* p;
	Word* sx;
	Word* dx;

	for (i=0; i < g->numprod; i++) {
		p = &g->productions[i];
		sx = &p->left;
		dx = &p->right;

		if (!(sx->length == 1 && is_nonterminal(sx->word[0])))
			return 0;

		else if (!( (dx->length==2 && (is_terminal(dx->word[0]) && is_nonterminal(dx->word[1]))) || (dx->length==1 && is_terminal(dx->word[0])) || (dx->word[0]=='\n') ))
			return 0;
	}
	return 1;
}
