CC=gcc
CFLAGS=-c -g

120++: main.o token.o tree_lydia.o 120gram_lydia.tab.o lex.yy.o table.o 
	$(CC) -o 120++ main.o lex.yy.o token.o 120gram_lydia.tab.o tree_lydia.o table.o

token.o: token.c
	$(CC) $(CFLAGS) token.c

#list.o: list.c
	#$(CC) $(CFLAGS) list.c
	
table.o: table.c
	$(CC) $(CFLAGS) table.c
	
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
	rm 120++
	rm *.o
