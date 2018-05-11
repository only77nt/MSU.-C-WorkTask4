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
#include "Poliz.h"
using namespace std;

/**************************************************************************************************/
void Argum(WordList *Lst);

type_var Check( string word);


void Check_var(WordList Lst, string word);


void Check_print(WordList *Lst);

void Check_if(WordList *Lst);

/*************************************************************************************************/

/*Функция ВЫРАЖЕНИЕ*/
void Expression(WordList *Lst)
{
	WordList Lex=*Lst;
	while(1)
	{
		Lex=GetLex(&(*Lst));
		cout << Lex->str << endl;
		if(Lex->type_l==LEX_ENDL || Lex->type_sem==SEM_RB)
		{
			if(Lex->str==")")
			{
				Bcount--;
				if(Bcount<0)
					Error("Правых скобок больше чем, левых!");
			}
			if(Lex->str=="\\n")
			{
				if(Bcount!=0)
					Error("Левых скобок больше, чем правых!");
			}
			cout << "Выхожу из рекурсии!" << endl;
			return;
		}
		if(Lex->type_sem==SEM_SEP)
		{
			if (Lex->str==":" && Lex->next->type_l==LEX_ENDL)
					return;
			if(Lex->str=="]")
			{
				if(Lex->next->type_l==LEX_ENDL)
				{
					return;
				}
				else
					Error("Массив заканчивается чем-то ещё, кроме ]");
			}
			Lex=GetLex(&(*Lst));
			cout << Lex->str << endl;
			if(Lex->type_sem==SEM_VAL)
				continue;
			else
			{
				if(Lex->type_sem==SEM_LB)
				{
					Bcount++;
					Lex=GetLex(&(*Lst));
					cout << Lex->str << " в скобках " << endl;
					if(Lex->type_sem==SEM_VAL)
					{
						Expression(&(*Lst));
					}
					else
					{
						while(Lex->type_sem==SEM_LB)
						{
							Bcount++;
							Lex=GetLex(&(*Lst));
						}
						if(Lex->type_sem==SEM_VAL)
						{
							Expression(&(*Lst));
						}
						else
							Error("После откр. скобки не переменная!");
					}
				}
				else
					Error("После сепаратора не скобка и не переменная!");
			}
		}
		else
			Error("Отсутствует сепаратор после переменной!");		
	}
};


/*Функция для лексемы print*/
void Print(WordList *Lst){
	WordList Lex=*Lst;
	while(1)
	{
		Lex=GetLex(&(*Lst));
		cout << Lex->str << endl;
		if(Lex->type_sem==SEM_RB)
		{
			Bcount--;
			if(Bcount<0)
				Error("Правых скобок больше чем, левых!");
			cout << "Выхожу из рекурсии!" << endl;
			return;
		}
		if(Lex->type_sem==SEM_SEP)
		{
			if(Lex->str=="=")
				Error("Мы не можем присваивать что-то в print!");
			if(Lex->str[0]=='+' && Lex->str[1]=='=')
				Error("Мы не можем присваивать что-то в print!");
			if(Lex->str[0]=='-' && Lex->str[1]=='=')
				Error("Мы не можем присваивать что-то в print!");
			if (Lex->str==":" && Lex->next->type_l==LEX_ENDL)
				return;
			if(Lex->str=="]")
				continue;
			Lex=GetLex(&(*Lst));
			cout << Lex->str << endl;
			if(Lex->type_sem==SEM_VAL)
				continue;
			if(Lex->type_sem==SEM_LB)
			{
				Bcount++;
				Lex=GetLex(&(*Lst));
				cout << Lex->str << " в скобках " << endl;
				if(Lex->type_sem==SEM_VAL)
				{	
					Print(&(*Lst));
				}
				else
					Error("Ошибка во вложенных выражениях!");
			}
		}
		else
			if(Lex->type_sem==SEM_LB)
			{
					Bcount++;
					Lex=GetLex(&(*Lst));
					cout << Lex->str << " в скобках " << endl;
					if(Lex->type_sem==SEM_VAL)
					{	
						Print(&(*Lst));
					}
					else
						Error("Ошибка во вложенных выражениях!");
			}
			else
				Error("Ошибка два сепаратора подряд (либо их нет)!");
			
	}
};

