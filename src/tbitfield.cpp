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
    MemLen = (len + 31) / 32;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
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
    delete pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM res;

    if ((n < 0) || (n > BitLen)) {
        throw "Wrong index";
    }

    if (n < sizeof(TELEM) * f) {
        res = 1 << (sizeof(TELEM) * f - n - 1);
    }
    else {
        res = 0;
    }

    return res;
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

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (BitLen != bf.BitLen) {
        throw "Different bit field sizes";
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

int TBitField::operator==(const TBitField &bf) const // сравнение
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

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 1;
    }

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 1;
        }
    }

    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField res = TBitField(std::max(BitLen, bf.BitLen));

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
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

    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }

    int lastBits = BitLen % (sizeof(TELEM) * f);

    if (lastBits > 0) {
        TELEM mask = (1 << lastBits) - 1;
        res.pMem[MemLen - 1] &= mask;
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.MemLen; i++) {
        istr >> bf.pMem[i];
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.BitLen; i >= 0; i -= 1) {
        ostr << bf.GetBit(i) << " ";
    }
    return ostr;
}
