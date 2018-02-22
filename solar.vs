// ==========================================================================
// $Id: solar.vs,v 1.3 2018/01/28 05:45:57 jlang Exp $
// Basic shader emulating fixed pipeline functionality
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
// $Log: solar.vs,v $
// Revision 1.3  2018/01/28 05:45:57  jlang
// Switch to core
//
// Revision 1.2  2014/01/26 16:19:31  jlang
// Verified program for OpenGL 3.3
//
// Revision 1.1  2014/01/25 21:58:13  jlang
// Created lab2 without fixed pipeline
//
// Revision 1.1  2014/01/18 16:43:12  jlang
// Created solution for lab1 in OpenGL 4
//
// Revision 1.1  2011/01/15 00:23:36  jlang
// Created lab1 including skeleton
//
// Revision 1.1  2011/01/08 04:36:11  jlang
// Lab 0 solution
//
// ==========================================================================
#version 330 core

layout (location=0) in vec4 position;

uniform vec3 objColor;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 colorVertFrag; // Pass the color on to rasterization

void main() {
  // map the vertex position into clipping space 
  // Here we use deprecated gl_Vertex to interface with fixed pipeline
  //
	gl_Position = ProjectionMatrix * ModelViewMatrix * position;
  // Here use deprecated gl_Color to interface with fixed pipeline
  colorVertFrag = vec4( objColor, 1.0f ); // gl_Color;
}

