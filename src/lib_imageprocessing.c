#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <FreeImage.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "imageprocessing.h"

#define n_threads 3000
#define n_processos 5

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
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
   //printf("Arquivo lido!\n");
  return I;

}

void liberar_imagem(imagem *I) {
	munmap(I->r, sizeof(float));
	munmap(I->g, sizeof(float));
	munmap(I->b, sizeof(float));
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  printf("Salvando imagem %d por %d...\n", I->width, I->height);
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
  //printf("Imagem salva!\n");
}

//Aplicacao de brilho varrendo as colunas
void aplicar_brilho_col(imagem *I, float valor) {

  struct timeval t1, t2;
  double time_taken;
  gettimeofday(&t1,NULL);

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);

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

  gettimeofday(&t2,NULL);

  time_taken = (t2.tv_sec - t1.tv_sec)*1000.0; //s para ms
  time_taken += (t2.tv_usec - t1.tv_usec)/1000.0; //us para ms

  printf("Procedimento por colunas realizado em %f ms.\n", time_taken);
}

//Aplicacao de brilho por linhas
void aplicar_brilho_lin(imagem *I, float valor) {

  struct timeval t1, t2;
  double time_taken;
  gettimeofday(&t1,NULL);


  for (int i=0; i<I->height; i++) {
     for (int j=0; j<I->width; j++) {
      int idx;

      idx = i + (j*I->height);

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

  gettimeofday(&t2,NULL);

  time_taken = (t2.tv_sec - t1.tv_sec)*1000.0; //s para ms
  time_taken += (t2.tv_usec - t1.tv_usec)/1000.0; //us para ms

  printf("Procedimento por linhas realizado em %f ms.\n", time_taken);
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
  struct timeval t1, t2;
  double time_taken;
  gettimeofday(&t1,NULL);

  //Varre a matriz por linhas
  for (int i = 0; i < I->height; i++) {
  	//Quantidade de pixels em cada thread
    int N = ((I->width)/n_threads);
    float Nx = ((float)(I->width)/n_threads);
        if(N < Nx)	N++;
    	//Varre os blocos de pixels de tamanho n_threads
        for(int j = 0; j < N; j++){
            struct Argumentos *thread_args = (struct Argumentos *) malloc(sizeof(struct Argumentos));
                        
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

  gettimeofday(&t2,NULL);

  time_taken = (t2.tv_sec - t1.tv_sec)*1000.0; //s para ms
  time_taken += (t2.tv_usec - t1.tv_usec)/1000.0; //us para ms

  printf("Procedimento com multithreads realizado em %0.4f ms.\n", time_taken);
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
  struct timeval t1, t2;
  double time_taken;
  gettimeofday(&t1,NULL);

  //Varre a imagem por linhas
  for(int i = 0; i < I->height; i++){
  	//Quantidade de pixels por processo
	int N = ((I->width)/n_processos);
	float Nx = ((float)(I->width)/n_processos);
	if(N < Nx)	N++;
	//Varre os blocos de processos
    for(int j = 0; j < n_processos; j++){
        mult_proc(I, valor, i, j, N);
    }
  }
  wait(NULL);

  gettimeofday(&t2,NULL);

  time_taken = (t2.tv_sec - t1.tv_sec)*1000.0; //s para ms
  time_taken += (t2.tv_usec - t1.tv_usec)/1000.0; //us para ms

  printf("Procedimento com multiprocessos realizado em %0.4f ms.\n", time_taken);
}

void valor_maximo(imagem *I){
  float maximo = 0;

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;
      float intensidade;

      idx = i + (j*I->width);

      intensidade = (I->r[idx] + I->g[idx] + I->b[idx])/3.0;
      if(intensidade > maximo)
        maximo = intensidade;
    }
  }
  printf("Intensidade maxima: %.2f\n", maximo);
}