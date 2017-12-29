/*compress_main.cpp
MIT License

Copyright (c) 2017 Yoshimasa Takabatake

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <string>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "cmdline.h"

using namespace std;

int main(int argc, char *argv[])
{
  cmdline::parser p;
  p.add<string>("csv_file", 'c', "output csv file name", true);

  p.parse_check(argc, argv);
  const string csv_file  = p.get<string>("csv_file");

  ofstream outputfileC(csv_file);
  outputfileC.close();
  if(1){
	  ofstream outputfileC(csv_file,ios::app);
  	outputfileC << "Read_Size,Method,Memory,Time" << endl;
  }
  return 0;
}

