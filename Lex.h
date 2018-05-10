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
#include "Pars.h"
using namespace std;

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
