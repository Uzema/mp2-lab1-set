// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
const int f = 8;

TBitField::TBitField(int len)
{
    if (len < 1) {
        throw "Incorrect length";
    }

    BitLen = len;
    MemLen = (len + sizeof(TELEM) * f - 1) / (sizeof(TELEM) * f);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n < 0) || (n >= BitLen)) {
        throw "Wrong index";
    }

    return n / (sizeof(TELEM) * f);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n < 0) || (n >= BitLen)) {
        throw "Wrong index";
    }

    return static_cast<TELEM>(1) << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n > BitLen)) {
        throw "Wrong index";
    }

    TELEM bitmask = GetMemMask(n);
    int index = GetMemIndex(n);

    pMem[index] = pMem[index] | bitmask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen)) {
        throw "Wrong index";
    }

    TELEM bitmask = GetMemMask(n);
    int index = GetMemIndex(n);

    pMem[index] = pMem[index] & (~bitmask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n > BitLen)) {
        throw "Wrong index";
    }

    TELEM bitmask = GetMemMask(n);
    int index = GetMemIndex(n);

    if ((pMem[index] & bitmask) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }

    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return ~(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField res = TBitField(std::max(BitLen, bf.BitLen));

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > bf.MemLen) {
        for (int i = std::min(MemLen, bf.MemLen); i < std::max(MemLen, bf.MemLen); i++) {
            res.pMem[i] = pMem[i];
        }
    }
    else {
        for (int i = std::min(MemLen, bf.MemLen); i < std::max(MemLen, bf.MemLen); i++) {
            res.pMem[i] = bf.pMem[i];
        }
    }

    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField res = TBitField(std::max(BitLen, bf.BitLen));

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < BitLen; i++) {
        if (this->GetBit(i) == 1) {
            res.ClrBit(i);
        }
        else {
            res.SetBit(i);
        }
    }

    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char ch;
    for (int i = 0; i < bf.BitLen; i++) {

        istr >> ch;

        if (ch == '1') {
            bf.SetBit(i);
        }
        else if (ch == '0') {
            bf.ClrBit(i);
        }
        else {
            throw "invalid input";
        }
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i) << " ";
    }

    return ostr;
}
