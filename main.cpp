#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <memory>
#include <iterator>
#include <string.h>
#include "Lex.h"
#include "Exec.h"
using namespace std;

int main(){
char Filebuf[255];
int Str=0;
vector<string> Pol;
setlocale(LC_ALL, "rus"); /*Нормальное отображение кириллицы*/
cout << "Введите имя файла для разбора: " << endl;
cin >> Filebuf; 
cout << endl;
try{ 
	Scanner Scan(Filebuf); 
	WordList Lst=Scan.GetWordList();
	WordList Lst1=Lst;
	WordList Lst2=Lst;
	Str=Endl_count(Lst1);
		//Parser Pars; 
		//Pars.ParsList(&Lst);/*Вызов парсера!*/
	cout << endl << "Всё хорошо!" << endl << endl;
	cout << endl << "Вызываю ПОЛИЗ" << endl << endl;
	Poliz P1;
	Pol=P1.PolizList(Lst2);
	cout << endl << "Начинаю выполнение: " << endl << endl;
		Executor E1;
		E1.Execution(Pol);
	//cout << "У функции аргументов: " << Pcount << endl;
	//Print_L(Scan.List);
	cout << endl << "В файле строк: " << Str << endl;
}
catch(Excpt& e) {cout << e.what() << endl << "Эта ошибка произошла в " << ErrorCount << " строке! (Но это не точно!)" << endl;} 
/*Отлавливаем исключение и пишем ошибку*/

return 0;
}

/*ЗАМЕЧАНИЯ*/
/*
 В Полизе операции str, type и len имеют наивысший приоритет, поэтому нужно соблюдать порядок и заключать всё в скобочки для правильной работы!
 Если Вы хотите вызвать функцию, лучше описать её тело прямо над вызовом.
*/
