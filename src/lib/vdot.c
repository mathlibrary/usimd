//#include "common.h"

FLOAT usimd_dot(int n, FLOAT *x, int inc_x, FLOAT *y, int inc_y)
{
	int i=0;
	int ix=0,iy=0;

#if defined(DSDOT)
	double dot = 0.0 ;
#else
	FLOAT  dot = 0.0 ;
#endif

	if ( n < 0 )  return(dot);

	if ( (inc_x == 1) && (inc_y == 1) )
	{
        int n1 = n & -4;
#if NPY_SIMD && !defined(DSDOT)
        const int vstep = npyv_nlanes_f32;
        const int unrollx4 = n & (-vstep * 4);
        const int unrollx  = n &  -vstep;
		npyv_f32 vsum0 = npyv_zero_f32();
        npyv_f32 vsum1 = npyv_zero_f32();
        npyv_f32 vsum2 = npyv_zero_f32();
        npyv_f32 vsum3 = npyv_zero_f32();
		while(i < unrollx4)
        {
            vsum0 = npyv_muladd_f32(
                npyv_load_f32(x + i),           npyv_load_f32(y + i),           vsum0
            );
            vsum1 = npyv_muladd_f32(
                npyv_load_f32(x + i + vstep),   npyv_load_f32(y + i + vstep),   vsum1
            );
            vsum2 = npyv_muladd_f32(
                npyv_load_f32(x + i + vstep*2), npyv_load_f32(y + i + vstep*2), vsum2
            );
            vsum3 = npyv_muladd_f32(
                npyv_load_f32(x + i + vstep*3), npyv_load_f32(y + i + vstep*3), vsum3
            );
            i += vstep*4;
        }
        vsum0 = npyv_add_f32(
            npyv_add_f32(vsum0, vsum1), npyv_add_f32(vsum2 , vsum3)
        );
		while(i < unrollx)
        {
            vsum0 = npyv_muladd_f32(
                npyv_load_f32(x + i), npyv_load_f32(y + i), vsum0
            );
            i += vstep;
        }
        dot = npyv_sum_f32(vsum0);
#elif defined(DSDOT)
		for (; i < n1; i += 4)
		{
			dot += (double) y[i] * (double) x[i]
			    + (double) y[i+1] * (double) x[i+1]
			    + (double) y[i+2] * (double) x[i+2]
			    + (double) y[i+3] * (double) x[i+3] ;
		}
#else
		for (; i < n1; i += 4)
		{
			dot += y[i] * x[i]
			    + y[i+1] * x[i+1]
			    + y[i+2] * x[i+2]
			    + y[i+3] * x[i+3] ;
		}
#endif
		while(i < n)
		{

#if defined(DSDOT)
			dot += (double) y[i] * (double) x[i] ;
#else
			dot += y[i] * x[i] ;
#endif
			i++ ;

		}
		return(dot);


	}

	while(i < n)
	{

#if defined(DSDOT)
		dot += (double) y[iy] * (double) x[ix] ;
#else
		dot += y[iy] * x[ix] ;
#endif
		ix  += inc_x ;
		iy  += inc_y ;
		i++ ;

	}
	return(dot);
}