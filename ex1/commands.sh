gcc -std=gnu11 -c array.c && gcc -std=gnu11 -c e1.c && gcc -o foobar array.o e1.o && ./foobar