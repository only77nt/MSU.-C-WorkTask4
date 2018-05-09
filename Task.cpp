#include "stdafx.h" /*Тут лежат все библиотеки, мой файл!*/
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

/**************************************************************************************************/
void Argum(WordList *Lst);

type_var Check( string word);


void Check_var(WordList Lst, string word);


void Check_print(WordList *Lst);

void Check_if(WordList *Lst);

/*************************************************************************************************/

/*Функция поиска в первой таблице сепараторов*/
bool IsInTableSep1(string str, string* mas){
	int i=0;
	while(i<countSep1)
	{
		if(mas[i]==str)
			return true;
		i++;
	}
	return false;
}

/*Функция поиска во второй таблице сепараторов*/
bool IsInTableSep2(string str, string* mas){
	int i=0;
	while(i<countSep2)
	{
		if(mas[i]==str)
			return true;
		i++;
	}
	return false;
}

/*Функция поиска в таблице пробелов,табуляций,\n*/
bool IsInTableSpace(string str, string* mas){
	int i=0;
	while(i<countSpace)
	{
		if(mas[i]==str)
			return true;
		i++;
	}
	return false;
}
 
/*Создание и добавление в конец списка новой лексемы*/
WordList create(string s,WordList a)
{
	string str;
	WordList d,b=a;
	d=new(Node);
	d->str=s;
	/*Проверяем тип лексемы*/
	d->type_l=LEX_NULL;
	d->type_sem=SEM_NULL;
	if(IsInTableSep1(s,Separators1))
	{
		d->type_l=LEX_SEP;
		d->type_sem=SEM_SEP;
	}
	if(IsInTableSep2(s,Separators2))
	{
		if(s=="=")
		{
			d->type_l=LEX_EQ;
			d->type_sem=SEM_SEP;
		}
		else
			if(s=="<" || s==">")
			{
				d->type_l=LEX_LOG;
				d->type_sem=SEM_SEP;
			}
			else
			{
				d->type_l=LEX_SEP;
				d->type_sem=SEM_SEP;
			}
	}
	if(s=="(")
	{
		d->type_l=LEX_LB;
		d->type_sem=SEM_LB;
	}
	if(s==")")
	{
		d->type_l=LEX_RB;
		d->type_sem=SEM_RB;
	}
	if((s[0]=='+' && s[1]=='=')||(s[0]=='-' && s[1]=='=')||(s[0]=='*' && s[1]=='='))
	{
		d->type_l=LEX_EQ;
		d->type_sem=SEM_SEP;
	}
	if((s[0]=='<' && s[1]=='=')||(s[0]=='>' && s[1]=='=')||(s[0]=='=' && s[1]=='=')||(s[0]=='!' && s[1]=='='))
	{
		d->type_l=LEX_LOG;
		d->type_sem=SEM_SEP;
	}
	if(s=="\\n")
	{
		d->type_l=LEX_ENDL;
		d->type_sem=SEM_ENDL;
	}
	if(s=="\\t")
	{
		d->type_l=LEX_TAB;
		d->type_sem=SEM_TAB;
	}
	if(s=="def")
	{
		d->type_l=LEX_DEF;
		d->type_sem=SEM_VAL;
	}
	if(s=="print")
	{
		d->type_l=LEX_PRINT;
		d->type_sem=SEM_VAL;
	}
	if(s=="input")
	{
		d->type_l=LEX_INPUT;
		d->type_sem=SEM_VAL;
	}
	if(s=="for" || s=="in" || s=="range")
	{
		d->type_l=LEX_FOR;
		d->type_sem=SEM_VAL;
	}
	if(s=="if" || s=="elif" || s=="else")
	{
		d->type_l=LEX_IF;
		d->type_sem=SEM_IF;
	}
	if(s=="type")
	{
		d->type_l=LEX_TYPE;
		d->type_sem=SEM_VAL;
	}
	if(s=="str")
	{
		d->type_l=LEX_STRR;
		d->type_sem=SEM_VAL;
	}
	if(s=="while")
	{
		d->type_l=LEX_WHILE;
		d->type_sem=SEM_VAL;
	}
	if(s=="len")
	{
		d->type_l=LEX_LEN;
		d->type_sem=SEM_VAL;
	}
	if(s[0]=='"')
	{
		d->type_l=LEX_STR;
		d->type_sem=SEM_VAL;
	}
	if(s[0]=='\'')
	{
		d->type_l=LEX_CHAR;
		d->type_sem=SEM_VAL;
	}
	if(d->type_l==LEX_NULL)
	{
		if(s[0]>='0' && s[0]<='9')
		{
			d->type_l=LEX_CONST;
			d->type_sem=SEM_VAL;
		}
		else
		{
			d->type_l=LEX_VAR;
			d->type_sem=SEM_VAL;
		}
	}
	/*Закончили*/
	d->next=NULL;
	if (a==NULL) 
		return d;
	else 
		while ( b->next != NULL ) 
			b=b->next;
		b->next=d;//в b мы прошли до конца списка
	return a;
}

