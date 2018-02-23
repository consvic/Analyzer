###################
#
#
#
###################
COPT    = -O2
COPT2   = -Wall -O2
CDBG    = -g -DDEBUG
CC      = gcc
LEX     = flex
YACC = bison
#
TARGET_LEX =  Scanner.l
TARGET_GRAM = tinyC.y
TARGET_NAME=  compiladores
TARGET_USER= UserDefined.c
#
all:
	$(YACC) -v $(TARGET_GRAM) -o $(TARGET_NAME).tab.c
	$(LEX) $(TARGET_LEX)
	$(CC) -DGRAMMAR $(COPT) -o $(TARGET_NAME) $(TARGET_NAME).tab.c -ll `pkg-config --cflags --libs glib-2.0` $(TARGET_USER)

clean:
	rm -f *~ core lex.yy.c $(TARGET_NAME).tab.* $(TARGET_NAME).output

clobber: clean
	rm -f $(TARGET_NAME)
