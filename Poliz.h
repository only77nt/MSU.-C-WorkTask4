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
using namespace std;

#define countKey 21
#define countSep1 12
#define countSep2 7
#define countSpace 2

enum type_of_lex{LEX_TAB /*0*/, LEX_DEF /*1*/, LEX_PRINT /*2*/, LEX_INPUT /*3*/, LEX_FOR /*4*/, LEX_IF /*5*/, LEX_TYPE /*6*/, LEX_LEN /*7*/, LEX_CONST /*8*/, LEX_VAR /*9*/, LEX_ENDL /*10*/, LEX_SEP /*11*/, LEX_NULL /*12*/, LEX_STR /*13*/, LEX_CHAR/*14*/, LEX_EQ/*15*/, LEX_LOG/*16*/,LEX_LB/*17*/,LEX_RB/*18*/,LEX_FUNC/*19*/, LEX_WHILE/*20*/, LEX_STRR/*21*/};

enum type_of_sem{SEM_NULL/*0*/, SEM_SEP/*1*/, SEM_VAL/*2*/, SEM_LB/*3*/, SEM_RB/*4*/, SEM_ENDL/*5*/, SEM_TAB/*6*/, SEM_IF/*7*/};

enum type_var {LOG/*0*/,SEP/*1*/,RB/*2*/,LB/*3*/,STRING/*4*/,NUMBER/*5*/,NUL/*6*/};


/*Таблица ключевых слов*/
string KeyWords[]={"def","print","input","range","for","in","if","elif","else","type","len"};
/*Таблица одиночных разделителей*/
string Separators1[]={"\\","%","(",")",",",":","```","'","[","]",";","."}; 
/*Таблица двойных разделителей*/
string Separators2[]={"+","-","=","*","<",">","!"};
/*Таблица пробелов*/
string Spacing[]={" ","\n"};

/*Глобальные переменные*/
int Bcount=0;
int Vcount=0;
int IFcount=0;
int FORcount=0;
int TABcount=0;
int FuncNum=0;
bool FUNCflag=false;
int Pcount[50]; /*Счётчик кол-ва параметров функции*/
int ErrorCount=1;

/*Класс исключений*/
class Excpt : public exception {
public:
	explicit Excpt( string _msg ): msg( move( _msg ) ) {}
	const char* what() const noexcept override { return msg.c_str(); }
private:
	const string msg;
};

/*Структура списка*/
typedef struct Node *WordList;
struct Node
{
	string str;
	type_of_lex type_l;
	type_of_sem type_sem;
	WordList next;
} wordlist;

struct VAR
{
	string name;
	type_var type;
};

/*Структура для Полиза*/
struct ForPoliz
{
	string name;
	int weight;
};

/*Структура для функций в Полизе*/
struct FuncPoliz
{
	string name;
	int PolizPoz;
	int CountPar;
};

/*Взять лексему*/
WordList GetLex(WordList *List){
	(*List)=(*List)->next;
	WordList Lst=*List;
return Lst;
}

/*Функция выкидывающая исключение*/
void Error(string buf)
{
	throw Excpt( move( buf ) );
}

