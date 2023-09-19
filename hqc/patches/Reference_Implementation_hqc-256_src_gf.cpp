--- upstream/Reference_Implementation/hqc-256/src/gf.cpp
+++ upstream-patched/Reference_Implementation/hqc-256/src/gf.cpp
@@ -1,48 +1,12 @@
 /**
- * @file gf.cpp
- * @brief Galois field implementation with multiplication using the pclmulqdq instruction
+ * @file gf.c
+ * @brief Galois field implementation
  */
 
 #include "gf.h"
 #include "parameters.h"
+#include <stddef.h>
 #include <stdint.h>
-#include <stdio.h>
-
-static uint16_t gf_reduce(uint64_t x, size_t deg_x);
-void gf_carryless_mul(uint8_t *c, uint8_t a, uint8_t b);
-uint16_t trailing_zero_bits_count(uint16_t a);
-
-/**
- * @brief Generates exp and log lookup tables of GF(2^m).
- *
- * The logarithm of 0 is defined as 2^PARAM_M by convention. <br>
- * The last two elements of the exp table are needed by the gf_mul function from gf_lutmul.cpp
- * (for example if both elements to multiply are zero).
- * @param[out] exp Array of size 2^PARAM_M + 2 receiving the powers of the primitive element
- * @param[out] log Array of size 2^PARAM_M receiving the logarithms of the elements of GF(2^m)
- * @param[in] m Parameter of Galois field GF(2^m)
- */
-void gf_generate(uint16_t *exp, uint16_t *log, const int16_t m) {
-    uint16_t elt = 1;
-    uint16_t alpha = 2; // primitive element of GF(2^PARAM_M)
-    uint16_t gf_poly = PARAM_GF_POLY;
-
-    for (size_t i = 0 ; i < (1U << m) - 1 ; ++i){
-        exp[i] = elt;
-        log[elt] = i;
-
-        elt *= alpha;
-        if(elt >= 1 << m)
-            elt ^= gf_poly;
-    }
-
-    exp[(1 << m) - 1] = 1;
-    exp[1 << m] = 2;
-    exp[(1 << m) + 1] = 4;
-    log[0] = 0; // by convention
-}
-
-
 
 /**
  * @brief Computes the number of trailing zero bits.
@@ -50,10 +14,10 @@
  * @returns The number of trailing zero bits in a.
  * @param[in] a An operand
  */
