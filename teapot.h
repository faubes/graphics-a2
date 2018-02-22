// ==========================================================================
// $Id: teapot.h,v 1.2 2018/02/17 03:21:12 jlang Exp $
// Simple structure for data
// ==========================================================================
// (C)opyright:
//
//   Jochen Lang
//   SITE, University of Ottawa
//   800 King Edward Ave.
//   Ottawa, On., K1N 6N5
//   Canada.
//   http://www.site.uottawa.ca
//
// Creator: jlang (Jochen Lang)
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: teapot.h,v $
// Revision 1.2  2018/02/17 03:21:12  jlang
// Simplified shape hierarchy for simplifying modifications
//
// Revision 1.1  2018/01/28 05:45:57  jlang
// Switch to core
//
//
// ==========================================================================
#ifndef CSI4130_TEAPOT_H_
#define CSI4130_TEAPOT_H_

// gl types
#include <GL/glew.h>
// glm types
#include <glm/glm.hpp>

#include "render_shape.h"

class Teapot : public RenderShape {
 public:
  Teapot();
};
#endif
