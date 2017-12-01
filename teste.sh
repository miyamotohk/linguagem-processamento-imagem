#!/bin/bash
echo "INICIO DOS TESTES"

make

# Testes para a imagem pequena
echo "
"
echo "IMAGEM PEQUENA"
echo "teste.jpg = dora.jpg *2 thr" | ./main
echo "teste.jpg = dora.jpg *2 prc" | ./main
echo "teste.jpg = dora.jpg *2 lin" | ./main
echo "teste.jpg = dora.jpg *2 col" | ./main

echo "
"
echo "IMAGEM MEDIA"
echo "teste.jpg = ssa.jpg *2 thr" | ./main
echo "teste.jpg = ssa.jpg *2 prc" | ./main
echo "teste.jpg = ssa.jpg *2 lin" | ./main
echo "teste.jpg = ssa.jpg *2 col" | ./main

echo "
"
echo "IMAGEM GRANDE"
echo "teste.jpg = demo.jpg *2 thr" | ./main
echo "teste.jpg = demo.jpg *2 prc" | ./main
echo "teste.jpg = demo.jpg *2 lin" | ./main
echo "teste.jpg = demo.jpg *2 col" | ./main

echo "
FIM"
exit