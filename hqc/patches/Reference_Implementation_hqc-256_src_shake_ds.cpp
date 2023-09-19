--- upstream/Reference_Implementation/hqc-256/src/shake_ds.cpp
+++ upstream-patched/Reference_Implementation/hqc-256/src/shake_ds.cpp
@@ -1,6 +1,6 @@
 
 /**
- * @file shake_ds.cpp
+ * @file shake_ds.c
  * @brief Implementation SHAKE-256 with incremental API and domain separation
  */
 
@@ -10,7 +10,7 @@
 /**
  * @brief SHAKE-256 with incremental API and domain separation
  *
- * Derived from function SHAKE_256 in fips202.cpp
+ * Derived from function SHAKE_256 in fips202.c
  *
  * @param[out] state Internal state of SHAKE
  * @param[in] output Pointer to output
@@ -33,4 +33,7 @@
 
     /* Squeeze output */
     shake256_inc_squeeze(output, 512/8, state);
+
+    /* Release ctx */
+    shake256_inc_ctx_release(state);
 }

