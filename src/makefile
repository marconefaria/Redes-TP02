all:
	gcc -Wall -c util.c
	gcc -Wall equipment.c util.o -o equipment.o
	gcc -Wall server.c util.o -lpthread -o server.o
clean:
	rm util.o equipment.o server.o 

# all:
# 	gcc -Wall -c common.c
# 	gcc -Wall -c handlers.c 
# 	gcc -Wall client.c common.o -o client
# 	gcc -Wall server.c common.o handlers.o -o server
# clean:
# 	rm common.o client server server-mt

