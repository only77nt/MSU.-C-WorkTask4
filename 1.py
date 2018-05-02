#!/usr/bin/env python3 
def DoTwo(num1,num2):
		print ( "%d + %d = %d" % (num1,num2, num1+num2)) #Lol
#первая строка указывает что нужно компилировать при помощи интерпритатора питон3
print ("Hello World!!!")
name = input("What is your name?\n")
print ("HELLO there, " + name + "!")
strng = 'Nikita'
print(strng*2) 
print("printf")
var =2+1
print(type(var))
print(var)
	#privet
print(str(var)*5) 
#внутри этого принта мы сделали var типа char после выхода изменения не сохраняются
for i in range(-2,3):
	print("new print:   ", i)
		'''цикл идет по вызовам пока перед ними есть табуляции или пробелы'''
month =['янв', 'февр', 'март', 'апр', 'май','июнь','июль','август','сентябрь','октябрь','ноябрь','декабрь']
print (month[0:2])
if len(month)==1:
	print("first if")
elif len(month)>2:
	print("next if")
else:
	print("last if")
'''Example
'''
mas1 ="CMC msu best faculty"
mas2='in the world'
mas1=mas1+mas2
print(mas1)
print(type(mas2))
print (type(month))
DoTwo(1,2)
