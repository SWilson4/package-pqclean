--- upstream/Reference_Implementation/hqc-256/src/api.h
+++ upstream-patched/Reference_Implementation/hqc-256/src/api.h
@@ -6,6 +6,8 @@
 #ifndef API_H
 #define API_H
 
+#include <stdint.h>
+
 #define CRYPTO_ALGNAME                      "HQC-256"
 
 #define CRYPTO_SECRETKEYBYTES               7317
@@ -16,8 +18,8 @@
 // As a technicality, the public key is appended to the secret key in order to respect the NIST API.
 // Without this constraint, CRYPTO_SECRETKEYBYTES would be defined as 32
 
-int crypto_kem_keypair(unsigned char* pk, unsigned char* sk);
-int crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk);
-int crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk);
+int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
+int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
+int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
 
 #endif

