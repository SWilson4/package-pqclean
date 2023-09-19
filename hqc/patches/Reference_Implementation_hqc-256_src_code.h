--- upstream/Reference_Implementation/hqc-256/src/code.h
+++ upstream-patched/Reference_Implementation/hqc-256/src/code.h
@@ -3,14 +3,12 @@
 
 /**
  * @file code.h
- * @brief Header file of code.cpp
+ * @brief Header file of code.c
  */
 
-#include "parameters.h"
-#include <stddef.h>
 #include <stdint.h>
 
-void code_encode(uint64_t *codeword, const uint64_t *message);
-void code_decode(uint64_t *message, const uint64_t *vector);
+void code_encode(uint64_t *em, const uint8_t *message);
+void code_decode(uint8_t *m, const uint64_t *em);
 
 #endif

