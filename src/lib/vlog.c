/* NEON implementation of sin, cos, exp and log

   Inspired by Intel Approximate Math library, and based on the
   corresponding algorithms of the cephes math library
*/

/* Copyright (C) 2011  Julien Pommier

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  (this is the zlib license)
*/

#include <arm_neon.h>
#include<stdio.h>
typedef float32x4_t v4sf;  // vector of 4 float
typedef uint32x4_t v4su;  // vector of 4 uint32
typedef int32x4_t v4si;  // vector of 4 uint32

#define c_inv_mant_mask ~0x7f800000u
#define c_cephes_SQRTHF 0.707106781186547524
#define c_cephes_log_p0 7.0376836292E-2
#define c_cephes_log_p1 - 1.1514610310E-1
#define c_cephes_log_p2 1.1676998740E-1
#define c_cephes_log_p3 - 1.2420140846E-1
#define c_cephes_log_p4 + 1.4249322787E-1
#define c_cephes_log_p5 - 1.6668057665E-1
#define c_cephes_log_p6 + 2.0000714765E-1
#define c_cephes_log_p7 - 2.4999993993E-1
#define c_cephes_log_p8 + 3.3333331174E-1
#define c_cephes_log_q1 -2.12194440e-4
#define c_cephes_log_q2 0.693359375

/* natural logarithm computed for 4 simultaneous float 
   return NaN for x <= 0
*/
v4sf log_ps(v4sf x) {
  v4sf one = vdupq_n_f32(1);

  x = vmaxq_f32(x, vdupq_n_f32(0)); /* force flush to zero on denormal values */
  v4su invalid_mask = vcleq_f32(x, vdupq_n_f32(0));

  v4si ux = vreinterpretq_s32_f32(x);
  
  v4si emm0 = vshrq_n_s32(ux, 23);

  /* keep only the fractional part */
  ux = vandq_s32(ux, vdupq_n_s32(c_inv_mant_mask));
  ux = vorrq_s32(ux, vreinterpretq_s32_f32(vdupq_n_f32(0.5f)));
  x = vreinterpretq_f32_s32(ux);

  emm0 = vsubq_s32(emm0, vdupq_n_s32(0x7f));
  v4sf e = vcvtq_f32_s32(emm0);

  e = vaddq_f32(e, one);

  /* part2: 
     if( x < SQRTHF ) {
       e -= 1;
       x = x + x - 1.0;
     } else { x = x - 1.0; }
  */
  v4su mask = vcltq_f32(x, vdupq_n_f32(c_cephes_SQRTHF));
  v4sf tmp = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(x), mask));
  x = vsubq_f32(x, one);
  e = vsubq_f32(e, vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(one), mask)));
  x = vaddq_f32(x, tmp);

  v4sf z = vmulq_f32(x,x);

  v4sf y = vdupq_n_f32(c_cephes_log_p0);
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p1));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p2));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p3));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p4));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p5));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p6));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p7));
  y = vmulq_f32(y, x);
  y = vaddq_f32(y, vdupq_n_f32(c_cephes_log_p8));
  y = vmulq_f32(y, x);

  y = vmulq_f32(y, z);
  

  tmp = vmulq_f32(e, vdupq_n_f32(c_cephes_log_q1));
  y = vaddq_f32(y, tmp);


  tmp = vmulq_f32(z, vdupq_n_f32(0.5f));
  y = vsubq_f32(y, tmp);

  tmp = vmulq_f32(e, vdupq_n_f32(c_cephes_log_q2));
  x = vaddq_f32(x, y);
  x = vaddq_f32(x, tmp);
  x = vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(x), invalid_mask)); // negative arg will be NAN
  return x;
}