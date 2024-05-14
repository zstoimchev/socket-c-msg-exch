#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/evp.h>

const int kBits = 1024;
const int kExp = 3;
int keylen;
char *pem_key;

int main() {
    EVP_PKEY *pkey = EVP_PKEY_new();
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

    if(EVP_PKEY_assign_RSA(pkey, rsa)){
        /* pkey now owns rsa, so we don't free rsa directly */
        rsa = NULL;
    } else {
        /* error */
        RSA_free(rsa);
        return -1;
    }

    /* To get the C-string PEM form: */
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(bio, pkey, NULL, NULL, 0, NULL, NULL);

    keylen = BIO_pending(bio);
    pem_key = calloc(keylen+1, 1); /* Null-terminate */
    BIO_read(bio, pem_key, keylen);

    printf("Private Key:\n%s\n", pem_key);

    BIO_free_all(bio);
    free(pem_key);

    /* Now let's write the public key to a string */
    bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bio, pkey);

    keylen = BIO_pending(bio);
    pem_key = calloc(keylen+1, 1); /* Null-terminate */
    BIO_read(bio, pem_key, keylen);

    printf("Public Key:\n%s\n", pem_key);

    BIO_free_all(bio);
    EVP_PKEY_free(pkey);
    free(pem_key);

    return 0;
}

