#include "myrandom.h"
#ifdef _WIN32
#include <Windows.h>
#endif

unsigned int _g_random_seed_xyz = 0;

void _shift_random()
{
#ifdef _WIN32
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    int n = li.LowPart % 10;
    for(int i=0; i<n; i++)
        _g_random_seed_xyz=214013*_g_random_seed_xyz+2531011;
#endif
}

void random_seed(int seed)
{
    _g_random_seed_xyz = seed;
}

int random_int(int min, int max)
{
    _shift_random();
    _g_random_seed_xyz=214013*_g_random_seed_xyz+2531011;
    return min+(_g_random_seed_xyz ^ _g_random_seed_xyz>>15)%(max-min+1);
}

float random_float(float min, float max)
{
    _shift_random();
    _g_random_seed_xyz=214013*_g_random_seed_xyz+2531011;
    return min+_g_random_seed_xyz*(1.0f/4294967295.0f)*(max-min);
}

