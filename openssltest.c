#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

const int kBits = 1024;
const int kExp = 3;
int keylen;
char *pem_key;

int main() {
    BIGNUM *bne = BN_new();
    int ret = BN_set_word(bne, kExp);
    if(ret != 1){
        // handle error
    }

    RSA *rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, kBits, bne, NULL);
    if(ret != 1){
        // handle error
    }

    /* To get the C-string PEM form: */
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);

    keylen = BIO_pending(bio);
    pem_key = calloc(keylen+1, 1); /* Null-terminate */
    BIO_read(bio, pem_key, keylen);

    printf("%s", pem_key);

    BIO_free_all(bio);
    RSA_free(rsa);
    free(pem_key);

    return 0;
}
