#include <stdio.h>
//#include "../tbb/include/tbb/tbbmalloc_proxy.h"
#include <ppl.h>
#include <omp.h>
#include <vector>
#include <list>
#include "ChunkPoolAllocator.hpp"
using namespace std;

void test1() {
  using namespace concurrency;
  const int N = 1024;
  double t0 = omp_get_wtime();
  double *a[N];
  parallel_for( 0, N - 1, [&](int i) { a[i] = new double[N]; } );
  parallel_for( 0, N - 1, [&](int i) { delete []a[i];       } );
  double t1 = omp_get_wtime();
  printf("test1 used time: %fs \n", t1 - t0);
}

template<class Alloc>
void pushbackTest(int n, char *str) {
  double t0 = omp_get_wtime();
  {
    std::list<int, Alloc> l;
    for (int i = 0; i < n; i++) {
      l.push_back(i);
    }
  }
  double t1 = omp_get_wtime();
  printf("pusbackTest(%s) used time %fs\n", str, t1 - t0);
}

void test_list_ChunkPoolAlloator(int n) {
  double t0 = omp_get_wtime();
  using alloc = ChunkPoolAllocator<int>;
  std::list<int, alloc> l;
  for (int i = 0; i < n; i++) {
    l.push_back(i);
  }
  double t1 = omp_get_wtime();
  printf("list(ChunkPoolAllocator) push_back used time: %fs l.size()=%d\n", t1 - t0, l.size());
}
int main() {
#ifdef __TBB_tbbmalloc_proxy_H
  printf("__TBB_tbbmalloc_proxy_H used.\n");
#else
  printf("__TBB_tbbmalloc_proxy_H not used.\n");
#endif
  //test1();
  pushbackTest<std::allocator<int>>(1024, "std");
  pushbackTest < ChunkPoolAllocator<int> >(1024, "chunkPool");
  pushbackTest<std::allocator<int>>(1024 * 1024, "std");
  pushbackTest < ChunkPoolAllocator<int> >(1024 * 1024, "chunkPool");

  return 0;

  //1.win64 default malloc is 16byte align.
  //2.win32, tbb is 8yte align.
  for (int i = 0; i < 4; i++) {
    int *p = new int[7 + i];
    printf("p=%p\n", p);
    //delete[]p;
  }

  for (int i = 0; i < 4; i++) {
    vector<int> v(i + 1);
    printf("&v[0]=%p\n", &v[0]);
  }
  return 0;
}