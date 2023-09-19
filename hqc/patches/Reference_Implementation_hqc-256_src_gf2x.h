--- upstream/Reference_Implementation/hqc-256/src/gf2x.h
+++ upstream-patched/Reference_Implementation/hqc-256/src/gf2x.h
@@ -1,18 +1,13 @@
 /**
- * \file gf2x.h
- * \brief Header file for gf2x.cpp
+ * @file gf2x.h
+ * @brief Header file for gf2x.c
  */
 
 #ifndef GF2X_H
 #define GF2X_H
 
-#include <NTL/GF2X.h>
-#include <inttypes.h>
-
-#include "parameters.h"
-
-using namespace NTL;
+#include <stdint.h>
 
 void vect_mul(uint64_t *o, const uint64_t *v1, const uint64_t *v2);
 
-#endif
\ No newline at end of file
+#endif

