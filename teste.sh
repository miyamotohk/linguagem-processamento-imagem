#!/bin/bash
echo "INICIO DOS TESTES"

make

echo "
***Comparação entre diferentes números de threads e processos***"
echo "MULTITHREADS"
i=1
max=1000
while [ $i -lt $max ]
do
    echo "Multithreads com $i threads"
    echo "teste.jpg = ssa.jpg *2 thr $i" | ./main
    true $(( i+=100 ))
done

echo "
MULTIPROCESSOS"
for j in `seq 10`
do
    echo "Multiprocessos com $j processos"
    echo "teste.jpg = ssa.jpg *2 prc $j" | ./main
done

echo "
***Comparação entre métodos diferentes em diferentes tamanhos***"
echo "IMAGEM PEQUENA"
echo "teste.jpg = dora.jpg *2 thr 800" | ./main
echo "teste.jpg = dora.jpg *2 prc 1" | ./main
echo "teste.jpg = dora.jpg *2 lin" | ./main
echo "teste.jpg = dora.jpg *2 col" | ./main

echo "
"
echo "IMAGEM MEDIA"
echo "teste.jpg = ssa.jpg *2 thr 800" | ./main
echo "teste.jpg = ssa.jpg *2 prc 1" | ./main
echo "teste.jpg = ssa.jpg *2 lin" | ./main
echo "teste.jpg = ssa.jpg *2 col" | ./main

echo "
"
echo "IMAGEM GRANDE"
echo "teste.jpg = demo.jpg *2 thr 800" | ./main
echo "teste.jpg = demo.jpg *2 prc 2" | ./main
echo "teste.jpg = demo.jpg *2 lin" | ./main
echo "teste.jpg = demo.jpg *2 col" | ./main

echo "
FIM"
exit