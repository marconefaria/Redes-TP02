all:
	gcc -Wall -c common.c
	gcc -Wall equipment.c common.o -o equipment
	gcc -Wall server.c common.o -lpthread -o server
clean:
	rm common.o equipment server server

# all:
# 	gcc -Wall -c common.c
# 	gcc -Wall -c handlers.c 
# 	gcc -Wall client.c common.o -o client
# 	gcc -Wall server.c common.o handlers.o -o server
# clean:
# 	rm common.o client server server-mt

