#!/bin/bash
gcc -pthread ../libs/afficher.c ../libs/course.c ../libs/session.c ../libs/share.c ../main.c -o main
# clang ../libs/afficher.c ../libs/course.c ../libs/session.c ../main.c
# clang -c -pthread ../libs/afficher.c
# clang -c -pthread ../libs/course.c
# clang -c ../libs/session.c 
# clang -c -pthread ../main.c
# clang -v afficher.o course.o session.o main.o 