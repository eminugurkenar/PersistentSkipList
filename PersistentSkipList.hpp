//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentSkipList.h                                             //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// PURPOSE: Implements a persistent skip list data structure.                //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTSKIPLIST_HPP
#define PERSISTENTSKIPLIST_HPP

// Standard libraries
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstdlib>
#include <ctime>

// My libraries
#include "TimeStampedArray.hpp"
#include "lib/SmartPointer/SmartPointer.hpp"
#include "ListNode.hpp"

using namespace std;
using namespace timestamped_array;
using namespace smart_ptr;

namespace persistent_skip_list {

  template < class T >
  class PersistentSkipList {
    typedef TimeStampedArray< SmartPointer< ListNode<T> > > TSA;
    int height;
    int present;
    vector< TSA* > head;
    set< T > data_set;
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addHead                                                //
    //                                                                       //
    // PURPOSE:       Adds a new array of pointers to the head of the        //
    //                skiplist at each height.                               //
    //                                                                       //
    // SECURITY:      private                                                //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   TSA* /tsa                                              //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int addHead(TSA* tsa);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setHead                                                //
    //                                                                       //
    // PURPOSE:       Sets a new array of pointers to the head of the        //
    //                skiplist at each height.                               //
    //                                                                       //
    // SECURITY:      private                                                //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   TSA* /tsa                                              //
    //   Description: A pointer to the node to set                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         Assumes list at time t exists or is one past the end.  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setHead(TSA* tsa);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: initialInsert                                          //
    //                                                                       //
    // PURPOSE:       Inserts the first element                              //
    //                                                                       //
    // SECURITY:      private                                                //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   const T&/data                                          //
    //   Description: The initial data to insert                             //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: A return code, 0 means success.                        //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int initialInsert(const T& data);
    
  public:
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: PersistentSkipList                                     //
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
    PersistentSkipList();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ~PersistentList                                        //
    //                                                                       //
    // PURPOSE:       Destructor                                             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         Frees all allocated memory.                            //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    virtual ~PersistentSkipList();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getPresent                                             //
    //                                                                       //
    // PURPOSE:       Returns the latest time index of the skip list.        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The latest time index of the skip list.                //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getPresent() const;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: incTime                                                //
    //                                                                       //
    // PURPOSE:       Increments the time index of the skip list.            //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void incTime();
    PersistentSkipList<T>& operator++();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: drawPresent                                            //
    //                                                                       //
    // PURPOSE:       Draws the data structure at present to cout.           //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void drawPresent();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: draw                                                   //
    //                                                                       //
    // PURPOSE:       Draws the data structure at the given time to cout.    //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to draw the structure.               //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void draw(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getHead                                                //
    //                                                                       //
    // PURPOSE:       Gets the head of the list at time t.                   //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the head.                //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   TSA*                                                   //
    //   Description: The head at the given time.                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TSA* getHead(int t);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insert                                                 //
    //                                                                       //
    // PURPOSE:       Inserts a data node into the present version of        //
    //                the structure.                                         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   const T&/data                                          //
    //   Description: The data to be inserted                                //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: 0 for success.                                         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int insert(const T& data);
    const PersistentSkipList<T>& operator+=(const T& data);
  };

}

// finally, include the implementation because C++ requires that class
// templates have both interface and implementation together
#include "PersistentSkipList.cpp"
#endif
