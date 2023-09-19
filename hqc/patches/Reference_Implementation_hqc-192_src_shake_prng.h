--- upstream/Reference_Implementation/hqc-192/src/shake_prng.h
+++ upstream-patched/Reference_Implementation/hqc-192/src/shake_prng.h
@@ -3,18 +3,17 @@
 
 /**
  * @file shake_prng.h
- * @brief Header file of shake_prng.cpp
+ * @brief Header file of shake_prng.c
  */
 
 #include <stdint.h>
 #include "fips202.h"
-#include "domains.h"
+#include <stddef.h>
 
 typedef shake256incctx seedexpander_state;
 
-void shake_prng_init(uint8_t *entropy_input, uint8_t *personalization_string, uint32_t enlen, uint32_t perlen);
-void shake_prng(uint8_t *output, uint32_t outlen);
-void seedexpander_init(seedexpander_state *state, const uint8_t *seed, uint32_t seedlen);
-void seedexpander(seedexpander_state *state, uint8_t *output, uint32_t outlen);
+void seedexpander_init(seedexpander_state *state, const uint8_t *seed, size_t seedlen);
+void seedexpander(seedexpander_state *state, uint8_t *output, size_t outlen);
+void seedexpander_release(seedexpander_state *state);
 
 #endif

