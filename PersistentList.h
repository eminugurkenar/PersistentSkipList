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
// persistent_list::coord_t             the type of a coordinate             //
// persistent_list::point2d             the type of a point in a 2d plane    //
// persistent_list::PersistentList      an implementation of a linked        //
//                                      list, in which previous              //
//                                      versions may be accessed and         //
//                                      modified                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// PersistentList::PersistentList()                                          //
// PersistentList::insertPoint(coord_t x, coord_t y)                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTLIST_H
#define PERSISTENTLIST_H

#include <vector>

using namespace std;

namespace persistent_list {

  typedef double coord_t;
  
  typedef struct point2d {
    coord_t x, y;
  } point2d;

  class PointListNode {
  public:
    // data
    point2d point;
    PointListNode* next;
    
    // methods
    PointListNode(point2d p)
      : point(p), next(NULL)
    {}
  };

  ostream& operator<<(ostream& os, const point2d& p);
  
  class PersistentList {
    // Data
    vector<point2d> points_sorted_by_x;
    vector< PointListNode* > points_right;
    // Methods
    int binarySearchX(coord_t x);
    PointListNode* searchY(int t, coord_t y);
    bool xInArray(coord_t x);
  public:
    PersistentList()
      : points_sorted_by_x(), points_right()
    {}
    int insertPoint(coord_t x,coord_t y);
    void printArray();
  };
}

#endif
