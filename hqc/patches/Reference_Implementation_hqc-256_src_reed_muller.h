--- upstream/Reference_Implementation/hqc-256/src/reed_muller.h
+++ upstream-patched/Reference_Implementation/hqc-256/src/reed_muller.h
@@ -3,14 +3,12 @@
 
 /**
  * @file reed_muller.h
- * @brief Header file of reed_muller.cpp
+ * @brief Header file of reed_muller.c
  */
 
-#include "parameters.h"
-#include <stddef.h>
 #include <stdint.h>
 
-void reed_muller_encode(uint64_t* cdw, const uint64_t* msg);
-void reed_muller_decode(uint64_t* msg, const uint64_t* cdw);
+void reed_muller_encode(uint64_t *cdw, const uint8_t *msg);
+void reed_muller_decode(uint8_t *msg, const uint64_t *cdw);
 
 #endif

