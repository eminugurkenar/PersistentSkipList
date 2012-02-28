///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PolygonalSubdivision.hpp                                         //
//                                                                           //
// MODULE:  Planar Point Location                                            //
//                                                                           //
// PURPOSE: Solves the planar point location problem using a                 //
//          persistent skiplist.                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////

#ifndef POLYGONALSUBDIVISION_HPP
#define POLYGONALSUBDIVISION_HPP

#include <vector>
#include "Point2D.hpp"
#include "LineSegment.hpp"
#include "PersistentSkipList.hpp"

using namespace persistent_skip_list;

namespace geometry {
  class PolygonalSubdivision {
  private:
    vector< Point2D > sweep_points;
    PersistentSkipList< LineSegment > line_crossings;
  public:
    PolygonalSubdivision();
    ~PolygonalSubdivision();
    
    LineSegment& locate_point(Point2D&);
  };
}

#endif
