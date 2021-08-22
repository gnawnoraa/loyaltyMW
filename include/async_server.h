/* ==================== async_server.h ====================
 * Boost library功能。
 * Written by:	Ggg
 * Date:				2021-08-19
 * ----------------------------------------------------------
 * 2021-08-08 v1.0.0  Ggg - 可建立socket連線。
 * 2021-08-19 v1.1.0  Ggg - client、server互傳二進位訊息。
 * ========================================================== */
#include <iostream>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "msgFormat.h"
#include "request_response.h"

//	Global Declarations
using namespace std;
using namespace boost::asio;
using namespace boost::placeholders;
using ip::tcp;

//	BOOST 1_70版本以後移除get_io_service
#if BOOST_VERSION >= 107000
	#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).get_executor().context())
#else
	#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif

/* ==================== class: con_handler ==================== */
class con_handler : public boost::enable_shared_from_this<con_handler>
{
private:
  tcp::socket sock;
  enum { max_length = 1024 };
  char data[max_length];
  string writeStr;
	
public:
  //  constructor
  con_handler(boost::asio::io_service& io_service) : sock(io_service){}

  //  member functions
  typedef boost::shared_ptr<con_handler> pointer;
  static pointer create(boost::asio::io_service& io_service){return pointer(new con_handler(io_service));}
  tcp::socket& socket(){return sock;}
  void start();
  void handle_read (const boost::system::error_code& err, size_t bytes_transferred);
  void handle_write(const boost::system::error_code& err, size_t bytes_transferred);

};

/* ==================== con_handler::start ==================== */
void con_handler::start()
{
//  Statements
  //  非同步讀取
  sock.async_read_some(
    boost::asio::buffer(data, max_length),
    boost::bind(&con_handler::handle_read,
      shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
} //  con_handler::start

/* ==================== con_handler::handle_read ==================== */
void con_handler::handle_read(const boost::system::error_code& err, size_t bytes_transferred)
{
//  Statements
  if (!err)
  {
    //  將buffer讀取到的資料，產生request物件並初始化
    cout << "============================================================" << endl;
    string readStr(data, data[1]); //  data[1] = length位置
    cout << "readStr = " << readStr << endl;
    All_Msg reqMsg(readStr, REQUEST_MSG);

    //  印出收到電文之16進位結果
    reqMsg.show();

    //  準備回傳的string，再交給con_handler::handle_write處理
    writeStr.clear();
    writeStr += readStr;

    //  印出準備傳送電文之16進位結果
    cout << "============================================================" << endl;
    cout << "writeStr = " << writeStr << endl;
    All_Msg resMsg(readStr, RESPONSE_MSG);
    resMsg.show();
    cout << "============================================================" << endl;

    //  非同步寫入
    sock.async_write_some(
	    boost::asio::buffer(writeStr, max_length),
      boost::bind(&con_handler::handle_write, 
		    shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    std::cerr << "err (recv): " << err.message() << std::endl;
    sock.close();
  }
} //  con_handler::handle_read

/* ==================== con_handler::handle_write ==================== */
void con_handler::handle_write(const boost::system::error_code& err, size_t bytes_transferred)
{
//  Statements
  if (!err)
  {
    cout << "Server sent message successfully!"<< endl << endl;
    //cout << "============================================================" << endl;
    //cout << "writeStr = " << writeStr << endl;
  }
	else
  {
    std::cerr << "err (recv): " << err.message() << std::endl;
    sock.close();
  }
} //  con_handler::handle_write

/* ==================== class: Server ==================== */
class Server
{
private:
  tcp::acceptor acceptor_;
  void start_accept()
  {
    // creates a socket
    // acceptor_.get_io_service()改成GET_IO_SERVICE(acceptor_)
    con_handler::pointer connection = con_handler::create(GET_IO_SERVICE(acceptor_));

    // initiates an asynchronous accept operation 
    // to wait for a new connection. 
    acceptor_.async_accept(connection->socket(),
      boost::bind(&Server::handle_accept, this, connection,
        boost::asio::placeholders::error));
  } //  start_accept

public:
  //  constructor
  Server(boost::asio::io_service& io_service): acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234)){start_accept();}
	
  //  member functions
  void handle_accept(con_handler::pointer connection, const boost::system::error_code& err);

};

/* ==================== Server::handle_accept ==================== */
void Server::handle_accept(con_handler::pointer connection, const boost::system::error_code& err)
{
//  Statements
  if (!err)
    connection->start();
	start_accept();
} //  Server::handle_accept