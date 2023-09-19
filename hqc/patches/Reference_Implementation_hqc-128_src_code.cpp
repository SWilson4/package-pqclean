--- upstream/Reference_Implementation/hqc-128/src/code.cpp
+++ upstream-patched/Reference_Implementation/hqc-128/src/code.cpp
@@ -1,5 +1,5 @@
 /**
- * @file code.cpp
+ * @file code.c
  * @brief Implementation of concatenated code
  */
 
@@ -8,7 +8,6 @@
 #include "reed_solomon.h"
 #include "parameters.h"
 #include <stdint.h>
-#include <string.h>
 #ifdef VERBOSE
 #include <stdio.h>
 #include "vector.h"
@@ -25,8 +24,8 @@
  * @param[out] em Pointer to an array that is the tensor code word
  * @param[in] m Pointer to an array that is the message
  */
-void code_encode(uint64_t *em, const uint64_t *m) {
-    uint64_t tmp[VEC_N1_SIZE_64] = {0};
+void code_encode(uint64_t *em, const uint8_t *m) {
+    uint8_t tmp[VEC_N1_SIZE_BYTES] = {0};
 
     reed_solomon_encode(tmp, m);
     reed_muller_encode(em, tmp);
@@ -45,8 +44,8 @@
  * @param[out] m Pointer to an array that is the message
  * @param[in] em Pointer to an array that is the code word
  */
-void code_decode(uint64_t *m, const uint64_t *em) {
-    uint64_t tmp[VEC_N1_SIZE_64] = {0};
+void code_decode(uint8_t *m, const uint64_t *em) {
+    uint8_t tmp[VEC_N1_SIZE_BYTES] = {0};
 
     reed_muller_decode(tmp, em);
     reed_solomon_decode(m, tmp);

