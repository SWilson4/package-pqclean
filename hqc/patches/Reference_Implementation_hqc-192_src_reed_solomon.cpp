--- upstream/Reference_Implementation/hqc-192/src/reed_solomon.cpp
+++ upstream-patched/Reference_Implementation/hqc-192/src/reed_solomon.cpp
@@ -1,5 +1,5 @@
 /**
- * @file reed_solomon.cpp
+ * @file reed_solomon.c
  * @brief Constant time implementation of Reed-Solomon codes
  */
 
@@ -9,74 +9,11 @@
 #include "parameters.h"
 #include <stdint.h>
 #include <string.h>
-#include <stdio.h>
 #ifdef VERBOSE
 #include <stdbool.h>
 #include <stdio.h>
 #endif
 
-static uint16_t mod(uint16_t i, uint16_t modulus);
-static void compute_syndromes(uint16_t *syndromes, uint8_t *cdw);
-static uint16_t compute_elp(uint16_t *sigma, const uint16_t *syndromes);
-static void compute_roots(uint8_t *error, uint16_t *sigma);
-static void compute_z_poly(uint16_t *z, const uint16_t *sigma, const uint16_t degree, const uint16_t *syndromes);
-static void compute_error_values(uint16_t *error_values, const uint16_t *z, const uint8_t *error);
-static void correct_errors(uint8_t *cdw, const uint16_t *error_values);
-
-
-/**
- * Returns i modulo the given modulus.
- * i must be less than 2*modulus.
- * Therefore, the return value is either i or i-modulus.
- * @returns i mod (modulus)
- * @param[in] i The integer whose modulo is taken
- * @param[in] modulus The modulus
- */
-static uint16_t mod(uint16_t i, uint16_t modulus) {
-    uint16_t tmp = i - modulus;
-
-    // mask = 0xffff if(i < PARAM_GF_MUL_ORDER)
-    int16_t mask = -(tmp >> 15);
-
-    return tmp + (mask & modulus);
-}
-
-
-
-/**
- * @brief Computes the generator polynomial of the primitive Reed-Solomon code with given parameters.
- *
- * Code length is 2^m-1. <br>
- * PARAM_DELTA is the targeted correction capacity of the code
- * and receives the real correction capacity (which is at least equal to the target). <br>
- * gf_exp and gf_log are arrays giving antilog and log of GF(2^m) elements.
- *
- * @param[out] poly Array of size (2*PARAM_DELTA + 1) receiving the coefficients of the generator polynomial
- */
-void compute_generator_poly(uint16_t* poly) {
-    poly[0] = 1;
-    int tmp_degree = 0;
-
-    for (uint16_t i = 1; i < (2 * PARAM_DELTA + 1); ++i) {
-
-        for(size_t j = tmp_degree; j; --j) {
-            poly[j] = gf_exp[mod(gf_log[poly[j]] + i, PARAM_GF_MUL_ORDER)] ^ poly[j - 1];
-        }
-
-        poly[0] = gf_exp[mod(gf_log[poly[0]] + i, PARAM_GF_MUL_ORDER)];
-        poly[++tmp_degree] = 1;
-
-    }
-
-    printf("\n");
-    for (int i = 0; i < (PARAM_G); ++i) {
-        printf("%d, ", poly[i]);
-    }
-    printf("\n");
-}
-
-
-
 /**
  * @brief Encodes a message message of PARAM_K bits to a Reed-Solomon codeword codeword of PARAM_N1 bytes
  *
@@ -87,34 +24,29 @@
  * @param[out] cdw Array of size VEC_N1_SIZE_64 receiving the encoded message
  * @param[in] msg Array of size VEC_K_SIZE_64 storing the message
  */
