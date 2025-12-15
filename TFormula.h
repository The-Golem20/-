#ifndef __TFORMULA_H__
#define __TFORMULA_H__

#include "TStack.h"
#include <string>

const int MaxLength = 255;

class TFormula
{
private:
	char Formula[MaxLength];      
	char PostfixForm[MaxLength];  

	int GetPriority(char op);             
	bool IsOperation(char c);               
	bool IsDigit(char c);                   
	void ParseNumber(const char* expr, int& pos, char* buf); 

public:
	TFormula(const char* form = "");

	int FormulaChecker(int brackets[][2], int& size); 
	int FormulaConverter();                           
	double FormulaCalculator();                       

	const char* GetFormula() const { return Formula; }
	const char* GetPostfixForm() const { return PostfixForm; }
};

#endif 