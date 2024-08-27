#pragma once

void Send_JsonString( Json::Value valueSend );
void Send_SemParams( short nIndex );
void Send_DoubleParam( BYTE bcommand );
void Send_DoubleParams( BYTE bcommand );
void Send_IntParam( BYTE bcommand );
void Send_IntParams( BYTE bcommand );
void Send_SemGetValueArray();
void Recv_SemGetType( const char* param );
void Recv_SemGetTypeArray( Json::Value arrayObj );
short Recv_SemGetValue( const char* param, BOOL bSend );
void Recv_SemGetValueArray( Json::Value arrayObj );
void Recv_SemSetValue( Json::Value param );
void Recv_SemSetValues( Json::Value root );
short Recv_StageGetParam( const char* param, BOOL bSend );
void Send_StageGetValueArray();
void Recv_StageGetParamArray( Json::Value arrayObj );
void Recv_StageSetParam( Json::Value root, const char* method );
void Recv_StageSetCmd( const char* method );
void Recv_StageSetCmdArray( Json::Value arrayObj );
void Recv_AppSetCommand( const char* param );
void Recv_AppSetCommandArray( Json::Value arrayObj );
void Recv_SetScanParam( const char* method, Json::Value param );
void Recv_SetScanCommand( const char* method, int nValue );
void Recv_GetScanParam( const char* method );
void Recv_TranslateJson(const std::string& strData);
