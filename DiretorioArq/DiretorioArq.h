struct diretorio{
    char caminho[256];
    char tipo[10];
    struct diretorio* irmao;
    struct diretorio* filho;
};
typedef struct diretorio Diretorio;

Diretorio* cria_usuario();
void executar(Diretorio* dir);
void registra_arquivo(Diretorio* dir, FILE* arquivo);
