--- upstream/ref/api.h
+++ upstream-patched/ref/api.h
@@ -1,26 +1,33 @@
 #ifndef API_H
 #define API_H
 
-#include "params.h"
-
-#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
-#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
-#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
-#define CRYPTO_BYTES           KYBER_SSBYTES
+#include <stdint.h>
 
 #if   (KYBER_K == 2)
+#define CRYPTO_SECRETKEYBYTES  1632
+#define CRYPTO_PUBLICKEYBYTES  800
+#define CRYPTO_CIPHERTEXTBYTES 768
+#define CRYPTO_BYTES           32
 #ifdef KYBER_90S
 #define CRYPTO_ALGNAME "Kyber512-90s"
 #else
 #define CRYPTO_ALGNAME "Kyber512"
 #endif
 #elif (KYBER_K == 3)
+#define CRYPTO_SECRETKEYBYTES  2400
+#define CRYPTO_PUBLICKEYBYTES  1184
+#define CRYPTO_CIPHERTEXTBYTES 1088
+#define CRYPTO_BYTES           32
 #ifdef KYBER_90S
 #define CRYPTO_ALGNAME "Kyber768-90s"
 #else
 #define CRYPTO_ALGNAME "Kyber768"
 #endif
 #elif (KYBER_K == 4)
+#define CRYPTO_SECRETKEYBYTES  3168
+#define CRYPTO_PUBLICKEYBYTES  1568
+#define CRYPTO_CIPHERTEXTBYTES 1568
+#define CRYPTO_BYTES           32
 #ifdef KYBER_90S
 #define CRYPTO_ALGNAME "Kyber1024-90s"
 #else
@@ -29,16 +36,12 @@
 #endif
 
 #define crypto_kem_keypair KYBER_NAMESPACE(_keypair)
-int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
+int crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
 
 #define crypto_kem_enc KYBER_NAMESPACE(_enc)
-int crypto_kem_enc(unsigned char *ct,
-                   unsigned char *ss,
-                   const unsigned char *pk);
+int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
 
 #define crypto_kem_dec KYBER_NAMESPACE(_dec)
-int crypto_kem_dec(unsigned char *ss,
-                   const unsigned char *ct,
-                   const unsigned char *sk);
+int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
 
 #endif

