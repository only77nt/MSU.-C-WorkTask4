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
							if(buf2[0]>='0' && buf2[0]<='9' && buf1[0]>='0' && buf1[0]<='9' || buf2[0]=='-' || buf1[0]=='-') /*Кидаем число*/
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
								//cout<< bufint << "bufint" << endl;
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

