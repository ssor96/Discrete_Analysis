#include <cstdio>
#include <cstring>

template <class T> class TVector {

private:
    static const int MinCapacity = 4;
    int Size;
    int Capacity;
    T *Data;
    int GetOptimalCapacity (int newCap,int oldCap) {
        int res = oldCap;
        while (res < newCap) {
            res *= 2;
        }
        while ( (res / 2 > newCap) && (res > MinCapacity) ) {
            res /= 2;
        }
        return res;
    }

public:
    T& operator [] (int idx) const {
        if (idx >= Size || idx < 0) {
//            printf("LOL: idx = %d ; size = %d\n",idx,Size );
            throw;
        }
        return Data[idx];
    }
    T* Begin() const {
        return Data;
    }
    TVector () {
        Size = 0;
        Capacity = MinCapacity;
        Data = new T [Capacity];
    }
    void PushBack (T value){
        Resize (Size + 1);
        Data[Size - 1] = value;
    }
    int GetSize () {
        return Size;
    }
    void Resize (int newSize) {
        int optimalCap = GetOptimalCapacity (newSize,Capacity);
        if (optimalCap > Capacity){
            T *tmp = new T [optimalCap];
            for (int i = 0;i < Capacity;i++) {
                tmp[i] = Data[i];
            }
            Capacity = optimalCap;
            delete [] Data;
            Data = tmp;
        } else if (optimalCap<Capacity) {
            T *tmp = new T [optimalCap];
            for (int i = 0;i < newSize;i++) {
                tmp[i] = Data[i];
            }
            Capacity = optimalCap;
            delete [] Data;
            Data = tmp;
        }
        Size = newSize;
    }
    ~TVector () {
        delete [] Data;
        Capacity = Size = 0;
    }
};

