--- GeMSS-Round2_V2.a/Reference_Implementation/sign/GeMSS128/src/genSecretMQS_gf2.c
+++ GeMSS-Round2_V2.a-patched/Reference_Implementation/sign/GeMSS128/src/genSecretMQS_gf2.c
@@ -483,15 +483,13 @@
     for(i=2;i<HFEDegI;++i)
     {
         /* Here a_vec = row i */
+        j = 0;
         #if ENABLED_REMOVE_ODD_DEGREE
-        j=(((ONE32<<i)+ONE32)<=HFE_odd_degree)?0:1;
+        if (((ONE32<<i)+ONE32) > HFE_odd_degree) j=1;
+        #endif
+        /* Here a_vec = row i */
         a_vecj=alpha_vec+j*(HFEn-1)*NB_WORD_GFqn;
         for(;j<i;++j)
-        #else
-        /* Here a_vec = row i */
-        a_vecj=alpha_vec;
-        for(j=0;j<i;++j)
-        #endif
         {
             a_veci=a_vec;
             QUADRATIC_CASE_REF(a_veci,a_vecj);
@@ -509,19 +507,19 @@
     /* The current term is X^(q^HFEDegi + q^j) */
 
     /* Here a_vec = row i */
+    j = 0;
     #if ENABLED_REMOVE_ODD_DEGREE
-    j=(((ONE32<<i)+ONE32)<=HFE_odd_degree)?0:1;
-    a_vecj=alpha_vec+j*(HFEn-1)*NB_WORD_GFqn;
-    for(;j<HFEDegJ;++j)
-    #else
-    /* Here a_vec = row i */
-    a_vecj=alpha_vec;
-    for(j=0;j<HFEDegJ;++j)
+    if (((ONE32<<i)+ONE32) > HFE_odd_degree) j=1;
     #endif
+    /* Here a_vec = row i */
+    a_vecj=alpha_vec+j*(HFEn-1)*NB_WORD_GFqn;
+    #if HFEDegJ
+    for(; j<HFEDegJ;++j)
     {
         a_veci=a_vec;
         QUADRATIC_CASE_REF(a_veci,a_vecj);
     }
+    #endif
     /* Here a_veci = row i+1 */
 
     /* j=HFEDegJ */

