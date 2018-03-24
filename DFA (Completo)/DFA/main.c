/*  
   Implementazione di automi DFA per il riconoscmento di linguaggi regolari
   v 1.3 - 16/05/2006
   Corrado Mencar
   
   History:
           1.0 (11/04/2005): Prima release
           1.1 (11/04/2005): inserita fclose(...) e #define
           1.2 (22/04/2005): aggiunto test su fgets
           1.3 (16/05/2006): aggiunto system("PAUSE") su controllo file       
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 100

int main(int argc, char* argv[])
{
	char buffer[MAX_LENGTH+1]; //questo array conterrà una riga del file da passare all'automa
 	char* filename = argv[1];
 	FILE* file;
 
 	file = fopen(filename,"r");
 	if (file == NULL) {
      	printf("File inesistente!");
      	system("PAUSE");
      	return -1;
	}

 	while (!feof(file)) {
  		if (fgets(buffer,MAX_LENGTH,file))  //fgets memorizza in buffer una riga del file (si ferma quando incontra \n o EOF)
     		if (scan(buffer))
        		printf("%s RICONOSCIUTO\n", buffer);
     		else
         		printf("%s NON RICONOSCIUTO\n", buffer);
 	}
 
 	fclose(file);
 	system("PAUSE");
 	return 0;
}
