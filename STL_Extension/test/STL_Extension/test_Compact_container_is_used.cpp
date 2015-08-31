// test program for Compact_container.

#include <CGAL/basic.h>
#include <cassert>
#include <cstddef>
#include <list>
#include <vector>
#include <CGAL/Compact_container.h>
#include <CGAL/Random.h>
#include <CGAL/Testsuite/use.h>

#include <CGAL/tags.h>
#include <CGAL/use.h>
#include <CGAL/assertions.h>

class Node_1
{
  union {
    Node_1 * p;
    void * p_cc;
  };

public:

  Node_1() : p(NULL)
  {}

  void *   for_compact_container() const { return p_cc; }
  void * & for_compact_container()       { return p_cc; }
};

int main()
{
  typedef CGAL::Compact_container<Node_1> C1;

  C1 c1;
  if (!c1.empty())
  {
    std::cout<<"PB new container is not empty."<<std::endl;
    return EXIT_FAILURE;
  }

  for (int i = 0 ; i < 1000 ; ++i)
  {
    C1::iterator it=c1.emplace();
    if ( !c1.is_used(it) )
    {
      std::cout<<"PB new emplace element is not used."<<std::endl;
      return EXIT_FAILURE;
    }
  }

  std::size_t nb=0;
  for (C1::iterator it = c1.begin(), itend=c1.end(); it!=itend; ++it, ++nb)
  {
    c1.erase(it);
    if ( c1.is_used(it) )
    {
      std::cout<<"PB erase element is used."<<std::endl;
      return EXIT_FAILURE;
    }
    if ( c1.is_used(nb) )
    {
      std::cout<<"PB erase element is used (2)."<<std::endl;
      return EXIT_FAILURE;
    }
  }

  if (!c1.empty())
  {
    std::cout<<"PB container at the end is not empty."<<std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
