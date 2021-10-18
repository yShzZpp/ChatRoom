#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>
#include "../inc/cjson.h"
#include "../inc/my_tcp.h"


int jsonProtocol(char *payload)
{
	int protocol=-1;
	cJSON *rootJson=cJSON_Parse(payload);
	assert(rootJson!=NULL);
	cJSON *getProtocol=cJSON_GetObjectItem(rootJson,"protocol");
	assert(getProtocol!=NULL);
	protocol=getProtocol->valueint;
	return protocol;
}

/** 客户端包装首次连接数据  protocol 0*/
void jsonFirstConnect_P(User *user,char *out)
{
	cJSON *rootJson = cJSON_CreateObject();
	cJSON *dataJson = cJSON_CreateObject();

	cJSON_AddNumberToObject(rootJson,"protocol",PROTO_FIRST);
	cJSON_AddStringToObject(dataJson,"username",user->userName);
	cJSON_AddStringToObject(dataJson,"charWithWho",user->charWithWho);
	
	cJSON_AddItemToObject(rootJson,"data",dataJson);
	strcpy(out,cJSON_PrintUnformatted(rootJson));
}

/** 服务端分析首次连接数据  protocol 0*/
void jsonFirstConnect_A(char *payload,Map *map)
{
	cJSON *rootJson=cJSON_Parse(payload);
	assert(rootJson!=NULL);
	cJSON *dataJson=cJSON_GetObjectItem(rootJson,"data");
	assert(dataJson!=NULL);
	cJSON *getUserName=cJSON_GetObjectItem(dataJson,"username");
	assert(getUserName!=NULL);
	strcpy(map->userName,getUserName->valuestring);

	cJSON *getCharWithWho=cJSON_GetObjectItem(dataJson,"charWithWho");
	assert(getCharWithWho!=NULL);
	strcpy(map->charWithWho,getCharWithWho->valuestring);
}

/** 客户端发送数据 protocol 1 
 * payload 是用户想说的话 out是json格式的输出
 * 可同时为一个指针*/
void jsonChat_P(User *user,char *payload,char *out)
{
	cJSON *rootJson = cJSON_CreateObject();
	cJSON *dataJson = cJSON_CreateObject();

	cJSON_AddNumberToObject(rootJson,"protocol",PROTO_SEND);
	cJSON_AddStringToObject(dataJson,"username",user->userName);
	cJSON_AddStringToObject(dataJson,"charWithWho",user->charWithWho);

	cJSON_AddStringToObject(dataJson,"words",payload);

	cJSON_AddItemToObject(rootJson,"data",dataJson);
	bzero(out,strlen(out));
	strcpy(out,cJSON_PrintUnformatted(rootJson));
}

/** 服务器收到信息后 分析 到哪去 */
void jsonToWho_A(char *payload,char *charWithWho)
{
	cJSON *rootJson=cJSON_Parse(payload);
	assert(rootJson!=NULL);
	cJSON *dataJson=cJSON_GetObjectItem(rootJson,"data");
	assert(dataJson!=NULL);
	cJSON *getCharWithWho=cJSON_GetObjectItem(dataJson,"charWithWho");
	assert(getCharWithWho!=NULL);
	strcpy(charWithWho,getCharWithWho->valuestring);

}

/** 客户端收到信息后 分析 从哪来 带了什么 */
void jsonFromWho_A(char *payload)
{
	cJSON *rootJson=cJSON_Parse(payload);
	assert(rootJson!=NULL);
	cJSON *dataJson=cJSON_GetObjectItem(rootJson,"data");
	assert(dataJson!=NULL);
	cJSON *getCharWithWho=cJSON_GetObjectItem(dataJson,"charWithWho");
	assert(getCharWithWho!=NULL);
	cJSON *getWords=cJSON_GetObjectItem(dataJson,"words");
	assert(getWords!=NULL);
	printf("from:%s:\n%s\n",getCharWithWho->valuestring,getWords->valuestring);
}




