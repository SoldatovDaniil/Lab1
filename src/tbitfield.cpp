#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0)
    {
        throw "Invalid lenght";
    }
    BitLen = len;
    MemLen = len / (sizeof(TELEM) * 8) + 1;

    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n < 0) || (n >= BitLen))
    {
        throw "Invalid index";
    }

    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return ((TELEM)1) << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
    {
        throw "Index out of range";
    }
    else
    {
        int i = GetMemIndex(n);
        pMem[i] = pMem[i] | GetMemMask(n);
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
    {
        throw "Index out of range";
    }
    pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen)
    {
        throw "Index out of range";
    }

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (&bf == this)
    {
        return *this;
    }
    TELEM* tmp = new TELEM[bf.MemLen];

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    for (int i = 0; i < MemLen; i++)
    {
        tmp[i] = bf.pMem[i];
    }

    delete[] pMem;
    pMem = tmp;

    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen)
    {
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] != bf.pMem[i])
            {
                return 0;
            }
        }
        return 1;
    }

    return 0;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen)
    {
        int count = 0;
        for (int i = 0; i < MemLen; i++)
        {
            if (pMem[i] == bf.pMem[i])
            {
                count++;
            }
        }
        if (count != MemLen)
        {
            return 1;
        }

        return 0;
    }
    return 1;
}
TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField res(0);

    if (BitLen > bf.BitLen)
    {
        res = *this;
    }
    else
    {
        res = bf;
    }

    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
        res.pMem[i] = pMem[i] | bf.pMem[i];

    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField res(std::max(BitLen, bf.BitLen));
    int minLen = std::min(MemLen, bf.MemLen);

    for (int i = 0; i < minLen; i++)
    {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < MemLen - 1; i++)
    {
        res.pMem[i] = ~(pMem[i]);
    }

    for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++)
    {
        if (GetBit(i) == 0)
        {
            res.SetBit(i);
        }
    }

    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    TELEM tmp = 0;

    for (size_t i = 0; i < bf.BitLen; ++i) {
        istr >> tmp;
        if (tmp == 1) {
            bf.SetBit(i);
        }
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
    {
        ostr << bf.GetBit(i);
    }

    return ostr;
}