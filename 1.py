#!/usr/bin/env python3 

#первая строка указывает что нужно компилировать при помощи интерпритатора питон3
print ("Hello World!!!")
name = input("What is your name?\n")
print ("HELLO there, " + name + "!")
strng = 'Nikita'
print(strng) 
print("printf")
var =2+1
print(type(var))
print(var)
	#privet
print(str(var)*5) 
a=3
b=4
c=1
d=4
e=3
#внутри этого принта мы сделали var типа char после выхода изменения не сохраняются
for i in range(1,3):
	print("new print:   ", i)
	for i in range(3,5):
		a=b+c
	c=d+e
month =['янв', 'февр', 'март', 'апр', 'май','июнь','июль','август','сентябрь','октябрь','ноябрь','декабрь']
print (month[0:2])
'''
цикл идет по вызовам пока перед ними есть табуляции или пробелы
'''
while 1+3>2:
	a=b+c
	while 2>3:
		b=e+c
	c=b+d
a=b+c
if len(month)==1:
	print("first if")
	a=b+c
elif len(month)>2:
	print("next if")
else:
	print("last if")
'''
Example
'''
mas1 ="CMC msu best faculty"
mas2='in the world'
mas1=mas1+mas2
print(mas1)
print(type(mas2))
print (type(month))
'''
будет реализован:
массив(список)
функции
вывод 
ввод
инициализация переменнных
циклы и условные операторы
возможность коментировать 
возможно специальные возможности вывода такие как *2 чтобы вывелось 2 раза и т.п.
возможно функции str,len,type
возможно добавление других возможностей:)
'''
