#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <sstream>
#include "drob.h"
//#include "sysfile.h"

Drob::Drob() {
  znam = 1;
  cel = chisl = 0;
}

Drob::Drob(int ce, int ch, int zn) {
  cel = ce + ch / zn;
  chisl = ch % zn;
  znam = zn;
}

void Drob::getVol(SystemFile& file) {
  long size = file.getSize() / 2;
  char *test_data = new char[ size ];
  std::vector<int> data;

  file.readf((void*)test_data, size * sizeof(char));
  std::string str(test_data);

  extractIntegers(str, data);

  cel = data[0];
  chisl = data[1];
  znam = data[2];

  delete [] test_data;
}

void Drob::print() {
  printf("%d / %d\n", cel * znam + chisl, znam);
}

Drob Drob::operator+(const Drob& d) {
  Drob res;
  int sign = 1;

  res.znam = znam * d.znam;
  res.chisl = chisl * d.znam + d.chisl * znam;
  if (res.chisl < 0) { sign = -1; }

  res.cel = cel + d.cel + res.chisl / res.znam;

  // making res.chisl positive temporary to compute lowestFraction
  res.chisl = (sign * res.chisl % res.znam); // % operator with negative numbers - UB
  lowestFraction(&res.chisl, &res.znam);
  res.chisl *= sign;
  res.normalize();

  return res;
}

Drob& Drob::operator+=(const Drob& d) {
  Drob res;
  int sign = 1;

  res.znam = znam * d.znam;
  res.chisl = chisl * d.znam + d.chisl * znam;
  if (res.chisl < 0) { sign = -1; }

  res.cel = cel + d.cel + res.chisl / res.znam;

  // making res.chisl positive temporary to compute lowestFraction
  res.chisl = (sign * res.chisl % res.znam); // % operator with negative numbers - UB
  lowestFraction(&res.chisl, &res.znam);
  res.chisl *= sign;
  res.normalize();

  *this = res;
  return *this;
}

Drob Drob::operator+(int n) {
  Drob res;
  res = *this;
  res.cel += n;
  res.normalize();

  return res;
}

Drob Drob::operator-(const Drob& d) {
  Drob res;
  int sign = 1;

  res.znam = znam * d.znam;
  res.chisl = chisl * d.znam - d.chisl * znam;
  if (res.chisl < 0) { sign = -1; }

  res.cel = cel - d.cel + res.chisl / res.znam;

  // making res.chisl positive temporary to compute lowestFraction
  res.chisl = (sign * res.chisl % res.znam); // % operator with negative numbers - UB
  lowestFraction(&res.chisl, &res.znam);
  res.chisl *= sign;
  res.normalize();
  
  return res;
}

Drob Drob::operator-(int n){
  Drob res;
  res = *this;
  res.cel -= n;
  res.normalize();

  return res;
}

Drob& Drob::operator-=(const Drob& d) {
  Drob res;
  int sign = 1;

  res.znam = znam * d.znam;
  res.chisl = chisl * d.znam - d.chisl * znam;
  if (res.chisl < 0) { sign = -1; }

  res.cel = cel - d.cel + res.chisl / res.znam;

  // making res.chisl positive temporary to compute lowestFraction
  res.chisl = (sign * res.chisl % res.znam); // % operator with negative numbers - UB
  lowestFraction(&res.chisl, &res.znam);
  res.chisl *= sign;
  res.normalize();

  *this = res;
  return *this;
}

Drob Drob::operator*(const Drob& d) {
  Drob res;
  int sign = 1;

  res.chisl = (chisl + cel * znam) * (d.chisl + d.cel * d.znam);
  if (res.chisl < 0) { sign = -1; }
  res.chisl *= sign;
  res.znam = znam * d.znam;
  res.cel = 0;

  lowestFraction(&res.chisl, &res.znam);
  res.chisl *= sign;
  res.normalize();

  return res;
}

Drob Drob::operator/(const Drob& d) {
  Drob res = *this, temp = d;

  res = (res) * (!temp);
  return res;
}

Drob& Drob::operator!() {
  if (cel == 0 && chisl == 0) { 
    std::cout << "Error: division by zero\n";
    exit(1);
  }

  int temp;
  int sign = 1;

  chisl += cel * znam;
  if (chisl < 0) { sign = -1; }
  cel = 0;

  temp = sign * chisl;
  chisl = sign * znam;
  znam = temp;

  return *this;
}

Drob operator+(int n, const Drob d) {
  Drob res, temp = d;
  res = temp + n;
  return res;
}

void Drob::normalize(){
  int sign = 1;

  chisl += cel * znam;
  if (chisl < 0) { sign = -1; }

  cel = chisl / znam;
  chisl = sign * (sign * chisl % znam);
}

void lowestFraction(int *chisl, int *znam) {
  int denom;
  denom = findGCD(*chisl, *znam);
  *chisl/=denom;
  *znam/=denom;
}

int findGCD(int a, int b) {
  if (b == 0)
      return a;
  return findGCD(b, a % b);
}

void extractIntegers(string str, std::vector<int>& data)
{
    stringstream ss;
 
    /* Storing the whole string into string stream */
    ss << str;
 
    /* Running loop till the end of the stream */
    string temp;
    int found;
    while (!ss.eof()) {
 
        /* extracting word by word from stream */
        ss >> temp;
 
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found)
            data.emplace_back(found);
 
        /* To save from space at the end of string */
        temp = "";
    }
}