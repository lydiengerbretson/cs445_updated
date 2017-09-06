EXEC=120++
SOURCES=main.c token.c lex.yy.c
OBJECTS=$(SOURCES:.c=.o)

LEX=flex
CC=gcc
RM=rm -f

all: $(EXEC)

test: $(EXEC)
	./$(EXEC) $(TESTS)

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c -g $< -o $@

lex.yy.c: clex.l ytab.h
	$(LEX) $<

clean:
	$(RM) $(EXEC) $(OBJECTS) lex.yy.c 


