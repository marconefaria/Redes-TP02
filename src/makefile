all:
	gcc -Wall -c util.c
	gcc -Wall equipment.c util.o -o equipment
	gcc -Wall server.c util.o -lpthread -o server
clean:
	rm util.o equipment server

# all:
# 	gcc -Wall -c common.c
# 	gcc -Wall -c handlers.c 
# 	gcc -Wall client.c common.o -o client
# 	gcc -Wall server.c common.o handlers.o -o server
# clean:
# 	rm common.o client server server-mt

