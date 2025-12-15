#ifndef __TSTACK_H__
#define __TSTACK_H__

#include <iostream>
#include <stdexcept>

const int MaxMemSize = 25;


template<class T>
class TStack
{
protected:
    T* pMem;          
    int MemSize;      
    int DataCount;    
    int Hi;          

   
    virtual int GetNextIndex(int index) { return ++index; }

public:
    TStack(int size = MaxMemSize)
    {
        if (size <= 0)
            size = MaxMemSize;

        MemSize = size;
        pMem = new T[MemSize];
        DataCount = 0;
        Hi = -1;
    }

    TStack(const TStack<T>& stack)
    {
        MemSize = stack.MemSize;
        DataCount = stack.DataCount;
        Hi = stack.Hi;
        pMem = new T[MemSize];
        for (int i = 0; i <= Hi; i++)
            pMem[i] = stack.pMem[i];
    }

    TStack(TStack<T>&& stack) noexcept
        : pMem(stack.pMem), MemSize(stack.MemSize), DataCount(stack.DataCount), Hi(stack.Hi)
    {
        stack.pMem = nullptr;
        stack.MemSize = 0;
        stack.DataCount = 0;
        stack.Hi = -1;
    }

    ~TStack()
    {
        delete[] pMem;
    }

    TStack<T>& operator=(const TStack<T>& stack)
    {
        if (this != &stack)
        {
            delete[] pMem;
            MemSize = stack.MemSize;
            DataCount = stack.DataCount;
            Hi = stack.Hi;
            pMem = new T[MemSize];
            for (int i = 0; i <= Hi; i++)
                pMem[i] = stack.pMem[i];
        }
        return *this;
    }

    TStack<T>& operator=(TStack<T>&& stack) noexcept
    {
        if (this != &stack)
        {
            delete[] pMem;
            pMem = stack.pMem;
            MemSize = stack.MemSize;
            DataCount = stack.DataCount;
            Hi = stack.Hi;

            stack.pMem = nullptr;
            stack.MemSize = 0;
            stack.DataCount = 0;
            stack.Hi = -1;
        }
        return *this;
    }

    T Top() const
    {
        if (IsEmpty())
            throw std::runtime_error("Stack is empty");
        return pMem[Hi];
    }

    void Put(const T& val)
    {
        if (IsFull())
            throw std::runtime_error("Stack is full");

        Hi = GetNextIndex(Hi);
        pMem[Hi] = val;
        DataCount++;
    }

    T Get()
    {
        if (IsEmpty())
            throw std::runtime_error("Stack is empty");

        T val = pMem[Hi];
        Hi--;
        DataCount--;
        return val;
    }

    int IsEmpty() const
    {
        return DataCount == 0;
    }

    int IsFull() const
    {
        return DataCount == MemSize;
    }

    bool operator==(const TStack<T>& stack) const
    {
        if (DataCount != stack.DataCount)
            return false;

        for (int i = 0; i <= Hi; i++)
            if (pMem[i] != stack.pMem[i])
                return false;

        return true;
    }

    bool operator!=(const TStack<T>& stack) const
    {
        return !(*this == stack);
    }

    friend std::ostream& operator<<(std::ostream& os, const TStack<T>& stack)
    {
        for (int i = 0; i <= stack.Hi; i++)
        {
            os << stack.pMem[i] << " ";
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, TStack<T>& stack)
    {
        T val;
        while (is >> val)
        {
            stack.Put(val);
        }
        return is;
    }
};

#endif