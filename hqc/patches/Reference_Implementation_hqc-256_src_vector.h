--- upstream/Reference_Implementation/hqc-256/src/vector.h
+++ upstream-patched/Reference_Implementation/hqc-256/src/vector.h
@@ -3,21 +3,18 @@
 
 /**
  * @file vector.h
- * @brief Header file for vector.cpp
+ * @brief Header file for vector.c
  */
 
 #include "shake_prng.h"
+#include <stddef.h>
 #include <stdint.h>
 
 void vect_set_random_fixed_weight(seedexpander_state *ctx, uint64_t *v, uint16_t weight);
 void vect_set_random(seedexpander_state *ctx, uint64_t *v);
-void vect_set_random_from_prng(uint64_t *v, uint32_t size_v);
 
-void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size);
-uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size);
+void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, size_t size);
+uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, size_t size);
 void vect_resize(uint64_t *o, uint32_t size_o, const uint64_t *v, uint32_t size_v);
 
-void vect_print(const uint64_t *v, const uint32_t size);
-void vect_print_sparse(const uint32_t *v, const uint16_t weight);
-
 #endif
