#include "TFormula.h"
#include <cstring>
#include <cctype>
#include <cmath>
#include <stdexcept>

TFormula::TFormula(const char* form)
{
    strncpy_s(Formula, form, MaxLength - 1);
    Formula[MaxLength - 1] = '\0';
    PostfixForm[0] = '\0';
}

int TFormula::GetPriority(char op)
{
    switch (op)
    {
    case '(': return 0;
    case '+':
    case '-': return 1;
    case '*':
    case '/': return 2;
    default: return -1;
    }
}

bool TFormula::IsOperation(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool TFormula::IsDigit(char c)
{
    return std::isdigit(c) || c == '.';
}

void TFormula::ParseNumber(const char* expr, int& pos, char* buf)
{
    int i = 0;
    while (IsDigit(expr[pos]))
    {
        buf[i++] = expr[pos++];
    }
    buf[i] = '\0';
}

int TFormula::FormulaChecker(int brackets[][2], int& size)
{
    TStack<int> stack; 
    int errorCount = 0;
    size = 0;

    for (int i = 0; Formula[i] != '\0'; i++)
    {
        if (Formula[i] == '(')
        {
            stack.Put(i + 1);
        }
        else if (Formula[i] == ')')
        {
            if (!stack.IsEmpty())
            {
                int openIdx = stack.Get();
                brackets[size][0] = openIdx;
                brackets[size][1] = i + 1;
                size++;
            }
            else
            {
                brackets[size][0] = 0;
                brackets[size][1] = i + 1;
                size++;
                errorCount++;
            }
        }
    }

    while (!stack.IsEmpty())
    {
        int openIdx = stack.Get();
        brackets[size][0] = openIdx;
        brackets[size][1] = 0;
        size++;
        errorCount++;
    }

    return errorCount;
}

int TFormula::FormulaConverter()
{
    TStack<char> opStack; 
    int outPos = 0;       

    for (int i = 0; Formula[i] != '\0'; i++)
    {
        if (Formula[i] == ' ')
            continue;

        if (IsDigit(Formula[i]))
        {
            char numBuf[32];
            int pos = i;
            ParseNumber(Formula, pos, numBuf);
            for (int j = 0; numBuf[j] != '\0'; j++)
            {
                PostfixForm[outPos++] = numBuf[j];
            }
            PostfixForm[outPos++] = ' '; 
            i = pos - 1; 
        }
        else if (Formula[i] == '(')
        {
            opStack.Put('(');
        }
        else if (Formula[i] == ')')
        {
            while (!opStack.IsEmpty() && opStack.Top() != '(')
            {
                PostfixForm[outPos++] = opStack.Get();
                PostfixForm[outPos++] = ' ';
            }
            if (!opStack.IsEmpty() && opStack.Top() == '(')
            {
                opStack.Get();
            }
        }
        else if (IsOperation(Formula[i]) && Formula[i] != '(' && Formula[i] != ')')
        {
            int curPriority = GetPriority(Formula[i]);
            while (!opStack.IsEmpty() && GetPriority(opStack.Top()) >= curPriority)
            {
                PostfixForm[outPos++] = opStack.Get();
                PostfixForm[outPos++] = ' ';
            }
            opStack.Put(Formula[i]);
        }
        else
        {
            return -1;
        }
    }

    while (!opStack.IsEmpty())
    {
        PostfixForm[outPos++] = opStack.Get();
        PostfixForm[outPos++] = ' ';
    }

    PostfixForm[outPos] = '\0';
    return 0;
}

double TFormula::FormulaCalculator()
{
    TStack<double> numStack;
    char buf[32];
    int bufPos = 0;

    for (int i = 0; PostfixForm[i] != '\0'; i++)
    {
        if (PostfixForm[i] == ' ')
            continue;

        if (IsDigit(PostfixForm[i]))
        {
            bufPos = 0;
            while (IsDigit(PostfixForm[i]))
            {
                buf[bufPos++] = PostfixForm[i++];
            }
            buf[bufPos] = '\0';
            numStack.Put(std::atof(buf));
            i--;
        }
        else if (IsOperation(PostfixForm[i]))
        {
            if (numStack.IsEmpty())
                throw std::runtime_error("Not enough operands");

            double b = numStack.Get();
            double a = numStack.Get();

            switch (PostfixForm[i])
            {
            case '+': numStack.Put(a + b); break;
            case '-': numStack.Put(a - b); break;
            case '*': numStack.Put(a * b); break;
            case '/':
                if (b == 0.0)
                    throw std::runtime_error("Division by zero");
                numStack.Put(a / b);
                break;
            default:
                throw std::runtime_error("Unknown operation");
            }
        }
        else
        {
            throw std::runtime_error("Invalid character in postfix form");
        }
    }

    if (numStack.IsEmpty())
        throw std::runtime_error("No result");

    double result = numStack.Get();

    if (!numStack.IsEmpty())
        throw std::runtime_error("Too many operands");

    return result;
}