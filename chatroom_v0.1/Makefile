CC               = gcc 		#指定编译器 gnueabihf-
ELF              = ./bin/chatroom	#指定生成的可执行文件及路径
SRC              = $(wildcard ./src/*.c)	#匹配查找所有的.c文件
# MQTT_LIB_DIR     = -L ./mosquitto-1.6.3/lib
# MQTT_INCLUDE_DIR = -I ./mosquitto-1.6.3/lib
LIB              = -L ./lib
INC              = -I ./inc 
OBJ              = $(patsubst %.c,%.o,$(SRC)) #把所有的SRC中的.c 替换成.o


$(ELF):$(OBJ)
	$(CC) $^ -o $@ $(CONFIG) $(MQTT_INCLUDE_DIR) $(MQTT_LIB_DIR) $(LIB) $(INC) -lpthread  -lssl -lcrypto -lm
%.o:%.c
	$(CC) $< -o $@  -c $(CONFIG) $(MQTT_INCLUDE_DIR) $(MQTT_LIB_DIR) $(LIB) $(INC)

gdb:
	$(CC) -g $(OBJ) -o $(ELF2) $(CONFIG) $(MQTT_INCLUDE_DIR) $(MQTT_LIB_DIR) $(LIB) $(INC) -lpthread  -lmosquitto -lssl -lcrypto -lm

clean:
	$(RM) $(ELF) $(OBJ)

# $(subst <from>,<to>,<test>)将test的from内容替换为to

# $(patsubst <pattern>,<replacement>,<text>)
#		 test符合pattern就用replacement代替

# $(dir <names...>)提取names的目录部分

# $(notdir <names...>)提取names非目录部分

# $(foreach <var>,<list>,<text>)
#<list>中的单词逐一取出来放到参数<var>中,然后再执行<text>							所包含的表达式。每次<text>都会返回一个字符串,循环的过程中,<text>				中所包含的每个字符串会以空格隔开,最后当整个循环结束时, <text>						所返回的每个字符串所组成的整个字符串将会是 函数 foreach 								函数的返回值。
# 
# $(wildcard path)提取path下的文件
# 
# $@ 规则中的目标集合,在模式规则中,如果有多个目标的话,“$@”表示匹配模式中定义的目标集合。
#$< 依赖文件集合中的第一个文件,如果依赖文件是以模式(即“%”)定义的,那么“$<”就是符合模式的一系列的文件集合。
#$^ 所有依赖文件的集合,使用空格分开,如果在依赖文件中有多个重复的文件,“$^”会去除重复的依赖文件,值保留一份。
# 
# 
# 
