#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

template <class T, class Comparator>
void mergeSort (vector<T>& myArray, Comparator comp);

template <class T, class Comparator>
void merge (vector<T> array1, vector<T> array2, Comparator comp);

struct AlphaSort
{
  bool operator()(const string& lhs, const string& rhs)
  {
    int length;

    if (lhs.length() > rhs.length()) //Find longer of the two
      length = lhs.length();
    else
      length = rhs.length();

    for (int i=0; i<length; i++)
    {
      if (isdigit(lhs[i]) && isdigit(rhs[i])) //If both are numbers
      {
        int tempL=i, tempR=i;
        while (1) //Keep incrementing until all numbers have been taken
        {
          if (isdigit(lhs[tempL+1]))         
            tempL++;
          else
            break;
        }
        while (1)
        {
          if (isdigit(rhs[tempR+1]))
            tempR++;
          else
            break;
        }
        string Ltemp, Rtemp;
        int L, R;
        Ltemp = lhs.substr(i, tempL);
        Rtemp = rhs.substr(i, tempR);
        L = atoi(Ltemp.c_str());
        R = atoi(Rtemp.c_str());
        if (L < R)
          return true;
        if (R < L)
          return false;
        if (L == R)
        {
          i = tempL+1;
          continue;
        }        
      }
      if (tolower(lhs[i]) < tolower(rhs[i])) //Return the lower
        return true;  
      if (tolower(lhs[i]) == tolower(rhs[i])) //If they start with the same letter     
        continue;   

      return false;
    }
    return false;
  }
};

struct prSort
{
  bool operator()(const double& lhs, const double& rhs)
  {
    if (lhs > rhs) //Sort from greatest to least
      return true;
    else
      return false;
  }
};

template <class T, class Comparator>
void mergeSort (vector<T>& myArray, Comparator comp)
{
  if (myArray.size() > 1)
  {
    vector<T> tempArray1 (myArray.begin(), (myArray.begin() + (myArray.size()/2) ));

    mergeSort(tempArray1, comp);
    vector<T> tempArray2 ( (myArray.begin() + (myArray.size()/2) ), myArray.end() );

    mergeSort(tempArray2, comp);
    myArray.clear();
    
    myArray = merger(tempArray1, tempArray2, comp);
  }  
}

template <class T, class Comparator>
vector<T> merger (vector<T> array1, vector<T> array2, Comparator comp)
{
  vector<T> tempArray3;
  typename vector<T>::iterator IT1 = array1.begin();
  typename vector<T>::iterator IT2 = array2.begin();
  bool flag = false;

  while (1)  //While there are still #'s
  {
    if ( comp(*IT1, *IT2) )
    {
      tempArray3.push_back(*IT1);
      if (IT1+1 == array1.end())
        break;
      ++IT1;
    }
    else
    {
      tempArray3.push_back(*IT2); //Insert rhs
      if (IT2+1 == array2.end())
      {
	flag = true;
        break;
      }
      ++IT2;
    }
  }

  if ( IT1+1 == array1.end() && flag != true ) //If first array is finished, insert rest of second
  {
    while (1)
    {     
      tempArray3.push_back(*IT2);
      if (IT2+1 == array2.end())
        break;
      else
         ++IT2;
    }
  }
  else if ( IT2+1 == array2.end() )
  {
    while (1)
    {
      tempArray3.push_back(*IT1);
       if (IT1+1 == array1.end())
         break;
       else 
         ++IT1;
    }
  }

  return tempArray3;
}

