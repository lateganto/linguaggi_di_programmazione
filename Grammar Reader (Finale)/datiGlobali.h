#include <stdio.h>
#include <stdlib.h>
#define MAX_WORD_LENGTH 100
#define MAX_PRODUCTIONS 100
#define NUM_ERR 3 //costante che definisce il numero di errori classificabili in ogni produzione

// Definizione dei tipi ------------------------------------------------------*/

typedef char Symbol;
typedef enum {F, T} Bool; //tipo di dati booleano per indicare VERO (T) o FALSO (F)

typedef struct
{
    Symbol word [MAX_WORD_LENGTH];
    unsigned length;
} Word;

typedef struct
{
    Word left;
    Word right;
    Bool prodErr[NUM_ERR]; //array di booleani che contiene gli errori che una produzione può avere
} Production;

typedef struct
{
    Production productions[MAX_PRODUCTIONS];
    unsigned numprod;
    Bool error; //variabile booleana che indica se la grammatica presenta uno o più errori
} Grammar;
