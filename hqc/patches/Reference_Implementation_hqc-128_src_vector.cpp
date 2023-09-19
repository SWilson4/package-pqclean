--- upstream/Reference_Implementation/hqc-128/src/vector.cpp
+++ upstream-patched/Reference_Implementation/hqc-128/src/vector.cpp
@@ -1,32 +1,59 @@
 /**
- * @file vector.cpp
+ * @file vector.c
  * @brief Implementation of vectors sampling and some utilities for the HQC scheme
  */
 
-#include "shake_prng.h"
 #include "parameters.h"
+#include "parsing.h"
+#include "randombytes.h"
 #include "vector.h"
 #include <stdint.h>
 #include <string.h>
-#include <stdio.h>
-
-static inline uint32_t compare_u32(const uint32_t v1, const uint32_t v2);
 
+static uint32_t m_val[75] = { 243079, 243093, 243106, 243120, 243134, 243148, 243161, 243175, 243189, 243203, 243216, 243230, 243244, 243258, 243272, 243285, 243299, 243313, 243327, 243340, 243354, 243368, 243382, 243396, 243409, 243423, 243437, 243451, 243465, 243478, 243492, 243506, 243520, 243534, 243547, 243561, 243575, 243589, 243603, 243616, 243630, 243644, 243658, 243672, 243686, 243699, 243713, 243727, 243741, 243755, 243769, 243782, 243796, 243810, 243824, 243838, 243852, 243865, 243879, 243893, 243907, 243921, 243935, 243949, 243962, 243976, 243990, 244004, 244018, 244032, 244046, 244059, 244073, 244087, 244101 };
 
 /**
  * @brief Constant-time comparison of two integers v1 and v2
  *
  * Returns 1 if v1 is equal to v2 and 0 otherwise
- * https://gist.github.cppom/sneves/10845247
+ * https://gist.github.com/sneves/10845247
  *
- * @param[in] v1 
- * @param[in] v2 
+ * @param[in] v1
+ * @param[in] v2
  */
