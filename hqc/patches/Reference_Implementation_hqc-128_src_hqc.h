--- upstream/Reference_Implementation/hqc-128/src/hqc.h
+++ upstream-patched/Reference_Implementation/hqc-128/src/hqc.h
@@ -8,8 +8,8 @@
 
 #include <stdint.h>
 
-void hqc_pke_keygen(unsigned char* pk, unsigned char* sk);
-void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk);
-uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk);
+void hqc_pke_keygen(uint8_t *pk, uint8_t *sk);
+void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint8_t *m, unsigned char *theta, const unsigned char *pk);
+uint8_t hqc_pke_decrypt(uint8_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const unsigned char *sk);
 
-#endif
\ No newline at end of file
+#endif

