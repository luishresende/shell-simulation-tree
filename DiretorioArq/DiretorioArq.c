#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DiretorioArq.h"

Diretorio* cria_usuario(){
    Diretorio *dir;
    dir = (Diretorio*) malloc(sizeof(Diretorio));
    if(dir != NULL){
        strcpy(dir->tipo, "");
        dir->irmao = NULL;
        dir->filho = NULL;
        strcpy( dir->caminho, "C:");
        return dir;
    }else {
        return NULL;
    }

}

void mkdir(Diretorio* dir, char caminho[256], char tipo[10]){
    if(dir == NULL){ // Retornando para a main caso dir for nulo
        return;
    }
    if(strcmp(dir->tipo, "") != 0){ // Verificando se não é uma pasta
        printf("Retornando, tentando criar dentro do arquivo\n");
        return;
    }
    Diretorio* novo = (Diretorio*) malloc(sizeof(Diretorio));

    char caminhoPai[256]; // Criando string para armazenar o caminho do diretorio pai
    strcpy(caminhoPai, dir->caminho);
    if(strcmp(tipo, "") != 0){ // Se for um arquivo:
        strcpy(novo->tipo, tipo); // Tipo recebe o tipo
        strcpy(novo->caminho, strcat(strcat(strcat(strcat(caminhoPai, "/"), caminho), "."), tipo)); // Atribuindo o caminho ao novo arquivo
    }else {
        strcpy(novo->tipo, ""); // Definindo o tipo da pasta como "", ou seja, vazio, pois é uma pasta
        strcpy(novo->caminho, strcat(strcat(caminhoPai, "/"), caminho)); // Definindo o caminho da nova pasta
    }

    novo->irmao = NULL; //Definindo os ponteiros do novo arquivo/pasta como NULL
    novo->filho = NULL;

    Diretorio* aux = dir->filho; // Criando um diretório aux para percorrer até chegar no ultimo irmão
    while (aux != NULL) {
        if (strcmp(aux->caminho, novo->caminho) == 0) { 
            printf("Pasta ou arquivo j� existente!\n");
            free(novo); // Libera a mem�ria alocada para o novo diret�rio
            return;
        }
        aux = aux->irmao;
    }

    if(dir->filho == NULL){ // Se o novo arquivo/pasta, for o primeiro diretorio do pai
        dir->filho = novo;
    } else { 
        Diretorio* aux = dir->filho;
        while(aux->irmao != NULL){ // Percorrendo para chegar no ultimo irmao, e atribuindo o novo irmão(arquivo/pasta)
            aux = aux->irmao;
        }
        aux->irmao = novo;
    }
    return;
}

void help(){ // Função que imprime todos os comandos disponíveis no sistema
    printf(
            "Comandos dispon�veis:\n"
            "- cd     : entrar em uma pasta\n"
            "- search : busca uma pasta ou arquivo\n"
            "- rm     : remover uma pasta\n"
            "- list   : lista os componentes dentro da pasta em questao\n"
            "- mkdir  : cria uma nova pasta\n"
            "- clear  : limpa o conteudo da tela\n"
            "- help   : exibe a relacao completa dos comandos\n"
            "- exit   : fechar o programa\n"
            "- ..     : retornar\n"
    );
}

char cortarString(char *str, int indice) { // Função para cortar a string passada por parametro
    if (indice >= strlen(str)) {
        return ' ';  // �ndice fora dos limites da string, n�o faz nada
    }
    strcpy(str, str + indice);
}

int printar_nome(char caminho[265], int pos){ // Função recursiva criada para encontrar o indice da barra no caminho do arq/pasta.
    if(caminho[pos] == '/'){
        return pos;
    }else {
        pos = printar_nome(caminho, pos-1);
    }
    return pos;
}

void list(Diretorio* dir, int modo, char caminho[256]){ // Modo 1: Apenas mostrar os nomes disponiveis. Modo 2: Utilizando quando o
                                                        //usuario tenta acessar um diretorio invalido, mostrando as alternativas possiveis
    if(strcmp(dir->tipo, "") != 0){ // Verificando se o usuario está procurando arquivos dentro de um arquivo
        printf("Diretorio invalido!\n");
        return;
    }
    if(dir->filho == NULL){ // Se o filho for nulo do elemento for nulo, diretório está vazio
        printf("Diretorio vazio\n");
        return;
    }
    Diretorio* aux = dir->filho;
    char nome[256] = "";
    int printados = 0;
    do{
        strcpy(nome, aux->caminho);
        cortarString(nome, printar_nome(aux->caminho, strlen(aux->caminho) - 1));
        if(modo == 1){ // Se for modo 1, apenas printa
            printf("%s\n", nome);
        }else{ // Se não for, faço as verificações para cada diretório, se a string informanda pelo usuario, bate com o inicio de algum diretorio existente
            int igual = 0;
            for(int i = 0; i < strlen(caminho); i++){ // Verifico se cada elemento da string do usuario é igual ao nome do diretorio
                if(caminho[i] != nome[i+1]){
                    igual = 1; // Se for diferente, vai receber 1
                }
            }
            if(igual == 0){ // Printo as possiveis alternativas, caso contrario
                if(printados==0){ // Se o numero de alternativas printadas for igual a 0, imprimo a frase de auxilio
                    printf("Possiveis alternativas:\n");        //
                }
                printf("%s\n", nome); // Printo o nome do diretorio
                printados++; // Printados recebe mais um
            }
        }

        aux = aux->irmao;
    }while(aux != NULL);
    if(modo != 1 && printados == 0){ // Se chegar ao final, e modo for diferente de 1, e printados for igual a 0, informo 
        printf("Diretorio nao encontrado!\n"); //que nao foi encontrado nenhum diretorio
    }
}

