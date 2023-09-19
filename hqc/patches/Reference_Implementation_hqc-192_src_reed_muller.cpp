--- upstream/Reference_Implementation/hqc-192/src/reed_muller.cpp
+++ upstream-patched/Reference_Implementation/hqc-192/src/reed_muller.cpp
@@ -1,5 +1,5 @@
 /**
- * @file reed_muller.cpp
+ * @file reed_muller.c
  * @brief Constant time implementation of Reed-Muller code RM(1,7)
  */
 
@@ -11,25 +11,8 @@
 // number of repeated code words
 #define MULTIPLICITY                   CEIL_DIVIDE(PARAM_N2, 128)
 
-// codeword is 128 bits, seen multiple ways
-typedef union {
-    uint8_t u8[16];
-    uint32_t u32[4];
-} codeword;
-
-// Expanded codeword has a short for every bit, for internal calculations
-typedef int16_t expandedCodeword[128];
-
 // copy bit 0 into all bits of a 32 bit value
-#define BIT0MASK(x) (int32_t)(-((x) & 1))
-
-
-void encode(codeword *word, int32_t message);
-void hadamard(expandedCodeword *src, expandedCodeword *dst);
-void expand_and_sum(expandedCodeword *dest, codeword src[]);
-int32_t find_peaks(expandedCodeword *transform);
-
-
+#define BIT0MASK(x) (uint32_t)(-((x) & 1))
 
 /**
  * @brief Encode a single byte into a single codeword using RM(1,7)
@@ -48,26 +31,26 @@
  * @param[out] word An RM(1,7) codeword
  * @param[in] message A message
  */
