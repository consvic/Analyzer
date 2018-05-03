###################
#
# MAKE FILE
#
###################
COPT    = -O2
GCC      = gcc
LEX     = flex
BISON = bison
#
TARGET_LEX =  Scanner.l
TARGET_GRAMMAR = tinyC.y
TARGET_NAME=  compiladores
TARGET_USER= UserDefined.c
#
all:
	$(BISON) -v $(TARGET_GRAMMAR) -o $(TARGET_NAME).tab.c
	$(LEX) $(TARGET_LEX)
	$(GCC)  -o $(TARGET_NAME) $(TARGET_NAME).tab.c $(TARGET_USER) `pkg-config --cflags --libs glib-2.0` -lfl 
	

clean:
	rm -f *~ core lex.yy.c $(TARGET_NAME).tab.* $(TARGET_NAME).output

clobber: clean
	rm -f $(TARGET_NAME)
