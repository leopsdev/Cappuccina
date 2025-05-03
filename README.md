# Cappucinna

## Descrição
A cappucinna é uma linguagem de programação para a matéria de compiladores, onde tal linguagem teria uma estruturação baseada nas funções primitivas e μ-recursivas, proporcionando que estudantes utilizem essa linguagem como ferramenta pedagógica para explorar de forma prática os conceitos teóricos vivenciados em sala de aula, ao passo que sua construção auxiliará na compreensão de conceitos presentes na construção de um compilador.

## Compilar e usar :)

<pre>
bison -d parser.y        # Gera parser.tab.c e parser.tab.h
flex scanner.l           # Gera lex.yy.c 
gcc parser.tab.c lex.yy.c aux.c -o linguagem -lfl
./linguagem < teste.txt
</pre>
