//projeto imagens

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


//Aumentando brilho
void aumentarBrilho(int** original, int linhas, int colunas, int maxValor)
{   int l, c;
    for(l=0; l<linhas; l++)
    {    for(c=0; c<colunas; c++)
        {    original[l][c] = (int)(original[l][c] * 1.2);
            if(original[l][c] > maxValor)
                original[l][c] = maxValor;
        }
    }
}

//Diminuir brilho
void diminuirBrilho(int** original, int linhas, int colunas)
{   int l, c;
    for(l=0; l<linhas; l++)
    { for(c=0; c<colunas; c++)
        {         original[l][c]= (int)(original[l][c]*0.8);
        }
    }
}

//Inserir moldura
void moldura (int** original, int linhas, int colunas, int MaxValor)
{   int l, c;
    //Moldura Superior
    for(l=0; l<0.1*linhas; l++)
    { for(c=0; c<colunas; c++){
        original[l][c] = 255; //moldura branca
        }
    }
    //Moldura Esquerda
    for(l=0; l<linhas; l++)
    { for(c=0; c<0.1*colunas; c++){
        original[l][c] = 255; //moldura branca
        }
    }
    //Moldura Inferior
    for(l=0.9*linhas; l<linhas; l++)
       { for(c=0; c<colunas; c++){
           original[l][c] = 255; //moldura branca
           }
       }
    //Moldura Direita
    for(l=0; l<linhas; l++)
    { for(c=0.9*colunas; c<colunas; c++){
           original[l][c] = 255; //moldura branca
           }
       }
}

/* Exemplo mais enxuto de moldura
 
 if(l<0.1*linhas || l>0.9*linhas ||c<0.1*colunas||c>0.9*colunas)
 original[l][c] = 0;
 
 OU
 
 void moldura(int** original, int linhas, int colunas){
    int l, c;
        for(l=0; l<linhas; l++){
            for(c=0; c<colunas; c++){
                if(l<linhas*0.1 || l>linhas-linhas*0.1
                    || c<colunas*0.1 || c>colunas-colunas*0.1)
                original[l][c]=0; // cor preta
            }
        }
    }
 */

//Espelhamento horizontal
void espelhamentoHorizontal (int** original, int linhas, int colunas){
    int l, c, c2;
    for (l=0; l<linhas; l++){
        c2 = colunas -1;
        for (c=0; c<colunas/2; c++){
            int aux = original [l][c];
            original[l][c] = original[l][c2];
            original[l][c2] = aux;
            c2--;
        }
    }
}

//Espelhamento vertical
void espelhamentoVertical (int** original, int linhas, int colunas){
    int l, l2, c;
    for (l=0; l<linhas/2; l++){
        l2 = linhas -1;
        for (c=0; c<colunas; c++){
            int aux = original [l][c];
            original[l][c] = original[l][l2];
            original[l][l2] = aux;
            l2--;
        }
    }
}

//Desfoque (borrar)
void borrar(int** original, int linhas, int colunas, int tamanhoBorrao)
{   int l, c;
    //para cada pixel da imagem calculo a média aritmética do seu valor com o dos pixels vizinhos
    for(l=1; l<linhas-1; l++){
        for(c=1; c<colunas-1; c++){
            original[l][c] = (original[l][c] + original[l-1][c] + original[l+1][c] + original[l][c-1] + original[l][c+1] + original[l-1][c-1] + original[l+1][c+1] + original[l-1][c+1] + original[l+1][c-1]) / 9;
        }
    }
}

//Aumentar contraste
/* exemplo:
void aumentarContraste(int** original, int linhas, int colunas, int maxValor)
{   int l, c;
    for(l=0; l<linhas; l++){
        for(c=0; c<colunas; c++){
            if (original[l][c] > 150) original[l][c]= (int)(original[l][c]*1.2);
            else if (original[l][c] > 138) original[l][c] = (int)(original[l][c]*1.1);
            else if (original[l][c] > 127) original[l][c] = (int)(original[l][c]*1.05);
            else if (original[l][c] > 116) original[l][c] = (int)(original[l][c]*0.95);
            else if (original[l][c] > 100) original[l][c] = (int)(original[l][c]*0.9);
            else original[l][c]= (int)(original[l][c]*0.8);
            if(original[l][c] > maxValor) original[l][c] = maxValor;
        }
    }
}*/
void aumentarContraste(int** original, int linhas, int colunas, int maxValor){
    int l, c;
    for (l=0; l<linhas; l++){
        for (c=0; c<colunas; c++){
            original[l][c] = maxValor / (1.0 + exp(-(10.0 * original[l][c] / (float)maxValor - 5.0)));
        }
    }
}

