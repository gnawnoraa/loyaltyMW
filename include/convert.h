/* ======================== convert.h =======================
 * 資料格式轉換、印出資料等功能。
 * Written by:	Ggg
 * Date:        2021-08-17
 * ----------------------------------------------------------
 * 2021-08-08 v1.0.0  Ggg - 可建立socket連線。
 * 2021-08-19 v1.1.0  Ggg - client、server互傳二進位訊息。
 * ========================================================== */
#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

//  Global Delcarations
using namespace std;

//  Function Declarations
void readFile(ifstream &inPtr, vector<char> &pVec);
void hexdump(void *ptr, int buflen);

/* ==================== readFile ====================
   讀取檔案。
*/
void readFile(ifstream &inPtr, vector<char> &pVec)
{
//  statements
  inPtr.open("/home/loyalty_mw/msgTest.log", ios::in | ios::binary);
  if(inPtr.is_open())
  {
    //  讀取開頭和結尾的位置，並移動到開頭和結尾
    streampos start = inPtr.tellg();
    inPtr.seekg(0, std::ios::end);
    streampos end = inPtr.tellg();
    inPtr.seekg(0, std::ios::beg);

    //  定vector大小
    pVec.resize(static_cast<size_t>(end - start));

    //  讀取vector
    inPtr.read(&pVec[0], pVec.size());
  }
  return;
}   //  readFile

/* ==================== hexdump ====================
   印出16進位。
*/
void hexdump(void *ptr, int buflen)
{
//  Local Declarations
  unsigned char *buf = (unsigned char*)ptr;
  int i, j;

//  Statements
  for (i = 0; i < buflen; i += 16)
  {
    printf("%07x: ", i);
    for (j = 0; j < 16; j++)
    { 
      if (i + j < buflen)
        printf("%02x ", buf[i + j]);
      else
        printf("   ");
    }
    printf(" ");
    for (j = 0; j < 16; j++)
    {
      if (i + j < buflen)
        printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
    }
    printf("\n");
  }
  return;
} //  hexdump
#endif