#include <stdio.h>
#include <stdlib.h>
/*处理函数原型*/
void Assert(char * filename, unsigned int lineno);
#define ASSERT(condition)\
do{   \
    if(condition)\
       NULL; \
    else\
       Assert(__FILE__ , __LINE__);\
}while(0)

void Assert(char * filename, unsigned int lineno)
{
    fflush(stdout);
    fprintf(stderr,"\nAssert failed： %s, line %u\n",filename, lineno);
    fflush(stderr);
    abort();
}