void Argum1(WordList *Lst)
{
	WordList Lex=*Lst;
	if(Pcount[FuncNum]==0)
	{
		if(Lex->type_sem==SEM_RB)
			{	
				Bcount--;
				if(Bcount<0)
					Error("Закрыв. скобок больше, чем открыв.!");
				Lex=GetLex(&(*Lst));
				return;
			}
			else
				Error("После первого аргумента стоит не скобка, и не сепаратор");
	}
	if(Lex->type_sem==SEM_VAL)
	{	
		Pcount[FuncNum]--;
		Lex=GetLex(&(*Lst));
		if(Lex->str==",")
		{
			Lex=GetLex(&(*Lst));
			Argum1(&(*Lst));
		}
		else
			if(Lex->type_sem==SEM_RB)
			{	
				Bcount--;
				if(Bcount<0)
					Error("Закрыв. скобок больше, чем открыв.!");
				if(Pcount[FuncNum]!=0)
					Error("Несоответствие кол-ва параметров функции!");
				Lex=GetLex(&(*Lst));
				return;
			}
			else
				Error("После первого аргумента стоит не скобка, и не сепаратор");
	}
	else
		Error("Аргумент функции не переменная!");
return;
};

bool IsItFuncName(string name, string* mas)
{
int i=0;
FuncNum=0;
	while(mas[i]!="@")
	{
		if(mas[i]==name)
		{
			cout << FuncNum<<"СЕЙЧАС Я В ЭТОЙ ФУНКЦИИ"<<endl;
			return 1;
		}
		i++;
		FuncNum++;
	}
return 0;
}

/*Логическое выражение*/
void BoolExpression(WordList *Lst)
{
	WordList Lex=*Lst;
	while(1)
	{
		Lex=GetLex(&(*Lst));
		cout << Lex->str << endl;
		if(Lex->type_sem==SEM_RB)
		{
			Bcount--;
			if(Bcount<0)
				Error("Правых скобок больше чем, левых!");
			cout << "Выхожу из рекурсии!" << endl;
			return;
		}
		if(Lex->type_sem==SEM_SEP)
		{
			if(Lex->str=="=")
				Error("Равенства нет в Bool-выражениях!");
			if(Lex->str[0]=='+' && Lex->str[1]=='=')
				Error("Равенства нет в Bool-выражениях!");
			if(Lex->str[0]=='-' && Lex->str[1]=='=')
				Error("Равенства нет в Bool-выражениях!");
			if (Lex->str==":" && Lex->next->type_l==LEX_ENDL)
				return;
			if(Lex->str=="]" || Lex->str=="[")
				Error("Массивов не может быть в Bool-выражении!");
			Lex=GetLex(&(*Lst));
			cout << Lex->str << endl;
			if(Lex->type_sem==SEM_VAL)
				continue;
			if(Lex->type_sem==SEM_LB)
			{
				Bcount++;
				Lex=GetLex(&(*Lst));
				cout << Lex->str << " в скобках " << endl;
				if(Lex->type_sem==SEM_VAL)
				{	
					Print(&(*Lst));
				}
				else
					Error("Ошибка во вложенных выражениях!");
			}
		}
		else
			if(Lex->type_sem==SEM_LB)
			{
					Bcount++;
					Lex=GetLex(&(*Lst));
					cout << Lex->str << " в скобках " << endl;
					if(Lex->type_sem==SEM_VAL)
					{	
						Print(&(*Lst));
					}
					else
						Error("Ошибка во вложенных выражениях!");
			}
			else
				Error("Ошибка два сепаратора подряд (либо их нет)!");
			
	}
};

