CC          = gcc
ELF_SERVER  = ./server
ELF_CLIENT  = ./client
ELF         = $(ELF_SERVER)
ELF        += $(ELF_CLIENT)

SRC_ALL			= $(wildcard ./src/*.c)
SRC_SERVER		= $(filter-out ./src/client_tcp.c,$(SRC_ALL))
SRC_CLIENT		= $(filter-out ./src/server_tcp.c,$(SRC_ALL))

OBJ_ALL			= $(wildcard ./src/*.o)
OBJ_SERVER		= $(patsubst %.c,%_s.o,$(SRC_SERVER))
OBJ_CLIENT		= $(patsubst %.c,%_c.o,$(SRC_CLIENT))

OBJ		= $(OBJ_SERVER)
OBJ		+= $(OBJ_CLIENT)
C99		=-std=c99 
LIB		= -lm -lpthread
INC		= -I ./inc

all:server client
server:$(ELF_SERVER)
$(ELF_SERVER):$(OBJ_SERVER)
	$(CC) $^ -o $@ $(INC) $(C99) $(LIB)
# $(OBJ_SERVER):$(SRC_SERVER)
$(OBJ_SERVER):%_s.o:%.c
	$(CC) $< -o $@ -c $(INC) $(C99) $(LIB)

client:$(ELF_CLIENT)
$(ELF_CLIENT):$(OBJ_CLIENT)
	$(CC) $^ -o $@ $(INC) $(C99) $(LIB)
# $(OBJ_CLIENT):$(SRC_CLIENT)
$(OBJ_CLIENT):%_c.o:%.c
	$(CC) $< -c -o $@ $(INC) $(C99) $(LIB)

print:
	@echo 服务端依赖.c文件 $(SRC_SERVER) 
	@echo 服务端输出.o文件 $(OBJ_SERVER)
	@echo 客户端依赖.c文件 $(SRC_CLIENT)
	@echo 客户端输出.o文件 $(OBJ_CLIENT)

# %.o:%.c
# 	$(CC) $< -o $@  -c 
# gcc -fPIC -shared -o $(SO) -I inc/ src/gdbtest.c 

clean:	
	$(RM) $(ELF) $(OBJ_ALL)
