#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>
#include <ctime>

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


bigint modPow(bigint base_val, bigint exp_val, bigint mod_val);
bigint gcd(bigint a, bigint b);

// Generate a random bigint
bigint randomBigInt(int bits){
    srand(time(NULL));

    //generate random number as string, approximation of bit length
    int digits = (bits + 2) / 3;
    if(digits < 1)
        digits = 1;

    string numStr;

    //first digit should be non-zero
    numStr += ('1' + (rand() % 9));

    int i {0};
    for(i = 1; i < digits; i++){
        numStr += ('0' + (rand() % base));
    }

    bigint result(numStr);
    result.isNegative = 0;
    return result;
}
// Returns true if n is probably prime acc to Fermat Primality Test
bool isPrimeFermat(bigint n, int iterations){
    bigint zero(0);
    bigint one(1);
    bigint two(2);
    bigint three(3);

    if(n <= one)
        return false;
    if(n == two || n == three)
        return true;
    if(n % two == zero)
        return false;

    //make n positive
    n.isNegative = 0;

    srand(time(NULL));

    int i {0};
    for(i = 0; i < iterations; i++){
        //generate random a in [2, n-2]
        bigint a(2 + (rand() % 100));
        if(a >= n - two){
            a = two;
        }

        //check gcd(a, n)
        bigint g = gcd(a, n);
        if(g != one && g != n){
            //composite number
            return false;
        }

        //compute a^(n-1) mod n
        bigint n_minus_1 = n - one;
        bigint result = modPow(a, n_minus_1, n);

        if(result != one){
            //composite number
            return false;
        }
    }

    //prob prime
    return true;
}

// Miller-Rabin Primality Test
bool isPrimeMillerRabin(bigint n, int iterations){
    bigint zero(0);
    bigint one(1);
    bigint two(2);
    bigint three(3);


    if(n <= one)
        return false;
    if(n == two || n == three)
        return true;
    if(n % two == zero)
        return false;

    //make n positive
    n.isNegative = 0;

    //n-1 as d * 2^r
    bigint d = n - one;
    int r {0};
    bigint d_copy = d;

    while(d_copy % two == zero){
        d_copy = d_copy / two;
        r++;
    }
    d = d_copy;

    srand(time(NULL));

    int i {0};
    for(i = 0; i < iterations; i++){
        //generate random a in [2, n-2]
        bigint a(2 + (rand() % 100));
        if(a >= n - two){
            a = two;
        }

        bigint x = modPow(a, d, n);

        if(x == one || x == n - one){
            //prob prime for this witness
            continue;
        }

        bool composite {true};
        int j {0};

        for(j = 0; j < r - 1; j++){
            x = modPow(x, two, n);
            if(x == n - one){
                composite = false;
                break;
            }
        }

        if(composite){
            //definitely composite
            return false;
        }
    }

    //prob prime
    return true;
}


// Generate a random prime number with specified number of bits
bigint generatePrime(int bits, bool useMillerRabin){
    bigint candidate;
    int max_attempts {1000};
    int attempts {0};

    while(attempts < max_attempts){
        candidate = randomBigInt(bits);

        //make sure the candidate is odd
        bigint two(2);
        if(candidate % two == bigint(0)){
            candidate = candidate + bigint(1);
        }

        bool isPrime {false};

        //choose primality test
        if(useMillerRabin){
            isPrime = isPrimeMillerRabin(candidate, 10);
        }else{
            isPrime = isPrimeFermat(candidate, 10);
        }

        if(isPrime){
            return candidate;
        }

        attempts++;
    }

    //fallback if no prime was found
    return bigint(3);
}