//Diminuir contraste
void diminuirContraste(int** original, int linhas, int colunas, int maxValor){
    int l, c;
    for (l=0; l<linhas; l++){
        for (c=0; c<colunas; c++){
            original[l][c] = maxValor / (1.0 + exp(-(2.0 * original[l][c] / (float)maxValor - 1.0)));
        }
    }
}

int** lerImagem(char * nomeArquivo, int *pLinhas, int *pColunas, int *pMaxValor)
{   FILE *fp;
    printf("Tentando abrir o arquivo %s\n", nomeArquivo);
    fp = fopen(nomeArquivo,"r");
    /* Arquivo ASCII, para leitura */
    if(!fp)
    {   printf( "\nErro na abertura do arquivo\n\n");
        exit(-1);
    }

    //leia tipo do arquivo
    char buffer[1001];
    fgets (buffer, 1000, fp); //Primeira linha
    if(strstr(buffer, "P2") == NULL) // o tipo de arquivo eh diferente de P2?
    {   printf( "\nErro no tipo do arquivo\n\n");
        exit(-2);
    }

    //leia comentario
    fgets (buffer, 1000, fp);
    
    //leia dados da imagem
    fscanf(fp, "%d%d%d", pColunas, pLinhas, pMaxValor);
    
    //criando a matriz
    int **mat, i;
    mat = (int **) malloc(*pLinhas * sizeof(int *));
    for(i=0; i< *pLinhas; i++)
        mat[i] = (int *) malloc(*pColunas * sizeof(int));

    int l, c;
    for(l=0; l<*pLinhas; l++)
    {    for(c=0; c<*pColunas; c++)
            fscanf(fp, "%d", &mat[l][c]);
    }
    fclose(fp);
    return mat;
}

void escreverImagem(char * nomeArquivo, int ** mat, int linhas, int colunas, int maxValor)
{    FILE *fp;
    fp = fopen(nomeArquivo,"w");
    // Arquivo ASCII, para leitura
    if(!fp)
    {    printf( "\nErro na abertura do arquivo\n\n");
        exit(-1);
    }

    //escreva tipo do arquivo
    fprintf (fp, "P2\n");
    //escreva comentario
    fprintf (fp, "#Figura modificada...\n");
    //colunas, linhas
    fprintf(fp, "%d %d\n", colunas, linhas);
    //maxValor
    fprintf(fp, "%d\n", maxValor);

    int l, c;
    for(l=0; l<linhas; l++)
    {    for(c=0; c<colunas; c++)
        {    fprintf(fp, "%d ", mat[l][c]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(int argc, char * argv[])
{    char opcao[10]="0";
    int linhas=0, colunas=0, maxValor=0, **mat=NULL;
    int tamanhoBorrao = 8;
    char nomeArquivo[100]="";
    char nomeArquivoLeitura[100]="";
    char nomeArquivoEscrita[100]="";
    while(opcao[0] != '9')
    {   printf("\n\nMini-photoshop\n\n");
        printf("1) Ler imagem\n");
        printf("2) Gravar imagem\n");
        printf("3) Aumentar o contraste\n");
        printf("4) Diminuir o contraste\n");
        printf("5) Espelhamento horizontal\n");
        printf("6) Espelhamento vertical\n");
        printf("7) Desfocar\n");
        printf("8) Fazer moldura\n");
        printf("9) Sair\n");
        printf("\nEntre a opcao desejada: ");
        fgets(opcao, 10, stdin);
        switch(opcao[0])
        {    case '1':
                printf("\n\nEntre com o nome da imagem (sem extensao): ");
                fgets(nomeArquivo, 100, stdin);
                nomeArquivo[strlen(nomeArquivo)-1]='\0';
                strcpy (nomeArquivoLeitura, nomeArquivo);
                strcat (nomeArquivoLeitura, ".pgm");
                mat = lerImagem(nomeArquivoLeitura, &linhas, &colunas, &maxValor);
                break;
            case '2':
                strcpy (nomeArquivoEscrita, nomeArquivo);
                strcat (nomeArquivoEscrita, "_editada.pgm");
                printf("\n\nA imagem sera salva como %s\n", nomeArquivoEscrita);
                escreverImagem(nomeArquivoEscrita, mat, linhas, colunas, maxValor);
                break;
            case '3':
                aumentarContraste(mat, linhas, colunas, maxValor);
                break;
            case '4':
                diminuirContraste(mat, linhas, colunas, maxValor);
                break;
            case '5':
                espelhamentoHorizontal(mat, linhas, colunas);
                break;
            case '6':
                espelhamentoVertical(mat, linhas, colunas);
                break;
            case '7':
                borrar(mat, linhas, colunas, tamanhoBorrao);
                break;
            case '8':
                moldura(mat, linhas, colunas, maxValor);
                break;
        }
    }
    return 0;
}
