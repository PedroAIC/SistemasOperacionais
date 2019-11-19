#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int globalQtdThreads = 0;
int iContArquivos, iArquivosLidos = 0, iContTotalLinhas = 0, iContNumber = 0, part = 0;
char sFileNames[100][100];
int *arrayNumbers;

void FileReader(char sFileName[100])
{
   FILE *fNumberFile;
   char buf[BUFSIZ];

   fNumberFile = fopen(sFileName, "r");

   // Contando a quantidade de linhas do arquivo
   while (!feof(fNumberFile))
   {
      if (fgets(buf, BUFSIZ, fNumberFile) != NULL)
      {
         iContTotalLinhas++;
      }
   }

   // Alocando o vetor com a quantidade de números que tem no arquivo
   if (iArquivosLidos == 0)
   {
      arrayNumbers = (int *)malloc(iContTotalLinhas * sizeof(int));
   }
   // Realocando o vetor com a quantidade que já existia mais a quantidade do novo arquivo
   else
   {
      arrayNumbers = (int *)realloc(arrayNumbers, iContTotalLinhas * sizeof(int));
   }

   // Alimentando o vetor com os números lidos no arquivo
   fseek(fNumberFile, 0, SEEK_SET);
   while (!feof(fNumberFile))
   {
      if (fgets(buf, BUFSIZ, fNumberFile) != NULL)
      {
         arrayNumbers[iContNumber] = atoi(buf);
         iContNumber++;
      }
   }

   iArquivosLidos++;
}

// merge function for merging two parts
void merge(int low, int mid, int high)
{
   int kSort, jSort, auxSort;
   int *left[mid - low + 1];
   int *right[high - mid];

   // n1 is size of left part and n2 is size
   // of right part
   int n1 = mid - low + 1, n2 = high - mid, i, j;

   // storing values in left part
   for (i = 0; i < n1; i++)
      left[i] = arrayNumbers[i + low];

   // storing values in right part
   for (i = 0; i < n2; i++)
      right[i] = arrayNumbers[i + mid + 1];

   int k = low;
   i = j = 0;

   // merge left and right in ascending order
   while (i < n1 && j < n2)
   {
      if (left[i] <= right[j])
         arrayNumbers[k++] = left[i++];
      else
         arrayNumbers[k++] = right[j++];
   }  

   // insert remaining values from left
   while (i < n1)
   {
      arrayNumbers[k++] = left[i++];
   }

   // insert remaining values from right
   while (j < n2)
   {
      arrayNumbers[k++] = right[j++];
   }

   for (kSort = low; kSort <= high; kSort++)
   {
      for (jSort = low; jSort <= high - 1; jSort++)
      {
         if (arrayNumbers[jSort] > arrayNumbers[jSort + 1])
         {
            auxSort                 = arrayNumbers[jSort];
            arrayNumbers[jSort]     = arrayNumbers[jSort + 1];
            arrayNumbers[jSort + 1] = auxSort;
         }
      }
   }
}

// merge sort function
void merge_sort1(int low, int high)
{
   // calculating mid point of array
   int mid = low + (high - low) / 2;
   // printf("Low - %d Mid 2 - %d - High - %d\n", low, mid, high);
   if (low < high)
   {
      // calling first half
      merge_sort1(low, mid);

      // calling second half
      merge_sort1(mid + 1, high);

      // merging the two halves
      merge(low, mid, high);
   }
}

// thread function for multi-threading
void *merge_sort(void *arg)
{
   // which part out of thread parts
   int thread_part = part++;
   int comparador = part;

   int low, high = 0;
   int qtdLinhasRestantes = (iContTotalLinhas % globalQtdThreads);
   if ((qtdLinhasRestantes) != 0)
   {
      // calculating low and high
      if (comparador == globalQtdThreads)
      {
         low = thread_part * (iContTotalLinhas / globalQtdThreads);
         high = (thread_part + 1) * (iContTotalLinhas / globalQtdThreads) - 1 + qtdLinhasRestantes;
      }
      else
      {
         low = thread_part * (iContTotalLinhas / globalQtdThreads);
         high = (thread_part + 1) * (iContTotalLinhas / globalQtdThreads) - 1;
      }
   }
   else
   {
      // calculating low and high
      low = thread_part * (iContTotalLinhas / globalQtdThreads);
      high = (thread_part + 1) * (iContTotalLinhas / globalQtdThreads) - 1;
   }
   
   // evaluating mid point
   int mid = low + (high - low) / 2;
   if (low < high)
   {
      merge_sort1(low, mid);
      merge_sort1(mid + 1, high);
      merge(low, mid, high);
   }

   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   // Quantidade de threads passada por linha de comando
   char *sQtdThreads = argv[1];
   int iQtdThreads = atoi(sQtdThreads);
   pthread_t threads[iQtdThreads];
   int rc;
   globalQtdThreads = iQtdThreads;

   // Quantidade de arquivos passada por linha de comando. É necessário subtrair 4 para desconsiderar os dois primeiros e os dois últimos
   iContArquivos = argc - 4;
   // Arquivo de saída
   FILE *fFinalFile;
   // Contador de tempo
   clock_t ctTempoInicial, ctTempoFinal;

   // Leitura dos arquivos e armazenando os valores inteiros no vetor arrayNumbers
   for (int nI = 0; nI < iContArquivos; nI++)
   {
      strcpy(sFileNames[nI], argv[nI + 2]);
      // printf("%s\n", sFileNames[nI]);
      FileReader(sFileNames[nI]);
   }

   // Criando as threads
   ctTempoInicial = clock();
   for (int nI = 0; nI < iQtdThreads; nI++)
   {
      rc = pthread_create(&threads[nI], NULL, merge_sort, (void *)sFileNames);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   // Fazendo join das threads para ordenar em uma sequência correta.
   for (int i = 0; i < iQtdThreads; i++)
      pthread_join(threads[i], NULL);

   // Ordenando os valores num intervalo de valores pequenos
   merge(0, (iContTotalLinhas / 2 - 1) / 2, iContTotalLinhas / 2 - 1);
   // Ordenando os valores num intervalo de valores médios
   merge(iContTotalLinhas / 2, iContTotalLinhas / 2 + (iContTotalLinhas - 1 - iContTotalLinhas / 2) / 2, iContTotalLinhas - 1);
   // Ordenando os valores num intervalo de valores grandes
   merge(0, (iContTotalLinhas - 1) / 2, iContTotalLinhas - 1);

   ctTempoFinal = clock();

   // Salvando em um arquivo de saída
   fFinalFile = fopen(argv[argc - 1], "w");

   for (int i = 0; i < iContTotalLinhas; i++)
      fprintf(fFinalFile, "%d\n", arrayNumbers[i]);

   printf("Tempo Decorrido : %f\n", (double)(ctTempoFinal - ctTempoInicial) / CLOCKS_PER_SEC);

   // Finalizando a thread
   pthread_exit(NULL);
}
