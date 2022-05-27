#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

#define base 10

class bigint{

    private:
    vector<int> num;
    void equalize_length(vector<int> &other_num);
    void normalize();

    public:
    bigint long_mul(bigint other);
    int isNegative {0};

    bigint();
    bigint(int n);
    bigint(string n);
    bigint(vector<int> n);

    bool operator>(const bigint &op) const;
    bool operator<(const bigint &op) const;
    bool operator>=(const bigint &op) const;
    bool operator<=(const bigint &op) const;
    bool operator==(const bigint &op) const;
    bool operator!=(const bigint &op) const;
    bigint operator+(bigint op);
    bigint operator-(bigint op);
    bigint operator*(bigint op);
    bigint operator/(bigint op);
    bigint operator%(bigint op);
    std::string toString() const;
    void print();
};

bigint::bigint(){}

bigint::bigint(int n){
    div_t divResult;
    if(n < 0){
        isNegative = 1;
        n = -n;
    }
    while(n > 0){
        divResult = div(n, base);
        num.push_back(divResult.rem);
        n = divResult.quot;
    }
    if(num.size() == 0)
        num.push_back(0);
}

bigint::bigint(string n){
    for(auto i = n.rbegin(); i != n.rend(); ++i){
        if(*i > 47 && *i < 58){
            num.push_back(*i - '0');
        }else if(*i == 45){
            isNegative = 1;
        }
    }
    normalize();
}

bigint::bigint(vector<int> n){
    num = n;
    normalize();
}

void bigint::normalize(){
    //removing leading zeroes
    while(num.size() > 1 && num[num.size() - 1] == 0)
        num.pop_back();

    if(num.size() == 1 && num[0] == 0)
        isNegative = 0;
}

void bigint::equalize_length(vector<int> &other_num){
    int size_diff = num.size() - other_num.size();
    vector<int> *temp;

    if(size_diff < 0){
        temp = &num;
        size_diff = -size_diff;
    }else{
        temp = &other_num;
    }

    vector<int> zero_vector(size_diff, 0);
    temp->insert(temp->end(), zero_vector.begin(), zero_vector.end());
}

bigint bigint::long_mul(bigint other){
    bigint prod;
    div_t divResult;
    prod.num.assign(num.size() + other.num.size() + 1, 0);

    int carry {0}, i {0}, j {0}, temp;

    vector<int>* l {num.size() > other.num.size() ? &num : &other.num};
    vector<int>* s {num.size() <= other.num.size() ? &num : &other.num};

    for(i = 0; i < s->size(); i++){
        for(j = 0; j < l->size(); j++){
            temp = (s->at(i) * l->at(j)) + carry + prod.num.at(i + j);
            if(temp >= base){
                divResult = div(temp, base);
                temp = divResult.rem;
                carry = divResult.quot;
            }else{
                carry = 0;
            }
            prod.num.at(i + j) = temp;
        }
        if(carry != 0)
            prod.num.at(i + j) += carry;
        carry = 0;
    }

    prod.isNegative = isNegative ^ other.isNegative;
    prod.normalize();
    return prod;
}

bool bigint::operator<(const bigint &op) const{
    int i {0};
    bool res;

    if(isNegative != op.isNegative)
        return isNegative;

    if(num.size() < op.num.size()){
        res = true;
    }else if(num.size() == op.num.size()){
        for(i = num.size() - 1; i >= 0; i--){
            if(num.at(i) < op.num.at(i)){
                res = true;
                break;
            }else if(num.at(i) > op.num.at(i)){
                res = false;
                break;
            }
            if(i == 0){
                res = false;
                break;
            }
        }
    }else{
        res = false;
    }

    if(isNegative && op.isNegative)
        res = !res;

    return res;
}

bool bigint::operator>(const bigint &op) const{
    int i {0};
    bool res;

    if(isNegative != op.isNegative)
        return !isNegative;

    if(num.size() > op.num.size()){
        res = true;
    }else if(num.size() == op.num.size()){
        for(i = num.size() - 1; i >= 0; i--){
            if(num.at(i) > op.num.at(i)){
                res = true;
                break;
            }else if(num.at(i) < op.num.at(i)){
                res = false;
                break;
            }
            if(i == 0){
                res = false;
                break;
            }
        }
    }else{
        res = false;
    }

    if(isNegative && op.isNegative)
        res = !res;

    return res;
}

bigint bigint::operator+(bigint op){
    int carry {0}, i {0}, temp;

    if(isNegative && !op.isNegative){
        bigint t = *this;
        t.isNegative = 0;
        return op - t;
    }

    if(!isNegative && op.isNegative){
        bigint t = op;
        t.isNegative = 0;
        return *this - t;
    }

    bigint sum;
    equalize_length(op.num);

    for(i = 0; i < num.size(); i++){
        temp = num.at(i) + op.num.at(i) + carry;
        if(temp >= base){
            temp = temp % base;
            carry = 1;
        }else{
            carry = 0;
        }
        sum.num.push_back(temp);
    }

    if(carry == 1)
        sum.num.push_back(1);

    sum.isNegative = isNegative;
    sum.normalize();
    return sum;
}

