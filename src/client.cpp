/* ======================= client.cpp =======================
 * 常客優惠回饋點數案Client端。
 * Written by:	Ggg
 * Date:				2021-08-19
 * ----------------------------------------------------------
 * 2021-08-08 v1.0.0  Ggg - 可建立socket連線。
 * 2021-08-19 v1.1.0  Ggg - client、server互傳二進位訊息。
 * ========================================================== */
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <boost/asio.hpp>
#include "../include/request_response.h"
#include "../include/convert.h"


//  Global Declarations
using namespace boost::asio;
using ip::tcp;
using std::string;
using namespace std;

//  Function Declarations
void hexdump(void *ptr, int buflen);
void readFile(ifstream &inPtr, vector<char> &pVec);

/* ==================== main ====================*/
int main()
{
//  Local Declarations
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);
  enum { max_length = 1024 };
  char data[max_length];

  ifstream in;
  vector<char> contents;
  string writeMsg;

//  Statements
  //  對目標server建立TCP連線
  socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("192.168.74.128"), 1234));

  //  讀取電文內容並印出，將電文內容存在string內送給buffer(測試讀檔)
  cout << "============================================================" << endl;
  readFile(in, contents);
  for(char c: contents) writeMsg += c;
  cout << "writeMsg = " << writeMsg << endl;
  hexdump(contents.data(), contents.size());
  cout << "============================================================" << endl;

  //  將buffer內的message送出給目標server
  boost::system::error_code error;
  boost::asio::write(socket, boost::asio::buffer(writeMsg.c_str(), writeMsg.length()), error);

  //  判斷送出有沒有成功
  if( !error )
    cout << "Client sent message successfully!" << endl << endl;
  else
    cout << "send failed: " << error.message() << endl;

  //  準備接收server端送過來的message
  //boost::asio::streambuf receive_buffer;
  //boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
  boost::asio::read(socket, boost::asio::buffer(data, max_length), boost::asio::transfer_all(), error);
  //cout << "dataLength = " << data[1] << endl;
  string readMsg(data, data[1]);

  //  判斷接收server的message有沒有成功
  if( error && error != boost::asio::error::eof )
    cout << "receive failed: " << error.message() << endl;
  else
  {
    //const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
    cout << "============================================================" << endl;
    cout << "readMsg: " << readMsg << endl;
    cout << "============================================================" << endl;
    hexdump(data, readMsg.length());
  }

  return 0;
} //  main