-static inline uint32_t compare_u32(const uint32_t v1, const uint32_t v2) {
-    return 1 ^ (((v1 - v2)|(v2 - v1)) >> 31);
+static inline uint32_t compare_u32(uint32_t v1, uint32_t v2) {
+    return 1 ^ ((uint32_t)((v1 - v2) | (v2 - v1)) >> 31);
+}
+
+static uint64_t single_bit_mask(uint32_t pos) {
+    uint64_t ret = 0;
+    uint64_t mask = 1;
+    uint64_t tmp;
+
+    for (size_t i = 0; i < 64; ++i) {
+        tmp = pos - i;
+        tmp = -(1 - ((uint64_t)(tmp | -tmp) >> 63));
+        ret |= mask & tmp;
+        mask <<= 1;
+    }
+
+    return ret;
 }
 
+static inline uint32_t cond_sub(uint32_t r, uint32_t n) {
+    uint32_t mask;
+    r -= n;
+    mask = -(r >> 31);
+    return r + (n & mask);
+}
 
+static inline uint32_t reduce(uint32_t a, size_t i) {
+    uint32_t q, n, r;
+    q = ((uint64_t) a * m_val[i]) >> 32;
+    n = PARAM_N - i;
+    r = a - q * n;
+    return cond_sub(r, n);
+}
 
 /**
  * @brief Generates a vector of a given Hamming weight
@@ -38,42 +65,47 @@
  * @param[in] weight Integer that is the Hamming weight
  */
 void vect_set_random_fixed_weight(seedexpander_state *ctx, uint64_t *v, uint16_t weight) {
-    uint32_t rand_u32[PARAM_OMEGA_R] = {0};
+    uint8_t rand_bytes[4 * PARAM_OMEGA_R] = {0}; // to be interpreted as PARAM_OMEGA_R 32-bit unsigned ints
     uint32_t support[PARAM_OMEGA_R] = {0};
     uint32_t index_tab [PARAM_OMEGA_R] = {0};
     uint64_t bit_tab [PARAM_OMEGA_R] = {0};
+    uint32_t pos, found, mask32, tmp;
+    uint64_t mask64, val;
 
-    seedexpander(ctx, (uint8_t *)&rand_u32, 4 * weight);
+    seedexpander(ctx, rand_bytes, 4 * weight);
 
     for (size_t i = 0; i < weight; ++i) {
-        support[i] = i + rand_u32[i] % (PARAM_N - i);
+        support[i] = rand_bytes[4 * i];
+        support[i] |= rand_bytes[4 * i + 1] << 8;
+        support[i] |= (uint32_t)rand_bytes[4 * i + 2] << 16;
+        support[i] |= (uint32_t)rand_bytes[4 * i + 3] << 24;
+        support[i] = i + reduce(support[i], i); // use constant-tme reduction
     }
 
-    for (int32_t i = (weight - 1); i -- > 0;) {
-        uint32_t found = 0;
+    for (size_t i = (weight - 1); i-- > 0;) {
+        found = 0;
 
         for (size_t j = i + 1; j < weight; ++j) {
             found |= compare_u32(support[j], support[i]);
         }
 
-        uint32_t mask = -found;
-        support[i] = (mask & i) ^ (~mask & support[i]);
+        mask32 = -found;
+        support[i] = (mask32 & i) ^ (~mask32 & support[i]);
     }
 
-    for (size_t i = 0; i < weight; i++) {
+    for (size_t i = 0; i < weight; ++i) {
         index_tab[i] = support[i] >> 6;
-        int32_t pos = support[i] & 0x3f;
-        bit_tab[i]  = ((uint64_t) 1) << pos;
+        pos = support[i] & 0x3f;
+        bit_tab[i] = single_bit_mask(pos); // avoid secret shift
     }
 
-    uint64_t val = 0;
-    for (uint32_t i = 0; i < VEC_N_SIZE_64; i++) {
+    for (size_t i = 0; i < VEC_N_SIZE_64; ++i) {
         val = 0;
-        for (uint32_t j = 0 ; j < weight ; j++) {
-            uint32_t tmp = i - index_tab[j];
-            int val1 = 1 ^ ((tmp | -tmp) >> 31);
-            uint64_t mask = -val1;
-            val |= (bit_tab[j] & mask);
+        for (size_t j = 0 ; j < weight ; ++j) {
+            tmp = i - index_tab[j];
+            tmp = 1 ^ ((uint32_t)(tmp | -tmp) >> 31);
+            mask64 = -(uint64_t)tmp;
+            val |= (bit_tab[j] & mask64);
         }
         v[i] |= val;
     }
@@ -95,25 +127,8 @@
 
     seedexpander(ctx, rand_bytes, VEC_N_SIZE_BYTES);
 
-    memcpy(v, rand_bytes, VEC_N_SIZE_BYTES);
-    v[VEC_N_SIZE_64 - 1] &= BITMASK(PARAM_N, 64);
-}
-
-
-
-/**
- * @brief Generates a random vector
- *
- * This function generates a random binary vector. It uses the the prng function.
- *
- * @param[in] v Pointer to an array
- * @param[in] size_v Size of v
- */
-void vect_set_random_from_prng(uint64_t *v, uint32_t size_v) {
-    uint8_t rand_bytes [32] = {0}; // set to the maximum possible size - 256 bits
-
-    shake_prng(rand_bytes, size_v << 3);
-    memcpy(v, rand_bytes, size_v << 3);
+    load8_arr(v, VEC_N_SIZE_64, rand_bytes, VEC_N_SIZE_BYTES);
+    v[VEC_N_SIZE_64 - 1] &= RED_MASK;
 }
 
 
@@ -126,8 +141,8 @@
  * @param[in] v2 Pointer to an array that is the second vector
  * @param[in] size Integer that is the size of the vectors
  */
-void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size) {
-    for (uint32_t i = 0 ; i < size ; ++i) {
+void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, size_t size) {
+    for (size_t i = 0; i < size; ++i) {
         o[i] = v1[i] ^ v2[i];
     }
 }
@@ -140,17 +155,16 @@
  * @param[in] v1 Pointer to an array that is first vector
  * @param[in] v2 Pointer to an array that is second vector
  * @param[in] size Integer that is the size of the vectors
- * @returns 0 if the vectors are equals and a negative/positive value otherwise
+ * @returns 0 if the vectors are equal and 1 otherwise
  */
-uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size) {
-    uint64_t r = 0;
+uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, size_t size) {
+    uint16_t r = 0x0100;
 
     for (size_t i = 0; i < size; i++) {
         r |= v1[i] ^ v2[i];
     }
 
-    r = (~r + 1) >> 63;
-    return (uint8_t) r;
+    return (r - 1) >> 8;
 }
 
 
@@ -165,7 +179,7 @@
  */
 void vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v) {
     uint64_t mask = 0x7FFFFFFFFFFFFFFF;
-    int8_t val = 0;
+    size_t val = 0;
     if (size_o < size_v) {
 
         if (size_o % 64) {
@@ -174,61 +188,10 @@
 
         memcpy(o, v, VEC_N1N2_SIZE_BYTES);
 
-        for (int8_t i = 0 ; i < val ; ++i) {
+        for (size_t i = 0; i < val; ++i) {
             o[VEC_N1N2_SIZE_64 - 1] &= (mask >> i);
         }
     } else {
-        memcpy(o, v, CEIL_DIVIDE(size_v, 8));
-    }
-}
-
-
-
-/**
- * @brief Prints a given number of bytes
- *
- * @param[in] v Pointer to an array of bytes
- * @param[in] size Integer that is number of bytes to be displayed
- */
-void vect_print(const uint64_t *v, const uint32_t size) {
-    if(size == VEC_K_SIZE_BYTES) {
-        uint8_t tmp [VEC_K_SIZE_BYTES] = {0};
-        memcpy(tmp, v, VEC_K_SIZE_BYTES);
-        for (uint32_t i = 0; i < VEC_K_SIZE_BYTES; ++i) {
-            printf("%02x", tmp[i]);
-        }
-    } else if (size == VEC_N_SIZE_BYTES) {
-        uint8_t tmp [VEC_N_SIZE_BYTES] = {0};
-        memcpy(tmp, v, VEC_N_SIZE_BYTES);
-        for (uint32_t i = 0; i < VEC_N_SIZE_BYTES; ++i) {
-            printf("%02x", tmp[i]);
-        }
-    } else if (size == VEC_N1N2_SIZE_BYTES) {
-        uint8_t tmp [VEC_N1N2_SIZE_BYTES] = {0};
-        memcpy(tmp, v, VEC_N1N2_SIZE_BYTES);
-        for (uint32_t i = 0; i < VEC_N1N2_SIZE_BYTES; ++i) {
-            printf("%02x", tmp[i]);
-        }
-    }  else if (size == VEC_N1_SIZE_BYTES) {
-        uint8_t tmp [VEC_N1_SIZE_BYTES] = {0};
-        memcpy(tmp, v, VEC_N1_SIZE_BYTES);
-        for (uint32_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
-            printf("%02x", tmp[i]);
-        }
-    }
-}
-
-
-
-/**
- * @brief Prints a vector stored by positions
- *
- * @param[in] v Pointer to an array of integers
- * @param[in] weight Integer that is number positions to be displayed
- */
-void vect_print_sparse(const uint32_t *v, const uint16_t weight) {
-    for (uint16_t i = 0; i < weight-1; ++i) {
-        printf("%d ,", v[i]);
+        memcpy(o, v, 8 * CEIL_DIVIDE(size_v, 64));
     }
-    printf("%d", v[weight - 1]);
 }

