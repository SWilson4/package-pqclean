--- upstream/Reference_Implementation/hqc-192/src/parameters.h
+++ upstream-patched/Reference_Implementation/hqc-192/src/parameters.h
@@ -8,9 +8,7 @@
 
 #include "api.h"
 
-#define CEIL_DIVIDE(a, b)  (((a)/(b)) + ((a) % (b) == 0 ? 0 : 1)) /*!< Divide a by b and ceil the result*/
-#define BITMASK(a, size) ((1UL << (a % size)) - 1) /*!< Create a mask*/
-
+#define CEIL_DIVIDE(a, b)  (((a)+(b)-1)/(b)) /*!< Divide a by b and ceil the result*/
 
 /*
   #define PARAM_N                             	Define the parameter n of the scheme
@@ -20,18 +18,15 @@
   #define PARAM_OMEGA                         	Define the parameter omega of the scheme
   #define PARAM_OMEGA_E                       	Define the parameter omega_e of the scheme
   #define PARAM_OMEGA_R                       	Define the parameter omega_r of the scheme
-  #define PARAM_SECURITY                      	Define the security level corresponding to the chosen parameters
-  #define PARAM_DFR_EXP                       	Define the decryption failure rate corresponding to the chosen parameters
 
   #define SECRET_KEY_BYTES                    	Define the size of the secret key in bytes
   #define PUBLIC_KEY_BYTES                    	Define the size of the public key in bytes
   #define SHARED_SECRET_BYTES                 	Define the size of the shared secret in bytes
   #define CIPHERTEXT_BYTES                    	Define the size of the ciphertext in bytes
 
-  #define UTILS_REJECTION_THRESHOLD           	Define the rejection threshold used to generate given weight vectors (see vector_set_random_fixed_weight function)
   #define VEC_N_SIZE_BYTES                    	Define the size of the array used to store a PARAM_N sized vector in bytes
   #define VEC_K_SIZE_BYTES                    	Define the size of the array used to store a PARAM_K sized vector in bytes
-  #define VEC_N1Y_SIZE_BYTES                  	Define the size of the array used to store a PARAM_N1 sized vector in bytes
+  #define VEC_N1_SIZE_BYTES                  	Define the size of the array used to store a PARAM_N1 sized vector in bytes
   #define VEC_N1N2_SIZE_BYTES                 	Define the size of the array used to store a PARAM_N1N2 sized vector in bytes
 
   #define VEC_N_SIZE_64                       	Define the size of the array used to store a PARAM_N sized vector in 64 bits
@@ -52,11 +47,10 @@
                                               	The smallest power of 2 greater than 24+1 is 32=2^5
   #define RS_POLY_COEFS                       	Coefficients of the generator polynomial of the Reed-Solomon code
 
-  #define RED_MASK                            	A mask fot the higher bits of a vector
+  #define RED_MASK                            	A mask for the higher bits of a vector
   #define SHAKE256_512_BYTES                    Define the size of SHAKE-256 output in bytes
   #define SEED_BYTES                          	Define the size of the seed in bytes
   #define SALT_SIZE_BYTES                       Define the size of a salt in bytes
-  #define SALT_SIZE_64                          Define the size of a salt in 64 bits words
 */
 
 #define PARAM_N																35851
@@ -66,15 +60,12 @@
 #define PARAM_OMEGA                         	100
 #define PARAM_OMEGA_E                       	114
 #define PARAM_OMEGA_R                       	114
-#define PARAM_SECURITY                      	192
-#define PARAM_DFR_EXP                       	192
 
 #define SECRET_KEY_BYTES                    	CRYPTO_SECRETKEYBYTES
 #define PUBLIC_KEY_BYTES                    	CRYPTO_PUBLICKEYBYTES
 #define SHARED_SECRET_BYTES                 	CRYPTO_BYTES
 #define CIPHERTEXT_BYTES                    	CRYPTO_CIPHERTEXTBYTES
 
-#define UTILS_REJECTION_THRESHOLD             16742417
 #define VEC_N_SIZE_BYTES                    	CEIL_DIVIDE(PARAM_N, 8)
 #define VEC_K_SIZE_BYTES                    	PARAM_K
 #define VEC_N1_SIZE_BYTES                   	PARAM_N1
@@ -96,10 +87,9 @@
 #define PARAM_FFT                           	5
 #define RS_POLY_COEFS 45,216,239,24,253,104,27,40,107,50,163,210,227,134,224,158,119,13,158,1,238,164,82,43,15,232,246,142,50,189,29,232,1
 
-#define RED_MASK                            	BITMASK(PARAM_N, 64)
+#define RED_MASK                            	0x7ff
 #define SHAKE256_512_BYTES                    64
 #define SEED_BYTES                          	40
 #define SALT_SIZE_BYTES                       16
-#define SALT_SIZE_64                          2
 
-#endif
\ No newline at end of file
+#endif

