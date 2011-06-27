//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentList.h                                                 //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// PURPOSE: Implements a persistent list data structure.                     //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
// T ListNode.data                      The data to be stored within a node  //
// ListNode<T>* next                    The next node in the list            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// ListNode<T>::ListNode(int t, const T& original_data)                      //
// ListNode<T>::getNextIndex(int t)                                          //
// ListNode<T>::getNextAtIndex(int t)                                        //
// ListNode<T>::numberOfChangeIndices(int t)                                 //
// ListNode<T>::getNext(int t)                                               //
// ListNode<T>::setNext(int t, ListNode<T>* ln)                              //
// ListNode<T>::printList(int t)                                             //
//                                                                           //
// PersistentList<T>::PersistentList()                                       //
// int PersistentList<T>::setHead(int t, ListNode<T>* ln)                    //
// ListNode<T>* PersistentList<T>::getList(int t)                            //
// size_t PersistentList<T>::size()                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTLIST_H
#define PERSISTENTLIST_H

#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

namespace persistent_list {
  /////////////////////////////////////////////////////////////////////////////
  // ListNode interface                                                      //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class ListNode {
  protected:
    vector<int> time;
    vector<ListNode<T>* > next;
  public:
    T data;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ListNode                                               //
    //                                                                       //
    // PURPOSE:       Basic constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   T/original_data                                        //
    //   Description: The value to store in the data of the ListNode         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(const T& original_data)
      : time(), next(), data(original_data) {
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextIndex                                           //
    //                                                                       //
    // PURPOSE:       Given a time t, gives the change index of the nearest  //
    //                time.                                                  //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time for which to search.                          //
    //                                                                       //
    // RETURN:        The nearest change index in the next pointer vector.   //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getNextIndex(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextAtIndex                                         //
    //                                                                       //
    // PURPOSE:       Allows next pointer to be retrieved by change          //
    //                index.  This is useful for efficient                   //
    //                construction of persistent lists.                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/ci                                                 //
    //   Description: The change index at which to retrieve the next pointer.//
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: The next pointer at the given change index.            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getNextAtIndex(int ci);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: numberOfChangeIndices                                  //
    //                                                                       //
    // PURPOSE:       Returns the number of change indices, i.e. the number  //
    //                of times this node's next pointer has been changed and //
    //                stored.                                                //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The number of change indices of the current node.      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int numberOfChangeIndices();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNext                                                //
    //                                                                       //
    // PURPOSE:       Retrieves the next pointer at a given time.  If        //
    //                the given time is not set, returns the pointer         //
    //                which would directly precede it.                       //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the pointer              //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: The next pointer at time t.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getNext(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setNext                                                //
    //                                                                       //
    // PURPOSE:       Set the next pointer at time t                         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to set the pointer.                  //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: The pointer to which to assign next.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success                      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    virtual int setNext(int t, ListNode<T>* ln);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: printList                                              //
    //                                                                       //
    // PURPOSE:       Prints a list at time t to cout.                       //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to print the list.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success.                     //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int printList(int t);
  };

  /////////////////////////////////////////////////////////////////////////////
  // ListNode implementation                                                 //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  int ListNode<T>::getNextIndex(int t) {
    assert(this != NULL);
    assert(t >= 0);
    int index = -1;
    int begin = 0, end = (int)next.size() -1;
    while(begin <= end) {
      index = (begin+end)/2;
      if(t == time[index]) {
	break;
      } else if(t < time[index]) {
	end = index -1;
      } else {
	begin = index +1;
      }
    }
    return index;
  }

  template <class T>
  ListNode<T>* ListNode<T>::getNextAtIndex(int ci) {
    assert(this != NULL);
    assert(ci >= 0);
    assert(ci < numberOfChangeIndices());
    return next[ci];
  }

  template <class T>
  int ListNode<T>::numberOfChangeIndices() {
    assert(this != NULL);
    return (int)time.size();
  }
  
  template <class T>
  ListNode<T>* ListNode<T>::getNext(int t) {
    assert(this != NULL);
    assert(t >= 0);
    int index = getNextIndex(t);
    if(index == -1) return NULL;
    ListNode<T>* ln = NULL;
    if(time[index] > t) { // overshot
      if(index > 0)
	ln = getNextAtIndex(index-1);
    } else {
      ln = getNextAtIndex(index);
    }
    return ln;
  }

  template <class T>
  int ListNode<T>::setNext(int t, ListNode<T>* ln) {
    assert(this != NULL);
    // since we have restricted point insertion to strictly increasing
    // time, we can simply push these values to the back of their
    // respective lists.
    time.push_back(t);
    next.push_back(ln);
    // success
    return 0;
  }

  template <class T>
  int ListNode<T>::printList(int t) {
    assert(this != NULL);
    assert(t >= 0);
    cout << data << "->";
    ListNode<T>* ln = getNext(t);
    if(ln != NULL)
      ln->printList(t);
    else
      cout << "NULL";
    // success
    return 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // PersistentList interface                                                //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  class PersistentList {
    vector< ListNode<T>* > lists;
    
  public:   
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: PersistentList                                         //
    //                                                                       //
    // PURPOSE:       Empty constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PersistentList()
      : lists()
    {}

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setHead                                                //
    //                                                                       //
    // PURPOSE:       Sets a node to the head of the list at time t.         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: Time t at which to set                                 //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setHead(int t, ListNode<T>* ln);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getList                                                //
    //                                                                       //
    // PURPOSE:       Gets the list at time t.                               //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the list.                //
    //                                                                       //
    // RETURN:        The first node in the list at time t.                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getList(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: size                                                   //
    //                                                                       //
    // PURPOSE:       Returns the number of points in the structure          //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   size_t                                                 //
    //   Description: The number of points in the structure                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    size_t size();
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // PersistentList implementation                                           //
  /////////////////////////////////////////////////////////////////////////////
  template <class T>
  int PersistentList<T>::setHead(int t, ListNode<T>* ln) {
    assert(t >= 0);
    assert(t <= (int)lists.size());
    if(t == (int)lists.size())
      lists.push_back(ln);
    else
      lists[t] = ln;
    return 0; // success
  }

  template <class T>
  ListNode<T>* PersistentList<T>::getList(int t) {
    assert(t < (int)lists.size());
    return lists[t];
  }

  template <class T>
  size_t PersistentList<T>::size() {
    return lists.size();
  }
}

#endif
