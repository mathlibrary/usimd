#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined(__CYGWIN32__) || defined(__linux__)
#include <sys/time.h>
#endif

#if defined(__WIN32__) || defined(__WIN64__)
#include <windows.h>
#ifndef DELTA_EPOCH_IN_MICROSECS
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif

int gettimeofday(struct timeval *tv, void *tz)
{

  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10; /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  return 0;
}

#endif

struct timeval start, stop;

double getsecs()
{
  double time = (double)(stop.tv_sec - start.tv_sec) +
                (double)((stop.tv_usec - start.tv_usec)) * 1.e-6;
  return time;
}

void begin()
{
  gettimeofday(&start, (struct timezone *)0);
}

void end()
{
  gettimeofday(&stop, (struct timezone *)0);
}

void getMFlops(int scale, double time)
{
  fprintf(stderr,
          " %10.2f MFlops %10.6f sec\n",
          4. * (double)scale / time * 1.e-6, time);
  printf("###############################\n");
}

FLOAT_T *getFinput(int scale)
{
  FLOAT_T *input;
  if ((input = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL)
  {
    fprintf(stderr, "Out of Memory!!\n");
    exit(1);
  }
  for (int i = 0; i < scale; i++)
  {
    input[i] = ((FLOAT_T)rand() / (FLOAT_T)RAND_MAX) - 0.5;
  }
  return input;
}

FLOAT_T **get2Darr(int M, int N)
{
  FLOAT_T **input;
  input = (FLOAT_T **)malloc(M * sizeof(FLOAT_T *));
  for (int i = 0; i < M; i++)
  {
    input[i] = (FLOAT_T *)malloc(N * sizeof(FLOAT_T));
  }
  return input;
}