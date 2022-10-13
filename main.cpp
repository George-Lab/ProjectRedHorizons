#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include "./libraries/drob.h"
//#include "./libraries/sysfile.h"

int main(int argc, char** argv) {
  // создается объект типа  SystemFile
  // работает коструктор 
  SystemFile file;

  // функция openf() возвращает значение errno,
  // полученное в процессе создания или открытия файла
  if(file.openf(argv[1])){
     perror("Can't open the file");
     exit(1);
  }
	
  Drob a1, a2, res;
  a1.getVol(file);
  a2.getVol(file);

  file.closef();

  a1.print(); 
  a2.print(); 
  res.print(); 

  res = a1 + a2; 
  res.print(); 

  res = a1 + 5; 
  res.print(); 

  res += a2;
  res.print();

  res = a2 - a1; 
  res.print(); 

  res = a2 - 3; 
  res.print(); 

  res -= a2; 
  res.print(); 

  res = a1 * a2; 
  res.print(); 

  res = a1 / a2; 
  res.print(); 

  res = !a1; 
  res.print(); 

  return 0;
}