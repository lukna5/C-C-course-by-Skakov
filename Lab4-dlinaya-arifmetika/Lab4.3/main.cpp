#define _CRT_SECURE_NO_WARNINGS
#include "LN.h"
#include <stdlib.h>
#include <stack>

/*
	Задачи:
		+1) убрать usingи
		2) убрать лишние методы
		+3) добавить исключения
		+4) Добавить NaNы и операции с ними
		+5) Проверить эвкивалентность 0 и -0
		+6) из long long со значением по умолчанию: 0,
		+7) из строки символов C (const char *),
		+8) из строки std::string_view.
		+9) конструктор перемещения,
		10) Проверить все операции тестами
		+11) Почистить от мусорных переменных
		12) Проверить варнинги
		13) поменять везде инты на size_ или long long int
		+14) перемещения;
		+15) преобразования типа в: long long (с генерацией исключения в случае, когда значение не умещается), bool (неравенство нулю);
		+16) создания из литерала произвольной длины с суффиксом _ln (например, должно работать выражение: LN x; x = 123_ln;).
		+17) че за include guards
		18) % и / и ~ с 0 и знаки
		19) Проверить умножение, деление, корень
		+20) Добавить const куда надо
		21) Проверить перемещение и ln
*/
FILE* in;
int main(int argc, char** argv)
{
	try {
		if (argc != 3)
		{
			printf("Not enough arguments");
			return 1;
		}
		in = fopen(argv[1], "r");
		if (in == NULL) {
			std::cout << "Cant open reading file";
			return 1;
		}
		long long int count = 0;
		long long int counter = 0;
		long long int size = 0;
		long long int realSize = 2;
		std::stack<LN> stack;
		while (!feof(in)) {
			char symb = getc(in);
			char next;
			char* temp1;
			switch (symb)
			{
			case(-1): break;
			case(10): continue;
			case('!'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				symb = getc(in);
				stack.push(LN(x != y));
				break;
			}
			case('<'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				symb = getc(in);
				if (symb == '=')
				{
					stack.push(LN(x <= y));
				}
				else {
					stack.push(LN(x < y));
				}
				LN x1 = stack.top();
				break;
			}
			case('='): {
				symb = getc(in);
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				stack.push(LN(x == y));
				break;
			}
			case('>'): {
				symb = getc(in);
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				if (symb == '=')
				{
					stack.push(LN (x >= y));
				}
				else {
					stack.push(LN (x > y));
				}
				if ((int)symb == -1) break;
				break;
			}

			case('+'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				stack.push(x + y);
				break;
			}

			case('-'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				stack.push(x - y);
				break;
			}
			case('*'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				stack.push(x * y);
				break;
			}
			case('/'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				stack.push(x / y);
				break;
			}
			case('%'): {
				LN x, y;
				y = stack.top();
				stack.pop();
				x = stack.top();
				stack.pop();
				stack.push(x % y);
				break;
			}
			case('_'): {
				-stack.top();
				break;
			}
			case('~'): {
				LN x = stack.top();
				stack.pop();
				stack.push(~x);
				break;
			}

			default: {
				char* buf = (char*)malloc(2 * sizeof(char));
				if (buf == NULL)
				{
					throw 1;
				}
				buf[size++] = symb;
				while (((int)(symb = getc(in))) != 10) {
					if (symb == -1)
					{
						break;
					}
					buf[size++] = symb;
					if (size + 1 >= realSize)
					{
						realSize *= 2;
						char* temp = (char*)realloc(buf, realSize * sizeof(char));
						if (temp == NULL)
						{
							free(buf);
							throw 1;
						}
						buf = temp;
					}
				}
				for (int i = 0; i < size / 2; i++)
				{
					char temp = buf[i];
					buf[i] = buf[size - i - 1];
					buf[size - i - 1] = temp;
				}
				LN a(buf, size);
				stack.push(a);
				size = 0;
				realSize = 2;
			}
			}
		}
		fclose(in);
		FILE* output = fopen(argv[2], "w");
		if (output == NULL)
		{
			std::cout << "Cant open file for writing";
			throw 2;
		}
		while (stack.empty() != 1)
		{
			LN x(stack.top());
			stack.pop();
			x.print(output);
		}
		fclose(output);
	}

	catch (int e) {
		if (e == 1)
		{
			fclose(in);
			std::cout << "Can't allocate memory";
			return 2;
		}
		if (e == 2)
		{
			std::cout << "Can't allocate memory for file for writing";
			return 2;
		}
		if (e == 3)
		{
			std::cout << "Can't cast LN number into long long int(overflow)";
			return 2;
		}
	}


}