bigint bigint::operator-(bigint other){
    int i {0}, carry {0}, temp;
    vector<int> *l, *s;
    bigint diff;

    //handling cases where signs are different
    if(isNegative && !other.isNegative){
        bigint t = *this;
        t.isNegative = 0;
        diff = t + other;
        diff.isNegative = 1;
        return diff;
    }

    if(!isNegative && other.isNegative){
        bigint t = other;
        t.isNegative = 0;
        diff = *this + t;
        return diff;
    }

    //finding which is larger integer and determining the sign of the diffference
    if(*this < other){
        l = &other.num;
        s = &num;
        diff.isNegative = 1;
    }else{
        l = &num;
        s = &other.num;
    }

    for(i = 0; i < l->size(); i++){
        if(i < s->size()){
            temp = l->at(i) - s->at(i) + carry;
            if(temp < 0){
                temp += base;
                carry = -1;
            }else{
                carry = 0;
            }
            diff.num.push_back(temp);
        }else{
            diff.num.push_back(l->at(i) + carry);
            carry = 0;
        }
    }

    diff.normalize();
    return diff;
}


bigint bigint::operator*(bigint op){
    equalize_length(op.num);
    int n = num.size();

    bigint prod;

    if(n < 32){
        prod = long_mul(op);
        return prod;
    }

    int m = n / 2;

    bigint x2(vector<int>(num.begin(), num.begin() + m));
    // x2.print(); cout << endl;

    bigint x1(vector<int>(num.begin() + m, num.end()));
    // x1.print(); cout << endl;

    bigint y2(vector<int>(op.num.begin(), op.num.begin() + m));
    // y2.print(); cout << endl;

    bigint y1(vector<int>(op.num.begin() + m, op.num.end()));
    // y1.print(); cout << endl;

    bigint z0 = x2 * y2;
    // cout << "z0: "; z0.print();

    bigint z2 = x1 * y1;
    // cout << "z2: "; z2.print();

    bigint z1 = (x1 + x2) * (y1 + y2);
    z1 = z1 - z0;
    z1 = z1 - z2;

    vector<int> zeroo(m * 2, 0);
    vector<int> zero1(m, 0);

    z2.num.insert(z2.num.begin(), zeroo.begin(), zeroo.end());
    z1.num.insert(z1.num.begin(), zero1.begin(), zero1.end());

    prod = z2 + z1 + z0;
    prod.isNegative = isNegative ^ op.isNegative;
    prod.normalize();

    return prod;
}

bool bigint::operator==(const bigint &op) const{
    if(isNegative != op.isNegative)
        return false;
    if(num.size() != op.num.size())
        return false;
    for(int i = 0; i < num.size(); i++){
        if(num.at(i) != op.num.at(i))
            return false;
    }
    return true;
}

bool bigint::operator!=(const bigint &op) const{
    return !(*this == op);
}

bool bigint::operator>=(const bigint &op) const{
    return !(*this < op);
}

bool bigint::operator<=(const bigint &op) const{
    return !(*this > op);
}

bigint bigint::operator/(bigint divisor){
    bigint zero(0);

    //division by zero case
    if(divisor == zero){
        //division by zero - return 0 for now
        return zero;
    }

    bigint dividend = *this;
    dividend.isNegative = 0;
    divisor.isNegative = 0;

    //if dividend is smaller than divisor
    if(dividend < divisor){
        return zero;
    }

    bigint quotient;
    bigint remainder;

    quotient.num.clear();

    int i {0};

    //long division algorithm
    for(i = dividend.num.size() - 1; i >= 0; i--){
        //shift remainder left by base
        remainder = remainder * bigint(base);

        //bring down next digit
        remainder = remainder + bigint(dividend.num.at(i));

        int q {0};
        bigint temp(0);

        //finding maximum q such that divisor*q <= remainder
        while(true){
            bigint next = temp + divisor;
            if(next > remainder)
                break;
            temp = next;
            q++;
        }

        quotient.num.insert(quotient.num.begin(), q);
        remainder = remainder - temp;
    }

    quotient.normalize();
    quotient.isNegative = isNegative ^ divisor.isNegative;

    return quotient;
}


bigint bigint::operator%(bigint divisor){
    bigint zero(0);

    //modulo by zero case
    if(divisor == zero){
        //return 0 for now
        return zero;
    }

    bigint dividend = *this;
    dividend.isNegative = 0;
    divisor.isNegative = 0;

    //if dividend is smaller than divisor
    if(dividend < divisor){
        return *this;
    }

    //a % b = a - (a / b) * b
    bigint quotient = dividend / divisor;
    bigint remainder = dividend - (quotient * divisor);

    remainder.isNegative = isNegative;
    remainder.normalize();

    return remainder;
}


std::string bigint::toString() const{
    std::string result;
    vector<int> temp = num;
    
    // Remove trailing zeroes
    while(temp.size() > 1 && temp[temp.size() - 1] == 0)
        temp.pop_back();
    
    if(isNegative && !(temp.size() == 1 && temp[0] == 0))
        result += "-";
    
    for(int j = temp.size() - 1; j >= 0; j--)
        result += std::to_string(temp.at(j));
    
    return result;
}

void bigint::print(){
    std::cout << toString();
}