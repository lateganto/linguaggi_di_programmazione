/*  
Automa a stati finiti che riconosce il linguaggio di tutte leparole su {a,b}*
tali che non contengono come sottostringa "bba"      
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 100

int main(int argc, char* argv[])
{
	char buffer[MAX_LENGTH+1]; //questo array conterrà una riga del file da passare all'automa
 	char* filename = argv[1];
 	FILE* file;
 
// 	file = fopen(filename,"r");
	file = fopen("prova.txt","r");
 	if (file == NULL) {
      	printf("File inesistente!");
      	system("PAUSE");
      	return -1;
	}

 	while (!feof(file)) {
  		if (fgets(buffer,MAX_LENGTH,file))  //fgets memorizza in buffer una riga del file (si ferma quando incontra \n o EOF)
     		if (scan(buffer)==0) //modifica ES 4
        		printf("%s RICONOSCIUTO\n", buffer);
     		else if (scan(buffer)==1)
         		printf("%s TRANSAZIONE TERMINATA IN UNO STATO NON FINALE\n", buffer);
         	else if (scan(buffer)==2)
         		printf("%s PRESENTE SIMBOLO ESTERNO ALL'ALFABETO\n", buffer);
         	else if (scan(buffer)==3)
         		printf("%s NON RICONOSCIUTO (POZZA)\n", buffer);
 	}
 
 	fclose(file);
 	system("PAUSE");
 	return 0;
}
