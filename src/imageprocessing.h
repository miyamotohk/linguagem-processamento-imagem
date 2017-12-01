#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

typedef struct{
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *I);
void aplicar_brilho_lin(imagem *I, float valor);
void aplicar_brilho_thr(imagem *I, float valor, int n);
void aplicar_brilho_prc(imagem *I, float valor, int n);
void aplicar_brilho_col(imagem *I, float valor);
void mult_proc(imagem *I, float valor, int linha, int bloco, int N);
void valor_maximo(imagem *I);

#endif