CC		= gcc
ELF_SERVER		= ./server
ELF_CLIENT		= ./client
ELF		= $(ELF_SERVER)
ELF		+= $(ELF_CLIENT)
SRC_BOTH		= $(wildcard ./src/cjson*.c)
SRC_SERVER		= $(wildcard ./src/server*.c)
SRC_CLIENT		= $(wildcard ./src/client*.c)

OBJ_SERVER		= $(patsubst %.c,%.o,$(SRC_SERVER))
OBJ_CLIENT		= $(patsubst %.c,%.o,$(SRC_CLIENT))

OBJ		= $(OBJ_SERVER)
OBJ		+= $(OBJ_CLIENT)
C99		=-std=c99
INC		= -I ./inc

all:server client
server:$(ELF_SERVER)
$(ELF_SERVER):$(OBJ_SERVER)
	$(CC) -g $^ -o $@ $(INC) $(C99)
$(OBJ_SERVER):$(SRC_SERVER)
	$(CC) -g $< -o $@  -c $(C99) $(INC)

client:$(ELF_CLIENT)
$(ELF_CLIENT):$(OBJ_CLIENT)
	$(CC) $^ -o $@ $(INC) $(C99)
$(OBJ_CLIENT):$(SRC_CLIENT)
	$(CC) $< -o $@  -c $(C99) $(INC)
# %.o:%.c
# 	$(CC) $< -o $@  -c 
# gcc -fPIC -shared -o $(SO) -I inc/ src/gdbtest.c 

clean:
	$(RM) $(ELF) $(OBJ)
