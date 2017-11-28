#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

typedef struct{
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
<<<<<<< HEAD
void aplicar_brilho_col(imagem *I, float valor);
=======
>>>>>>> bea6c2046cd625d0d456b30be31e483ae150256d
void aplicar_brilho_lin(imagem *I, float valor);
void aplicar_brilho_thr(imagem *I, float valor);
void mult_proc(imagem *I, float valor, int linha, int bloco, int N);
void aplicar_brilho_prc(imagem *I, float valor);
void aplicar_brilho_col(imagem *I, float valor);
void valor_maximo(imagem *I);

#endif