// rsa.cpp - RSA encryption implementation

#include "rsa.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

#define base 10

// Forward declarations
bigint modPow(bigint base_val, bigint exp_val, bigint mod_val);
bigint modInverse(bigint a, bigint m);
bigint gcd(bigint a, bigint b);
bool isPrimeMillerRabin(bigint n, int iterations);
bigint generatePrime(int bits, bool useMillerRabin);

// Generate RSA key pair
RSAKeyPair generateKeyPair(int keySize){
    RSAKeyPair keyPair;

    //each prime should be approximately keySize/2 bits
    int primeBits {0};
    primeBits = keySize / 2;

    //generating two large primes p and q using miller-rabin primality test
    bigint p = generatePrime(primeBits, true);
    bigint q = generatePrime(primeBits, true);

    bigint n = p * q;

    //phi(n) = (p - 1) * (q - 1)
    bigint one(1);
    bigint phi_n = (p - one) * (q - one);

    //choosing public exponent e
    //for larger keys, use standard value 65537
    //for smaller keys, use smaller exponent
    bigint e;
    if(keySize >= 512){
        e = bigint(65537);
    }else{
        e = bigint(17);
    }

    //ensure gcd(e, phi(n)) == 1
    bigint g;
    bigint one_val(1);

    g = gcd(e, phi_n);
    while(g != one_val){
        e = e + one;
        g = gcd(e, phi_n);
    }

    //computing private exponent d 
    //e*d ≡ 1 (mod phi(n))
    bigint d = modInverse(e, phi_n);

    keyPair.publicKey_e = e;
    keyPair.publicKey_n = n;
    keyPair.privateKey_d = d;
    keyPair.privateKey_n = n;

    return keyPair;
}


// RSA Encryption: c = m^e mod n
bigint encrypt(bigint message, bigint e, bigint n) {
    return modPow(message, e, n);
}

// RSA Decryption: m = c^d mod n
bigint decrypt(bigint ciphertext, bigint d, bigint n) {
    return modPow(ciphertext, d, n);
}

