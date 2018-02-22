// ==========================================================================
// $Id: solar.fs,v 1.3 2018/02/04 01:24:53 jlang Exp $
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
// $Log: solar.fs,v $
// Revision 1.3  2018/02/04 01:24:53  jlang
// switch to core
//
// Revision 1.2  2014/01/26 16:19:31  jlang
// Verified program for OpenGL 3.3
//
// Revision 1.1  2014/01/25 21:58:12  jlang
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
//
// ==========================================================================
#version 330 core

in vec4 colorVertFrag;

out vec4 color;

void main() {
  // color the fragment 
  color = colorVertFrag;
}
