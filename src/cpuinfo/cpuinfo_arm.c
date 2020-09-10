#include "cpuinfo_utils.h"
enum v_arm_cpu_features
{
    // ARM
    V_CPU_FEATURE_NEON              = 0,
    V_CPU_FEATURE_NEON_FP16         = 1,
    // FMA
    V_CPU_FEATURE_NEON_VFPV4        = 2,
    // Advanced SIMD
    V_CPU_FEATURE_ASIMD             = 3,
    // ARMv8.2 half-precision
    V_CPU_FEATURE_FPHP              = 4,
    // ARMv8.2 half-precision vector arithm
    V_CPU_FEATURE_ASIMDHP           = 5,
    // ARMv8.2 dot product
    V_CPU_FEATURE_ASIMDDP           = 6,
    // ARMv8.2 single&half-precision multiply
    V_CPU_FEATURE_ASIMDFHM          = 7
};

#include <string.h>
#include <stdio.h>
static int 
get_feature(char *search)
{

#ifdef linux
	FILE *infile;
  	char buffer[2048], *p,*t;
  	p = (char *) NULL ;

  	infile = fopen("/proc/cpuinfo", "r");

	while (fgets(buffer, sizeof(buffer), infile))
	{

		if (!strncmp("Features", buffer, 8))
		{
			p = strchr(buffer, ':') + 2;
			break;
      		}
  	}

  	fclose(infile);


	if( p == NULL ) return 0;

	t = strtok(p," ");
	while( t = strtok(NULL," "))
	{
		if (!strcmp(t, search))   { return(1); }
	}

#endif
	return(0);
}

void v_init_features() {
    if(get_feature("neon")) v_cpu_have[V_CPU_FEATURE_NEON] = 1;
    if(get_feature("vfpv4")) v_cpu_have[V_CPU_FEATURE_NEON_VFPV4] = 1;
    if(get_feature("asimd")) v_cpu_have[V_CPU_FEATURE_ASIMD] = 1;
}

