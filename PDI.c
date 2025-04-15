#include <stdio.h>
#include <stdlib.h>

FILE *imgPrincipal, *imgMonoSimples, *imgMonoPonderada, *imgNegativa;
int lin, col, quantZ, **matR, **matG, **matB;

char *mudar_extensao(char arg[], int num)
{
  int sizeArg = 0;
  for (int i = 0; arg[i] != '\0'; i++)
    sizeArg++;
  char *name = (char *)malloc(sizeArg + 2);
  char *nvName = (char *)malloc(sizeArg + 2);

  if (nvName == NULL || name == NULL)
  {
    printf("----------------------------------------\n");
    printf("ERRO AO ALOCAR MEMORIA\n");
    printf("----------------------------------------\n");
    exit(1);
  }
  for (int i = 0; i <= sizeArg; i++)
    name[i] = arg[i];

  for (int i = 0; i < sizeArg; i++)
  {
    nvName[i] = name[i];
    if (name[i + 1] == '.')
    {
      if (num == 1)
      {
        nvName[i + 1] = '1';
        nvName[i + 2] = '.';
        nvName[i + 3] = 'p';
        nvName[i + 4] = 'g';
        nvName[i + 5] = 'm';
        nvName[i + 6] = '\0';
      }
      else if (num == 2)
      {
        nvName[i + 1] = '2';
        nvName[i + 2] = '.';
        nvName[i + 3] = 'p';
        nvName[i + 4] = 'g';
        nvName[i + 5] = 'm';
        nvName[i + 6] = '\0';
      }
      else
      {
        nvName[i + 1] = '3';
        nvName[i + 2] = '.';
        nvName[i + 3] = 'p';
        nvName[i + 4] = 'p';
        nvName[i + 5] = 'm';
        nvName[i + 6] = '\0';
      }
      break;
    }
  }
  return nvName;
}

void abrir_imagem(int argc, char *argv[])
{
  char *nvName;
  if (argc <= 1)
  {
    printf("----------------------------------------\n");
    printf("USO ERRADO DO PROGAMA!\n");
    printf("DIGITE <executavel> e <endereco imagem>\n");
    printf("----------------------------------------\n");
    exit(1);
  }
  if ((imgPrincipal = fopen(argv[1], "r")) == NULL)
  {
    printf("--------------------------------------------\n");
    printf("NAO FOI POSSIVEL ABRIR O ARQUIVO DA IMAGEM!\n");
    printf("--------------------------------------------\n");
    exit(1);
  }
  nvName = mudar_extensao(argv[1], 1);
  if ((imgMonoSimples = fopen(nvName, "w")) == NULL)
  {
    printf("--------------------------------------------------------------------\n");
    printf("NAO FOI POSSIVEL CRIAR O ARQUIVO DA IMAGEM MONOCROMATICA SIMPLES!\n");
    printf("--------------------------------------------------------------------\n");
    exit(1);
  }
  nvName = mudar_extensao(argv[1], 2);
  if ((imgMonoPonderada = fopen(nvName, "w")) == NULL)
  {
    printf("------------------------------------------------------\n");
    printf("NAO FOI POSSIVEL CRIAR O ARQUIVO DA IMAGEM NEGATIVA!\n");
    printf("------------------------------------------------------\n");
    exit(1);
  }
  nvName = mudar_extensao(argv[1], 3);
  if ((imgNegativa = fopen(nvName, "w")) == NULL)
  {
    printf("-----------------------------------------------------------------------\n");
    printf("NAO FOI POSSIVEL CRIAR O ARQUIVO DA IMAGEM MONOCROMATICA PONDERADA!\n");
    printf("-----------------------------------------------------------------------\n");
    exit(1);
  }
  free(nvName);
}

void ler_header()
{
  char tipo[3];
  fscanf(imgPrincipal, "%s\n", tipo);
  fscanf(imgPrincipal, "%d\n", &lin);
  fscanf(imgPrincipal, "%d\n", &col);
  fscanf(imgPrincipal, "%d\n", &quantZ);
}

