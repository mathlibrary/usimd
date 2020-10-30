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

#define c_minus_cephes_DP1 -0.78515625
#define c_minus_cephes_DP2 -2.4187564849853515625e-4
#define c_minus_cephes_DP3 -3.77489497744594108e-8
#define c_sincof_p0 -1.9515295891E-4
#define c_sincof_p1  8.3321608736E-3
#define c_sincof_p2 -1.6666654611E-1
#define c_coscof_p0  2.443315711809948E-005
#define c_coscof_p1 -1.388731625493765E-003
#define c_coscof_p2  4.166664568298827E-002
#define c_cephes_FOPI 1.27323954473516 // 4 / M_PI

/* evaluation of 4 sines & cosines at once.

   The code is the exact rewriting of the cephes sinf function.
   Precision is excellent as long as x < 8192 (I did not bother to
   take into account the special handling they have for greater values
   -- it does not return garbage for arguments over 8192, though, but
   the extra precision is missing).

   Note that it is such that sinf((float)M_PI) = 8.74e-8, which is the
   surprising but correct result.

   Note also that when you compute sin(x), cos(x) is available at
   almost no extra price so both sin_ps and cos_ps make use of
   sincos_ps..
  */
void sincos_ps(v4sf x, v4sf *ysin, v4sf *ycos) { // any x
  v4sf xmm1, xmm2, xmm3, y;

  v4su emm2;
  
  v4su sign_mask_sin, sign_mask_cos;
  sign_mask_sin = vcltq_f32(x, vdupq_n_f32(0));
  x = vabsq_f32(x);

  /* scale by 4/Pi */
  y = vmulq_f32(x, vdupq_n_f32(c_cephes_FOPI));

  /* store the integer part of y in mm0 */
  emm2 = vcvtq_u32_f32(y);
  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = vaddq_u32(emm2, vdupq_n_u32(1));
  emm2 = vandq_u32(emm2, vdupq_n_u32(~1));
  y = vcvtq_f32_u32(emm2);

  /* get the polynom selection mask 
     there is one polynom for 0 <= x <= Pi/4
     and another one for Pi/4<x<=Pi/2

     Both branches will be computed.
  */
  v4su poly_mask = vtstq_u32(emm2, vdupq_n_u32(2));
  
  /* The magic pass: "Extended precision modular arithmetic" 
     x = ((x - y * DP1) - y * DP2) - y * DP3; */
  xmm1 = vmulq_n_f32(y, c_minus_cephes_DP1);
  xmm2 = vmulq_n_f32(y, c_minus_cephes_DP2);
  xmm3 = vmulq_n_f32(y, c_minus_cephes_DP3);
  x = vaddq_f32(x, xmm1);
  x = vaddq_f32(x, xmm2);
  x = vaddq_f32(x, xmm3);

  sign_mask_sin = veorq_u32(sign_mask_sin, vtstq_u32(emm2, vdupq_n_u32(4)));
  sign_mask_cos = vtstq_u32(vsubq_u32(emm2, vdupq_n_u32(2)), vdupq_n_u32(4));

  /* Evaluate the first polynom  (0 <= x <= Pi/4) in y1, 
     and the second polynom      (Pi/4 <= x <= 0) in y2 */
  v4sf z = vmulq_f32(x,x);
  v4sf y1, y2;

  y1 = vmulq_n_f32(z, c_coscof_p0);
  y2 = vmulq_n_f32(z, c_sincof_p0);
  y1 = vaddq_f32(y1, vdupq_n_f32(c_coscof_p1));
  y2 = vaddq_f32(y2, vdupq_n_f32(c_sincof_p1));
  y1 = vmulq_f32(y1, z);
  y2 = vmulq_f32(y2, z);
  y1 = vaddq_f32(y1, vdupq_n_f32(c_coscof_p2));
  y2 = vaddq_f32(y2, vdupq_n_f32(c_sincof_p2));
  y1 = vmulq_f32(y1, z);
  y2 = vmulq_f32(y2, z);
  y1 = vmulq_f32(y1, z);
  y2 = vmulq_f32(y2, x);
  y1 = vsubq_f32(y1, vmulq_f32(z, vdupq_n_f32(0.5f)));
  y2 = vaddq_f32(y2, x);
  y1 = vaddq_f32(y1, vdupq_n_f32(1));

  /* select the correct result from the two polynoms */  
  v4sf ys = vbslq_f32(poly_mask, y1, y2);
  v4sf yc = vbslq_f32(poly_mask, y2, y1);
  *ysin = vbslq_f32(sign_mask_sin, vnegq_f32(ys), ys);
  *ycos = vbslq_f32(sign_mask_cos, yc, vnegq_f32(yc));
}

v4sf sin_ps(v4sf x) {
  v4sf ysin, ycos; 
  sincos_ps(x, &ysin, &ycos); 
  return ysin;
}

v4sf cos_ps(v4sf x) {
  v4sf ysin, ycos; 
  sincos_ps(x, &ysin, &ycos); 
  return ycos;
}