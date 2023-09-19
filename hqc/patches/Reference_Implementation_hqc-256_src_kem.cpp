--- upstream/Reference_Implementation/hqc-256/src/kem.cpp
+++ upstream-patched/Reference_Implementation/hqc-256/src/kem.cpp
@@ -1,12 +1,14 @@
 /**
- * @file kem.cpp
+ * @file kem.c
  * @brief Implementation of api.h
  */
 
 #include "api.h"
+#include "domains.h"
 #include "hqc.h"
 #include "parameters.h"
 #include "parsing.h"
+#include "randombytes.h"
 #include "shake_ds.h"
 #include "fips202.h"
 #include "vector.h"
@@ -29,7 +31,7 @@
  * @param[out] sk String containing the secret key
  * @returns 0 if keygen is successful
  */
-int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
+int crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
     #ifdef VERBOSE
         printf("\n\n\n\n### KEYGEN ###");
     #endif
@@ -48,37 +50,35 @@
  * @param[in] pk String containing the public key
  * @returns 0 if encapsulation is successful
  */
-int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
+int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
     #ifdef VERBOSE
         printf("\n\n\n\n### ENCAPS ###");
     #endif
 
     uint8_t theta[SHAKE256_512_BYTES] = {0};
-    uint8_t m[VEC_K_SIZE_BYTES] = {0};
     uint64_t u[VEC_N_SIZE_64] = {0};
     uint64_t v[VEC_N1N2_SIZE_64] = {0};
     uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
-    uint64_t salt[SALT_SIZE_64] = {0};
     uint8_t tmp[VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES] = {0};
+    uint8_t *m = tmp;
+    uint8_t *salt = tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES;
     shake256incctx shake256state;
 
     // Computing m
-    vect_set_random_from_prng((uint64_t *)m, VEC_K_SIZE_64);
+    randombytes(m, VEC_K_SIZE_BYTES);
 
     // Computing theta
-    vect_set_random_from_prng(salt, SALT_SIZE_64);
-    memcpy(tmp, m, VEC_K_SIZE_BYTES);
+    randombytes(salt, SALT_SIZE_BYTES);
     memcpy(tmp + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
-    memcpy(tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES, salt, SALT_SIZE_BYTES);
     shake256_512_ds(&shake256state, theta, tmp, VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES, G_FCT_DOMAIN);
 
     // Encrypting m
-    hqc_pke_encrypt(u, v, (uint64_t *)m, theta, pk);
+    hqc_pke_encrypt(u, v, m, theta, pk);
 
     // Computing shared secret
     memcpy(mc, m, VEC_K_SIZE_BYTES);
-    memcpy(mc + VEC_K_SIZE_BYTES, u, VEC_N_SIZE_BYTES);
-    memcpy(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
+    store8_arr(mc + VEC_K_SIZE_BYTES, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
+    store8_arr(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
     shake256_512_ds(&shake256state, ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, K_FCT_DOMAIN);
 
     // Computing ciphertext
@@ -104,7 +104,7 @@
  * @param[in] sk String containing the secret key
  * @returns 0 if decapsulation is successful, -1 otherwise
  */
-int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
+int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
     #ifdef VERBOSE
         printf("\n\n\n\n### DECAPS ###");
     #endif
@@ -112,34 +112,29 @@
     uint8_t result;
     uint64_t u[VEC_N_SIZE_64] = {0};
     uint64_t v[VEC_N1N2_SIZE_64] = {0};
-    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
-    uint8_t m[VEC_K_SIZE_BYTES] = {0};
+    const uint8_t *pk = sk + SEED_BYTES;
     uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
     uint8_t theta[SHAKE256_512_BYTES] = {0};
     uint64_t u2[VEC_N_SIZE_64] = {0};
     uint64_t v2[VEC_N1N2_SIZE_64] = {0};
     uint8_t mc[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {0};
-    uint64_t salt[SALT_SIZE_64] = {0};
     uint8_t tmp[VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES] = {0};
+    uint8_t *m = tmp;
+    uint8_t *salt = tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES;
     shake256incctx shake256state;
 
     // Retrieving u, v and d from ciphertext
     hqc_ciphertext_from_string(u, v , salt, ct);
 
-    // Retrieving pk from sk
-    memcpy(pk, sk + SEED_BYTES, PUBLIC_KEY_BYTES);
-
     // Decrypting
-    result = hqc_pke_decrypt((uint64_t *)m, sigma, u, v, sk);
+    result = hqc_pke_decrypt(m, sigma, u, v, sk);
 
     // Computing theta
-    memcpy(tmp, m, VEC_K_SIZE_BYTES);
     memcpy(tmp + VEC_K_SIZE_BYTES, pk, PUBLIC_KEY_BYTES);
-    memcpy(tmp + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES, salt, SALT_SIZE_BYTES);
     shake256_512_ds(&shake256state, theta, tmp, VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES + SALT_SIZE_BYTES, G_FCT_DOMAIN);
 
     // Encrypting m'
-    hqc_pke_encrypt(u2, v2, (uint64_t *)m, theta, pk);
+    hqc_pke_encrypt(u2, v2, m, theta, pk);
 
     // Check if c != c'
     result |= vect_compare((uint8_t *)u, (uint8_t *)u2, VEC_N_SIZE_BYTES);
@@ -152,8 +147,8 @@
     }
 
     // Computing shared secret
-    memcpy(mc + VEC_K_SIZE_BYTES, u, VEC_N_SIZE_BYTES);
-    memcpy(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
+    store8_arr(mc + VEC_K_SIZE_BYTES, VEC_N_SIZE_BYTES, u, VEC_N_SIZE_64);
+    store8_arr(mc + VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES, v, VEC_N1N2_SIZE_64);
     shake256_512_ds(&shake256state, ss, mc, VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, K_FCT_DOMAIN);
 
     #ifdef VERBOSE

