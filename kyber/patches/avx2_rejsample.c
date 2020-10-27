--- upstream/avx2/rejsample.c
+++ upstream-patched/avx2/rejsample.c
@@ -1,6 +1,7 @@
 #include <stdint.h>
 #include <immintrin.h>
 #include <string.h>
+#include "align.h"
 #include "params.h"
 #include "consts.h"
 #include "rejsample.h"
@@ -8,263 +9,264 @@
 //#define BMI
 
 #ifndef BMI
-static const uint8_t idx[256][8] = {
-  {-1, -1, -1, -1, -1, -1, -1, -1},
-  { 0, -1, -1, -1, -1, -1, -1, -1},
-  { 2, -1, -1, -1, -1, -1, -1, -1},
-  { 0,  2, -1, -1, -1, -1, -1, -1},
-  { 4, -1, -1, -1, -1, -1, -1, -1},
-  { 0,  4, -1, -1, -1, -1, -1, -1},
-  { 2,  4, -1, -1, -1, -1, -1, -1},
-  { 0,  2,  4, -1, -1, -1, -1, -1},
-  { 6, -1, -1, -1, -1, -1, -1, -1},
-  { 0,  6, -1, -1, -1, -1, -1, -1},
-  { 2,  6, -1, -1, -1, -1, -1, -1},
-  { 0,  2,  6, -1, -1, -1, -1, -1},
-  { 4,  6, -1, -1, -1, -1, -1, -1},
-  { 0,  4,  6, -1, -1, -1, -1, -1},
-  { 2,  4,  6, -1, -1, -1, -1, -1},
-  { 0,  2,  4,  6, -1, -1, -1, -1},
-  { 8, -1, -1, -1, -1, -1, -1, -1},
-  { 0,  8, -1, -1, -1, -1, -1, -1},
-  { 2,  8, -1, -1, -1, -1, -1, -1},
-  { 0,  2,  8, -1, -1, -1, -1, -1},
-  { 4,  8, -1, -1, -1, -1, -1, -1},
-  { 0,  4,  8, -1, -1, -1, -1, -1},
-  { 2,  4,  8, -1, -1, -1, -1, -1},
-  { 0,  2,  4,  8, -1, -1, -1, -1},
-  { 6,  8, -1, -1, -1, -1, -1, -1},
-  { 0,  6,  8, -1, -1, -1, -1, -1},
-  { 2,  6,  8, -1, -1, -1, -1, -1},
-  { 0,  2,  6,  8, -1, -1, -1, -1},
-  { 4,  6,  8, -1, -1, -1, -1, -1},
-  { 0,  4,  6,  8, -1, -1, -1, -1},
-  { 2,  4,  6,  8, -1, -1, -1, -1},
-  { 0,  2,  4,  6,  8, -1, -1, -1},
-  {10, -1, -1, -1, -1, -1, -1, -1},
-  { 0, 10, -1, -1, -1, -1, -1, -1},
-  { 2, 10, -1, -1, -1, -1, -1, -1},
-  { 0,  2, 10, -1, -1, -1, -1, -1},
-  { 4, 10, -1, -1, -1, -1, -1, -1},
-  { 0,  4, 10, -1, -1, -1, -1, -1},
-  { 2,  4, 10, -1, -1, -1, -1, -1},
-  { 0,  2,  4, 10, -1, -1, -1, -1},
-  { 6, 10, -1, -1, -1, -1, -1, -1},
-  { 0,  6, 10, -1, -1, -1, -1, -1},
-  { 2,  6, 10, -1, -1, -1, -1, -1},
-  { 0,  2,  6, 10, -1, -1, -1, -1},
-  { 4,  6, 10, -1, -1, -1, -1, -1},
-  { 0,  4,  6, 10, -1, -1, -1, -1},
-  { 2,  4,  6, 10, -1, -1, -1, -1},
-  { 0,  2,  4,  6, 10, -1, -1, -1},
-  { 8, 10, -1, -1, -1, -1, -1, -1},
-  { 0,  8, 10, -1, -1, -1, -1, -1},
-  { 2,  8, 10, -1, -1, -1, -1, -1},
-  { 0,  2,  8, 10, -1, -1, -1, -1},
-  { 4,  8, 10, -1, -1, -1, -1, -1},
-  { 0,  4,  8, 10, -1, -1, -1, -1},
-  { 2,  4,  8, 10, -1, -1, -1, -1},
-  { 0,  2,  4,  8, 10, -1, -1, -1},
-  { 6,  8, 10, -1, -1, -1, -1, -1},
-  { 0,  6,  8, 10, -1, -1, -1, -1},
-  { 2,  6,  8, 10, -1, -1, -1, -1},
-  { 0,  2,  6,  8, 10, -1, -1, -1},
-  { 4,  6,  8, 10, -1, -1, -1, -1},
-  { 0,  4,  6,  8, 10, -1, -1, -1},
-  { 2,  4,  6,  8, 10, -1, -1, -1},
-  { 0,  2,  4,  6,  8, 10, -1, -1},
-  {12, -1, -1, -1, -1, -1, -1, -1},
-  { 0, 12, -1, -1, -1, -1, -1, -1},
-  { 2, 12, -1, -1, -1, -1, -1, -1},
-  { 0,  2, 12, -1, -1, -1, -1, -1},
-  { 4, 12, -1, -1, -1, -1, -1, -1},
-  { 0,  4, 12, -1, -1, -1, -1, -1},
-  { 2,  4, 12, -1, -1, -1, -1, -1},
-  { 0,  2,  4, 12, -1, -1, -1, -1},
-  { 6, 12, -1, -1, -1, -1, -1, -1},
-  { 0,  6, 12, -1, -1, -1, -1, -1},
-  { 2,  6, 12, -1, -1, -1, -1, -1},
-  { 0,  2,  6, 12, -1, -1, -1, -1},
-  { 4,  6, 12, -1, -1, -1, -1, -1},
-  { 0,  4,  6, 12, -1, -1, -1, -1},
-  { 2,  4,  6, 12, -1, -1, -1, -1},
-  { 0,  2,  4,  6, 12, -1, -1, -1},
-  { 8, 12, -1, -1, -1, -1, -1, -1},
-  { 0,  8, 12, -1, -1, -1, -1, -1},
-  { 2,  8, 12, -1, -1, -1, -1, -1},
-  { 0,  2,  8, 12, -1, -1, -1, -1},
-  { 4,  8, 12, -1, -1, -1, -1, -1},
-  { 0,  4,  8, 12, -1, -1, -1, -1},
-  { 2,  4,  8, 12, -1, -1, -1, -1},
-  { 0,  2,  4,  8, 12, -1, -1, -1},
-  { 6,  8, 12, -1, -1, -1, -1, -1},
-  { 0,  6,  8, 12, -1, -1, -1, -1},
-  { 2,  6,  8, 12, -1, -1, -1, -1},
-  { 0,  2,  6,  8, 12, -1, -1, -1},
-  { 4,  6,  8, 12, -1, -1, -1, -1},
-  { 0,  4,  6,  8, 12, -1, -1, -1},
-  { 2,  4,  6,  8, 12, -1, -1, -1},
-  { 0,  2,  4,  6,  8, 12, -1, -1},
-  {10, 12, -1, -1, -1, -1, -1, -1},
-  { 0, 10, 12, -1, -1, -1, -1, -1},
-  { 2, 10, 12, -1, -1, -1, -1, -1},
-  { 0,  2, 10, 12, -1, -1, -1, -1},
-  { 4, 10, 12, -1, -1, -1, -1, -1},
-  { 0,  4, 10, 12, -1, -1, -1, -1},
-  { 2,  4, 10, 12, -1, -1, -1, -1},
-  { 0,  2,  4, 10, 12, -1, -1, -1},
-  { 6, 10, 12, -1, -1, -1, -1, -1},
-  { 0,  6, 10, 12, -1, -1, -1, -1},
-  { 2,  6, 10, 12, -1, -1, -1, -1},
-  { 0,  2,  6, 10, 12, -1, -1, -1},
-  { 4,  6, 10, 12, -1, -1, -1, -1},
-  { 0,  4,  6, 10, 12, -1, -1, -1},
-  { 2,  4,  6, 10, 12, -1, -1, -1},
-  { 0,  2,  4,  6, 10, 12, -1, -1},
-  { 8, 10, 12, -1, -1, -1, -1, -1},
-  { 0,  8, 10, 12, -1, -1, -1, -1},
-  { 2,  8, 10, 12, -1, -1, -1, -1},
-  { 0,  2,  8, 10, 12, -1, -1, -1},
-  { 4,  8, 10, 12, -1, -1, -1, -1},
-  { 0,  4,  8, 10, 12, -1, -1, -1},
-  { 2,  4,  8, 10, 12, -1, -1, -1},
-  { 0,  2,  4,  8, 10, 12, -1, -1},
-  { 6,  8, 10, 12, -1, -1, -1, -1},
-  { 0,  6,  8, 10, 12, -1, -1, -1},
-  { 2,  6,  8, 10, 12, -1, -1, -1},
-  { 0,  2,  6,  8, 10, 12, -1, -1},
-  { 4,  6,  8, 10, 12, -1, -1, -1},
-  { 0,  4,  6,  8, 10, 12, -1, -1},
-  { 2,  4,  6,  8, 10, 12, -1, -1},
-  { 0,  2,  4,  6,  8, 10, 12, -1},
-  {14, -1, -1, -1, -1, -1, -1, -1},
-  { 0, 14, -1, -1, -1, -1, -1, -1},
-  { 2, 14, -1, -1, -1, -1, -1, -1},
-  { 0,  2, 14, -1, -1, -1, -1, -1},
-  { 4, 14, -1, -1, -1, -1, -1, -1},
-  { 0,  4, 14, -1, -1, -1, -1, -1},
-  { 2,  4, 14, -1, -1, -1, -1, -1},
-  { 0,  2,  4, 14, -1, -1, -1, -1},
-  { 6, 14, -1, -1, -1, -1, -1, -1},
-  { 0,  6, 14, -1, -1, -1, -1, -1},
-  { 2,  6, 14, -1, -1, -1, -1, -1},
-  { 0,  2,  6, 14, -1, -1, -1, -1},
-  { 4,  6, 14, -1, -1, -1, -1, -1},
-  { 0,  4,  6, 14, -1, -1, -1, -1},
-  { 2,  4,  6, 14, -1, -1, -1, -1},
-  { 0,  2,  4,  6, 14, -1, -1, -1},
-  { 8, 14, -1, -1, -1, -1, -1, -1},
-  { 0,  8, 14, -1, -1, -1, -1, -1},
-  { 2,  8, 14, -1, -1, -1, -1, -1},
-  { 0,  2,  8, 14, -1, -1, -1, -1},
-  { 4,  8, 14, -1, -1, -1, -1, -1},
-  { 0,  4,  8, 14, -1, -1, -1, -1},
-  { 2,  4,  8, 14, -1, -1, -1, -1},
-  { 0,  2,  4,  8, 14, -1, -1, -1},
-  { 6,  8, 14, -1, -1, -1, -1, -1},
-  { 0,  6,  8, 14, -1, -1, -1, -1},
-  { 2,  6,  8, 14, -1, -1, -1, -1},
-  { 0,  2,  6,  8, 14, -1, -1, -1},
-  { 4,  6,  8, 14, -1, -1, -1, -1},
-  { 0,  4,  6,  8, 14, -1, -1, -1},
-  { 2,  4,  6,  8, 14, -1, -1, -1},
-  { 0,  2,  4,  6,  8, 14, -1, -1},
-  {10, 14, -1, -1, -1, -1, -1, -1},
-  { 0, 10, 14, -1, -1, -1, -1, -1},
-  { 2, 10, 14, -1, -1, -1, -1, -1},
-  { 0,  2, 10, 14, -1, -1, -1, -1},
-  { 4, 10, 14, -1, -1, -1, -1, -1},
-  { 0,  4, 10, 14, -1, -1, -1, -1},
-  { 2,  4, 10, 14, -1, -1, -1, -1},
-  { 0,  2,  4, 10, 14, -1, -1, -1},
-  { 6, 10, 14, -1, -1, -1, -1, -1},
-  { 0,  6, 10, 14, -1, -1, -1, -1},
-  { 2,  6, 10, 14, -1, -1, -1, -1},
-  { 0,  2,  6, 10, 14, -1, -1, -1},
-  { 4,  6, 10, 14, -1, -1, -1, -1},
-  { 0,  4,  6, 10, 14, -1, -1, -1},
-  { 2,  4,  6, 10, 14, -1, -1, -1},
-  { 0,  2,  4,  6, 10, 14, -1, -1},
-  { 8, 10, 14, -1, -1, -1, -1, -1},
-  { 0,  8, 10, 14, -1, -1, -1, -1},
-  { 2,  8, 10, 14, -1, -1, -1, -1},
-  { 0,  2,  8, 10, 14, -1, -1, -1},
-  { 4,  8, 10, 14, -1, -1, -1, -1},
-  { 0,  4,  8, 10, 14, -1, -1, -1},
-  { 2,  4,  8, 10, 14, -1, -1, -1},
-  { 0,  2,  4,  8, 10, 14, -1, -1},
-  { 6,  8, 10, 14, -1, -1, -1, -1},
-  { 0,  6,  8, 10, 14, -1, -1, -1},
-  { 2,  6,  8, 10, 14, -1, -1, -1},
-  { 0,  2,  6,  8, 10, 14, -1, -1},
-  { 4,  6,  8, 10, 14, -1, -1, -1},
-  { 0,  4,  6,  8, 10, 14, -1, -1},
-  { 2,  4,  6,  8, 10, 14, -1, -1},
-  { 0,  2,  4,  6,  8, 10, 14, -1},
-  {12, 14, -1, -1, -1, -1, -1, -1},
-  { 0, 12, 14, -1, -1, -1, -1, -1},
-  { 2, 12, 14, -1, -1, -1, -1, -1},
-  { 0,  2, 12, 14, -1, -1, -1, -1},
-  { 4, 12, 14, -1, -1, -1, -1, -1},
-  { 0,  4, 12, 14, -1, -1, -1, -1},
-  { 2,  4, 12, 14, -1, -1, -1, -1},
-  { 0,  2,  4, 12, 14, -1, -1, -1},
-  { 6, 12, 14, -1, -1, -1, -1, -1},
-  { 0,  6, 12, 14, -1, -1, -1, -1},
-  { 2,  6, 12, 14, -1, -1, -1, -1},
-  { 0,  2,  6, 12, 14, -1, -1, -1},
-  { 4,  6, 12, 14, -1, -1, -1, -1},
-  { 0,  4,  6, 12, 14, -1, -1, -1},
-  { 2,  4,  6, 12, 14, -1, -1, -1},
-  { 0,  2,  4,  6, 12, 14, -1, -1},
-  { 8, 12, 14, -1, -1, -1, -1, -1},
-  { 0,  8, 12, 14, -1, -1, -1, -1},
-  { 2,  8, 12, 14, -1, -1, -1, -1},
-  { 0,  2,  8, 12, 14, -1, -1, -1},
-  { 4,  8, 12, 14, -1, -1, -1, -1},
-  { 0,  4,  8, 12, 14, -1, -1, -1},
-  { 2,  4,  8, 12, 14, -1, -1, -1},
-  { 0,  2,  4,  8, 12, 14, -1, -1},
-  { 6,  8, 12, 14, -1, -1, -1, -1},
-  { 0,  6,  8, 12, 14, -1, -1, -1},
-  { 2,  6,  8, 12, 14, -1, -1, -1},
-  { 0,  2,  6,  8, 12, 14, -1, -1},
-  { 4,  6,  8, 12, 14, -1, -1, -1},
-  { 0,  4,  6,  8, 12, 14, -1, -1},
-  { 2,  4,  6,  8, 12, 14, -1, -1},
-  { 0,  2,  4,  6,  8, 12, 14, -1},
-  {10, 12, 14, -1, -1, -1, -1, -1},
-  { 0, 10, 12, 14, -1, -1, -1, -1},
-  { 2, 10, 12, 14, -1, -1, -1, -1},
-  { 0,  2, 10, 12, 14, -1, -1, -1},
-  { 4, 10, 12, 14, -1, -1, -1, -1},
-  { 0,  4, 10, 12, 14, -1, -1, -1},
-  { 2,  4, 10, 12, 14, -1, -1, -1},
-  { 0,  2,  4, 10, 12, 14, -1, -1},
-  { 6, 10, 12, 14, -1, -1, -1, -1},
-  { 0,  6, 10, 12, 14, -1, -1, -1},
-  { 2,  6, 10, 12, 14, -1, -1, -1},
-  { 0,  2,  6, 10, 12, 14, -1, -1},
-  { 4,  6, 10, 12, 14, -1, -1, -1},
-  { 0,  4,  6, 10, 12, 14, -1, -1},
-  { 2,  4,  6, 10, 12, 14, -1, -1},
-  { 0,  2,  4,  6, 10, 12, 14, -1},
-  { 8, 10, 12, 14, -1, -1, -1, -1},
-  { 0,  8, 10, 12, 14, -1, -1, -1},
-  { 2,  8, 10, 12, 14, -1, -1, -1},
-  { 0,  2,  8, 10, 12, 14, -1, -1},
-  { 4,  8, 10, 12, 14, -1, -1, -1},
-  { 0,  4,  8, 10, 12, 14, -1, -1},
-  { 2,  4,  8, 10, 12, 14, -1, -1},
-  { 0,  2,  4,  8, 10, 12, 14, -1},
-  { 6,  8, 10, 12, 14, -1, -1, -1},
-  { 0,  6,  8, 10, 12, 14, -1, -1},
-  { 2,  6,  8, 10, 12, 14, -1, -1},
-  { 0,  2,  6,  8, 10, 12, 14, -1},
-  { 4,  6,  8, 10, 12, 14, -1, -1},
-  { 0,  4,  6,  8, 10, 12, 14, -1},
-  { 2,  4,  6,  8, 10, 12, 14, -1},
-  { 0,  2,  4,  6,  8, 10, 12, 14}
+static const ALIGN32_ARRAY_2D(uint8_t, 256, 8) idx = {.arr = {
+    {-1, -1, -1, -1, -1, -1, -1, -1},
+    { 0, -1, -1, -1, -1, -1, -1, -1},
+    { 2, -1, -1, -1, -1, -1, -1, -1},
+    { 0,  2, -1, -1, -1, -1, -1, -1},
+    { 4, -1, -1, -1, -1, -1, -1, -1},
+    { 0,  4, -1, -1, -1, -1, -1, -1},
+    { 2,  4, -1, -1, -1, -1, -1, -1},
+    { 0,  2,  4, -1, -1, -1, -1, -1},
+    { 6, -1, -1, -1, -1, -1, -1, -1},
+    { 0,  6, -1, -1, -1, -1, -1, -1},
+    { 2,  6, -1, -1, -1, -1, -1, -1},
+    { 0,  2,  6, -1, -1, -1, -1, -1},
+    { 4,  6, -1, -1, -1, -1, -1, -1},
+    { 0,  4,  6, -1, -1, -1, -1, -1},
+    { 2,  4,  6, -1, -1, -1, -1, -1},
+    { 0,  2,  4,  6, -1, -1, -1, -1},
+    { 8, -1, -1, -1, -1, -1, -1, -1},
+    { 0,  8, -1, -1, -1, -1, -1, -1},
+    { 2,  8, -1, -1, -1, -1, -1, -1},
+    { 0,  2,  8, -1, -1, -1, -1, -1},
+    { 4,  8, -1, -1, -1, -1, -1, -1},
+    { 0,  4,  8, -1, -1, -1, -1, -1},
+    { 2,  4,  8, -1, -1, -1, -1, -1},
+    { 0,  2,  4,  8, -1, -1, -1, -1},
+    { 6,  8, -1, -1, -1, -1, -1, -1},
+    { 0,  6,  8, -1, -1, -1, -1, -1},
+    { 2,  6,  8, -1, -1, -1, -1, -1},
+    { 0,  2,  6,  8, -1, -1, -1, -1},
+    { 4,  6,  8, -1, -1, -1, -1, -1},
+    { 0,  4,  6,  8, -1, -1, -1, -1},
+    { 2,  4,  6,  8, -1, -1, -1, -1},
+    { 0,  2,  4,  6,  8, -1, -1, -1},
+    {10, -1, -1, -1, -1, -1, -1, -1},
+    { 0, 10, -1, -1, -1, -1, -1, -1},
+    { 2, 10, -1, -1, -1, -1, -1, -1},
+    { 0,  2, 10, -1, -1, -1, -1, -1},
+    { 4, 10, -1, -1, -1, -1, -1, -1},
+    { 0,  4, 10, -1, -1, -1, -1, -1},
+    { 2,  4, 10, -1, -1, -1, -1, -1},
+    { 0,  2,  4, 10, -1, -1, -1, -1},
+    { 6, 10, -1, -1, -1, -1, -1, -1},
+    { 0,  6, 10, -1, -1, -1, -1, -1},
+    { 2,  6, 10, -1, -1, -1, -1, -1},
+    { 0,  2,  6, 10, -1, -1, -1, -1},
+    { 4,  6, 10, -1, -1, -1, -1, -1},
+    { 0,  4,  6, 10, -1, -1, -1, -1},
+    { 2,  4,  6, 10, -1, -1, -1, -1},
+    { 0,  2,  4,  6, 10, -1, -1, -1},
+    { 8, 10, -1, -1, -1, -1, -1, -1},
+    { 0,  8, 10, -1, -1, -1, -1, -1},
+    { 2,  8, 10, -1, -1, -1, -1, -1},
+    { 0,  2,  8, 10, -1, -1, -1, -1},
+    { 4,  8, 10, -1, -1, -1, -1, -1},
+    { 0,  4,  8, 10, -1, -1, -1, -1},
+    { 2,  4,  8, 10, -1, -1, -1, -1},
+    { 0,  2,  4,  8, 10, -1, -1, -1},
+    { 6,  8, 10, -1, -1, -1, -1, -1},
+    { 0,  6,  8, 10, -1, -1, -1, -1},
+    { 2,  6,  8, 10, -1, -1, -1, -1},
+    { 0,  2,  6,  8, 10, -1, -1, -1},
+    { 4,  6,  8, 10, -1, -1, -1, -1},
+    { 0,  4,  6,  8, 10, -1, -1, -1},
+    { 2,  4,  6,  8, 10, -1, -1, -1},
+    { 0,  2,  4,  6,  8, 10, -1, -1},
+    {12, -1, -1, -1, -1, -1, -1, -1},
+    { 0, 12, -1, -1, -1, -1, -1, -1},
+    { 2, 12, -1, -1, -1, -1, -1, -1},
+    { 0,  2, 12, -1, -1, -1, -1, -1},
+    { 4, 12, -1, -1, -1, -1, -1, -1},
+    { 0,  4, 12, -1, -1, -1, -1, -1},
+    { 2,  4, 12, -1, -1, -1, -1, -1},
+    { 0,  2,  4, 12, -1, -1, -1, -1},
+    { 6, 12, -1, -1, -1, -1, -1, -1},
+    { 0,  6, 12, -1, -1, -1, -1, -1},
+    { 2,  6, 12, -1, -1, -1, -1, -1},
+    { 0,  2,  6, 12, -1, -1, -1, -1},
+    { 4,  6, 12, -1, -1, -1, -1, -1},
+    { 0,  4,  6, 12, -1, -1, -1, -1},
+    { 2,  4,  6, 12, -1, -1, -1, -1},
+    { 0,  2,  4,  6, 12, -1, -1, -1},
+    { 8, 12, -1, -1, -1, -1, -1, -1},
+    { 0,  8, 12, -1, -1, -1, -1, -1},
+    { 2,  8, 12, -1, -1, -1, -1, -1},
+    { 0,  2,  8, 12, -1, -1, -1, -1},
+    { 4,  8, 12, -1, -1, -1, -1, -1},
+    { 0,  4,  8, 12, -1, -1, -1, -1},
+    { 2,  4,  8, 12, -1, -1, -1, -1},
+    { 0,  2,  4,  8, 12, -1, -1, -1},
+    { 6,  8, 12, -1, -1, -1, -1, -1},
+    { 0,  6,  8, 12, -1, -1, -1, -1},
+    { 2,  6,  8, 12, -1, -1, -1, -1},
+    { 0,  2,  6,  8, 12, -1, -1, -1},
+    { 4,  6,  8, 12, -1, -1, -1, -1},
+    { 0,  4,  6,  8, 12, -1, -1, -1},
+    { 2,  4,  6,  8, 12, -1, -1, -1},
+    { 0,  2,  4,  6,  8, 12, -1, -1},
+    {10, 12, -1, -1, -1, -1, -1, -1},
+    { 0, 10, 12, -1, -1, -1, -1, -1},
+    { 2, 10, 12, -1, -1, -1, -1, -1},
+    { 0,  2, 10, 12, -1, -1, -1, -1},
+    { 4, 10, 12, -1, -1, -1, -1, -1},
+    { 0,  4, 10, 12, -1, -1, -1, -1},
+    { 2,  4, 10, 12, -1, -1, -1, -1},
+    { 0,  2,  4, 10, 12, -1, -1, -1},
+    { 6, 10, 12, -1, -1, -1, -1, -1},
+    { 0,  6, 10, 12, -1, -1, -1, -1},
+    { 2,  6, 10, 12, -1, -1, -1, -1},
+    { 0,  2,  6, 10, 12, -1, -1, -1},
+    { 4,  6, 10, 12, -1, -1, -1, -1},
+    { 0,  4,  6, 10, 12, -1, -1, -1},
+    { 2,  4,  6, 10, 12, -1, -1, -1},
+    { 0,  2,  4,  6, 10, 12, -1, -1},
+    { 8, 10, 12, -1, -1, -1, -1, -1},
+    { 0,  8, 10, 12, -1, -1, -1, -1},
+    { 2,  8, 10, 12, -1, -1, -1, -1},
+    { 0,  2,  8, 10, 12, -1, -1, -1},
+    { 4,  8, 10, 12, -1, -1, -1, -1},
+    { 0,  4,  8, 10, 12, -1, -1, -1},
+    { 2,  4,  8, 10, 12, -1, -1, -1},
+    { 0,  2,  4,  8, 10, 12, -1, -1},
+    { 6,  8, 10, 12, -1, -1, -1, -1},
+    { 0,  6,  8, 10, 12, -1, -1, -1},
+    { 2,  6,  8, 10, 12, -1, -1, -1},
+    { 0,  2,  6,  8, 10, 12, -1, -1},
+    { 4,  6,  8, 10, 12, -1, -1, -1},
+    { 0,  4,  6,  8, 10, 12, -1, -1},
+    { 2,  4,  6,  8, 10, 12, -1, -1},
+    { 0,  2,  4,  6,  8, 10, 12, -1},
+    {14, -1, -1, -1, -1, -1, -1, -1},
+    { 0, 14, -1, -1, -1, -1, -1, -1},
+    { 2, 14, -1, -1, -1, -1, -1, -1},
+    { 0,  2, 14, -1, -1, -1, -1, -1},
+    { 4, 14, -1, -1, -1, -1, -1, -1},
+    { 0,  4, 14, -1, -1, -1, -1, -1},
+    { 2,  4, 14, -1, -1, -1, -1, -1},
+    { 0,  2,  4, 14, -1, -1, -1, -1},
+    { 6, 14, -1, -1, -1, -1, -1, -1},
+    { 0,  6, 14, -1, -1, -1, -1, -1},
+    { 2,  6, 14, -1, -1, -1, -1, -1},
+    { 0,  2,  6, 14, -1, -1, -1, -1},
+    { 4,  6, 14, -1, -1, -1, -1, -1},
+    { 0,  4,  6, 14, -1, -1, -1, -1},
+    { 2,  4,  6, 14, -1, -1, -1, -1},
+    { 0,  2,  4,  6, 14, -1, -1, -1},
+    { 8, 14, -1, -1, -1, -1, -1, -1},
+    { 0,  8, 14, -1, -1, -1, -1, -1},
+    { 2,  8, 14, -1, -1, -1, -1, -1},
+    { 0,  2,  8, 14, -1, -1, -1, -1},
+    { 4,  8, 14, -1, -1, -1, -1, -1},
+    { 0,  4,  8, 14, -1, -1, -1, -1},
+    { 2,  4,  8, 14, -1, -1, -1, -1},
+    { 0,  2,  4,  8, 14, -1, -1, -1},
+    { 6,  8, 14, -1, -1, -1, -1, -1},
+    { 0,  6,  8, 14, -1, -1, -1, -1},
+    { 2,  6,  8, 14, -1, -1, -1, -1},
+    { 0,  2,  6,  8, 14, -1, -1, -1},
+    { 4,  6,  8, 14, -1, -1, -1, -1},
+    { 0,  4,  6,  8, 14, -1, -1, -1},
+    { 2,  4,  6,  8, 14, -1, -1, -1},
+    { 0,  2,  4,  6,  8, 14, -1, -1},
+    {10, 14, -1, -1, -1, -1, -1, -1},
+    { 0, 10, 14, -1, -1, -1, -1, -1},
+    { 2, 10, 14, -1, -1, -1, -1, -1},
+    { 0,  2, 10, 14, -1, -1, -1, -1},
+    { 4, 10, 14, -1, -1, -1, -1, -1},
+    { 0,  4, 10, 14, -1, -1, -1, -1},
+    { 2,  4, 10, 14, -1, -1, -1, -1},
+    { 0,  2,  4, 10, 14, -1, -1, -1},
+    { 6, 10, 14, -1, -1, -1, -1, -1},
+    { 0,  6, 10, 14, -1, -1, -1, -1},
+    { 2,  6, 10, 14, -1, -1, -1, -1},
+    { 0,  2,  6, 10, 14, -1, -1, -1},
+    { 4,  6, 10, 14, -1, -1, -1, -1},
+    { 0,  4,  6, 10, 14, -1, -1, -1},
+    { 2,  4,  6, 10, 14, -1, -1, -1},
+    { 0,  2,  4,  6, 10, 14, -1, -1},
+    { 8, 10, 14, -1, -1, -1, -1, -1},
+    { 0,  8, 10, 14, -1, -1, -1, -1},
+    { 2,  8, 10, 14, -1, -1, -1, -1},
+    { 0,  2,  8, 10, 14, -1, -1, -1},
+    { 4,  8, 10, 14, -1, -1, -1, -1},
+    { 0,  4,  8, 10, 14, -1, -1, -1},
+    { 2,  4,  8, 10, 14, -1, -1, -1},
+    { 0,  2,  4,  8, 10, 14, -1, -1},
+    { 6,  8, 10, 14, -1, -1, -1, -1},
+    { 0,  6,  8, 10, 14, -1, -1, -1},
+    { 2,  6,  8, 10, 14, -1, -1, -1},
+    { 0,  2,  6,  8, 10, 14, -1, -1},
+    { 4,  6,  8, 10, 14, -1, -1, -1},
+    { 0,  4,  6,  8, 10, 14, -1, -1},
+    { 2,  4,  6,  8, 10, 14, -1, -1},
+    { 0,  2,  4,  6,  8, 10, 14, -1},
+    {12, 14, -1, -1, -1, -1, -1, -1},
+    { 0, 12, 14, -1, -1, -1, -1, -1},
+    { 2, 12, 14, -1, -1, -1, -1, -1},
+    { 0,  2, 12, 14, -1, -1, -1, -1},
+    { 4, 12, 14, -1, -1, -1, -1, -1},
+    { 0,  4, 12, 14, -1, -1, -1, -1},
+    { 2,  4, 12, 14, -1, -1, -1, -1},
+    { 0,  2,  4, 12, 14, -1, -1, -1},
+    { 6, 12, 14, -1, -1, -1, -1, -1},
+    { 0,  6, 12, 14, -1, -1, -1, -1},
+    { 2,  6, 12, 14, -1, -1, -1, -1},
+    { 0,  2,  6, 12, 14, -1, -1, -1},
+    { 4,  6, 12, 14, -1, -1, -1, -1},
+    { 0,  4,  6, 12, 14, -1, -1, -1},
+    { 2,  4,  6, 12, 14, -1, -1, -1},
+    { 0,  2,  4,  6, 12, 14, -1, -1},
+    { 8, 12, 14, -1, -1, -1, -1, -1},
+    { 0,  8, 12, 14, -1, -1, -1, -1},
+    { 2,  8, 12, 14, -1, -1, -1, -1},
+    { 0,  2,  8, 12, 14, -1, -1, -1},
+    { 4,  8, 12, 14, -1, -1, -1, -1},
+    { 0,  4,  8, 12, 14, -1, -1, -1},
+    { 2,  4,  8, 12, 14, -1, -1, -1},
+    { 0,  2,  4,  8, 12, 14, -1, -1},
+    { 6,  8, 12, 14, -1, -1, -1, -1},
+    { 0,  6,  8, 12, 14, -1, -1, -1},
+    { 2,  6,  8, 12, 14, -1, -1, -1},
+    { 0,  2,  6,  8, 12, 14, -1, -1},
+    { 4,  6,  8, 12, 14, -1, -1, -1},
+    { 0,  4,  6,  8, 12, 14, -1, -1},
+    { 2,  4,  6,  8, 12, 14, -1, -1},
+    { 0,  2,  4,  6,  8, 12, 14, -1},
+    {10, 12, 14, -1, -1, -1, -1, -1},
+    { 0, 10, 12, 14, -1, -1, -1, -1},
+    { 2, 10, 12, 14, -1, -1, -1, -1},
+    { 0,  2, 10, 12, 14, -1, -1, -1},
+    { 4, 10, 12, 14, -1, -1, -1, -1},
+    { 0,  4, 10, 12, 14, -1, -1, -1},
+    { 2,  4, 10, 12, 14, -1, -1, -1},
+    { 0,  2,  4, 10, 12, 14, -1, -1},
+    { 6, 10, 12, 14, -1, -1, -1, -1},
+    { 0,  6, 10, 12, 14, -1, -1, -1},
+    { 2,  6, 10, 12, 14, -1, -1, -1},
+    { 0,  2,  6, 10, 12, 14, -1, -1},
+    { 4,  6, 10, 12, 14, -1, -1, -1},
+    { 0,  4,  6, 10, 12, 14, -1, -1},
+    { 2,  4,  6, 10, 12, 14, -1, -1},
+    { 0,  2,  4,  6, 10, 12, 14, -1},
+    { 8, 10, 12, 14, -1, -1, -1, -1},
+    { 0,  8, 10, 12, 14, -1, -1, -1},
+    { 2,  8, 10, 12, 14, -1, -1, -1},
+    { 0,  2,  8, 10, 12, 14, -1, -1},
+    { 4,  8, 10, 12, 14, -1, -1, -1},
+    { 0,  4,  8, 10, 12, 14, -1, -1},
+    { 2,  4,  8, 10, 12, 14, -1, -1},
+    { 0,  2,  4,  8, 10, 12, 14, -1},
+    { 6,  8, 10, 12, 14, -1, -1, -1},
+    { 0,  6,  8, 10, 12, 14, -1, -1},
+    { 2,  6,  8, 10, 12, 14, -1, -1},
+    { 0,  2,  6,  8, 10, 12, 14, -1},
+    { 4,  6,  8, 10, 12, 14, -1, -1},
+    { 0,  4,  6,  8, 10, 12, 14, -1},
+    { 2,  4,  6,  8, 10, 12, 14, -1},
+    { 0,  2,  4,  6,  8, 10, 12, 14}
+  }
 };
 #endif
 
@@ -331,10 +333,10 @@
     g0 = _mm256_inserti128_si256(g0, _mm_cvtsi64_si128(idx2), 1);
     g1 = _mm256_inserti128_si256(g1, _mm_cvtsi64_si128(idx3), 1);
 #else
-    g0 = _mm256_castsi128_si256(_mm_loadl_epi64((__m128i *)&idx[(good >>  0) & 0xFF]));
-    g1 = _mm256_castsi128_si256(_mm_loadl_epi64((__m128i *)&idx[(good >>  8) & 0xFF]));
-    g0 = _mm256_inserti128_si256(g0, _mm_loadl_epi64((__m128i *)&idx[(good >> 16) & 0xFF]), 1);
-    g1 = _mm256_inserti128_si256(g1, _mm_loadl_epi64((__m128i *)&idx[(good >> 24) & 0xFF]), 1);
+    g0 = _mm256_castsi128_si256(_mm_loadl_epi64((__m128i *)&idx.arr[(good >>  0) & 0xFF]));
+    g1 = _mm256_castsi128_si256(_mm_loadl_epi64((__m128i *)&idx.arr[(good >>  8) & 0xFF]));
+    g0 = _mm256_inserti128_si256(g0, _mm_loadl_epi64((__m128i *)&idx.arr[(good >> 16) & 0xFF]), 1);
+    g1 = _mm256_inserti128_si256(g1, _mm_loadl_epi64((__m128i *)&idx.arr[(good >> 24) & 0xFF]), 1);
 #endif
 
     g2 = _mm256_add_epi8(g0, ones);
@@ -374,7 +376,7 @@
     pilo = _mm_cvtsi64_si128(idx0);
 #else
     good = _pext_u32(good, 0x5555);
-    pilo = _mm_loadl_epi64((__m128i *)&idx[good]);
+    pilo = _mm_loadl_epi64((__m128i *)&idx.arr[good]);
 #endif
 
     pihi = _mm_add_epi8(pilo, _mm256_castsi256_si128(ones));
