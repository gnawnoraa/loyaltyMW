/* ====================== msgFormat.h =======================
 * 依據request_response.h的結構，產生物件給async_server.h的功能使用。
 * Written by:	Ggg
 * Date:		2021-08-18
 * ----------------------------------------------------------
 * 2021-08-08 v1.0.0  Ggg - 可建立socket連線。
 * 2021-08-19 v1.1.0  Ggg - client、server互傳二進位訊息。
 * ========================================================== */
#ifndef _MSGFORMAT_H_
#define _MSGFORMAT_H_

#include <iostream>
#include <cstring>
#include "request_response.h"
#include "convert.h"

//  Global Declarations
using namespace std;

/* ==================== class: All_msgFormat (Abstract Class) ==================== */
class All_msgFormat
{
private:
    string inputStr;
    MessageFormat* msg_ptr;

public:
    //  constructor, destructor
    All_msgFormat(){};
    All_msgFormat(const string & inputData);
    virtual ~All_msgFormat(){delete msg_ptr;}

    //  member functions
    virtual void setToZero();
    virtual const string & getInputString() const{return inputStr;}
    virtual void show() const;
};

/* ==================== class: All_Msg ==================== */
class All_Msg : public All_msgFormat
{
private:
    REQUEST* pRequest_Message;
    RESPONSE* pResponse_Message;
    int msgStatus;  // request_response.h定義REQUEST_MSG=0，RESPONSE_MSG=1
    int stringLen;

public:
    //  constructor, destructor
    All_Msg(){};
    All_Msg(const string & inputData, const int & inputStatus);
    ~All_Msg(); //  TODO: 把REQUEST、RESPONSE delete掉

    //  member functions
    void setToZero();
    const string & getInputString() const{return All_msgFormat::getInputString();}
    virtual void show() const;
};

/* ==================== All_msgFormat(constructor 2) ==================== */
All_msgFormat::All_msgFormat(const string & inputData) : inputStr(inputData)
{
//  Statements
    inputStr.assign(inputData, 0, inputData.length());
    setToZero();
    //  TODO: 初始化輸入的電文data
}   //  All_msgFormat

/* ==================== All_msgFormat::setToZero ==================== */
void All_msgFormat::setToZero()
{
//  Statements
    msg_ptr = new MessageFormat;
    memset(msg_ptr, '\0', sizeof(MessageFormat));
}   //  All_msgFormat::setToZero

/* ==================== All_msgFormat::show ==================== */
void All_msgFormat::show() const
{
//  Local Declarations
    char* inputChar;
    string tmpStr;

//  Statements
    //  string轉char array
    tmpStr = getInputString();
    inputChar = new char[tmpStr.length() + 1];
    tmpStr.copy(inputChar, tmpStr.length() + 1);    //  一定要用c++11的std::string::copy，用strcpy、memcpy都無法完整複製

    //  轉16進位印出結果
    hexdump(inputChar, tmpStr.length());
    delete inputChar;
}   //  All_msgFormat::show

/* ==================== All_Msg(constructor 2) ==================== */
All_Msg::All_Msg(const string & inputData, const int & inputStatus) : All_msgFormat(inputData)
{
//  Statements
    msgStatus = inputStatus;
    setToZero();
    //  TODO: 初始化輸入的電文data
}   //	All_Msg

/* ==================== All_Msg(destructor) ==================== */
All_Msg::~All_Msg()
{
//  Statements
    switch(msgStatus)
    {
		case REQUEST_MSG:
            delete pRequest_Message;
            break;		
		case RESPONSE_MSG:
            delete pResponse_Message;
            break;
		default: break;
    }
}   //	All_Msg

/* ==================== All_Msg::setToZero ==================== */
void All_Msg::setToZero()
{
//  Statements
    switch(msgStatus)
    {
		case REQUEST_MSG:
            pRequest_Message = new REQUEST;
            memset(pRequest_Message, '\0', sizeof(REQUEST));
            break;		
		case RESPONSE_MSG:
            pResponse_Message = new RESPONSE;
            memset(pResponse_Message, '\0', sizeof(RESPONSE));
            break;
		default: break;
    }
}   //  All_Msg::setToZero

/* ==================== All_Msg::show ==================== */
void All_Msg::show() const
{
//  Local Declarations


//  Statements
    All_msgFormat::show();  //  印出16進位

}   //  All_Msg::show
#endif