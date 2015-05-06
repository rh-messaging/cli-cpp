#ifndef __OBJECTCLOSER_H__
#define __OBJECTCLOSER_H__

#include <vector>
#include <qpid/types/Exception.h>


/** Closes all objects in given list/vector
  * void close_objects( vector<T>& in_list )
  *
  * vector<T>& in_list vector of objects
  */
template <class T>
int close_objects(std::vector<T>& in_list) {
  int int_ret = 0;

  for (unsigned int indx = 0; indx < in_list.size(); indx++) {
    if (in_list.at(indx) != NULL) {
      try {
        in_list.at(indx).close();
      }
      catch(const qpid::types::Exception& error) {
        std::cerr << error.what() << std::endl;
        int_ret |= 1;
      }
    }
  }

  return(int_ret);
}


#endif
