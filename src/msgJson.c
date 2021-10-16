#include <stdio.h>
#include <string.h>
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



void jsonConnect(char *temp)                                       
{                                                                 
 	cJSON *root_json = cJSON_CreateObject();                                         
 	cJSON *PubData = cJSON_CreateObject();                                         
 	cJSON_AddNumberToObject(root_json,"protocol",1);                                         
 	cJSON_AddNumberToObject(root_json,"code",0);                                         
 	cJSON_AddStringToObject(PubData,"orderId","single");                                         
 	cJSON_AddItemToObject(root_json,"data",PubData);                                         
 	char *buff=cJSON_PrintUnformatted(root_json);
 	memcpy(temp,buff,strlen(buff)); 
 	cJSON_Delete(root_json);                                         
}	              

