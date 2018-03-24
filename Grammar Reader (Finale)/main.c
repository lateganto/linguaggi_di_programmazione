/* 
   GrammarReader
   v. 1.2.2
   Corrado Mencar, Pasquale Lops
*/

#include <stdio.h>
#include "datiGlobali.h"

// MAIN ------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	char* filename = argv[1];
	FILE* gram_file;
	Grammar grammar;
  
  // controlla se è stato inserito il nome del file
  
	if (filename == 0) {
    	printf("Nome file non specificato!\n");
     	return -1;
  	}
  
  // apertura del file contenente la grammatica
  	//gram_file = fopen("Gramm_Esempio2.txt","r");
  	gram_file = fopen(filename,"r");
  	if (gram_file == NULL) {
     	printf("Nome di file errato!\n");
     	return -1;
  	}
  
  	print_grammar(load_grammar(gram_file,&grammar));
  
  	fclose(gram_file);
  
  
  	system("PAUSE");	
  	return 0;
}
