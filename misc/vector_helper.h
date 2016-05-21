#ifndef __VECTOR_HELPER_H__
#define __VECTOR_HELPER_H__
#include <vector>
using namespace std;

template<class T>
void release_vector(vector<T>& v)
{
    vector<T> d;
    d.swap(v);
}
#endif//__VECTOR_HELPER_H__