/*Функция удаления пробелов и \0 из списка лексем*/
bool Clean_L(WordList *a)
{
	WordList b=NULL;
	if(!(*a))
		return false;
	if((*a)->str[0] == '\0' || (*a)->str[0] == ' ')
	{
		if((*a)->next==NULL)
        {
            delete(*a);
            *a=NULL;
            return true;
        }
		else
		{
			b=(*a);
			(*a)=(*a)->next;
			delete(b);
			return true;
		}
	}
	else
		Clean_L(&(*a)->next);
}

/*Функция печати списка*/
void Print_L(WordList a)
{
	while(a!=NULL)
	{
		cout << a->str << " " << a->type_l << " " << a->type_sem << endl; /*Если нужно - убираем печать типа лексемы*/
		a=a->next;
	}
}

/*Функция считающая кол-во строк в файле*/
int Endl_count(WordList a)
{
	int count=0;
	while(a!=NULL)
	{
		if(a->str=="\\n")
			count++;
		a=a->next;
	}
	return count;
}

/*Функция поиска конца строки*/
bool Serch_end(string word, string end)
{
	if (end!="\\n")
		return false;
	string mas[]={".",";","%","*","=","+","-",">","<","<=",">="};
	for(int i=0; i<=10; i++)
		if(mas[i]==word)
			return true;
			
	return false;
}

/*КЛАСС СКАНЕР*/
class Scanner {
private:
	WordList List=NULL;

public:
    WordList GetWordList() { return List; }

