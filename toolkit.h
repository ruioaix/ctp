#ifndef CTP_C_API_TOOLKIT_H
#define CTP_C_API_TOOLKIT_H

#include "cpp_api/ThostFtdcUserApiStruct.h"

#include <vector>
#include <set>
#include <string>

using namespace std;

//输入路径，生成多级目录
void makedirs(const char* dir);

//将字符串按指定字符分割
void GetSetFromString(const char* szString,const char* seps,vector<string>& vct,set<string>& st);

//根据OnFrontDisconnected(int nReason)的值填上错误消息
void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

#endif