void rm_recursiva(Diretorio* dir){ // Função recursiva para eliminar todos os elementos dentro de um determinado diretorio
    if(dir->irmao != NULL){ // Caminho para o irmão
        rm_recursiva(dir->irmao);
    }
    if(dir->filho != NULL){ // Caminho para o filho
        rm_recursiva(dir->filho);
    }
    free(dir); // Libero a memória
    return;
}

void rm(Diretorio* dir, char nome[256]){ // Função para excluir um diretório/arquivo
    Diretorio* filho = dir->filho;
    Diretorio* ant = filho; // Crio um diretorio ant para auxiliar na jogada de ponteiros ao eliminar o diretorio/arquivo
    char diretorioPai[256]; // Crio uma string para receber o caminho do pai e realizar manipulações
    strcpy(diretorioPai, dir->caminho);
    nome = strcat(strcat(diretorioPai, "/"), nome);
    while(filho != NULL && strcmp(filho->caminho, nome) != 0){ // Caminho entre os irmão até encontrar o diretório
        ant = filho;
        filho = filho->irmao;
    }
    if(filho == NULL){ // Se filho for null, informo que o diretorio nao foi encontrado
        printf("Diretorio nao encontrado!\n");
        return;
    }
    if(dir->filho == filho){ // Se filho(elemento que percorreu até chegar no diretório desejado) for igual ao filho do diretorio pai
        ant = filho->irmao; // Anterior recebe o irmao do filho
        dir->filho = ant; // Filho do diretorio pai agora é o anterior
    }else{
        ant->irmao = filho->irmao; // Irmão do ant, é igual ao irmão do irmão
    }
    if(filho->filho != NULL){ // Se filho do filho não for nulo, chamo a função recursiva para apagar todos os diretorios dentro de filho, antes de elimina-lo
        rm_recursiva(filho->filho);
    }
    free(filho); // Libero o filho
}

void search(Diretorio* dir, char nome[256]){
    Diretorio* filho = dir->filho;
    char diretorioPai[256]; // Crio uma string para receber o caminho do pai e realizar manipulações
    strcpy(diretorioPai, dir->caminho);
    nome = strcat(strcat(diretorioPai, "/"), nome); // nome recebe o caminho informado pelo usuario
    while(filho  != NULL && strcmp(filho->caminho, nome) != 0){ // Percorro os irmaos afim de encontrar o diretorio desejado
        filho = filho->irmao;
    }
    if(filho == NULL){ // Se filho for nulo, informo que não encontrei o diretorio
        printf("Diretorio nao encontrado!\n");
        return;
    }else {
        if(strcmp(filho->tipo, "") == 0){ // Se o tipo do elemento encontrato for nulo, informo que a pasta foi encontrada, e o seu caminho
            printf("Pasta encontrada!\n"
                   "Caminho: %s\n", filho->caminho);
        }else { // Se o tipo do elemento encontrato não for nulo, informo que o arquivo foi encontrado, seu tipo, e o caminho do mesmo
            printf("Arquivo %s encontrado!\n"
                   "Caminho: %s\n", filho->tipo ,filho->caminho);
        }
    }
}

Diretorio* cd(Diretorio* dir, char nome[256]) {
    Diretorio *filho = dir->filho;
    char caminhoPai[256]; // Caminho do diretório pai
    strcpy(caminhoPai, dir->caminho);
    char nomeOriginal[256] = ""; // Nome original antes de modificar o valor
    strcpy(nomeOriginal, nome);
    nome = strcat(strcat(caminhoPai, "/"), nome); // Concatena o caminho do pai com o nome fornecido
    while (filho != NULL && strcmp(filho->caminho, nome) != 0) {
        filho = filho->irmao;
    }
    if (filho == NULL) {
        list(dir, 2, nomeOriginal); // Chama a função list para listar o diretório atual
        return dir;
    }

    if (strcmp(filho->tipo, "") == 0) {
        return filho; // Retorna o novo diretório encontrado
    } else {
        printf("Este caminho não corresponde a uma pasta!\n");
        return dir; // Retorna o diretório atual se o caminho não corresponder a uma pasta
    }
}

