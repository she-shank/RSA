#include "rsa.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
    cout << "--------------RSA Encryption--------------" << endl << endl;

    //generating rsa key pair
    cout << "Generating RSA key pair (128-bit)..." << endl;

    RSAKeyPair keys;
    keys = generateKeyPair(128);

    cout << "Key pair generated!" << endl << endl;

    //public key
    cout << "Public Key (e, n):" << endl;
    cout << "  e = ";
    keys.publicKey_e.print();
    cout << endl;

    cout << "  n = ";
    keys.publicKey_n.print();
    cout << endl << endl;

    // private key
    cout << "Private Key (d, n):" << endl;
    cout << "  d = ";
    keys.privateKey_d.print();
    cout << endl;

    cout << "  n = ";
    keys.privateKey_n.print();
    cout << endl << endl;

    //encrypt
    cout << "Encrypting message..." << endl;

    bigint message("12345");  // demo message
    cout << "Original message: ";
    message.print();
    cout << endl;

    bigint ciphertext;
    ciphertext = encrypt(message, keys.publicKey_e, keys.publicKey_n);

    cout << "Ciphertext: ";
    ciphertext.print();
    cout << endl << endl;

    //decrypt
    cout << "Decrypting ciphertext..." << endl;

    bigint decrypted;
    decrypted = decrypt(ciphertext, keys.privateKey_d, keys.privateKey_n);

    cout << "Decrypted message: ";
    decrypted.print();
    cout << endl << endl;

    //verify
    if(message == decrypted){
        cout << "Encryption/Decryption successful Message matches." << endl;
    }else{
        cout << "Error: Decrypted message does not match original." << endl;
    }

    return 0;
}