	Scanner(const char* Filebuf){
		
		WordList SepList=NULL;
		char in[512];
		char buf[512];
		int flag=0;

		string a,strbuf;
		ifstream file(Filebuf);
		while(file.getline(buf,512)){		
			int i=0;
			int j=0;
			int m=0;
			if(buf[i]=='\0') /*Пропускаем все бессмысленные \n*/
				continue;
			if(buf[i]=='#') /*Ловим комментарии с # в начале строки*/
			{
				List=create("\\n",List);
				continue;
			}
			if(buf[i]=='\'' && buf[i+1]=='\'' && buf[i+2]=='\'') /*Ловим комментарии с ''' (Если перед ними есть пробелы -> проблема)*/
			{
				while(file.getline(buf,512))
				{
					if(buf[i]=='\'' && buf[i+1]=='\'' && buf[i+2]=='\'')
					{
						file.getline(buf,512);
						break;
					}
				}
			}
			if(buf[i]=='\t')
			{
				while(buf[m]=='\t')
					m++;
				if(buf[m]=='\0')
					continue;
				if(buf[m]=='\'' && buf[m+1]=='\'' && buf[m+2]=='\'' || buf[m]=='#')
					continue;
			}
		/*Начинаем разбор строки*/
			while(buf[i]!='\0') /*Ловим комментарии с # в конце строки*/
			{	
	
				a=buf[i];
				if(buf[i]=='#') /*Убираем комменты*/
				{
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					break;
				}
				if(IsInTableSpace(a,Spacing))
				{
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					i++;
					continue;
				}
	
				if(buf[i]=='\t')
				{
					while(buf[i]=='\t')
					{
						List=create("\\t",List);
						i++;
					}
				}
			
				if(buf[i]=='"')
				{
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					in[j]='"';
					i++;
					j++;
					while(buf[i]!='"')
					{
						if (buf[i]=='\0')
							Error("Ошибка с балансом двойных кавычек!");
						in[j]=buf[i];
						i++;
						j++;
					}
					in[j]='"';
					j++;
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					i++;
					continue;
				}
				if(buf[i]=='\'')
				{
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					in[j]='\'';
					i++;
					j++;
					while(buf[i]!='\'')
					{
						if (buf[i]=='\0')
							Error("Ошибка с балансом одиночных кавычек!");
						in[j]=buf[i];
						i++;
						j++;
					}
					in[j]='\'';
					j++;
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					i++;
					continue;
				}
			
				if(IsInTableSep1(a,Separators1)) /*Выявляем все скобки, двоеочия, точки с запятой, запятые*/
				{
					in[j]='\0';
					List=create(in,List);
					memset(&in, 0, 512);
					j=0;
					List=create(a,List);
					i++;
					continue;
				}
				else
				{
					if(IsInTableSep2(a,Separators2)) /*"Выявляем все "двойные" разделители*/
					{
						in[j]='\0';
						List=create(in,List);
						memset(&in, 0, 512);
						j=0;
						strbuf=a;
						i++;
						a=buf[i];
						if(IsInTableSep2(a,Separators2))
						{
							strbuf+=a;
							if((strbuf[0]=='<' || strbuf[0]=='>') && (strbuf[1]=='+' || strbuf[1]=='-'))
							{
								a=strbuf[0];
								List=create(a,List);
								a=strbuf[1];
								List=create(a,List);
							}
							else
								List=create(strbuf,List);
							i++;
						}
						else
						{	/*Добавляем одиночные разделители*/
							List=create(strbuf,List);
						}
						continue;
					}
					else
					{
						in[j]=buf[i];
						i++;
						j++;
					}
				}
			}		
			in[j]='\0';
			List=create(in,List);
			memset(&in, 0, 512);
			j=0;
			List=create("\\n",List);
		}
		while(Clean_L(&List));
		}
};

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
	//cout<<Lex->type_sem<<"!!!!!!!!!"<<SEM_VAL<<endl;
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
			/*b.name="str";
			b.type=NUMBER;
			funk.push_back(b);
			b.name="len";
			b.type=NUMBER;
			funk.push_back(b);
			b.name="type";
			b.type=STRING;
			funk.push_back(b);*/
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
								//cout << Pcount[FuncNum]<<"Колво параметров в функции"<<endl;
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
										//cout << Lex->str << "?????" << endl;
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
									//cout << Lex->str << "?????" << endl;
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
						//cout << Lex->str<<" "<<Lex->type_l<<" "<<Lex->type_sem<<endl;
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
						/*if (Ls->str=="\\n")
							break;*/
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
				//cout << Pcount[FuncNum]<<"Колво параметров в функции"<<endl;
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
			//cout<<" 7 7 7 77  77 7 7 "<<Lst->str<<endl;
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
		/*if (Lst->str!="if" && Lst->str!="else" && Lst->str!="elif")
			return -1;*/
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
		//cout << Lst->str << "POLIZ!" << endl;
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
						//Tab_count.pop_back();
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

						//PozF.pop_back();
						//IfFlag--;
					}
				}
			else
				if(Lst->str=="else")
				{
					ClearStack();
					/*if (Tab<Tab_count.back())
					{
					}*/
					cout<<"позиция                "<<Poz<<endl;
					//cout<<"куда ставим            "<<PF<<endl;
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
					if (Tab!=Tab_count.back())
					{
						auto it=PozF.end();
						cout<<"LOL"<<endl;
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
			else
				if(Lst->str==",")
				{
					cout << "Нашёл запятую!" << endl;
					ClearStack(); /****************************** Точно? ***********/
				}
			else
				if(Lst->str==":" && !ArrayFlag)
				{
					//cout << "Нашёл двоеточие!        " << Poz<<endl;
					
					ClearStack();
					PozF.push_back(Poz);
					StackVAR.push_back("NULL");
					Poz++;
					StackVAR.push_back("!F");
					Poz++;
					cout << "Нашёл двоеточие!       позиция: " << Poz<<endl;
					//cout<<"						ЛОЛ КЕК ЧЕБУРЕК!!!"<<Poz<<endl;
					
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
					/*cout<<"				TAb "<<Tab<<"   "<<Tab_count.back()<<"  Icount "<<Icount<<" Ifflag "<<IfFlag<<endl;
					if (!Tab_count.empty() && Tab==Tab_count.back() && IfFlag && Icount>1)
					{
						cout<<"						ЛОЛ КЕК ЧЕБУРЕК2!!!"<<endl;
						//Tab_count.pop_back();
						int PF;
						auto it=PozF.end();
						cout<<"LOL LOL    "<<*(it-1)<<"   "<<*(it-2)<<endl;
						it-=2;
						PF=*it;
						PozF.erase(it);
						StackVAR[PF]=to_string(Poz);
						cout<<"позиция                "<<Poz<<endl;
						cout<<"куда ставим            "<<PF<<endl;
						cout<<"вложенность            "<<IfFlag<<endl;
						cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;

						PozF.pop_back();
						//IfFlag--;
					}*/
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
					//cout<<"22       "<<Tab_count.back()<<endl;
					//cout<<"				TAb "<<Tab<<"   "<<Tab_count.back()<<"  Wcount "<<Wcount<<" Ifflag "<<WhileFlag<<endl;
					if (!Tab_count.empty() && Tab==Tab_count.back() && IfFlag && ElFlag)
					{
						cout<<"						what the hell????!!!"<<endl;
						Tab_count.pop_back();
						int PF=PozF.back();
						StackVAR[PF]=to_string(Poz);
						PozF.pop_back();
						IfFlag--;
					}
					if (!Tab_count.empty() && Tab==Tab_count.back() && WhileFlag)
					{
						Tab_count.pop_back();
						cout<<"						what the hell123????!!!"<<endl;
						Wcount--;
						StackVAR.push_back(to_string(PozT.back()));
						PozT.pop_back();
						Poz++;
						StackVAR.push_back("!");
						Poz++;
						Pozz.push_back(Poz);
						cout<<"						POIZCION "<<Pozz[0]<<endl; 
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
					/**/
					/*Для if (на этом моменте if-выражение закончилось)*/



/*WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWww*/
					if(Lst->next!=NULL /*здесь нужна функция которая скажет
															 что число табуляций стало меньше или равно тому что
															 было до запуска if*/ && IfFlag  /*&& Tab_count.epty()!=false*/
						 && Check_tab(Lst->next)<=Tab_count.back())
/*WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW*/
					{	
						//auto it1=Tab_count.begin();
									
						cout << "			Я ТУТ1666666" << endl;
						cout<<endl<<"          "<<Check_tab(Lst->next)<<endl;
						cout<<"          "<<Tab_count.back()<<endl;
						cout<<"    Check_else   "<<Check_else(Lst)<<endl;
						/*if(Lst->next->str=="elif")
						{
							ClearStack();
							PozT.push_back(Poz);
							StackVAR.push_back("NULL");
							Poz++;
							StackVAR.push_back("!");
							Poz++;
							//IfFlag=true; хз зачем это!!!
						}
						/*if(Check_else(Lst))
						{
							cout<<"встретили else  "<<endl;
							ClearStack();
							PozT.push_back(Poz);
							StackVAR.push_back("NULL");
							Poz++;
							StackVAR.push_back("!");
							Poz++;
							Lst=Lst->next->next;
							ElFlag++;	
						}
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
						//cout<<"количество else        "<<Else_count(Lst->next)<<endl<<endl;
						PozF.pop_back();
						IfFlag=0;	
					}
					/**/
					/*Для else*/
					if(Lst->next!=NULL && Lst->next->str!="\\t" && ElFlag)
					{
						cout<<endl<<endl<<endl<<endl<<"					FUUUUCCKKKK!!!!!!"<<endl<<endl<<endl;
						for(vector<int>::iterator it=PozT.begin();it!=PozT.end();it++)
						{
							//cout << (*it) << "PozF1" << endl;
							StackVAR[(*it)]=to_string(Poz);
						}
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
					/*if(Check_tab(Lst->next)==Tab_count.back())
						IfFlag--;
					/**/
					/*Buf.name="	#";
					Buf.weight=-1;
					StackVAR.push_back(Buf.name);
					Poz++;*/
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

struct ForExecute
{
	string name;
	int ValueInt;
	string ValueStr;
};

/*Выполнение*/
class Executor{
	private:
		friend void Error(char* buf);
	public:
	vector<string> StackVAR;
	vector<ForExecute> StackBuf;
	vector<string> Functions;
	vector<string> Argum1;
	vector<string> Argum2;
	string buf1, buf2, bufstr, typebuf;
	int bufint;
	void Check(string name) /*Проверяет имена на совпадение и удаляет его если нашёл совпадение*/
	{
		for(vector<ForExecute>::iterator it=StackBuf.begin();it!=StackBuf.end();it++)
		{	
			if((*it).name==name)
			{
				StackBuf.erase(it); /*erase ~ удаление элемента*/
				return;
			}
		}
	}
	int FindInt(string name) /*Ищет в стеке и передаёт значение типа int*/
	{	
		for(vector<ForExecute>::iterator it=StackBuf.begin();it!=StackBuf.end();it++)
		{
			if((*it).name==name && (*it).ValueInt!=-99999)
			{
				return (*it).ValueInt;
			}
		}
		return -99999;
	}
	string FindStr(string name) /*Ищет в стеке и передаёт значение типа int*/
	{   
		for(vector<ForExecute>::iterator it=StackBuf.begin();it!=StackBuf.end();it++)
		{
			if((*it).name==name && (*it).ValueStr!="#")
			{
				return (*it).ValueStr;
			}
		}
		return "#";
	}
	string ClearString(string str,char c)
	{
		if(str[0]==c)
		{
			str.erase(str.find(c), 1);
			str.erase(str.find(c), 1);
		}
		return str;
	}
	bool ItIsFunc(string name)
	{
		for(vector<string>::iterator it=Functions.begin();it!=Functions.end();it++)
		{
			if((*it)==name)
			{
				return true;
			}
		}
		return false;
	}
	string Exchange(int Poz)
	{
		for(vector<string>::iterator it=Argum1.begin();it!=Argum1.end();it++)
		{
			if(Poz==0)			
				return (*it);	
			else
				Poz--;		
		}
	}
	int FindArgum(string name)
	{
		int Poz=0;
		for(vector<string>::iterator it=Argum2.begin();it!=Argum2.end();it++)
		{
			if((*it)==name)
			{
				return Poz;
			}
			Poz++;
		}
		return -1;
	}

	void Execution(vector<string> Poliz)
	{
		//buf1=Poliz.back();
		//cout << "buf1= " << buf1 << endl;
		int IntBuf;
		int StrLen;
		string FuncBuf;
		string StrBuf;
		int FuncPoz=0;
		bool FuncFlag=false;
		for(vector<string>::iterator it=Poliz.begin();it!=Poliz.end();it++)
		{	
			//cout << (*it) << " лекс" << endl;
				/*for(vector<ForExecute>::iterator it=StackBuf.begin();it!=StackBuf.end();it++)
			{
				cout << (*it).name << " " << (*it).ValueInt << endl;
			}*/
			if((*it)=="@")
			{
				it++;
				Functions.push_back((*it));
				it++;
				while((*it)!="$")
				{
					Argum2.push_back((*it));
					it++; /*Пропустили имя функции*/
				}
				it++; /*Мы за $*/
				while((*it)!="!")
					it++; /*Пропускаем тело функции*/
			}	
			else
				if((*it)=="FARG")
				{
					FuncFlag=false;
					Argum1.clear(); /*Удаляем все временные аргументы*/
					it=Poliz.begin();
					//cout << FuncPoz-1 << "FARG FuncPoz" << endl;
					advance(it,FuncPoz-1);
					continue;
				}
			else
				if(ItIsFunc((*it))) /*Записываем аргументы функции*/
				{   //cout << "Записываем аргументы!" << endl;
					FuncFlag=true;
					it++;
					while((*(it+1))!="!")
					{
						FuncBuf=(*it);
						if(FindInt(FuncBuf)>=0)
							FuncBuf=to_string(FindInt(FuncBuf));
						if(FindStr(buf1)!="#")
							FuncBuf=FindStr(FuncBuf);	
						Argum1.push_back(FuncBuf);
						it++;
					}
					it++;
					it++;
					//cout << (*it) << "$$$" << endl;
					FuncPoz=(it-Poliz.begin());
					//cout << FuncPoz << " FuncPoz" << endl;
					it--;
					it--;
					it--;
					//cout << "Записали!" << endl;
				}
			else
				if((*it)=="!")
					{
						buf2=StackVAR.back(); /*Метка перехода*/
						StackVAR.pop_back();
						//cout << buf2 << " buf2!!!!!" << endl;
						it=Poliz.begin();
						advance(it,(stoi(buf2)-1)); /*Advance - перемещает НА столько позиций иттератор*/
						//cout << (stoi(buf2)) << "Куда прыгнули!!!!!" << endl;
						//cout << (*it) << "!!!!!" << endl;
						//cout << "Прыгаем!" << endl;
						continue;
					}
			else
				if((*it)=="!F")
				{
					buf2=StackVAR.back(); /*Метка перехода*/
					StackVAR.pop_back();
					buf1=StackVAR.back(); /*Результат*/
					StackVAR.pop_back();
					//cout << stoi(buf1) << " buf1" << endl;
					if(stoi(buf1)<=0)
					{
						it=Poliz.begin();
						advance(it,(stoi(buf2)-1)); /*Advance - перемещает НА столько позиций иттератор*/
						//cout << "Advance!" << endl;
					}
					//cout << (stoi(buf2)) << "!!!!!" << endl;
					//cout << (*it) << "!!!!!" << endl;
					continue;
				}
			else
				if((*it)=="input")
				{
					continue;
				}
			else
				if((*it)=="str")
				{
					typebuf=StackVAR.back();
					StackVAR.pop_back();
					if(FindInt(typebuf)>=0)
					{
						StrBuf=to_string(FindInt(typebuf));
						StrBuf.insert(0,"\"");
						StrBuf.push_back('"');
						StackVAR.push_back(StrBuf);
					}
					else
						if(typebuf[0]!='\'' && typebuf[0]!='"')
						{
							StrBuf=typebuf;
							StrBuf.insert(0,"\"");
							StrBuf.push_back('"');
							StackVAR.push_back(StrBuf);
						}
				}
			else
				if((*it)=="len")
				{
					typebuf=StackVAR.back();
					StackVAR.pop_back();
					if(FindInt(typebuf)>=0)
						StackVAR.push_back("1");
					else
						if(FindStr(typebuf)!="#")
						{
							StrBuf=FindStr(typebuf);
							StrLen=StrBuf.length()-2;
							StackVAR.push_back(to_string(StrLen));
						}
					else
						if(typebuf[0]=='\'' || typebuf[0]=='"')
						{
							StrBuf=FindStr(typebuf);
							StrLen=StrBuf.length()-2;
							StackVAR.push_back(to_string(StrLen));
						}
						else
							StackVAR.push_back("1");	
				}
			else
				if((*it)=="type")
				{
					//cout << "Захожу в type!" << endl;
					typebuf=StackVAR.back();
					StackVAR.pop_back();
					if(FindInt(typebuf)>=0)
						StackVAR.push_back("int");
					else
						if(FindStr(typebuf)!="#")
						{
							StrBuf=FindStr(typebuf);
							if(StrBuf[0]=='\'')
								StackVAR.push_back("char");
							else
								StackVAR.push_back("string");
						}
					else
						if(typebuf[0]=='\'')
								StackVAR.push_back("char");
						else
							if(typebuf[0]=='"')
								StackVAR.push_back("string");
							else
								StackVAR.push_back("int");	
					//cout << typebuf << " TypeBuf" << endl;				
				}
			else
				if((*it)=="print")
				{	
					vector<string>::iterator it1=StackVAR.begin();
					while(!StackVAR.empty()) /*empty() - пустой или нет?*/
					{
						it1=StackVAR.begin();
						buf1=(*it1);
						StackVAR.erase(it1);
						//cout << buf1[0] << " нулевое" << endl;
						if(FindArgum(buf1)!=-1)
						{
							buf1=Exchange(FindArgum(buf1));
							//cout << buf1 << " Buf1" << endl;
							//cout << "НАШЁЛ2!" << endl;
						}
						//cout << buf1 << endl;
						if(FindInt(buf1)>=0)
						{	
							IntBuf=FindInt(buf1);
							//cout << "!!!!!" << endl;
							cout << IntBuf << endl;
						}
						else
							if(FindStr(buf1)!="#")
							{	
								StrBuf=FindStr(buf1);
								StrBuf=ClearString(StrBuf,'\'');
								StrBuf=ClearString(StrBuf,'"');
								cout << StrBuf << endl;
							}
						else
						{	
							if(buf1[0]=='"' || buf1[0]=='\'')
							{
								buf1=ClearString(buf1,'\'');
								buf1=ClearString(buf1,'"');
							}
							cout << buf1 << endl;
						}
					}
				}
			else
				if((*it)=="+" || (*it)=="-" || (*it)=="*" || (*it)=="=" || (*it)==">" || (*it)=="<" || (*it)=="==" || (*it)=="!=" || (*it)==">=" || (*it)=="<=")
				{
					buf2=StackVAR.back();
					StackVAR.pop_back();
					buf1=StackVAR.back();
					StackVAR.pop_back();
					if(FindArgum(buf1)!=-1)
					{
						buf1=Exchange(FindArgum(buf1));
						//cout << "НАШЁЛ1!" << endl;
					}
					if(FindArgum(buf2)!=-1)
					{
						buf2=Exchange(FindArgum(buf2));
						//cout << "НАШЁЛ3!" << endl;
					}	
					//cout << "buf1= " << buf1 << " buf2= " << buf2<< endl;
					if((*it)=="=") /*=*/
					{
						ForExecute buf; /**/
						buf.name=buf1;
						buf.ValueInt=-99999;
						buf.ValueStr="#";
						Check(buf1);
						if(buf2[0]>='0' && buf2[0]<='9')
							buf.ValueInt=stoi(buf2); /*stoi ~ stirng -> int*/
						else
							buf.ValueStr=buf2;
						//cout << buf.ValueInt << " Значение!" << endl;
						//cout << buf.name << " Buf" << endl;
						StackBuf.push_back(buf);
					}
					if(FindInt(buf1)>=0)
						buf1=to_string(FindInt(buf1));
					if(FindStr(buf1)!="#")
						buf1=FindStr(buf1);
					if(FindInt(buf2)>=0)
						buf2=to_string(FindInt(buf2));
					if(FindStr(buf2)!="#")
						buf2=FindStr(buf2);
					if((*it)=="+" || (*it)=="-" || (*it)=="*" || (*it)==">" || (*it)=="<" || (*it)=="==" || (*it)=="!=" || (*it)==">=" || (*it)=="<=")
					{
						if((buf1[0]=='"' || buf1[0]=='\'') && (buf2[0]=='"' || buf2[0]=='\'')) /*Удаляем кавычки, кидаем строку*/
						{
							if(buf1[0]=='"')
							{
								buf1.erase(buf1.rfind('"'), 1);
								//buf1.erase(buf1.find('"'), 1);
							}
							else
							{
								buf1.erase(buf1.find('\''), 1);
								buf1.erase(buf1.find('\''), 1);
								buf1.insert(0,"\"");
							}
							if(buf2[0]=='"')
							{
								buf2.erase(buf2.find('"'), 1);
								//buf2.erase(buf2.find('"'), 1);
							}
							else
							{
								buf2.erase(buf2.find('\''), 1);
								buf2.erase(buf2.find('\''), 1);
								buf2.push_back('"');
							}
							bufstr=buf1+buf2;
							StackVAR.push_back(bufstr);
							//cout << bufstr << " BUFFER!" << endl;
						}
						else
							if((buf1[0]=='"' || buf1[0]=='\'') && buf2[0]>='0' && buf2[0]<='9' && (*it)=="*") /*Удаляем кавычки, кидаем строку*/
							{
								if(buf1[0]=='"')
								{
									buf1.erase(buf1.find('"'), 1);
									buf1.erase(buf1.find('"'), 1);
								}
								else
								{
									buf1.erase(buf1.find('\''), 1);
									buf1.erase(buf1.find('\''), 1);
								}
								int Itter=stoi(buf2);
								//cout << Itter << " Itter" << endl;
								for(int i=0; i!=Itter;i++)
									bufstr+=buf1;
								//cout << bufstr << " bufstr" << endl;
								StackVAR.push_back(bufstr);
								//cout << bufstr << " BUFFER!" << endl;
							}
						else
							if((buf2[0]=='"' || buf2[0]=='\'') && buf1[0]>='0' && buf1[0]<='9' && (*it)=="*") /*Удаляем кавычки, кидаем строку*/
							{
								if(buf2[0]=='"')
								{
									buf2.erase(buf2.find('"'), 1);
									buf2.erase(buf2.find('"'), 1);
								}
								else
								{
									buf2.erase(buf2.find('\''), 1);
									buf2.erase(buf2.find('\''), 1);
								}
								int Itter=stoi(buf1);
								for(int i=0; i!=Itter;i++)
									bufstr+=buf2;
								StackVAR.push_back(bufstr);
								//cout << bufstr << " BUFFER!" << endl;
							}
						else
							if(buf2[0]>='0' && buf2[0]<='9' && buf1[0]>='0' && buf1[0]<='9') /*Кидаем число*/
							{
								if((*it)=="+")
									bufint=stoi(buf1)+stoi(buf2);
								if((*it)=="-")
									bufint=stoi(buf1)-stoi(buf2);
								if((*it)=="*")
									bufint=stoi(buf1)*stoi(buf2);
								if((*it)==">")
									bufint=stoi(buf1)>stoi(buf2);
								if((*it)=="<")
									bufint=stoi(buf1)<stoi(buf2);
								if((*it)=="==")
									bufint=stoi(buf1)==stoi(buf2);
								if((*it)=="!=")
									bufint=stoi(buf1)!=stoi(buf2);
								if((*it)==">=")
									bufint=stoi(buf1)>=stoi(buf2);
								if((*it)=="<=")
									bufint=stoi(buf1)<=stoi(buf2);
								//cout<< bufint << endl;
								StackVAR.push_back(to_string(bufint));
							}
					}
				}
			else
			{
				/*if((*it)[0]=='\'')
				{
					(*it).erase((*it).find('\''), 1);
					(*it).erase((*it).find('\''), 1);
				}*/
				StackVAR.push_back((*it));
			}
		}
	}
};

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
 В программе написанной на Питоне лучше не допускать лишних \n,
потому что обработчик ошибок тогда будет выдавать неверную строку!
 Цикл for может идти только по неотрицательным счётчикам!
 В Полизе операции str, type и len имеют наивысший приоритет, поэтому нужно соблюдать порядок и заключать всё в скобочки для правильной работы!
 Если Вы хотите вызвать функцию, лучше описать её тело прямо над вызовом.
*/


/*Нужно рекурсивным спуском сделать проверку выражений*/