int verificarComando(char comando[256]){
    if(strcmp(comando, "cd") == 0){
        return 1;
    }else if(strcmp(comando,"search") == 0){
        return 2;
    }else if(strcmp(comando, "rm") == 0){
        return 3;
    }else if(strcmp(comando, "list") == 0){
        return 4;
    }else if(strcmp(comando, "mkdir") == 0){
        return 5;
    }else if(strcmp(comando, "clear") == 0){
        return 6;
    }else if(strcmp(comando, "help") == 0){
        return 7;
    }else if(strcmp(comando, "exit") == 0){
        return 8;
    }else if(strcmp(comando, "..") == 0){
        return 9;
    }
    return 0;
}

void executar(Diretorio* dir){
    while(1){
        printf("%s ", dir->caminho); // Imprime o caminho atual
        char comando[256] = "";
        char comando_interno[256] ="";
        int pos;
        scanf(" %[^\n]", comando); // Lê o comando fornecido pelo usuário
        int i;
        for(i = 0; i < 256; i++){
            if(comando[i] != ' '){
                comando_interno[i] = comando[i];
            }else{
                cortarString(comando, i+1); // Corta a parte do comando após o espaço
                break;
            }
        }
        Diretorio* novoDiretorio;
        char tipo[20];
        char caminho[256];
        switch (verificarComando(comando_interno)) {
            case 1:
                novoDiretorio = cd(dir, comando); // Chama a função cd para mudar de diretório
                if(novoDiretorio != NULL) {
                    executar(novoDiretorio); // Executa o novo diretório retornado
                }
                break;

            case 2:
                search(dir, comando); // Chama a função search para pesquisar
                break;

            case 3:
                rm(dir, comando); // Chama a função rm para remover
                break;

            case 4:
                list(dir, 0, ""); // Chama a função list para listar
                break;

            case 5:
                printf("Você deseja criar uma pasta ou um arquivo?\n"
                       "1 - Arquivo\n"
                       "2 - Pasta\n"
                       "Escolha: ");
                scanf("%d", &i);
                if(i == 1){
                    printf("Informe o tipo: ");
                    scanf(" %[^\n]", tipo);
                    printf("Informe o nome do arquivo: ");
                    scanf(" %[^\n]", caminho);
                    mkdir(dir, caminho, tipo); // Chama a função mkdir para criar um diretório ou arquivo
                }else if(i == 2){
                    strcpy(tipo, "");
                    printf("Informe o nome da pasta: ");
                    scanf(" %[^\n]", caminho);
                    mkdir(dir, caminho, ""); // Chama a função mkdir para criar uma pasta
                }else {
                    printf("Comando inválido!\n");
                }
                break;

            case 6:
                system("cls"); // Limpa a tela
                break;

            case 7:
                help(); // Chama a função help para exibir ajuda
                break;

            case 8:
                rm(dir, dir->caminho); // Remove o diretório atual
                system("cls"); // Limpa a tela
                exit(1); // Sai do programa
                break;

            case 9:
                return; // Retorna para o diretório pai
                break;

            case 0:
                printf("Comando inválido! Utilize 'help' para obter os comandos.\n");
        }
        tipo[20] = '\0';
        caminho[256] = '\0';
        comando[0] = '\0';
        comando_interno[0] = '\0';
    }
}

void ler_txt(Diretorio* dirPai, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r"); // Abre o arquivo em modo de leitura
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) { // Lê cada linha do arquivo
        // Remover o caractere de quebra de linha no final da linha
        linha[strcspn(linha, "\n")] = '\0';

        char* token = strtok(linha, "/"); // Divide a linha em tokens usando '/' como delimitador
        Diretorio* dirAtual = dirPai;

        char caminho_atual[256] = "";
        strcat(caminho_atual, "C:"); // Caminho inicial
        while (token != NULL) {
            // Verificar se o token já existe como filho do diretório atual
            int encontrado = 0;
            Diretorio* filho = dirAtual->filho;
            strcat(caminho_atual, "/");
            strcat(caminho_atual, token);

            while (filho != NULL) {
                if (strcmp(filho->caminho,caminho_atual) == 0) {
                    encontrado = 1;
                    break;
                }
                filho = filho->irmao;
            }

            if (!encontrado) {
                // Criar um novo diretório ou arquivo
                char tipo[10] = "";
                if (strchr(token, '.') != NULL) {
                    // É um arquivo
                    strcpy(tipo, strchr(token, '.') + 1);
                    token[strcspn(token, ".")] = '\0';
                }

                mkdir(dirAtual, token, tipo); // Chama a função mkdir para criar um diretório ou arquivo
            }

            // Avançar para o próximo nível do diretório
            dirAtual = dirAtual->filho;
            while (dirAtual->irmao != NULL) {
                dirAtual = dirAtual->irmao;
            }
            token = strtok(NULL, "/"); // Obtém o próximo token
        }
        caminho_atual[0] = '\0'; // Reinicia o caminho atual
    }

    fclose(arquivo); // Fecha o arquivo
}


