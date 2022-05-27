// modular.cpp

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

#define base 10

class bigint {
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
    string toString() const;
    void print();
};

//base_val^exp_val mod mod_val - square-and-multiply method
bigint modPow(bigint base_val, bigint exp_val, bigint mod_val){
    bigint result(1);

    //reduce base initially
    base_val = base_val % mod_val;

    bigint zero(0);
    bigint one(1);
    bigint two(2);

    //loop until exponent becomes zero
    while(exp_val > zero){
        //checking if exponent is odd
        bigint exp_mod_2 = exp_val % two;

        if(exp_mod_2 == one){
            result = (result * base_val) % mod_val;
        }

        //dividing exponent by 2
        exp_val = exp_val / two;

        //squaring base
        base_val = (base_val * base_val) % mod_val;
    }

    return result;
}

bigint extendedGCD(bigint a, bigint b, bigint &x, bigint &y){
    bigint zero(0);
    bigint one(1);

    //base case
    if(a == zero){
        x = zero;
        y = one;
        return b;
    }

    bigint x1, y1;

    // recursive call
    bigint gcd_val = extendedGCD(b % a, a, x1, y1);

    //updating x and y using previous values
    x = y1 - (b / a) * x1;
    y = x1;

    return gcd_val;
}


//Greatest Common Divisor
bigint gcd(bigint a, bigint b){
    bigint zero(0);

    //making both numbers positive
    a.isNegative = 0;
    b.isNegative = 0;

    //iterative euclidean algorithm
    while(b != zero){
        bigint temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}


//find x such that (a * x) % m == 1
bigint modInverse(bigint a, bigint m){
    bigint zero(0);
    bigint one(1);

    //making values positive
    a.isNegative = 0;
    m.isNegative = 0;

    //checking if inverse exists
    bigint g = gcd(a, m);
    if(g != one){
        //inverse does not exist
        return zero;
    }

    bigint x, y;

    //calling extended gcd to find coefficients
    extendedGCD(a, m, x, y);

    //making x positive
    x = (x % m + m) % m;

    return x;
}


