# Cappuccina

## Descrição
A cappucinna é uma linguagem de programação para a matéria de compiladores, onde tal linguagem teria uma estruturação baseada nas funções primitivas e μ-recursivas, proporcionando que estudantes utilizem essa linguagem como ferramenta pedagógica para explorar de forma prática os conceitos teóricos vivenciados em sala de aula, ao passo que sua construção auxiliará na compreensão de conceitos presentes na construção de um compilador.

Para saber mais sobre a construção da Cappuccina acesse nosso artigo ainda não publicado.

## instalação
Faça o seguintes comando no terminal do linux ou WSL
<pre>
sudo apt update
sudo apt install bison flex -y
bison --version //verificar a instalação correta
flex --version //verificar a instalação correta
</pre>

## Compilar e usar :)

<pre>
make
./capuccina < [nome do arquivo a ser compilado].moca
</pre>

# Uso da extensão ".mocha"

Dentro da pasta mocha há um arquivo vsix, ao instalar ele como extensão no vscode os arquivos .mocha serão identificados automáticamente.
