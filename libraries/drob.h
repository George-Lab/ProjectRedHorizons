#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "sysfile.h"

class Drob{
 int cel, chisl, znam;
public:
// конструктор дроби по-умолчанию
  Drob(); 

// Инициализирующий конструктор  
  Drob(int,int,int);

// Устанавливаем значение дроби из файла fd 
  void getVol(SystemFile&);

// Печать дроби в формате простой дроби
  void print();

// Сложение двух дробей
	Drob operator+(const Drob&);
  Drob& operator+=(const Drob&);

// Сложение дроби с целым числом
	Drob operator+(int);
	
// Все дроби положительные. Здесь ищется РАЗНИЦА между дробями
  Drob operator-(const Drob&);
  Drob operator-(int);
  Drob& operator-=(const Drob&);

// Умножение двух дробей
	Drob operator*(const Drob&);

// Деление двух дробей
	Drob operator/(const Drob&);

// инвертирование дроби 5/7 -> 7/5
	Drob& operator!();

// Нормализует дробь - приводит числитель и целую часть к значениям одного знака
  void normalize();
};

// Свободная функция
Drob operator+(int, const Drob);

// Поиск наибольшего общего делителя
int findGCD(int a, int b);

// Упрощение дроби
void lowestFraction(int *chisl, int *znam);

// Целые числи из строки записывает в вектор
void extractIntegers(string str, std::vector<int>& data);