/*ПОЛИЗ*/
class Poliz{
private:
	friend WordList GetLex(WordList *Lst);
	friend void Error(char* buf);
public:
	vector<string> StackVAR;
	vector<ForPoliz> StackOP;
	ForPoliz Buf; /*Создаём в момент, когда это нужно*/
	FuncPoliz FuncBuf;
	Poliz(){};
	string a;
	int Poz=0;
	vector<int> PozF;
	vector<int> PozT;
	vector<FuncPoliz> StackFUNC;
	int ItIsPoz(string fname)
	{
		for(vector<FuncPoliz>::iterator it=StackFUNC.begin();it!=StackFUNC.end();it++)
		{
			if((*it).name==fname)
				return (*it).PolizPoz;
		}
	}
	bool ItIsFunc(string fname)
	{
		for(vector<FuncPoliz>::iterator it=StackFUNC.begin();it!=StackFUNC.end();it++)
		{
			if((*it).name==fname)
				return true;
		}
		return false;
	}
	void PrintStackVAR() /*Функция печати ПОЛИЗА*/
	{	int i=0;
		for(vector<string>::iterator it=StackVAR.begin();it!=StackVAR.end();it++)
		{	
			cout << i << "	" << (*it) << endl;
			i++;
		}
	}	
	void ClearStack() /*Функция очищения стека*/
	{
		ForPoliz Buf;
		Buf=StackOP.back();
		while(Buf.name!="@" && Buf.name!="=" && Buf.name!="(")
		{
			Poz++;
			StackVAR.push_back(Buf.name);
			StackOP.pop_back();
			Buf=StackOP.back();
		}
	}
	void Check() /*Функция проверки*/
	{
		ForPoliz Buf1,Buf2;
		cout << "Check!" << endl;
		Buf1=StackOP.back();
		cout << Buf1.name << " Buf1" << endl;
		StackOP.pop_back();
		Buf2=StackOP.back();
		cout << Buf2.name << " Buf2" << endl;
		if(Buf1.name==")")
		{
			StackOP.pop_back();
			ClearStack();
			StackOP.pop_back();
		}
		if(Buf1.weight < Buf2.weight)
		{
			ClearStack();
			cout << "Отчистил стэк операций!" <<endl;
			StackOP.push_back(Buf1);
		}
		else
		{
			StackOP.push_back(Buf1);
		}
	};
	int Else_count(WordList Lst)
	{
		int i=0;
		while(1)
		{
			if (Lst->str=="else" )
				i++;
			if (Lst->str=="\\n"  &&( Lst->next==NULL || Lst->next->str!="\t"))
				break;
			Lst=Lst->next;
		}	
		return i;
	}
	bool Check_else(WordList& Lst)
	{
		while (Lst->str=="\\t")
			Lst=Lst->next;
		if (Lst->str!="else")
			return true;
		return false;
	}
	int Check_tab(WordList Lst)
	{
		
		int i=0;
		while (Lst->str=="\\t")
		{
			Lst=Lst->next;
			i++;
		}
		return i;
	}
	vector<string> PolizList(WordList& Lst) /*Построение ПОЛИЗА*/
	{
		bool WhileFlag=false;
		int IfFlag=0;
		int ElFlag=0;
		bool FuncFlag=false;
		bool NowFunc=false;
		bool FuncAgain=false;
		bool ArrayFlag=false;
		int Tab=0;
		vector<int> Tab_count;
		int Wcount=0;
		int Icount=0;
		int NowPoz=0;
		vector<int> Pozz;
		Buf.name="@";
		Buf.weight=-1;
		StackOP.push_back(Buf);
		while(1)
		{
			cout<<"Мы на ходимся на позиции :"<<Poz<<endl<<endl;
			cout << Lst->str << " Сейчас!" << endl;
			if(Lst->str=="def")
			{
				FuncFlag=true;
				StackVAR.push_back("@");
				Poz++;
				Lst=Lst->next;
				FuncBuf.CountPar=0;
				FuncBuf.name=Lst->str;
				StackVAR.push_back(Lst->str);
				Poz++;
				Lst=Lst->next->next; /*Пропустили открыв. скобку*/
				while(Lst->str!=")")
				{
					if(Lst->str!=",")
					{
						FuncBuf.CountPar++;
						StackVAR.push_back(Lst->str);
						Poz++;
					}
					Lst=Lst->next;
				}
				StackVAR.push_back("$"); /********************************************************* 2 новые строки *******************/
				Poz++;
				FuncBuf.PolizPoz=Poz; /*Это позиция начала функции!*/
				StackFUNC.push_back(FuncBuf); /*Занесли нашу функцию в стек функций*/
				Lst=Lst->next; /*Пропускаем :*/
			}
			else			
				if(Lst->str=="if" )
				{
					IfFlag++;
					cout<<"                  TAB "<<Tab<<endl;
					Tab_count.push_back(Tab);//вектор хранящий количество табуляций перед ифами
					Icount++;
				}
			else			
				if( Lst->str=="elif")
				{
					
					Icount++;
					cout<<"Icount            "<<Icount<<endl<<endl;
					cout<<"				TAb "<<Tab<<"   "<<Tab_count.back()<<"  Icount "<<Icount<<" Ifflag "<<IfFlag<<endl;
					if (!Tab_count.empty() && Tab==Tab_count.back() && IfFlag && Icount>1)
					{
						cout<<"						ЛОЛ КЕК ЧЕБУРЕК2!!!"<<endl;
						int PF;
						auto it=PozF.end();
						cout<<"LOL LOL    "<<*(it-1)<<"   "<<*(it-2)<<endl;
						it--;
						PF=*it;
						PozF.erase(it);
						StackVAR[PF]=to_string(Poz);
						cout<<"позиция                "<<Poz<<endl;
						cout<<"куда ставим            "<<PF<<endl;
						cout<<"вложенность            "<<IfFlag<<endl;
						cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;
					}
				}
			else
				if(Lst->str=="else")
				{
					ClearStack();
					cout<<"позиция                "<<Poz<<endl;
					cout<<"вложенность            "<<IfFlag<<endl;
					cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;
					cout<<"количество табуляций    "<<Tab<<endl;
					cout<<"количество табуляций при последнем ифе"<<Tab_count.back()<<endl;
					PozT.push_back(Poz);
					StackVAR.push_back("NULL");
					Poz++;
					StackVAR.push_back("!");
					Poz++;
					Lst=Lst->next->next->next;
					ElFlag++;
					int PF;
					if (Tab!=Tab_count.back() && !WhileFlag)
					{
						auto it=PozF.end();
						cout<<"LOL"<<endl;
						it-=IfFlag;
						cout<<"1"<<endl;
						PF=*it;
						PozF.erase(it);
					}
					else
					{
						PF=PozF.back();
						PozF.pop_back();
					}
					cout<<"позиция                "<<Poz<<endl;
					cout<<"куда ставим            "<<PF<<endl;
					cout<<"вложенность            "<<IfFlag<<endl;
					cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;

					StackVAR[PF]=to_string(Poz);
					IfFlag--;	
					Tab_count.pop_back();
				}
			else
				if(Lst->str==",")
				{
					cout << "Нашёл запятую!" << endl;
					ClearStack(); /****************************** Точно? ***********/
				}
			else
				if(Lst->str==":" && !ArrayFlag)
				{
					ClearStack();
					PozF.push_back(Poz);
					StackVAR.push_back("NULL");
					Poz++;
					StackVAR.push_back("!F");
					Poz++;
					cout << "Нашёл двоеточие!       позиция: " << Poz<<endl;
					
					if (!Tab_count.empty() && Tab<Tab_count.back() && IfFlag)
					{
						cout<<"						ЛОЛ КЕК WTF!!!"<<endl;
						Tab_count.pop_back();
						int PF=PozF.back();
						StackVAR[PF]=to_string(Poz);
						cout<<"позиция                "<<Poz<<endl;
						cout<<"куда ставим            "<<PF<<endl;
						cout<<"вложенность            "<<IfFlag<<endl;
						cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;

						PozF.pop_back();
						IfFlag--;
					}
				}
			else
				if(Lst->str=="while" || Lst->str=="for")
				{
					Tab_count.push_back(Tab);//вектор хранящий количество табуляций перед ифами
					WhileFlag=true;
					Wcount++;
					PozT.push_back(Poz);
					cout << "Зашёл в while (for)!" << endl;
				}
			else
				if(Lst->str=="\\t")
				{
					Tab=1;
					cout<<"IfFlag            "<<IfFlag<<endl;
					cout<<"ElFlag            "<<ElFlag<<endl;
					cout<<"else_count        "<<Else_count(Lst->next)<<endl<<endl<<endl<<endl;
					while(Lst->next->str=="\\t")
					{
						Tab++;
						Lst=Lst->next;
					}
					cout<<"11       "<<Tab<<endl;
					if (!Tab_count.empty() && Tab==Tab_count.back() && IfFlag )
					{
						cout<<"						what the hell????!!!"<<endl;
						Tab_count.pop_back();
						int PF=PozF.back();
						StackVAR[PF]=to_string(Poz);
						PozF.pop_back();
						IfFlag--;
						Lst=Lst->next;
						continue;
					}
					if (!Tab_count.empty() && Tab==Tab_count.back() && WhileFlag )
					{
						Tab_count.pop_back();
						cout<<"						what the hell123????!!!"<<endl;
						Wcount--;
						if(!IfFlag)
						{
							StackVAR.push_back(to_string(PozT.back()));
							PozT.pop_back();
							Poz++;
							StackVAR.push_back("!");
							Poz++;
						}
						Pozz.push_back(Poz);
						cout<<"						POIZCION "<<Pozz[0]<<endl; 
						Lst=Lst->next;
						continue;
					}
					
					cout <<endl<<endl<< "Пропускаю табуляции!" << endl;
				}
			else
/*****************ЕСЛИ МЫ ПОПАЛИ В КОНЕЦ СТРОКИ*******************************/
				if(Lst->str=="\\n")
				{
					Tab=0;
					ClearStack();
					Buf=StackOP.back();
					while(Buf.name!="@")
					{
						cout << Buf.name << " Последние символы!" << endl;
						StackVAR.push_back(Buf.name);
						Poz++;
						StackOP.pop_back();
						Buf=StackOP.back();
					}
					/*Для функций*/
					if(NowFunc)
					{
						ClearStack();
						StackVAR.push_back(to_string(FuncBuf.PolizPoz));
						Poz++;
						StackVAR.push_back("!");
						Poz++;
						NowFunc=false;
						NowPoz=Poz; /*Возвращаемся из функции!*/
					}
					if(Lst->next!=NULL && Lst->next->str!="\\t" && FuncFlag) /*While-помощник, вставляем позицию*/
					{
						ClearStack();
						StackVAR.push_back("FARG");
						Poz++;
						StackVAR.push_back("!");
						Poz++;
						FuncFlag=false;
					}
					if(Lst->next==NULL && FuncFlag) /*Аналогичный while-помощник для конца файла (не можем считать лексему!)*/
					{
						ClearStack();
						StackVAR.push_back("FARG");
						Poz++;
						StackVAR.push_back("!");
						Poz++;
						FuncFlag=false;
					}
					/*Для if (на этом моменте if-выражение закончилось)*/



/*WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWww*/
					if(Lst->next!=NULL /*здесь нужна функция которая скажет
															 что число табуляций стало меньше или равно тому что
															 было до запуска if*/ && IfFlag  /*&& Tab_count.epty()!=false*/
						 && Check_tab(Lst->next)<=Tab_count.back())
/*WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW*/
					{	
						cout << "			Я ТУТ1666666" << endl;
						cout<<endl<<"          "<<Check_tab(Lst->next)<<endl;
						cout<<"          "<<Tab_count.back()<<endl;
						cout<<"    Check_else   "<<Check_else(Lst)<<endl;
						/*итератор меняем на int переменную и убираем for, т.е. делаем int a=PozF.back(), после этого StackVAR[a]=to_string(Poz);*/
							if(!Check_else(Lst))
							{
								int PF;
								if (IfFlag>(1+Else_count(Lst->next)))
								{
									auto it=PozF.end();
									it-=IfFlag;
									PF=*it;
									PozF.erase(it);
								}
								else
								{
									PF=PozF.back();
									PozF.pop_back();
								}
								cout<<"позиция                "<<Poz<<endl;
								cout<<"куда ставим            "<<PF<<endl;
								cout<<"вложенность            "<<IfFlag<<endl;
								cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;

								StackVAR[PF]=to_string(Poz);
								IfFlag--;	
								Tab_count.pop_back();
							}
					}
					cout<<"IFFALG:    "<<IfFlag<<endl;
					if(Lst->next==NULL && IfFlag)
					{	
						int PF=PozF.back();
						StackVAR[PF]=to_string(Poz);
						cout << "			Я ТУТ228" << endl;
						cout<<"позиция                "<<Poz<<endl;
						cout<<"куда ставим            "<<PF<<endl;
						cout<<"вложенность            "<<IfFlag<<endl;
						PozF.pop_back();
						IfFlag=0;	
					}
					/**/
					/*Для else*/
					if(Lst->next!=NULL && Lst->next->str!="\\t" && ElFlag)
					{
						cout<<endl<<endl<<endl<<endl<<"					FUUUUCCKKKK!!!!!!"<<endl<<endl<<endl;
						for(vector<int>::iterator it=PozT.begin();it!=PozT.end();it++)
							StackVAR[(*it)]=to_string(Poz);
						PozT.pop_back();
						ElFlag--;	
					}
					if(Lst->next==NULL && ElFlag)
					{
						for(vector<int>::iterator it=PozT.begin();it!=PozT.end();it++)
						{
							cout <<endl<< (*it) << "PozF1" << endl;
							StackVAR[(*it)]=to_string(Poz);
						}
						PozT.pop_back();
						ElFlag--;	
					}
					/**/
					/*Для while*/
					if(Lst->next!=NULL && Lst->next->str!="\\t" && WhileFlag) /*While-помощник, вставляем позицию*/
					{	
						int Wcount1=Wcount;
						int i=0;
						while(Wcount1)
						{
							StackVAR.push_back(to_string(PozT.back()));
							PozT.pop_back();
							Poz++;
							StackVAR.push_back("!");
							Poz++;
							Pozz.push_back(Poz);
							Wcount1--;
						}
						auto it1=Pozz.rbegin();
						for (;it1!=Pozz.rend();it1++)
							cout<<"				lol   "<<*it1<<endl;
						it1=Pozz.rbegin();
						for(vector<int>::iterator it=PozF.begin();it!=PozF.end();it++, it1++)
						{
							cout <<"				"<< *(it1) << "PozF1" << endl;
							StackVAR[(*it)]=to_string(*it1);
							i++;
						}
						while(Wcount)
						{
							PozF.pop_back();
							Wcount--;
						}
						WhileFlag=false;
					}
					if(Lst->next==NULL && WhileFlag) /*Аналогичный while-помощник для конца файла (не можем считать лексему!)*/
					{
						int Wcount1=Wcount;
						int i=0;
						while(Wcount1)
						{
							StackVAR.push_back(to_string(PozT.back()));
							PozT.pop_back();
							Poz++;
							StackVAR.push_back("!");
							Poz++;
							Pozz.push_back(Poz);
							Wcount1--;
						}
						auto it1=Pozz.rbegin();
						for(vector<int>::iterator it=PozF.begin();it!=PozF.end();it++, it1++)
						{
							cout << (*it) << "PozF1" << endl;
							StackVAR[(*it)]=to_string(*it1);
							i++;
						}
						while(Wcount)
						{
							PozF.pop_back();
							Wcount--;
						}
						WhileFlag=false;
					}
					if(Lst->next==NULL)
						break;
				}
/*************************************************************************************/
			else
				if(Lst->str=="=" || Lst->str=="+=" || Lst->str=="-=")
				{
					Buf.name=Lst->str;
					Buf.weight=0;
					StackOP.push_back(Buf);
					Check();
				}
			else
				if(Lst->str=="==" || Lst->str=="!=")
				{
					Buf.name=Lst->str;
					Buf.weight=1;
					StackOP.push_back(Buf);
					Check();
				}
			else
				if(Lst->str=="<" || Lst->str=="<=" || Lst->str==">" || Lst->str==">=")
				{
					Buf.name=Lst->str;
					Buf.weight=2;
					StackOP.push_back(Buf);
					Check();
				}
			else
				if(Lst->str=="+" || Lst->str=="-")
				{
					Buf.name=Lst->str;
					Buf.weight=3;
					StackOP.push_back(Buf);
					Check();
					
				}
			else
				if(Lst->str=="*")
				{
					Buf.name=Lst->str;
					Buf.weight=4;
					StackOP.push_back(Buf);
					Check();
				}
			else
				if(Lst->str=="(")
				{
					Buf.name=Lst->str;
					Buf.weight=6;
					StackOP.push_back(Buf);
				}
			else
				if(Lst->str==")")
				{
					ClearStack();
					StackOP.pop_back();
				}
			else
				if(Lst->str=="[")
				{
					ArrayFlag=true;
					StackVAR.push_back("[");
				}
			else
				if(Lst->str=="]")
				{
					ArrayFlag=false;
					StackVAR.push_back("]");
				}
			else
				if(Lst->str=="print" || Lst->str=="input")
				{
					Buf.name=Lst->str;
					Buf.weight=0;
					StackOP.push_back(Buf);
					Check();
				}
			else
				if(Lst->str=="len" || Lst->str=="type" || Lst->str=="str")
				{
					Buf.name=Lst->str;
					Buf.weight=5;
					StackOP.push_back(Buf);
					Check();
				}
			else
			{
				StackVAR.push_back(Lst->str);
				Poz++;
				if(ItIsFunc(Lst->str))
				{
					cout << "			Я нашёл функцию!" << endl;
					NowFunc=true;
					FuncBuf.PolizPoz=ItIsPoz(Lst->str);
					cout << FuncBuf.PolizPoz <<"			Позиция в Полизе" << endl;
					FuncBuf.name=Lst->str;
				}
			}
		Lst=Lst->next;
		cout << "Перехожу к следующему символу!" << endl;
		}
		cout << "Я печатаю главный стек" << endl;
		PrintStackVAR(); /*Печатаем стек переменных (сам ПОЛИЗ)*/
		cout << "Сейчас в векторе элементов: " << Poz << endl;
		return StackVAR;
	};
};
/*Конец ПОЛИЗА*/