/*КЛАСС ПАРСЕР*/
class Parser {
	private:
		friend WordList GetLex(WordList *Lst);
		friend void Error(char* buf);
		friend void Expression(WordList *Lst);
		friend void Print(WordList *Lst);
		friend void If_pars(WordList *Lst);
		friend void Argum(WordList *Lst);
		friend void Argum1(WordList *Lst);
		friend type_var Check( string word);  
		friend void Check_var(WordList Lst, string word);  
		friend void Check_print(WordList *Lst);
		friend void Check_if(WordList *Lst);
	public:
/*Начало синтаксического анализа*/
		vector<VAR> a;
		vector<VAR> funk;
		Parser(){};
		void ParsList(WordList *Lst)
		{
			VAR b;
			b.name="i";
			b.type=NUMBER;
			a.push_back(b);
			string FuncName[50];
			int i=0;
			int TABcountBuf=1;
			FuncName[0]="@";
			WordList Lex=*Lst;
			while(1)
			{	cout << "Захожу в цикл!" << endl;	
				cout << Lex->str << endl;
				if(Lex->type_l==LEX_ENDL)
				{
					TABcount=0;
					Lex=GetLex(Lst);
					cout << "Готовлюсь выходить из цикла!" << endl;
					ErrorCount++;
					if((*Lst)==NULL)
						return;				
					continue;
				}
				/*Проверка табуляций*/
				if(Lex->type_l==LEX_TAB)
				{
					while(Lex->str=="\\t")
					{
						cout << "FORcount="<<FORcount<< endl;
						cout << "TABcountBUF="<<TABcountBuf<< endl;	
						TABcount++;
						cout << "TABcount="<<TABcount<< endl;						
						Lex=GetLex(Lst);
					}
				}
				if(TABcount==0 && FUNCflag)
				{
					FUNCflag=false;
					a.clear();//очистили вектор для того чтобы после окончания функции были новые переменные
					VAR b;
					b.name="i";
					b.type=NUMBER;
					a.push_back(b);
					cout<<"Я опускаю FUNCflag, тело функции кончилось!   "<<FUNCflag<<endl;
				}
				cout << "FORcount1="<<FORcount<< endl;
				cout << "TABcountBUF1="<<TABcountBuf<< endl;	
				cout << "TABcount1="<<TABcount<< endl;
				if(Lex->str!="elif" && Lex->str!="else")
				{
					if(TABcount==0)
					{
						cout << "Обнулил FORcount" << endl;
						FORcount=0;
					}
					else
						if(TABcount<TABcountBuf)
								FORcount--;	
					cout << "FORcount2="<<FORcount<< endl;
					cout << "TABcountBUF2="<<TABcountBuf<< endl;	
					cout << "TABcount2="<<TABcount<< endl;
					if(TABcount!=FORcount)
						Error("Число табуляций не совпадает со вложенностью FOR(в функции)");
					TABcountBuf=TABcount; /*В буфере лежит предыдущее, в коунтере - нынешнее*/
				}			
				/*Проверка FOR`ОВ*/
				if(Lex->type_l==LEX_FOR)
				{
					if(Lex->str=="for")
					{
						Lex=GetLex(&(*Lst));
						if(Lex->type_l==LEX_VAR)
						{
							Lex=GetLex(&(*Lst));
							if(Lex->str=="in")
							{
								Lex=GetLex(&(*Lst));
								if(Lex->str=="range")
								{
									Lex=GetLex(&(*Lst));
									if(Lex->str=="(")
									{
										Lex=GetLex(&(*Lst));
										if(Lex->type_l==LEX_CONST)
										{
											Lex=GetLex(&(*Lst));
											if(Lex->str==",")
											{
												Lex=GetLex(&(*Lst));
												if(Lex->type_l==LEX_CONST)
												{
													Lex=GetLex(&(*Lst));
													if(Lex->str==")")
													{
														Lex=GetLex(&(*Lst));
														if(Lex->str==":")
														{
															FORcount++;
															if(Lex->next->type_l!=LEX_ENDL)
																Error("В for после : что-то ещё есть!");
															TABcount=0;
															Lex=GetLex(&(*Lst));
															Lex=GetLex(&(*Lst));
															ErrorCount++;
															continue;
														}
														else
															Error("Пропущено :");
													}
													else
														Error("Пропущена закр. скобка!");
												}
												else
													Error("Нет второго предела для цикла!");
											}
											else
												Error("Пропущена запятая в цикле!");
										}
										else
											Error("Это не константа в цикле!");	
									}
									else
										Error("Пропущена отк. скобка!");
								}
								else
									Error("Пропущен range!");
							}
							else
								Error("Пропущен in!");
						}
						else
							Error("Цикл не по переменной!");
					}
					else
						Error("Цикл должен начинаться с for!");
				}
				/*Функции*/				
				if(Lex->type_l==LEX_DEF)
				{
					Lex=GetLex(&(*Lst));
					if(Lex->type_l==LEX_VAR)
					{	
						Lex->type_l=LEX_FUNC;
						FuncName[i]=Lex->str;
						i++;
						FuncName[i]="@";
						cout<<Lex->type_l<< " Я LEX_FUNC /*19*/" <<endl;
						Lex=GetLex(&(*Lst));
						if(Lex->type_sem==SEM_LB)
						{
							Bcount++;
							Lex=GetLex(&(*Lst));
							if(Lex->type_sem==SEM_RB)
							{
								Bcount--;
								if(Bcount<0)
									Error("Закрыв. скобок больше, чем открыв.!");
								Lex=GetLex(&(*Lst));
								if(Lex->str==":")
								{
									if(Lex->next->str!="\\n")
										Error("В функции за : что-то есть!");
									else
									{
										Lex=GetLex(&(*Lst));
										Lex=GetLex(&(*Lst));
										FUNCflag=true; /*Подняли флаг тела функции*/
										cout<<"Я поднимаю FUNCflag!"<<endl;
										cout<<Lex->str<<"ЛЕКСЕМА ТУТ"<<endl;
										if(Lex->type_l!=LEX_TAB)
											Error("Функция с пустым телом!");
										cout << "Я прибавил флаг!" <<endl;
										FORcount++;
										continue;
									}
								}
								else
									Error("Пропущено : после объявления функции!");
							}
							cout<<"Я вхожу в аргумент!"<<endl;
							Argum(&(*Lst));
							Lex=GetLex(&(*Lst));
							if(Lex->str==":")
							{
								if(Lex->next->str!="\\n")
									Error("В функции за : что-то есть!");
								else
								{
									Lex=GetLex(&(*Lst));
									Lex=GetLex(&(*Lst));
									cout<<Lex->str<<"ЛЕКСЕМА ТУТ"<<endl;
									if(Lex->type_l!=LEX_TAB)
										Error("Функция с пустым телом!");
									cout << "Я прибавил флаг!" <<endl;
									FORcount++;
									continue;
								}
							}
							else
								Error("Пропущено : после объявления функции!");
						}
						else
							Error("Пропущена откр. скобка!");
					}
					else
						Error("Имя функции не соответствует!");
				FuncNum++;
				}
				/*Вызов функции*/
				if(IsItFuncName(Lex->str,FuncName))
				{	cout << FuncNum <<" НОМЕР ФУНКЦИИ"<<endl;
					cout<< Pcount[FuncNum]<<"У этой функции сейчас столько аргументов"<<endl;
					Lex=GetLex(&(*Lst));
					if(Lex->type_sem==SEM_LB)
					{
						Bcount++;
						Lex=GetLex(&(*Lst));
						cout<<"Вызываю аргумент1!"<< Lex->str <<endl;
						Argum1(&(*Lst));
						cout << (*Lst)->str<< "!!!!!"<< endl;
						cout<<"Вышёл из аргумента1!"<<endl;						
						if((*Lst)->type_l==LEX_ENDL)
						{
							ErrorCount++;
							FORcount=0;
							TABcount=0;
							Lex=GetLex(&(*Lst));
							cout<<"Выхожу из вызова функции!"<<endl;
							if((*Lst)->next==NULL)
								break;
							continue;
						}
						else
							Error("После объявления функции ещё что-то есть!");
					}
					else
						Error("Пропущена откр. скобка при вызове функции!");
				}
				/*Проверка IF`ОВ*/				
				if(Lex->type_l==LEX_IF)
				{
					cout<<"вошли в IF"<<endl;
					if (Lex->str=="if")
					{
						FORcount++;
						Lex=GetLex(&(*Lst));
						WordList Ls=*Lst;
						BoolExpression(&(*Lst));
						cout<<"LEX   "<<(*Lst)->str<<endl;
						if ((*Lst)->str==":")
							Lex=GetLex(&(*Lst));
						else
							Error("Ошибка в IF");
						Check_if(&Ls);					
					}
					else
						if (Lex->str=="elif")
						{
							FORcount--;
							if (FORcount<0)
								Error("Ошибка в ELIF");
							Lex=GetLex(&(*Lst));
							WordList Ls=*Lst;
							BoolExpression(&(*Lst));
							cout<<"LEX   "<<(*Lst)->str<<endl;
							if ((*Lst)->str==":")
								Lex=GetLex(&(*Lst));
							else
								Error("Ошибка в ELIF");
							Check_if(&Ls);
							FORcount++;						
						}
						else
						{
							FORcount--;
							if (FORcount<0)
								Error("Ошибка в ELSE");
							FORcount++;
							Lex=GetLex(&(*Lst));
							if ((*Lst)->str==":")
								Lex=GetLex(&(*Lst));
							else
								Error("Ошибка в IF");
							if((*Lst)->next->str!="\\t")
								Error("Пустой else!");
						}							
				}
				/*Проверка WHILE`ОВ*/				
				if(Lex->type_l==LEX_WHILE)
				{
					cout<<"вошли в While"<<endl;
					Lex=GetLex(&(*Lst));
					FORcount++;
					WordList Ls=*Lst;
					BoolExpression(&(*Lst)); //BoolExpression(&(*Lst));
					cout<<"LEX   "<<(*Lst)->str<<endl;
					if ((*Lst)->str==":")
						Lex=GetLex(&(*Lst));
					else
						Error("Ошибка в While");
					Check_if(&Ls);
					cout<<Lex->str<<"LEXEMA"<<endl;
				}
				/*Выражение*/			
				if(Lex->type_l==LEX_VAR)
				{	cout << "Зашёл в выражение!" << endl;
					/*ARRAYS*/
					if((*Lst)->next->next->str=="[")
					{
						string word=(*Lst)->str;
						Lex=GetLex(&(*Lst));
						if(Lex->str=="=")
							Lex=GetLex(&(*Lst));
						else
							Error("Ошибка в Arrays1!");
						WordList Ls=*Lst;
						Lex=GetLex(&(*Lst));
						if(Lex->type_sem!=SEM_VAL)
							Error("Массив начинается не с переменной!");
						Expression(&(*Lst));
						Check_var(Ls->next, word);
					}
					else
						/*Input*/
						if(Lex->next->next->type_l==LEX_INPUT)
						{
							string word=(*Lst)->str;
							Lex=GetLex(&(*Lst));
							if(Lex->str=="=")
								Lex=GetLex(&(*Lst));
							else
								Error("Нету равно в input!");
							WordList Ls=*Lst;
							Lex=GetLex(&(*Lst));
							if(Lex->type_sem==SEM_LB)
							{
								Bcount++;
								Lex=GetLex(&(*Lst));
							}
							else
								Error("Нет откр. скобки в input!");
							if(Lex->type_sem!=SEM_VAL)
								Error("Ввод начинается не с переменной!");
							Print(&(*Lst));
							Lex=GetLex(&(*Lst));
							if(Lex->type_sem!=SEM_ENDL)
								Error("После конца input стоит ещё что-то!");
							Check_var(Ls->next->next, word);
						}
						else
						{
							WordList Ls=*Lst;
							Expression(&(*Lst));
							if (Ls->next->str=="=")
								Check_var(Ls->next->next, Ls->str);
							else
								Error("Ошибка после переменной нет операции присваивания!");
						}
				}
				/*Print*/
				if(Lex->type_l==LEX_PRINT)
				{
					Lex=GetLex(&(*Lst));
					if(Lex->type_sem==SEM_LB)
					{
						Bcount++;
						Lex=GetLex(&(*Lst));
					}
					else
						Error("Нет откр. скобки в print!");
					if(Lex->type_sem!=SEM_VAL)
						Error("Печать начинается не с переменной!");
					WordList Ls=(*Lst);
					Print(&(*Lst));
					Lex=GetLex(&(*Lst));
					if(Lex->type_sem!=SEM_ENDL)
						Error("После конца принта стоит ещё что-то!");
					while (Ls->str!="\\n")
					{
						cout<<Ls->str<<endl;
						Check_print(&Ls);
						Ls=Ls->next;
					}
				}
				/*Переход к следующему элементу списка*/
				TABcount=0;
				Lex=GetLex(&(*Lst));
				cout << "Готовлюсь выходить из цикла!" << endl;
				ErrorCount++;
				if((*Lst)==NULL)
					return;				
			}	
		}

void Argum(WordList *Lst)
{
	WordList Lex=*Lst;
	VAR b;
	if(Lex->type_l==LEX_VAR)
	{	cout<<Lex->str<<"@@@@@"<<endl;
		b.name=Lex->str;
		b.type=NUL;
		a.push_back(b);
		Pcount[FuncNum]++;
		Lex=GetLex(&(*Lst));
		if(Lex->str==",")
		{
			Lex=GetLex(&(*Lst));
			Argum(&(*Lst));
		}
		else
			if(Lex->type_sem==SEM_RB)
			{
				Bcount--;
				if(Bcount<0)
					Error("Закрыв. скобок больше, чем открыв.!");
				return;
			}
			else
				Error("После первого аргумента стоит не скобка, и не сепаратор");
	}
	else
		Error("Аргумент функции не переменная!");
};

/**************************************************************************************************/



/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/




void type_inside( vector<type_var> *var, vector<type_var> *sep )
{
	while(1)
	{
		if(((*sep).begin()+1)==(*sep).end())
			return;
		vector<type_var>::reverse_iterator it_var=(*var).rbegin();
		vector<type_var>::reverse_iterator it_sep=(*sep).rbegin();
		if(*(it_var)!=RB || *(it_var)!=LB)
			if(*(it_var+1)!=RB || *(it_var+1)!=LB)
				if(*(it_sep)!=LOG)	
					if(*(it_var)==*(it_var+1))
					{
						(*var).pop_back();
						(*var).pop_back();
						(*sep).pop_back();
						(*var).push_back(*(it_var));
					}
					else
						Error("Ошибка с типами в выражении!");
				else
					if(*(it_var)==*(it_var+1))
					{
						(*var).pop_back();
						(*var).pop_back();
						(*sep).pop_back();
						(*var).push_back(NUMBER);
					}
					else
						Error("Ошибка с типами в выражении!");
			else
			{		
				type_var va=*(it_var);
				type_var se=*(it_sep);		
				(*var).pop_back();
				(*sep).pop_back();
				(*var).pop_back();
				(*sep).pop_back();
				type_inside(var,sep);	//проверит все что внутри скобок
				(*var).push_back(va);
				(*sep).push_back(se);
			}
		else
		{					
			if(*(it_var)==LB)
			{
				(*var).pop_back();
				(*sep).pop_back();
				return;
			}
			(*var).pop_back();
			(*sep).pop_back();
			type_inside(var,sep);	//проверит все что внутри скобок
		} 
	}
}



void do_vect( WordList& Lst, vector<type_var> *var, vector<type_var> *sep )//нужно смотреть все это в одном вайле
{
	while (Lst->str !="\\n")
	{
		if (Lst->type_l==LEX_SEP)
		{
			type_var b=SEP;
			(*sep).push_back(b);
		}
		if (Lst->type_l==LEX_LOG)
		{
			type_var b=LOG;
			(*sep).push_back(b);
		}
		if (Lst->type_l==LEX_RB)
		{
			type_var b=RB;
			(*var).push_back(b);
			(*sep).push_back(b);
		}
		if (Lst->type_l==LEX_LB)
		{
			type_var b=LB;
			(*var).push_back(b);
			(*sep).push_back(b);
		}
		if (Lst->type_l==LEX_STR || Lst->type_l==LEX_CHAR)
		{
			type_var b=STRING;
			(*var).push_back(b);
		}
		if (Lst->type_l==LEX_VAR)
		{
			type_var b=Check(Lst->str);
			(*var).push_back(b);
		}
		if (Lst->type_l==LEX_CONST)
		{
			type_var b=NUMBER;
			(*var).push_back(b);
		}
	}	
	
	
	
	return;
}


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

void Check_funk(WordList *Lst)
{
	int count=1;
	(*Lst)=(*Lst)->next;
	while(count!=0)
	{
		Check_print(&(*Lst));
		if ((*Lst)->str=="(")
			count++;
		if ((*Lst)->str==")")
			count--;
		(*Lst)=(*Lst)->next;
	}
}
/*возвращает тип переменной*/
type_var Check( string word)  
{
	
	for (vector<VAR>::iterator it=a.begin();it!=a.end();it++)
		if((*it).name==word)
			return (*it).type;
	Error("Неизвестная переменная!");
}
/*проверка выражений с присваиванием */
void Check_var(WordList Lst, string word)  
{
	type_var type=NUL;
	cout<<1<<endl;
	type_var typ2=NUL;
	cout<<2<<endl;
	while(Lst->str!="\\n" && Lst->str!=",")
	{
		cout<<3<<endl;
		if (Lst->str=="len" || Lst->str=="str")
			typ2=NUMBER;
		if (Lst->str=="type" )
			typ2=STRING;
		if (Lst->str=="len" || Lst->str=="str" || Lst->str=="type")
			Check_funk(&Lst);
		if (Lst->type_l==LEX_VAR)
			typ2=Check(Lst->str);
		cout<<4<<endl;
		if (Lst->type_l==LEX_CONST)
			typ2=NUMBER;
		if (Lst->type_l==LEX_CHAR || Lst->type_l==LEX_STR)
			typ2=STRING;
		if (typ2!=type && type!=NUL && typ2!=NUL)
			Error("Ошибка с типами в выражении2!");
		type=typ2;
		Lst=Lst->next;
	}
	cout<<5<<endl;
	vector<VAR>::iterator it=a.begin();
	for (;it!=a.end();it++)
		if((*it).name==word)
		{
			(*it).type=type;
			break;
		}
	if(a.end()==it)
	{	
		VAR b;
		b.name=word;
		b.type=type;
		a.push_back(b);
	}

}

/*проверка выражений таких как в принте */
void Check_print(WordList *Lst)
{
	type_var type=NUL;
	type_var typ2=NUL;
	cout<<"LOL"<<endl;
	while(1)
	{
		if ((*Lst)->str=="len" || (*Lst)->str=="str")
			typ2=NUMBER;
		if ((*Lst)->str=="type" )
			typ2=STRING;
		if ((*Lst)->str=="len" || (*Lst)->str=="str" || (*Lst)->str=="type")
			Check_funk(&(*Lst));
		if((*Lst)->str=="\\n" || (*Lst)->str=="," 
			|| (*Lst)->str=="+" || (*Lst)->str==")" 
			||  (*Lst)->str=="[" ||  (*Lst)->str=="]"   )
			break;
		cout<<"*****"<<(*Lst)->str<<endl;
		if ((*Lst)->type_l==LEX_VAR)
			typ2=Check((*Lst)->str);
		if ((*Lst)->type_l==LEX_CONST)
			typ2=NUMBER;
		if ((*Lst)->type_l==LEX_CHAR || (*Lst)->type_l==LEX_STR)
			typ2=STRING;
		if (typ2!=type && type!=NUL  && typ2!=NUL)
			Error("Ошибка с типами в выражении3!");
		type=typ2;
		(*Lst)=(*Lst)->next;
	}
}

/*проверка выражений таких как в  if, while*/
void Check_if(WordList *Lst)
{
	type_var type=NUL;
	type_var typ2=NUL;
	cout<<"LOL"<<endl;
	while(1)
	{
		if ((*Lst)->str=="len" || (*Lst)->str=="str")
			typ2=NUMBER;
		if ((*Lst)->str=="type" )
			typ2=STRING;
		if ((*Lst)->str=="len" || (*Lst)->str=="str" || (*Lst)->str=="type")
			Check_funk(&(*Lst));
		if((*Lst)->str=="\\n" ||  (*Lst)->str=="[" ||  (*Lst)->str=="]"   )
			break;
		cout<<"*****"<<(*Lst)->str<<endl;
		if ((*Lst)->type_l==LEX_VAR)
			typ2=Check((*Lst)->str);
		if ((*Lst)->type_l==LEX_CONST)
			typ2=NUMBER;
		if ((*Lst)->type_l==LEX_CHAR || (*Lst)->type_l==LEX_STR)
			typ2=STRING;
		if (typ2!=type && type!=NUL  && typ2!=NUL)
			Error("Ошибка с типами в выражении3!");
		type=typ2;
		(*Lst)=(*Lst)->next;
	}
}

/*************************************************************************************************/
/*Конец синтаксического анализа*/
};
