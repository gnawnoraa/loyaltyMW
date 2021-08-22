/* ==================== async_server.cpp ====================
 * 常客優惠回饋點數案Server端。
 * Written by:	Ggg
 * Date:				2021-08-08
 * ----------------------------------------------------------
 * 2021-08-08 v1.0.0  Ggg - 可建立socket連線。
 * 2021-08-19 v1.1.0  Ggg - client、server互傳二進位訊息。
 * ========================================================== */

#include <iostream>
#include "../include/async_server.h"
#include "../include/request_response.h"

//	Global Declarations
using namespace std;

/* ==================== main ====================*/
int main(int argc, char *argv[])
{
//  Statements
  try 
  {
    boost::asio::io_service io_service;   
    Server server(io_service);
    io_service.run();
  }
  catch(std::exception& e) 
  {
    std::cerr << e.what() << endl;
  }

  return 0;
} //  main
