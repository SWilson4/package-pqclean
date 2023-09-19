--- upstream/Reference_Implementation/hqc-256/src/fft.cpp
+++ upstream-patched/Reference_Implementation/hqc-256/src/fft.cpp
@@ -1,12 +1,12 @@
 /**
- * @file fft.cpp
+ * @file fft.c
  * @brief Implementation of the additive FFT and its transpose.
  * This implementation is based on the paper from Gao and Mateer: <br>
  * Shuhong Gao and Todd Mateer, Additive Fast Fourier Transforms over Finite Fields,
  * IEEE Transactions on Information Theory 56 (2010), 6265--6272.
- * http://www.math.cpplemson.edu/~sgao/papers/GM10.pdf <br>
+ * http://www.math.clemson.edu/~sgao/papers/GM10.pdf <br>
  * and includes improvements proposed by Bernstein, Chou and Schwabe here:
- * https://binary.cppr.yp.to/mcbits-20130616.pdf
+ * https://binary.cr.yp.to/mcbits-20130616.pdf
  */
 
 #include "fft.h"
@@ -15,12 +15,7 @@
 #include <stdint.h>
 #include <string.h>
 
-static void compute_fft_betas(uint16_t *betas);
-static void compute_subset_sums(uint16_t *subset_sums, const uint16_t *set, uint16_t set_size);
-static void radix(uint16_t *f0, uint16_t *f1, const uint16_t *f, uint32_t m_f);
 static void radix_big(uint16_t *f0, uint16_t *f1, const uint16_t *f, uint32_t m_f);
-static void fft_rec(uint16_t *w, uint16_t *f, size_t f_coeffs, uint8_t m, uint32_t m_f, const uint16_t *betas);
-
 
 /**
  * @brief Computes the basis of betas (omitting 1) used in the additive FFT and its transpose
@@ -64,7 +59,7 @@
  *
  * Computes f0 and f1 such that f(x) = f0(x^2-x) + x.f1(x^2-x)
  * as proposed by Bernstein, Chou and Schwabe:
- * https://binary.cppr.yp.to/mcbits-20130616.pdf
+ * https://binary.cr.yp.to/mcbits-20130616.pdf
  *
  * @param[out] f0 Array half the size of f
  * @param[out] f1 Array half the size of f
@@ -258,9 +253,9 @@
  * This implementation is based on the paper from Gao and Mateer: <br>
  * Shuhong Gao and Todd Mateer, Additive Fast Fourier Transforms over Finite Fields,
  * IEEE Transactions on Information Theory 56 (2010), 6265--6272.
- * http://www.math.cpplemson.edu/~sgao/papers/GM10.pdf <br>
+ * http://www.math.clemson.edu/~sgao/papers/GM10.pdf <br>
  * and includes improvements proposed by Bernstein, Chou and Schwabe here:
- * https://binary.cppr.yp.to/mcbits-20130616.pdf <br>
+ * https://binary.cr.yp.to/mcbits-20130616.pdf <br>
  * Note that on this first call (as opposed to the recursive calls to fft_rec), gammas are equal to betas,
  * meaning the first gammas subset sums are actually the subset sums of betas (except 1). <br>
  * Also note that f is altered during computation (twisted at each level).

