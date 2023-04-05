#include<stdio.h>

#include "math.h"

double sqrt(double number, int degree)
{
	double number_result= 1.0f;

	int flg = 1;
	while (flg)
	{
		double result = 1.0f;

		for (int i = 0; i < degree; i++)
		{
			result *= number_result;
		}

		if (result< number+0.00001f && result > number - 0.00001f)
			break;

		number_result -= 0.000001f;	

		if (number_result == 0.0f)
			flg = 0;
	}

	return number_result;
}

int findSubModule(int num1, int num2)
{
	int module = 0;

	if (num1 >= num2)
		module = num1 - num2;
	else
		module = num2 - num1;

	return module;
}