-uint16_t trailing_zero_bits_count(uint16_t a) {
+static uint16_t trailing_zero_bits_count(uint16_t a) {
     uint16_t tmp = 0;
     uint16_t mask = 0xFFFF;
-    for (int i = 0; i < 14; ++i) {
+    for (size_t i = 0; i < 14; ++i) {
         tmp += ((1 - ((a >> i) & 0x0001)) & mask);
         mask &= - (1 - ((a >> i) & 0x0001));
     }
@@ -71,22 +35,21 @@
 static uint16_t gf_reduce(uint64_t x, size_t deg_x) {
     uint16_t z1, z2, rmdr, dist;
     uint64_t mod;
-    size_t steps, i, j;
 
     // Deduce the number of steps of reduction
-    steps = CEIL_DIVIDE(deg_x - (PARAM_M - 1), PARAM_GF_POLY_M2);
+    size_t steps = CEIL_DIVIDE(deg_x - (PARAM_M - 1), PARAM_GF_POLY_M2);
 
     // Reduce
-    for (i = 0; i < steps; ++i) {
+    for (size_t i = 0; i < steps; ++i) {
         mod = x >> PARAM_M;
         x &= (1 << PARAM_M) - 1;
         x ^= mod;
 
         z1 = 0;
         rmdr = PARAM_GF_POLY ^ 1;
-        for (j = PARAM_GF_POLY_WT - 2; j; --j) {
+        for (size_t j = PARAM_GF_POLY_WT - 2; j; --j) {
             z2 = trailing_zero_bits_count(rmdr);
-            dist = (uint16_t) (z2 - z1);
+            dist = z2 - z1;
             mod <<= dist;
             x ^= mod;
             rmdr ^= 1 << z2;
@@ -109,39 +72,39 @@
  * @param[in] a The first polynomial
  * @param[in] b The second polynomial
  */
-void gf_carryless_mul(uint8_t *c, uint8_t a, uint8_t b) {
-  uint16_t       h = 0, l = 0, g, u[4];
+static void gf_carryless_mul(uint8_t c[2], uint8_t a, uint8_t b) {
+  uint16_t       h = 0, l = 0, g = 0, u[4];
+  uint32_t tmp1, tmp2;
+  uint16_t mask;
   u[0] = 0;
-  u[1] = b & ((1UL << 7) - 1UL);
+  u[1] = b & 0x7F;
   u[2] = u[1] << 1;
   u[3] = u[2] ^ u[1];
+  tmp1 = a & 3;
 
-  g = 0;
-  uint16_t tmp1 = a & 3;
-
-  for(int i =0; i < 4; i++) {
-    uint32_t tmp2 = tmp1 - i;
-    g ^= (u[i] & -(1 - ((tmp2 | -tmp2) >> 31)));
+  for (size_t i = 0; i < 4; i++) {
+    tmp2 = tmp1 - i;
+    g ^= (u[i] & (uint32_t)(-(1 - ((uint32_t)(tmp2 | -tmp2) >> 31))));
   }
 
   l = g;
   h = 0;
 
-  for (uint8_t i = 2; i < 8; i+=2) {
+  for (size_t i = 2; i < 8; i += 2) {
     g = 0;
-    uint16_t tmp1 = (a >> i) & 3;
-    for (int j = 0; j < 4; ++j) {
-      uint32_t tmp2 = tmp1 - j;
-      g ^= (u[j] & -(1 - ((tmp2 | -tmp2) >> 31)));
+    tmp1 = (a >> i) & 3;
+    for (size_t j = 0; j < 4; ++j) {
+      tmp2 = tmp1 - j;
+      g ^= (u[j] & (uint32_t)(-(1 - ((uint32_t)(tmp2 | -tmp2) >> 31))));
     }
     
     l ^= g << i;
     h ^= g >> (8 - i);
   }
 
-  uint16_t mask = (-((b >> 7) & 1));
+  mask = (-((b >> 7) & 1));
   l ^= ((a << 7) & mask);
-  h ^= ((a >> (1)) & mask);
+  h ^= ((a >> 1) & mask);
 
   c[0] = l;
   c[1] = h;
@@ -158,16 +121,16 @@
 uint16_t gf_mul(uint16_t a, uint16_t b) {
     uint8_t c[2] = {0};
     gf_carryless_mul(c, (uint8_t) a, (uint8_t) b);
-    uint16_t tmp = (uint16_t) (c[0] ^ (c[1] << 8));
+    uint16_t tmp = c[0] ^ (c[1] << 8);
     return gf_reduce(tmp, 2*(PARAM_M-1));
 }
 
 
 
 /**
- * Squares an element of GF(2^GF_M).
+ * @brief Squares an element of GF(2^PARAM_M).
  * @returns a^2
- * @param[in] a Element of GF(2^GF_M)
+ * @param[in] a Element of GF(2^PARAM_M)
  */
 uint16_t gf_square(uint16_t a) {
     uint32_t b = a;
@@ -183,10 +146,10 @@
 
 
 /**
- * Computes the inverse of an element of GF(2^8),
+ * @brief Computes the inverse of an element of GF(2^PARAM_M),
  * using the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
- * @returns the inverse of a
- * @param[in] a Element of GF(2^GF_M)
+ * @returns the inverse of a if a != 0 or 0 if a = 0
+ * @param[in] a Element of GF(2^PARAM_M)
  */
 uint16_t gf_inverse(uint16_t a) {
     uint16_t inv = a;
@@ -205,21 +168,3 @@
     inv = gf_square(inv); /* a^254 */
     return inv;
 }
-
-
-
-/**
- * Returns i modulo 2^GF_M-1.
- * i must be less than 2*(2^GF_M-1).
- * Therefore, the return value is either i or i-2^GF_M+1.
- * @returns i mod (2^GF_M-1)
- * @param[in] i The integer whose modulo is taken
- */
-uint16_t gf_mod(uint16_t i) {
-    uint16_t tmp = i - PARAM_GF_MUL_ORDER;
-
-    // mask = 0xffff if (i < GF_MUL_ORDER)
-    int16_t mask = -(tmp >> 15);
-
-    return tmp + (mask & PARAM_GF_MUL_ORDER);
-}

