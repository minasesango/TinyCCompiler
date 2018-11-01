TARGET = tcc
CC = gcc
CFLAGS = -g -O2 -Wall
YACC = bison
YFLAGS = -d
LEX = flex
LFLAGS =
OBJS = tcc.tab.o lex.yy.o tree.o util.o emit.o main.o
TEMPSRCS = tcc.tab.c lex.yy.c tcc.tab.h
.SUFFIXES:.c .o
all: $(TARGET)
tcc.tab.c: tcc.y
	$(YACC) $(YFLAGS) $< 
tcc.tab.h: tcc.y
lex.yy.c: tcc.l tcc.tab.h
	$(LEX) $(LFLAGS) $<
.c.o:
	$(CC) $(CFLAGS) -c $<
tcc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(TEMPSRCS)