-void reed_solomon_encode(uint64_t *cdw, const uint64_t *msg) {
-    size_t i, j, k;
+void reed_solomon_encode(uint8_t *cdw, const uint8_t *msg) {
     uint8_t gate_value = 0;
 
     uint16_t tmp[PARAM_G] = {0};
     uint16_t PARAM_RS_POLY [] = {RS_POLY_COEFS};
 
-    uint8_t msg_bytes[PARAM_K] = {0};
-    uint8_t cdw_bytes[PARAM_N1] = {0};
-
-    memcpy(msg_bytes, msg, PARAM_K);
+    memset(cdw, 0, PARAM_N1);
 
-    for (i = 0; i < PARAM_K; ++i) {
-        gate_value = msg_bytes[PARAM_K - 1 - i] ^ cdw_bytes[PARAM_N1 - PARAM_K - 1];
+    for (size_t i = 0; i < PARAM_K; ++i) {
+        gate_value = msg[PARAM_K - 1 - i] ^ cdw[PARAM_N1 - PARAM_K - 1];
 
-        for (j = 0; j < PARAM_G; ++j) {
+        for (size_t j = 0; j < PARAM_G; ++j) {
             tmp[j] = gf_mul(gate_value, PARAM_RS_POLY[j]);
         }
 
-        for(k = PARAM_N1 - PARAM_K - 1; k; --k) {
-            cdw_bytes[k] = cdw_bytes[k - 1] ^ tmp[k];
+        for (size_t k = PARAM_N1 - PARAM_K - 1; k; --k) {
+            cdw[k] = cdw[k - 1] ^ tmp[k];
         }
 
-        cdw_bytes[0] = tmp[0];
+        cdw[0] = tmp[0];
     }
 
-    memcpy(cdw_bytes + PARAM_N1 - PARAM_K, msg_bytes, PARAM_K);
-    memcpy(cdw, cdw_bytes, PARAM_N1);
+    memcpy(cdw + PARAM_N1 - PARAM_K, msg, PARAM_K);
 }
 
 
@@ -125,7 +57,7 @@
  * @param[out] syndromes Array of size 2 * PARAM_DELTA receiving the computed syndromes
  * @param[in] cdw Array of size PARAM_N1 storing the received vector
  */
-void compute_syndromes(uint16_t *syndromes, uint8_t *cdw) {	
+static void compute_syndromes(uint16_t *syndromes, uint8_t *cdw) {	
     for (size_t i = 0; i < 2 * PARAM_DELTA; ++i) {
         for (size_t j = 1; j < PARAM_N1; ++j) {
             syndromes[i] ^= gf_mul(cdw[j], alpha_ij_pow[i][j-1]);
@@ -245,7 +177,7 @@
  * @param[in] degree Integer that is the degree of polynomial sigma
  * @param[in] syndromes Array of 2 * PARAM_DELTA storing the syndromes
  */
-static void compute_z_poly(uint16_t *z, const uint16_t *sigma, const uint16_t degree, const uint16_t *syndromes) {
+static void compute_z_poly(uint16_t *z, const uint16_t *sigma, uint16_t degree, const uint16_t *syndromes) {
     size_t i, j;
     uint16_t mask;
 
@@ -374,8 +306,7 @@
  * @param[out] msg Array of size VEC_K_SIZE_64 receiving the decoded message
  * @param[in] cdw Array of size VEC_N1_SIZE_64 storing the received word
  */
-void reed_solomon_decode(uint64_t *msg, uint64_t *cdw) {
-    uint8_t cdw_bytes[PARAM_N1] = {0};
+void reed_solomon_decode(uint8_t *msg, uint8_t *cdw) {
     uint16_t syndromes[2 * PARAM_DELTA] = {0};
     uint16_t sigma[1 << PARAM_FFT] = {0};
     uint8_t error[1 << PARAM_M] = {0};
@@ -383,11 +314,8 @@
     uint16_t error_values[PARAM_N1] = {0};
     uint16_t deg;
 
-    // Copy the vector in an array of bytes
-    memcpy(cdw_bytes, cdw, PARAM_N1);
-
     // Calculate the 2*PARAM_DELTA syndromes
-    compute_syndromes(syndromes, cdw_bytes);
+    compute_syndromes(syndromes, cdw);
 
     // Compute the error locator polynomial sigma
     // Sigma's degree is at most PARAM_DELTA but the FFT requires the extra room
@@ -403,10 +331,10 @@
     compute_error_values(error_values, z, error);
 
     // Correct the errors
-    correct_errors(cdw_bytes, error_values);
+    correct_errors(cdw, error_values);
 
     // Retrieve the message from the decoded codeword
-    memcpy(msg, cdw_bytes + (PARAM_G - 1) , PARAM_K);
+    memcpy(msg, cdw + (PARAM_G - 1) , PARAM_K);
 
     #ifdef VERBOSE
         printf("\n\nThe syndromes: ");

