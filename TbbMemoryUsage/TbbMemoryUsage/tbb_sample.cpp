#include <stdio.h>
#include "../tbb/include/tbb/tbbmalloc_proxy.h"
#include <vector>
using namespace std;
int main()
{
  //1.win64 default malloc is 16byte align.
  //2.win32, tbb is 8yte align.
  for (int i = 0; i < 4; i++)
  {
    int *p = new int[7 + i];
    printf("p=%p\n", p);
    //delete[]p;
  }

  for (int i = 0; i < 4; i++)
  {
    vector<int> v(i + 1);
    printf("&v[0]=%p\n", &v[0]);
  }
  return 0;
}