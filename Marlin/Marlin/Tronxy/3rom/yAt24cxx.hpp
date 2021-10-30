#ifndef _Y_24CXX_HPP
#define _Y_24CXX_HPP
#include "ySimulateIIC.hpp"
#define AT24CXX_Init() si2cInit()
void AT24CXX_Write(int& WriteAddr,const void *buffer,int len);
void AT24CXX_Read(int& ReadAddr,void *buffer,int len, const bool writing = true);
#endif