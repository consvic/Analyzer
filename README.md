# Analyzer

#### Syntactic and Semantic Analysis
## Autores
Constanza Lievanos Sanchez A01361506

Allan Francisco Julian Novoa A01361651

Cesar Guadarrama Cantu A01364853

### Compilación

* flex Scanner.l 				-.Genera el archivo lex.yy.c
* bison tinyC.y 				-.Genera el archivo tinyC.tab.c
* gcc tinyC.tab.c -o "Nombre del ejecutable" -lfl
* ./"Nombre del ejecutable" < "Archivos de Prueba"(test1.txt,test2.txt,test3.txt,test4.txt)

o

  $ make

  $ ./compiladores < test1.txt

