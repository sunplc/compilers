
CC = gcc
CFLAGS = -Wall

parser: token.c ast_node.c parser.c
	$(CC) $(CFLAGS) token.c ast_node.c parser.c -o parser 

a.out: token.c ast_node.c parser.c
	$(CC) $(CFLAGS) -g token.c ast_node.c parser.c -o parser 

