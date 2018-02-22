// ==========================================================================
// $Id: steps.h,v 1.3 2018/01/29 19:57:07 jlang Exp $
// Animation class for solar example 
// Modified code from solar2.cpp created by Francisco Javier Ovalle Martinez
// inspired by the OpenGL programming guide by Shreiner et al.
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
// Creator: Jochen Lang
// Email:   jlang@site.uottawa.ca
// ==========================================================================
// $Log: steps.h,v $
// Revision 1.3  2018/01/29 19:57:07  jlang
// Updated descriptions
//
// Revision 1.2  2014/09/19 17:19:34  jlang
// Took care of compiler warnings switched to radians.
//
// Revision 1.1  2014/01/25 21:58:13  jlang
// Created lab2 without fixed pipeline
//
// Revision 1.1  2011/01/29 19:27:04  jlang
// Solution for lab3
//
// ==========================================================================
#ifndef CSI4130_STEPS_H
#define CSI4130_STEPS_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <cstdlib>
#include <cassert>
#include <GL/gl.h>

namespace CSI4130 {

// Animation steps
class Steps {
	GLfloat incSun; // increment per step for the sun 
	GLfloat incPlanet[4]; // increment per step for the planets
	GLfloat incRing; // increment per step for saturn ring and moon
	GLfloat rotSun; // rotation angle of sun
	GLfloat rotPlanet[2]; // rotation angle of planets
	GLfloat rotRing; // rotation angle of moon and ring around saturn

 public:    
 Steps(): incSun(0.01f), incRing( 0.2f ), rotSun(0.0f), rotRing(0.0f)
		{
			for (int i=0; i<2; ++i ) {
				incPlanet[i] = (2-i)*0.05f; // years
			}
			rotPlanet[1] = rotPlanet[0] = 0.0f;
		}

	// Accessors
	GLfloat getRotSun() { return rotSun; }
	GLfloat getRotPlanet( int i ) { 
		assert( i>=0 && i<2); 
		return rotPlanet[i]; 
	}
	GLfloat getRotRing() { return rotRing; }

	void update(void) {
		rotSun += incSun;
		rotSun = rotSun>360.0f?rotSun
			-static_cast<int>(rotSun/360.0f)*360.0f:rotSun;
		for (int i=0; i<2; ++i ) {
			rotPlanet[i] += incPlanet[i]; 
			rotPlanet[i] = rotPlanet[i]>360.0f?rotPlanet[i] - 
				static_cast<int>(rotPlanet[i]/360.0f)*360.0f:rotPlanet[i];
		}
		rotRing += incRing;
		rotRing = rotRing>360.0f?rotRing
			-static_cast<int>(rotRing/360.0f)*360.0f:rotRing;
	}

	void faster() {
		// Ring/moon rotations are fastest -- limit speed
		if ( incRing < 180.0f ) {
			incRing *= 2.0f; 
			incSun *= 2.0f;
			for (int i=0; i<4; ++i ) {
				incPlanet[i] *= 2.0f;
			}
		}
	}

	void slower() {
		// sun is slowest
		if ( incSun > 1e-3 ) {
			incSun /= 2.0f;
			incRing /= 2.0f; 
			for (int i=0; i<4; ++i ) {
				incPlanet[i] /= 2.0f;
			}
		}
	}
};

} // end namespace

#endif
