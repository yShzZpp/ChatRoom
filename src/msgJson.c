#include <stdio.h>
#include <string.h>
#include "../inc/cjson.h"

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

