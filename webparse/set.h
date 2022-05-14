#ifndef SET_H
#define SET_H
#include <set>
using namespace std;

template <class T>
class Set : public set<T> {
  public: 
    Set<T> setIntersection (const Set<T> & other) const;
      /* Returns the intersection of the current set with other.
         That is, returns the set of all items that are both in this
         and in other. */

    Set<T> setUnion (const Set<T> & other) const;
      /* Returns the union of the current set with other.
         That is, returns the set of all items that are in this set
         or in other (or both).
         The resulting set should not contain duplicates. */

  private:
    // other private variables you think you need.
};

template <class T>
Set<T> Set<T>::setIntersection (const Set<T> & other) const
{
  Set<T> temp;
  if (this->empty())
    return temp;
  if (other.empty()) //IF other set is empty, nothing in common
    return temp;

  typename Set<T>::iterator i = this->begin();
  for (i = this->begin(); i != this->end(); ++i)
  {
    if (other.find(*i) != other.end())
      temp.insert(*i);
  }
  return temp;
}

template <class T>
Set<T> Set<T>::setUnion(const Set<T> & other) const
{
  Set<T> temp = other;
  typename Set<T>::iterator i = this->begin();

  for (i = this->begin(); i != this->end(); ++i)
  {
    if (other.find(*i) == other.end())
      temp.insert(*i);
  }
  return temp;
}



#endif
