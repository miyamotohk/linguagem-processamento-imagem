#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <FreeImage.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "imageprocessing.h"

#define n_threads 1000
#define n_processos 2

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

int controle = 0;

struct Argumentos{
  int posicao;
  float valor;
  imagem * I;
  int qtd;
};

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  //double valor_max_red = 0, valor_max_green = 0, valor_max_blue = 0;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    //printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  
  // I.r = malloc(sizeof(float) * x * y);
  // I.g = malloc(sizeof(float) * x * y);
  // I.b = malloc(sizeof(float) * x * y);

  //Aloca a imagem como memoria compartilhada
  I.r = (float*) mmap(NULL, sizeof(float)*x*y, PROT_WRITE|PROT_READ, MAP_SHARED | MAP_ANON, 0, 0);
  I.g = (float*) mmap(NULL, sizeof(float)*x*y, PROT_WRITE|PROT_READ, MAP_SHARED | MAP_ANON, 0, 0);
  I.b = (float*) mmap(NULL, sizeof(float)*x*y, PROT_WRITE|PROT_READ, MAP_SHARED | MAP_ANON, 0, 0);


   for (int i=0; i<x; i++) {
     for (int j=0; j<y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      //if(I.r[idx] > valor_max_red){
        //(valor_max_red = I.r[idx]);
      //}
      I.g[idx] = color.rgbGreen;
      //if(I.g[idx] > valor_max_green){
        //(valor_max_green = I.g[idx]);
      //}
      I.b[idx] = color.rgbBlue;
      //if(I.b[idx] > valor_max_blue){
        //(valor_max_blue = I.b[idx]);
      //}
    }
   }
   /*printf("Valor máximo do bit vermelho: %lf.\n", valor_max_red);
   printf("Valor máximo do bit verde: %lf.\n", valor_max_green);
   printf("Valor máximo do bit azul: %lf.\n", valor_max_blue);*/
   printf("Arquivo lido!\n");
  return I;

}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  //printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx];
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
  printf("Imagem salva!\n");
}

<<<<<<< HEAD
//Aplicacao de brilho por colunas
=======
//Aplicacao de brilho varrendo as colunas
>>>>>>> bea6c2046cd625d0d456b30be31e483ae150256d
void aplicar_brilho_col(imagem *I, float valor) {

  clock_t t;
  t = clock();

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      //printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      I->r[idx] = I->r[idx] * valor;
      if(I->r[idx] > 255)
        I->r[idx] = 255;
      I->g[idx] = I->g[idx] * valor;
      if(I->g[idx] > 255)
        I->g[idx] = 255;
      I->b[idx] = I->b[idx] * valor;
      if(I->b[idx] > 255)
        I->b[idx] = 255;
    }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento original realizado em %f segundos.\n", time_taken);
}

