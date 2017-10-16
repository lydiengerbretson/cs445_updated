CC=gcc
CFLAGS=-c -g

120++: main.o token.o tree_lydia.o 120gram_lydia.tab.o lex.yy.o hash.o utility_func.o symbol_table.o type.o
	$(CC) -o 120++ main.o lex.yy.o token.o 120gram_lydia.tab.o tree_lydia.o hash.o utility_func.o symbol_table.o type.o 

token.o: token.c
	$(CC) $(CFLAGS) token.c
    
utility_func.o: utility_func.c
	$(CC) $(CFLAGS) utility_func.c

type.o: type.c
	$(CC) $(CFLAGS) type.c
    
symbol_table.o: symbol_table.c
	$(CC) $(CFLAGS) symbol_table.c
	
hash.o: hash.c
	$(CC) $(CFLAGS) hash.c
	
tree_lydia.o: tree_lydia.c
	$(CC) $(CFLAGS) tree_lydia.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) lex.yy.c

lex.yy.c: 120lex_lydia.l 120gram_lydia.tab.c
	flex 120lex_lydia.l

120gram_lydia.tab.o: 120gram_lydia.tab.c
	$(CC) $(CFLAGS) 120gram_lydia.tab.c

120gram_lydia.tab.c: 120gram_lydia.y
	bison -dtv 120gram_lydia.y

clean:
	rm lex.yy.c
	rm 120gram_lydia.tab.c
	rm 120gram_lydia.output
	rm *.o
	rm 120++