-void encode(codeword *word, int32_t message) {
-    int32_t first_word;
-
+static void encode(uint64_t *cword, uint8_t message) {
+    uint32_t first_word;
+    // bit 7 flips all the bits, do that first to save work
     first_word = BIT0MASK(message >> 7);
-
+    // bits 0, 1, 2, 3, 4 are the same for all four longs
+    // (Warning: in the bit matrix above, low bits are at the left!)
     first_word ^= BIT0MASK(message >> 0) & 0xaaaaaaaa;
     first_word ^= BIT0MASK(message >> 1) & 0xcccccccc;
     first_word ^= BIT0MASK(message >> 2) & 0xf0f0f0f0;
     first_word ^= BIT0MASK(message >> 3) & 0xff00ff00;
     first_word ^= BIT0MASK(message >> 4) & 0xffff0000;
-
-    word->u32[0] = first_word;
-
+    // we can store this in the first quarter
+    cword[0] = first_word;
+    // bit 5 flips entries 1 and 3; bit 6 flips 2 and 3
     first_word ^= BIT0MASK(message >> 5);
-    word->u32[1] = first_word;
+    cword[0] |= (uint64_t)first_word << 32;
     first_word ^= BIT0MASK(message >> 6);
-    word->u32[3] = first_word;
+    cword[1] = (uint64_t)first_word << 32;
     first_word ^= BIT0MASK(message >> 5);
-    word->u32[2] = first_word;
-    return;
+    cword[1] |= first_word;
 }
 
 
@@ -81,19 +64,20 @@
  * @param[out] src Structure that contain the expanded codeword
  * @param[out] dst Structure that contain the expanded codeword
  */
-void hadamard(expandedCodeword *src, expandedCodeword *dst) {
+static void hadamard(uint16_t src[128], uint16_t dst[128]) {
     // the passes move data:
     // src -> dst -> src -> dst -> src -> dst -> src -> dst
     // using p1 and p2 alternately
-    expandedCodeword *p1 = src;
-    expandedCodeword *p2 = dst;
-    for (int32_t pass = 0; pass < 7; pass++) {
-        for (int32_t i = 0; i < 64; i++) {
-            (*p2)[i] = (*p1)[2 * i] + (*p1)[2 * i + 1];
-            (*p2)[i + 64] = (*p1)[2 * i] - (*p1)[2 * i + 1];
+    uint16_t *p1 = src;
+    uint16_t *p2 = dst;
+    uint16_t *p3;
+    for (size_t pass = 0; pass < 7; ++pass) {
+        for (size_t i = 0; i < 64; ++i) {
+            p2[i] = p1[2 * i] + p1[2 * i + 1];
+            p2[i + 64] = p1[2 * i] - p1[2 * i + 1];
         }
         // swap p1, p2 for next round
-        expandedCodeword *p3 = p1;
+        p3 = p1;
         p1 = p2;
         p2 = p3;
     }
@@ -114,18 +98,18 @@
  * @param[out] dest Structure that contain the expanded codeword
  * @param[in] src Structure that contain the codeword
  */
-void expand_and_sum(expandedCodeword *dest, codeword src[]) {
+static void expand_and_sum(uint16_t dest[128], const uint64_t src[2 * MULTIPLICITY]) {
     // start with the first copy
-    for (int32_t part = 0; part < 4; part++) {
-        for (int32_t bit = 0; bit < 32; bit++) {
-            (*dest)[part * 32 + bit] = src[0].u32[part] >> bit & 1;
+    for (size_t part = 0; part < 2; ++part) {
+        for (size_t bit = 0; bit < 64; ++bit) {
+            dest[part * 64 + bit] = ((src[part] >> bit) & 1);
         }
     }
     // sum the rest of the copies
-    for (int32_t copy = 1; copy < MULTIPLICITY; copy++) {
-        for (int32_t part = 0; part < 4; part++) {
-            for (int32_t bit = 0; bit < 32; bit++) {
-                (*dest)[part * 32 + bit] += src[copy].u32[part] >> bit & 1;
+    for (size_t copy = 1; copy < MULTIPLICITY; ++copy) {
+        for (size_t part = 0; part < 2; ++part) {
+            for (size_t bit = 0; bit < 64; ++bit) {
+                dest[part * 64 + bit] += (uint16_t) ((src[2 * copy + part] >> bit) & 1);
             }
         }
     }
@@ -142,23 +126,22 @@
  * in the lowest 7 bits it taken
  * @param[in] transform Structure that contain the expanded codeword
  */
-int32_t find_peaks(expandedCodeword *transform) {
-    int32_t peak_abs_value = 0;
-    int32_t peak_value = 0;
-    int32_t peak_pos = 0;
-    for (int32_t i = 0; i < 128; i++) {
-        // get absolute value
-        int32_t t = (*transform)[i];
-        int32_t pos_mask = -(t > 0);
-        int32_t absolute = (pos_mask & t) | (~pos_mask & -t);
-        // all compilers nowadays compile with a conditional move
-        peak_value = absolute > peak_abs_value ? t : peak_value;
-        peak_pos = absolute > peak_abs_value ? i : peak_pos;
-        peak_abs_value = absolute > peak_abs_value ? absolute : peak_abs_value;
+static uint8_t find_peaks(const uint16_t transform[128]) {
+    uint16_t peak_abs = 0;
+    uint16_t peak = 0;
+    uint16_t pos = 0;
+    uint16_t t, abs, mask;
+    for (uint16_t i = 0; i < 128; ++i) {
+        t = transform[i];
+        abs = t ^ ((uint16_t)(-(t >> 15)) & (t ^ -t)); // t = abs(t)
+        mask = -(((uint16_t)(peak_abs - abs)) >> 15);
+        peak ^= mask & (peak ^ t);
+        pos ^= mask & (pos ^ i);
+        peak_abs ^= mask & (peak_abs ^ abs);
     }
     // set bit 7
-    peak_pos |= 128 * (peak_value > 0);
-    return peak_pos;
+    pos |= 128 & (uint16_t)((peak >> 15) - 1);
+    return (uint8_t) pos;
 }
 
 
@@ -172,20 +155,15 @@
  * @param[out] cdw Array of size VEC_N1N2_SIZE_64 receiving the encoded message
  * @param[in] msg Array of size VEC_N1_SIZE_64 storing the message
  */
-void reed_muller_encode(uint64_t *cdw, const uint64_t *msg) {
-    uint8_t *message_array = (uint8_t *) msg;
-    codeword *codeArray = (codeword *) cdw;
-    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
-        // fill entries i * MULTIPLICITY to (i+1) * MULTIPLICITY
-        int32_t pos = i * MULTIPLICITY;
+void reed_muller_encode(uint64_t *cdw, const uint8_t *msg) {
+    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
         // encode first word
-        encode(&codeArray[pos], message_array[i]);
+        encode(&cdw[2 * i * MULTIPLICITY], msg[i]);
         // copy to other identical codewords
-        for (size_t copy = 1; copy < MULTIPLICITY; copy++) {
-            memcpy(&codeArray[pos + copy], &codeArray[pos], sizeof(codeword));
+        for (size_t copy = 1; copy < MULTIPLICITY; ++copy) {
+            memcpy(&cdw[2 * i * MULTIPLICITY + 2 * copy], &cdw[2 * i * MULTIPLICITY], 16);
         }
     }
-    return;
 }
 
 
@@ -199,19 +177,17 @@
  * @param[out] msg Array of size VEC_N1_SIZE_64 receiving the decoded message
  * @param[in] cdw Array of size VEC_N1N2_SIZE_64 storing the received word
  */
-void reed_muller_decode(uint64_t *msg, const uint64_t *cdw) {
-    uint8_t *message_array = (uint8_t *) msg;
-    codeword *codeArray = (codeword *) cdw;
-    expandedCodeword expanded;
-    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
+void reed_muller_decode(uint8_t *msg, const uint64_t *cdw) {
+    uint16_t expanded[128];
+    uint16_t transform[128];
+    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
         // collect the codewords
-        expand_and_sum(&expanded, &codeArray[i * MULTIPLICITY]);
+        expand_and_sum(expanded, &cdw[2 * i * MULTIPLICITY]);
         // apply hadamard transform
-        expandedCodeword transform;
-        hadamard(&expanded, &transform);
+        hadamard(expanded, transform);
         // fix the first entry to get the half Hadamard transform
         transform[0] -= 64 * MULTIPLICITY;
         // finish the decoding
-        message_array[i] = find_peaks(&transform);
+        msg[i] = find_peaks(transform);
     }
 }