void alocar_matrizes_RGB()
{
  matR = (int **)malloc(lin * sizeof(int *));
  matG = (int **)malloc(lin * sizeof(int *));
  matB = (int **)malloc(lin * sizeof(int *));
  if (matR == NULL || matG == NULL || matB == NULL)
  {
    printf("-----------------------------\n");
    printf("ERRO AO ALOCAR MEMORIA\n");
    printf("-----------------------------\n");
    exit(1);
  }
  for (int i = 0; i < lin; i++)
  {
    matR[i] = (int *)malloc(col * sizeof(int));
    matG[i] = (int *)malloc(col * sizeof(int));
    matB[i] = (int *)malloc(col * sizeof(int));
    if (matR == NULL || matG == NULL || matB == NULL)
    {
      printf("-----------------------------\n");
      printf("ERRO AO ALOCAR MEMORIA\n");
      printf("-----------------------------\n");
      exit(1);
    }
  }
}

void preencher_matrizes_RGB()
{
  for (int i = 0; i < lin; i++)
  {
    for (int j = 0; j < col; j++)
    {
      fscanf(imgPrincipal, "%d", &matR[i][j]);
      fscanf(imgPrincipal, "%d", &matG[i][j]);
      fscanf(imgPrincipal, "%d", &matB[i][j]);
    }
  }
}

void gerar_mono_simples()
{
  int soma = 0;
  fprintf(imgMonoSimples, "P2\n");
  fprintf(imgMonoSimples, "%d %d\n", lin, col);
  fprintf(imgMonoSimples, "%d\n", quantZ);
  for (int i = 0; i < lin; i++)
  {
    for (int j = 0; j < col; j++)
    {
      soma = (matR[i][j] + matG[i][j] + matB[i][j]) / 3;
      fprintf(imgMonoSimples, "%d\n", soma);
    }
  }
}

void gerar_mono_ponderada()
{
  int soma = 0;
  fprintf(imgMonoPonderada, "P2\n");
  fprintf(imgMonoPonderada, "%d %d\n", lin, col);
  fprintf(imgMonoPonderada, "%d\n", quantZ);
  for (int i = 0; i < lin; i++)
  {
    for (int j = 0; j < col; j++)
    {
      soma = (int)((matR[i][j] * 0.299) + (matG[i][j] * 0.587) + (matB[i][j] * 0.114));
      fprintf(imgMonoPonderada, "%d\n", soma);
    }
  }
}

void gerar_negativa()
{
  fprintf(imgNegativa, "P3\n");
  fprintf(imgNegativa, "%d\n", lin);
  fprintf(imgNegativa, "%d\n", col);
  fprintf(imgNegativa, "%d\n", quantZ);
  for (int i = 0; i < lin; i++)
  {
    for (int j = 0; j < col; j++)
    {
      matR[i][j] = 255 - matR[i][j];
      matG[i][j] = 255 - matG[i][j];
      matB[i][j] = 255 - matB[i][j];
    }
  }
  for (int i = 0; i < lin; i++)
  {
    for (int j = 0; j < col; j++)
    {
      fprintf(imgNegativa, "%d\n", matR[i][j]);
      fprintf(imgNegativa, "%d\n", matG[i][j]);
      fprintf(imgNegativa, "%d\n", matB[i][j]);
    }
  }
}

void liberar_memoria()
{
  for (int i = 0; i < lin; i++)
  {
    free(matR[i]);
    free(matG[i]);
    free(matB[i]);
  }
  free(matR);
  free(matG);
  free(matB);
}

void fechar_arquivos()
{
  fclose(imgPrincipal);
  fclose(imgMonoSimples);
  fclose(imgMonoPonderada);
  fclose(imgNegativa);
}

int main(int argc, char *argv[])
{
  abrir_imagem(argc, argv);
  ler_header();
  alocar_matrizes_RGB();
  preencher_matrizes_RGB();
  gerar_mono_simples();
  gerar_mono_ponderada();
  gerar_negativa();
  liberar_memoria();
  fechar_arquivos();
  return 0;
}