class TBigInt {
private:
    typedef short int TYPE;
    static const int BucketSize = 10000;
    TVector <TYPE> Data;
    int Lenght;
    void deleteFrontZeroes () {
        while (Lenght > 1 && Data[Lenght - 1] == 0) {
            --Lenght;
        }
        Data.Resize(Lenght);
    }
    TBigInt sum(TBigInt& num1, TBigInt& num2) {
        TBigInt res;
        res = num1;
        int remain = 0;
        int temp, i;
        for (i = 0; i < num2.Lenght;++i) {
            temp = res.Data[i] + num2.Data[i] + remain;
            remain = temp / BucketSize;
            res.Data[i] = temp % BucketSize;
        }
        while (remain && i < res.Lenght) {
            if (res.Data[i] == BucketSize - 1) {
                res.Data[i++] = 0;
            }
            else {
                res.Data[i]++;
                remain = 0;
            }
        }   
        if (remain) {
            res.Data.PushBack(1);
            res.Lenght++;
        }
        return res;
    }
    TBigInt* operator / (int num) {
        if (num >= BucketSize) {
            TBigInt temp(num);
            TBigInt* res = new TBigInt;
            *(res) = (*this) / temp;
            return res;  
        }
        if (num <= 0) {
            throw "Zero division";
        }
        int remain = 0;
        TBigInt* res = new TBigInt;
        res -> Lenght = Lenght;
        res -> Data.Resize(Lenght); 
        for (int i = Lenght - 1; i >= 0; --i) {
            remain = remain * BucketSize + Data[i];
            res -> Data[i] = remain / num;
            remain %= num;
        }
        res -> deleteFrontZeroes();
        return res;
    }
    TBigInt* mult(const TBigInt& num1, const TBigInt& num2) {
        TBigInt* res = new TBigInt;
        res -> Lenght = num1.Lenght * num2.Lenght + 1;
        res -> Data.Resize(res -> Lenght);
        memset(res -> Data.Begin(),0,res -> Lenght * sizeof(TYPE));
        unsigned long long int remain;
        for (int i = 0; i < num2.Lenght; i++) {
            remain = 0;
            if (!num2.Data[i]) {
                continue;
            }
            for (int j = 0; j < num1.Lenght; j++) {
                remain += num2.Data[i] * num1.Data[j] + res -> Data[i + j];
                res -> Data[i + j] = remain % BucketSize;
                remain /= BucketSize;
            }
            res -> Data[i + num1.Lenght] = remain;
        }
        res -> deleteFrontZeroes();
        return res;
    }
    TBigInt* selfMult (TBigInt* num1,TBigInt* num2) {
        TBigInt* res = mult(*(num1),*(num2));
        delete num1;
        num1 = res;
        return num1;
    }
    TBigInt* pow (TBigInt* num) {
        if (num -> Lenght == 1 && !num -> Data[0]) {
            TBigInt* res = new TBigInt;
            res -> Lenght = 1;
            res -> Data[0] = 1;
            return res;
        }
        if (num -> Data[0] & 1) {
            TBigInt* res;
            num -> Data[0]--;
            res = pow(num);
            return selfMult(res,this);
        }
        else {
            TBigInt* res;
            num = *(num) / 2;
            res = pow(num);
            delete num;
            return selfMult(res,res);
        }
    }
    void cp(TBigInt& num) {
        Lenght = num.Lenght;
        Data.Resize(Lenght);
        memcpy(Data.Begin(),num.Data.Begin(),Lenght*sizeof(TYPE));
    }
    void leftShift() {
        Lenght++;
        Data.Resize(Lenght);
        for (int i = Lenght - 1; i > 0; --i) {
            Data[i] = Data[i - 1];
        }
    }
public:
    TBigInt () {
        Lenght = 1;
        Data.Resize(1);
        Data[0] = 0;
    }
    TBigInt (const TBigInt& num) {
        Lenght = num.Lenght;
        Data.Resize(Lenght);
        memcpy(Data.Begin(),&(num.Data[0]),Lenght*sizeof(TYPE));
    }
    TBigInt (TVector <char>& num) {
        Lenght = 0;
        int digit = 0;
        int power = 1;
        for (int i = num.GetSize() - 1;i >= 0; --i) {
            digit = digit + (num[i] - '0') * power;
            power *= 10;
            if (power == BucketSize) {
                Data.PushBack(digit);
                digit = 0;
                power = 1;
                Lenght++;
            }
        }
        if (digit || Lenght == 0) {
            Data.PushBack(digit);
            Lenght++;
        }
        while (Lenght > 1 && Data[Lenght - 1] == 0) {
            Lenght--;
        }
    }
    TBigInt (int num) {
        for (Lenght = 0; num > 0; Lenght++) {
            Data.PushBack (num % BucketSize);
            num /= BucketSize;
        }
        if (num == 0) {
            Lenght = 1;
            Data.Resize(1);
            Data[0] = 0;
        }
    }
    TBigInt& operator = (const TBigInt num) {
        Lenght = num.Lenght;
        Data.Resize(Lenght);
        memcpy(Data.Begin(),num.Data.Begin(),Lenght*sizeof(TYPE));
        return *this;
    }
    bool operator < (TBigInt& num) {
        if (Lenght < num.Lenght) {
            return true;
        }
        else if (Lenght > num.Lenght) {
            return false;
        }
        for (int i = Lenght - 1;i >= 0;--i) {
            if (Data[i] < num.Data[i]) {
                return true;
            }
            else if (Data[i] > num.Data[i]) {
                return false;
            }
        }
        return false;
    }
    bool operator > (TBigInt& num) {
        return num < (*this);
    }
    bool operator == (TBigInt& num) {
        return !((*this) < num || num < (*this));
    }
    TBigInt operator + (TBigInt& num) {
        return Lenght < num.Lenght ? sum(num,*this): sum(*this,num);
    }
    TBigInt operator - (TBigInt& num) {
        if ((*this) < num) {
            throw "Answer is negative";
        }
        TBigInt res;
        res.cp(*this);
        int lend = 0;
        int temp;
        int i;
        for (i = 0;i < num.Lenght; ++i) {
            temp = res.Data[i] - num.Data[i] - lend;
            res.Data[i] = temp;
            if (temp < 0) {
                 lend = 1;
                 res.Data[i] += BucketSize;
            }
            else { 
                lend = 0;
            }
        }
        while (lend) {
            temp = res.Data[i] - lend;
            res.Data[i++] = temp;
            if (temp < 0) {
                 lend = 1;
                 res.Data[i - 1] += BucketSize;
            }
            else { 
                lend = 0;
            }
        }
        res.deleteFrontZeroes();
        return res;  
    }
    TBigInt operator - (int num) {
        if (num >= BucketSize) {
            TBigInt temp(num);
            return (*this) - temp;
        }
        if ((Lenght == 1 && Data[0] < num) || num < 0) {
            throw "Answer is negative";
        }
        TBigInt res;
        res.cp(*this);
        res.Data[0] -= num;
        for (int i = 0; res.Data[i] < 0; i++) {
            res.Data[i] += BucketSize;
            res.Data[i + 1]--;
        }
        res.deleteFrontZeroes();
        return res;
    }
    TBigInt operator * (TBigInt& num) {
        TBigInt* tmp = Lenght > num.Lenght? mult((*this),num):mult(num,(*this));
        TBigInt res;
        res = *(tmp);
        delete tmp;
        return res;
    }
    TBigInt operator * (int num) {
        TBigInt res;
        res.Lenght = Lenght;
        res.Data.Resize(Lenght);
        int temp;
        int remain = 0;
        int i;
        for (i = 0; i < Lenght; ++i) {
            temp = Data[i] * num + remain;
            res.Data[i] = temp % BucketSize;
            remain = temp / BucketSize;
        }
        if (remain) {
            res.Data.Resize(++res.Lenght);
            res.Data[res.Lenght - 1] = remain;
        }
        res.deleteFrontZeroes();
        return res;
    }
    TBigInt operator / (TBigInt &num) {
        if (*(this) < num) {
            TBigInt res(0);
            return 0;
        }
        if (num.Lenght == 1) {
            TBigInt* tmp = (*this) / num.Data[0];
            TBigInt res;
            res = *(tmp);
            delete tmp;
            return res;
        }
        int k = BucketSize / (num.Data[num.Data.GetSize() - 1] + 1);
        TBigInt num1 = (*this) * k;
        TBigInt num2 = num * k;
        TBigInt q;
        q.Data.Resize(num1.Data.GetSize());
        q.Lenght = 0;
        TBigInt r;
        r.Lenght = 0;
        int firstDigitNum2 = num2.Data[num2.Data.GetSize() - 1];
        for (int i = num1.Data.GetSize() - 1; i >= 0; --i) {
            r.leftShift();
            r.Data[0] = num1.Data[i];
            int firstDig = r.Lenght < 2 ? 0 : r.Data[r.Lenght - 1];
            int secondDig = r.Lenght == 0 ? 0 : r.Data[r.Lenght - 2 + (r.Lenght == 1)];
            int tempQ = (BucketSize * firstDig + secondDig) / firstDigitNum2;
            q.Lenght++;
            while ((num2 * tempQ) > r) {
                tempQ--;
            }
            q.Data[i] = tempQ;
            if (tempQ != 0) {
                TBigInt tmp;
                tmp = num2 * tempQ;
                r = r - tmp;
            }
        }
        q.deleteFrontZeroes();
        return q;
/*
        TBigInt res;
        res.Lenght = Lenght;
        res.Data.Resize(res.Lenght);
        TBigInt curValue;
        curValue.Lenght = 0;
        for (int i = Lenght - 1; i >= 0; i--) {
            curValue.leftShift();
            curValue.Data[0] = Data[i];
            int x = 0;
            int l = 0;
            int r = BucketSize;
            while (l <= r) {
                int m = (l + r) >> 1;
                TBigInt cur;
                cur = num * m;
                if (cur < curValue || cur == curValue) {
                    x = m;
                    l = m + 1;
                }
                else {
                    r = m - 1;
                }
            }
            res.Data[i] = x;
            TBigInt tmp;
            tmp = num * x;
            curValue = curValue - tmp;
        }
        res.deleteFrontZeroes();
        return res;
*/
    }
    TBigInt operator ^ (TBigInt num) {
        if (num.Lenght == 1 && num.Data[0] == 0) {
            if (Lenght == 1 && Data[0] == 0) {
                throw "undefined result 0^0";
            }
            TBigInt res;
            res.Lenght = 1;
            res.Data[0] = 1;
            return res;
        }
        if (Lenght == 1 && (Data[0] == 0 || Data[0] == 1)) {
            TBigInt res;
            res.Lenght = 1;
            res.Data[0] = Data[0];
            return res;
        }
        TBigInt* tmp = pow(&(num));
        TBigInt res;
        res = *(tmp);
        delete tmp; 
        return res;
    }
    void print() {
        printf("%d",Data[Lenght-1]);
        for (int i = Lenght - 2;i >= 0;--i) {
            printf("%04u",Data[i]);
        }
        printf("\n");
    }
};


void readStr(TVector <char>& string) {
    string.Resize(0);
    for (char c = getchar(); c != '\n' && c != EOF; c = getchar() ) {
        string.PushBack(c);
    }
}

int main() {
    TVector <char> string;
    char c;
    while ( (c = getchar()) != EOF) {
        ungetc(c,stdin);
        readStr(string);
        TBigInt num1(string);
        readStr(string);
        TBigInt num2(string);
        c = getchar();
        try {
            switch (c) {
            case '+':
                (num1 + num2).print();
                break;
            case '-':
                (num1 - num2).print();
                break;
            case '^':
                (num1 ^ num2).print();
                break;
            case '*':
                (num1 * num2).print();
                break;
            case '/':
                (num1 / num2).print();
                break;
            case '<':
                printf("%s\n",num1 < num2?"true":"false");
                break;
            case '>':
                printf("%s\n",num1 > num2?"true":"false");
                break;
            case '=':
                printf("%s\n",num1 == num2?"true":"false");
                break;
            default:
                printf("Error: wrong symbol '%c'\n",c);
            }
        } catch (char const* err) {
            printf("Error\n");
        }       
        getchar();
    }
    return 0;
}