//Aplicacao de brilho por linhas
void aplicar_brilho_lin(imagem *I, float valor) {

  clock_t t;
  t = clock();

  for (int i=0; i<I->height; i++) {
     for (int j=0; j<I->width; j++) {
      int idx;

      idx = i + (j*I->height);
      //printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      I->r[idx] = I->r[idx] * valor;
      if(I->r[idx] > 255)
        I->r[idx] = 255;
      I->g[idx] = I->g[idx] * valor;
      if(I->g[idx] > 255)
        I->g[idx] = 255;
      I->b[idx] = I->b[idx] * valor;
      if(I->b[idx] > 255)
        I->b[idx] = 255;
    }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento por colunas realizado em %f segundos.\n", time_taken);
}

//Aplicacao de brilho varrendo as linhas
void aplicar_brilho_lin(imagem *I, float valor) {

  clock_t t;
  t = clock();

  for (int i=0; i<I->height; i++) {
     for (int j=0; j<I->width; j++) {
      int idx;

      idx = j + (i*I->width);
      
      I->r[idx] = I->r[idx] * valor;
      if(I->r[idx] > 255)
      	I->r[idx] = 255;
      I->g[idx] = I->g[idx] * valor;
      if(I->g[idx] > 255)
      	I->g[idx] = 255;
      I->b[idx] = I->b[idx] * valor;
      if(I->b[idx] > 255)
      	I->b[idx] = 255;
    }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento por linhas realizado em %f segundos.\n", time_taken);
}

//Aplicacao de brilho com multithread
void* mult_thread(void *arg) {
  int k;
  controle++;
  
  struct Argumentos *thread_args = arg;
  
  int N = thread_args->qtd;
  float valor = thread_args->valor;
  imagem *I = thread_args->I;
  int pos = thread_args->posicao;
  //Varre os pixels dentro de cada bloco
    for(k=(thread_args->posicao); k<((thread_args->posicao) + n_threads); k++){
        if(k < (I->width)*(I->height)){
            if(controle%N == 0 && k > (controle/N)*I->width){
                //do nothing
            }
            else{
                I->r[k] = min(I->r[k] * valor, 255);
                I->g[k] = min(I->g[k] * valor, 255);
                I->b[k] = min(I->b[k] * valor, 255);
            }
        }
    }
  return NULL;
}

void aplicar_brilho_thr(imagem *I, float valor) {
  clock_t t;
  t = clock();

  //Varre a matriz por linhas
  for (int i = 0; i < I->height; i++) {
    int N = ((I->width)/n_threads);	//Quantidade de pixels em cada thread
    float Nx = ((float)(I->width)/n_threads);
        if(N < Nx)	N++;
    	//Varre os blocos de pixels de tamanho n_threads
        for(int j = 0; j < N; j++){
            struct Argumentos *thread_args = (struct Argumentos *) malloc(sizeof(struct Argumentos));
            
            //printf("i=%d, j=%d, posicao=%d\n",i,j,i*I->width + (j*n_threads));
            
            thread_args->posicao = i*I->width + (j*n_threads);
            thread_args->valor = valor;
            thread_args->I = I;
            thread_args->qtd = N;
            
            pthread_t threads;
            
            pthread_create(&(threads), NULL, mult_thread, thread_args);
            
            //Esperando threads terminarem!
            pthread_join(threads, NULL);
           
        }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento com multithreads realizado em %f segundos.\n", time_taken);
}

//Aplicacao de brilho com multiprocessos
void mult_proc(imagem *I, float valor, int linha, int bloco, int N) {
	pid_t filho;
	filho = fork();

	if (filho == 0) {
		//Codigo que executa no processo filho
		int fim = min(I->width, (bloco+1)*N);
		int i = linha;
		int j = bloco*N;

		for (j; j < fim; j++){
			 int k = i + (j*I->height);
			 I->r[k] = min(I->r[k] * valor, 255);
			 I->g[k] = min(I->g[k] * valor, 255);
			 I->b[k] = min(I->b[k] * valor, 255);
		}

		exit(EXIT_SUCCESS);
	}
	else {
		//printf("Filho com PID %d\n", filho);
	}
}

void aplicar_brilho_prc(imagem *I, float valor) {
  clock_t t;
  t = clock();

  //Varre a imagem por linhas
  for(int i = 0; i < I->height; i++){
	int N = ((I->width)/n_processos);	//Quantidade de pixels por processo
	float Nx = ((float)(I->width)/n_processos);
	if(N < Nx)	N++;
	//Varre os blocos de processos
    for(int j = 0; j < n_processos; j++){
        mult_proc(I, valor, i, j, N);
    }
  }
  wait(NULL);

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento com multiprocessos realizado em %f segundos.\n", time_taken);
}

void valor_maximo(imagem *I){
  float maximo = 0;

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;
      float intensidade;

      idx = i + (j*I->width);
      //printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      intensidade = (I->r[idx] + I->g[idx] + I->b[idx])/3.0;
      if(intensidade > maximo)
        maximo = intensidade;
    }
  }
  printf("Intensidade maxima: %.2f\n", maximo);
}