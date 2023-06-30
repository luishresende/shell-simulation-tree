#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "DiretorioArq.h"
#define MAX_STR_LENGTH = 256

int main() {
    setlocale(LC_ALL, "Portuguese");
    Diretorio* dir = cria_usuario();
    ler_txt(dir, "arquivos.txt");
    system("pause");
    while(1){
        executar(dir);
        //break nunca
    }
    return 0;
}