#ifndef RSA_H
#define RSA_H

#include <string>
#include <vector>

class bigint {
private:
    std::vector<int> num;
    void equalize_length(std::vector<int> &other_num);
    void normalize();

public:
    int isNegative {0};

    bigint();
    bigint(int n);
    bigint(std::string n);
    bigint(std::vector<int> n);

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

// RSA Key Pair structure
struct RSAKeyPair {
    bigint publicKey_e;  // Public exponent
    bigint publicKey_n;  // Public modulus
    bigint privateKey_d; // Private exponent
    bigint privateKey_n; // Private modulus (same as publicKey_n)
    
    RSAKeyPair() {}
};

// Generate RSA key pair
// keySize: size of the key in bits (e.g., 256, 512, 1024)
RSAKeyPair generateKeyPair(int keySize);

// RSA Encryption: c = m^e mod n
bigint encrypt(bigint message, bigint e, bigint n);

// RSA Decryption: m = c^d mod n
bigint decrypt(bigint ciphertext, bigint d, bigint n);

#endif // RSA_H

