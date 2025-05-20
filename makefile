# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
LEX = flex
YACC = bison -d


# Nome do executável final
TARGET = cappuccina

# Arquivos objeto
OBJS = parser.tab.o lex.yy.o ast.o eval.o main.o

# Regra principal
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ -lfl

# Regras de compilação
parser.tab.o: parser.tab.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c $<

eval.o: eval.c eval.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c
	$(CC) $(CFLAGS) -c $<

# Geração do parser
parser.tab.c parser.tab.h: parser.y
	$(YACC) $<

# Geração do lexer
lex.yy.c: lexer.l
	$(LEX) $<

# Limpeza
clean:
	rm -f $(TARGET) $(OBJS) lex.yy.c parser.tab.c parser.tab.h

.PHONY: all clean