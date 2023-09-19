--- upstream/Reference_Implementation/hqc-192/src/hqc.cpp
+++ upstream-patched/Reference_Implementation/hqc-192/src/hqc.cpp
@@ -1,5 +1,5 @@
 /**
- * @file hqc.cpp
+ * @file hqc.c
  * @brief Implementation of hqc.h
  */
 
@@ -7,6 +7,7 @@
 #include "gf2x.h"
 #include "parameters.h"
 #include "parsing.h"
+#include "randombytes.h"
 #include "shake_prng.h"
 #include "code.h"
 #include "vector.h"
@@ -27,7 +28,7 @@
  * @param[out] pk String containing the public key
  * @param[out] sk String containing the secret key
  */
-void hqc_pke_keygen(unsigned char* pk, unsigned char* sk) {
+void hqc_pke_keygen(uint8_t *pk, uint8_t *sk) {
     seedexpander_state sk_seedexpander;
     seedexpander_state pk_seedexpander;
     uint8_t sk_seed[SEED_BYTES] = {0};
@@ -39,11 +40,11 @@
     uint64_t s[VEC_N_SIZE_64] = {0};
 
     // Create seed_expanders for public key and secret key
-    shake_prng(sk_seed, SEED_BYTES);
-    shake_prng(sigma, VEC_K_SIZE_BYTES);
+    randombytes(sk_seed, SEED_BYTES);
+    randombytes(sigma, VEC_K_SIZE_BYTES);
     seedexpander_init(&sk_seedexpander, sk_seed, SEED_BYTES);
 
-    shake_prng(pk_seed, SEED_BYTES);
+    randombytes(pk_seed, SEED_BYTES);
     seedexpander_init(&pk_seedexpander, pk_seed, SEED_BYTES);
 
     // Compute secret key
@@ -59,6 +60,8 @@
     hqc_public_key_to_string(pk, pk_seed, s);
     hqc_secret_key_to_string(sk, sk_seed, sigma, pk);
 
+    seedexpander_release(&pk_seedexpander);
+    seedexpander_release(&sk_seedexpander);
     #ifdef VERBOSE
         printf("\n\nsk_seed: "); for(int i = 0 ; i < SEED_BYTES ; ++i) printf("%02x", sk_seed[i]);
         printf("\n\nsigma: "); for(int i = 0 ; i < VEC_K_SIZE_BYTES ; ++i) printf("%02x", sigma[i]);
@@ -87,8 +90,8 @@
  * @param[in] theta Seed used to derive randomness required for encryption
  * @param[in] pk String containing the public key
  */
-void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk) {
-    seedexpander_state seedexpander;
+void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint8_t *m, uint8_t *theta, const uint8_t *pk) {
+    seedexpander_state vec_seedexpander;
     uint64_t h[VEC_N_SIZE_64] = {0};
     uint64_t s[VEC_N_SIZE_64] = {0};
     uint64_t r1[VEC_N_SIZE_64] = {0};
@@ -98,15 +101,15 @@
     uint64_t tmp2[VEC_N_SIZE_64] = {0};
 
     // Create seed_expander from theta
-    seedexpander_init(&seedexpander, theta, SEED_BYTES);
+    seedexpander_init(&vec_seedexpander, theta, SEED_BYTES);
 
     // Retrieve h and s from public key
     hqc_public_key_from_string(h, s, pk);
 
     // Generate r1, r2 and e
-    vect_set_random_fixed_weight(&seedexpander, r1, PARAM_OMEGA_R);
-    vect_set_random_fixed_weight(&seedexpander, r2, PARAM_OMEGA_R);
-    vect_set_random_fixed_weight(&seedexpander, e, PARAM_OMEGA_E);
+    vect_set_random_fixed_weight(&vec_seedexpander, r1, PARAM_OMEGA_R);
+    vect_set_random_fixed_weight(&vec_seedexpander, r2, PARAM_OMEGA_R);
+    vect_set_random_fixed_weight(&vec_seedexpander, e, PARAM_OMEGA_E);
 
     // Compute u = r1 + r2.h
     vect_mul(u, r2, h);
@@ -122,6 +125,7 @@
     vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
     vect_resize(v, PARAM_N1N2, tmp2, PARAM_N);
 
+    seedexpander_release(&vec_seedexpander);
     #ifdef VERBOSE
         printf("\n\nh: "); vect_print(h, VEC_N_SIZE_BYTES);
         printf("\n\ns: "); vect_print(s, VEC_N_SIZE_BYTES);
@@ -144,9 +148,9 @@
  * @param[in] u Vector u (first part of the ciphertext)
  * @param[in] v Vector v (second part of the ciphertext)
  * @param[in] sk String containing the secret key
- * @returns 0 
+ * @returns 0
  */
-uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk) {
+uint8_t hqc_pke_decrypt(uint8_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk) {
     uint64_t x[VEC_N_SIZE_64] = {0};
     uint64_t y[VEC_N_SIZE_64] = {0};
     uint8_t pk[PUBLIC_KEY_BYTES] = {0};

