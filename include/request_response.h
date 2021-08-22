/* ==================== request_response.h ====================
 * 設備與Server端，Request和Response電文結構。
 * Written by:	Ggg
 * Date:		2021-08-19
 * ----------------------------------------------------------
 * 2021-08-08 v1.0.0  Ggg - 可建立socket連線。
 * 2021-08-19 v1.1.0  Ggg - client、server互傳二進位訊息。
 * ========================================================== */
#ifndef _REQUEST_RESPONSE_H_
#define _REQUEST_RESPONSE_H_

#include <cstdint>

#ifndef __packed
	#define __packed __attribute__((packed))
#endif

//  Global Declarations
#define REQUEST_MSG  0
#define RESPONSE_MSG 1

typedef unsigned char uchar8_t;
typedef char char8_t;

/* ==================== MessageFormat, DATA, MSG_BODY, REQUEST & RESPONSE, PhyID ==================== */
struct PhyID
{
	uint32_t LowID;
	uint32_t HighID;
}__packed;

/* ==================== MessageFormat, DATA, MSG_BODY, REQUEST ==================== */
typedef struct
{
	uint32_t deviceID;				//	設備編號
	uchar8_t locationID;			//	車站編號
	uchar8_t issuerID;				//	ecc=2、ipass=9、icash=11、happycash=12
	PhyID    cardPhysicalNo;		//	卡片內碼：悠遊卡、一卡通、happycash // CardID：icash // PurseID：悠遊付
	uint32_t txnDateTime;			//	交易時間
}__packed REQUEST;

/* ==================== MessageFormat, DATA, MSG_BODY, RESPONSE ==================== */
typedef struct
{
	PhyID    cardPhysicalNo;		//	卡片內碼
	uchar8_t cscStatus;				//	卡片尚未註記=0、卡片已註記完成=1、卡片註記失敗=2、卡片已取消註記完成=3、卡片取消註記失敗=4
	uchar8_t phoneNo[10];			//	電話號碼，儲存方式為ASCII。例0910123456 = 0x30, 0x39, 0x31, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36
	uchar8_t birthday[4];			//	生日，儲存方式為ASCII。例1225 = 0x31, 0x32, 0x32, 0x35
	uchar8_t responseCode[2];		//	回應碼，儲存方式為ASCII。執行成功 = 0x30, 0x30
}__packed RESPONSE;

/* ==================== MessageFormat, DATA, MSG_BODY ==================== */
typedef union
{
	REQUEST  requestInfo;			//	Request訊息主體
	RESPONSE responseInfo;			//	Response訊息主體
}__packed MSG_BODY;

/* ==================== MessageFormat, DATA ====================
 * 功能碼定義：
 * 0x0701 = 卡片狀態查詢Request
 * 0x0711 = 卡片狀態查詢Response
 * 0x0702 = 卡片驗證Request
 * 0x0712 = 卡片驗證Response
 * 0x0801 = 卡片註記成功Request
 * 0x0811 = 卡片註記成功Response
 * 0x0802 = 卡片註記失敗Request
 * 0x0812 = 卡片註記失敗Response
 * 0x0803 = 卡片取消註記成功Response
 * 0x0813 = 卡片取消註記成功Response
 * 0x0804 = 卡片取消註記失敗Request
 * 0x0814 = 卡片取消註記失敗Response
 * ============================================================= */
typedef struct
{
	uint16_t stationID;				//	車站代號
	uint16_t seqNumber;				//	序號(Sequence Number)
	uint32_t msgTime;				//	訊息時間(Message Time)
	uint16_t CMD;					//	功能碼(CMD，參考功能碼定義)
	MSG_BODY msgBody;				//	訊息主體(M Bytes)
	uchar8_t checksum;				//	訊息檢查碼(CHKSUM，= 上面1~5項做XOR)
}__packed DATA;

/* ==================== MessageFormat ==================== */
typedef struct
{
	uchar8_t msgHeader;				//	訊息起頭(0x02)
	uint16_t msgLength;				//	訊息長度(= 4 Bytes + N Bytes)
	DATA msgData;					//	訊息內容(N Bytes = 11 Bytes + 訊息主體M Bytes)
	uchar8_t msgTailer;				//	訊息結尾(0x03)
}__packed MessageFormat; 

#endif