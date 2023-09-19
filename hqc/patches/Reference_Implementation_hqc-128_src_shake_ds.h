--- upstream/Reference_Implementation/hqc-128/src/shake_ds.h
+++ upstream-patched/Reference_Implementation/hqc-128/src/shake_ds.h
@@ -3,12 +3,12 @@
 
 /**
  * @file shake_ds.h
- * @brief Header file of shake_ds.cpp
+ * @brief Header file of shake_ds.c
  */
 
 #include <stdint.h>
 #include "fips202.h"
-#include "domains.h"
+#include <stddef.h>
 
 void shake256_512_ds(shake256incctx *state, uint8_t *output, const uint8_t *input, size_t inlen, uint8_t domain);
 

