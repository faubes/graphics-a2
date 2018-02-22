// ==========================================================================
// $Id: solar.cpp,v 1.8 2018/02/17 03:21:06 jlang Exp $
// Solar example for hierachical modeling
// 
// Code inspired by the OpenGL programming guide by Shreiner et al.
// ==========================================================================
// (C)opyright:
//
//   Jochen Lang
//   EECS, University of Ottawa
//   800 King Edward Ave.
//   Ottawa, On., K1N 6N5
//   Canada.
//   http://www.eecs.uottawa.ca
//
// Creator: Jochen Lang
// Email:   jlang@uottawa.ca
// ==========================================================================
// $Log: solar.cpp,v $
// Revision 1.8  2018/02/17 03:21:06  jlang
// Simplified shape hierarchy for simplifying modifications
//
// Revision 1.6  2018/01/30 16:50:22  jlang
// fixed indentation
//
// Revision 1.5  2018/01/29 19:57:07  jlang
// Updated descriptions
//
// Revision 1.5  2018/01/28 05:45:57  jlang
// Switch to core
//
// Revision 1.4  2017/10/26 20:04:01  jlang
// Added algorithm library for min/max in windows.
//
// Revision 1.3  2014/09/19 17:19:34  jlang
// Took care of compiler warnings switched to radians.
//
// Revision 1.2  2014/01/26 16:19:31  jlang
// Verified program for OpenGL 3.3
//
// Revision 1.1  2014/01/25 21:58:13  jlang
// Created lab2 without fixed pipeline
//
// Revision 1.5  2011/01/31 01:51:16  jlang
// added skeleton
//
// Revision 1.4  2011/01/29 21:06:28  jlang
// Solution to lab3
//
// Revision 1.3  2011/01/29 19:26:30  jlang
// Solution for lab3
//
// Revision 1.2  2011/01/29 13:32:39  jlang
// Check-in for laptop
//
// Revision 1.1  2011/01/29 01:49:44  jlang
// Added lab 3
//
// Revision 1.3  2008/01/24 03:45:24  jlang
// Fixed stepping, window setup and comments
//
// Revision 1.2  2007/01/22 20:19:03  jlang
// Ayman's changes
//
// Revision 1.1  2006/01/23 21:40:31  jlang
// Added tutorial on transormation matrices
//
//
//
// ==========================================================================
#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <cstdlib>
#include <algorithm>
#include <stack>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


// glm types
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// matrix manipulation
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>
// printing
#include "glm/gtx/string_cast.hpp"

#include "shader.h"
#include "steps.h"
#include "render_shape.h"
#include "sphere.h"
#include "torus.h"
#include "teapot.h"


using namespace CSI4130;
using std::cerr;
using std::endl;


namespace CSI4130 {
// Window dimensions
struct WindowSize {
  const GLfloat d_near;
  const GLfloat d_far;
  GLint d_widthPixel;
  GLfloat d_width;
  GLint d_heightPixel;
  GLfloat d_height;
  WindowSize() : d_near(1.0f), d_far(21.0f),
		 d_widthPixel(512), d_width(12.5f),
		 d_heightPixel(512), d_height(12.5f)
  {}
}; 

/*
 * Helper structure holding the locations of the uniforms to transform matrices
 */
struct Transformations {
  GLint locMV;
  GLint locP;
  Transformations() : locMV(-1), locP(-1) {}
};

/** Global variables */
Sphere g_sphereShape;

Torus g_torusShape;
Teapot g_teapotShape;
GLuint g_sphere_vao;
GLuint g_torus_vao;
GLuint g_teapot_vao;

GLint g_color_loc;	
	
Transformations g_tfm;
Steps g_steps;
WindowSize g_winSize;  

void init(void) 
{	
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glEnable( GL_DEPTH_TEST );
  errorOut();

  // Make sure that our shaders run
  int major, minor;
  getGlVersion( major, minor );
  cerr << "Running OpenGL "<< major << "." << minor << endl; 
  if ( major < 3 || (major==3 && minor<3)) {
    cerr << "No OpenGL 3.3 or higher" <<endl;
    exit(-1);
  }

  // Load shaders
  vector<GLuint> sHandles;
  GLuint handle;
  Shader solar;
  if ( !solar.load("solar.vs", GL_VERTEX_SHADER )) {
    solar.installShader( handle, GL_VERTEX_SHADER );
    Shader::compile( handle );
    sHandles.push_back( handle );
  }
  if ( !solar.load("solar.fs", GL_FRAGMENT_SHADER )) {
    solar.installShader( handle, GL_FRAGMENT_SHADER ); 
    Shader::compile( handle );
    sHandles.push_back( handle );
  }
  cerr << "No of handles: " << sHandles.size() << endl;
  GLuint program;
  Shader::installProgram(sHandles, program); 
  errorOut();

  // Activate program in order to be able to set uniforms 
  glUseProgram(program);
  errorOut();
	// vertex attributes
	GLint locPos = glGetAttribLocation(program, "position");
  // find the locations of our uniforms and store them in a global structure for later access
  g_tfm.locMV = glGetUniformLocation( program, "ModelViewMatrix");
  g_tfm.locP = glGetUniformLocation( program, "ProjectionMatrix");
	g_color_loc = glGetUniformLocation( program, "objColor");	
  errorOut();

	//---------------------------------------------------------
  // Make the spheres nicer
  for (int i = 0; i < 3; ++i) {
	  g_sphereShape.subdivide();
  }
  // -------------------------------------------------------------
	// Generate a VAO for the sphere
  glGenVertexArrays(1, &g_sphere_vao );
  glBindVertexArray( g_sphere_vao );

  
  // Element array buffer object
	GLuint g_ebo; 
  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo );
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	       sizeof(GLushort) * g_sphereShape.getNIndices(),
							 g_sphereShape.getIndicies(), GL_STATIC_DRAW );
  errorOut();
	
  GLuint vbo;
  glGenBuffers( 1, &vbo );
  errorOut();
  glBindBuffer(GL_ARRAY_BUFFER, vbo );
  glBufferData(GL_ARRAY_BUFFER, 
	       sizeof(GLfloat) * g_sphereShape.getNPoints(),
							 g_sphereShape.getVertices(), GL_STATIC_DRAW);
	errorOut();
  // pointer into the array of vertices which is now in the VAO
  glVertexAttribPointer(locPos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(locPos); 
  errorOut();

	//---------------------------------------------------------
	
	// Generate a VAO for the torus
  glGenVertexArrays(1, &g_torus_vao );
  glBindVertexArray( g_torus_vao );

  // Element array buffer object
  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo );
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	       sizeof(GLushort) * g_torusShape.getNIndices(),
							 g_torusShape.getIndicies(), GL_STATIC_DRAW );
  errorOut();

  glGenBuffers( 1, &vbo );
  errorOut();
  glBindBuffer(GL_ARRAY_BUFFER, vbo );
  glBufferData(GL_ARRAY_BUFFER, 
	       sizeof(GLfloat) * 3 * g_torusShape.getNPoints(),
							 g_torusShape.getVertices(), GL_STATIC_DRAW);
  // pointer into the array of vertices which is now in the VAO
  glVertexAttribPointer(locPos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(locPos); 
  errorOut();

	//---------------------------------------------------------
	
	// Generate a VAO for the teapot
  glGenVertexArrays(1, &g_teapot_vao );
  glBindVertexArray( g_teapot_vao );

  // Element array buffer object
  glGenBuffers(1, &g_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo );
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	       sizeof(GLushort) * g_teapotShape.getNIndices(),
							 g_teapotShape.getIndicies(), GL_STATIC_DRAW );
  errorOut();

  glGenBuffers( 1, &vbo );
  errorOut();
  glBindBuffer(GL_ARRAY_BUFFER, vbo );
  glBufferData(GL_ARRAY_BUFFER, 
	       sizeof(GLfloat) * 3 * g_teapotShape.getNPoints(),
							 g_teapotShape.getVertices(), GL_STATIC_DRAW);
  // pointer into the array of vertices which is now in the VAO
  glVertexAttribPointer(locPos, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(locPos); 
  errorOut();

	//---------------------------------------------------------
	
  // set the projection matrix with a uniform
  glm::mat4 Projection = glm::ortho( -g_winSize.d_width/2.0f, g_winSize.d_width/2.0f, 
				     -g_winSize.d_height/2.0f, g_winSize.d_height/2.0f,
				     g_winSize.d_near, g_winSize.d_far );
  glUniformMatrix4fv(g_tfm.locP, 1, GL_FALSE, glm::value_ptr(Projection));
  errorOut();
}


void display(void)
{	
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Set the modelview matrix to I
  // Use glm to emulate fixed function pipeline
  // Load identity
  glm::mat4 ModelView = glm::mat4(1.0f);

  // Move the orgin to the center of our viewing volume
  // Our viewing volume is from -near to -far, i.e.,
  // we move our coordinate system to -(far-near)/2 
  ModelView = glm::translate(ModelView, glm::vec3( 0, 0,-(g_winSize.d_far-g_winSize.d_near)/2.0f ));
 
  // Sun is rotating around z in viewing coordinate system
  ModelView = glm::rotate(ModelView, glm::radians(g_steps.getRotSun()), glm::vec3(0, 0, 1.0f ));
  // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locMV, 1, GL_FALSE, glm::value_ptr(ModelView));
  // draw sun 
  glUniform3f(g_color_loc, 1.0f, 1.0f, 0.0f); // Sun's color
  // Sphere of radius 1
  glBindVertexArray(g_sphere_vao);
  glDrawElements(GL_TRIANGLES, g_sphereShape.getNIndices(),
		 GL_UNSIGNED_SHORT, 0);	
  errorOut();
  // save matrix state -- to get back to sun
  // Use std::stack to keep push and pop from fixed pipeline
  std::stack<glm::mat4>  ModelViewStack;
  ModelViewStack.push(ModelView);
  
  // Object transform:
  // Scale the planet to 1/3, translate away in x and then 
  // rotate the planet around y in the sun's coordinate system
  // Frame transform in reverse: (move the sun's to the planet's coordinate system)
  // Rotate the sun frame around its y axis
  // translate in the direction of the new x and finally reduce the axis to 1/3 to make the planet size 1 
  ModelView = glm::rotate(ModelView, glm::radians(g_steps.getRotPlanet(0)), glm::vec3(0, 1.0f, 0 ));
  ModelView = glm::translate(ModelView, glm::vec3( 3.0f, 0.0f, 0.0f));  
  ModelView = glm::scale(ModelView, glm::vec3( 0.33f, 0.33f, 0.33f));
  // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locMV, 1, GL_FALSE, glm::value_ptr(ModelView));
  glUniform3f(g_color_loc, 0.0f, 0.0f, 0.9f); // blue planet 
  glBindVertexArray(g_sphere_vao);
  glDrawElements(GL_TRIANGLES, g_sphereShape.getNIndices(),
		 GL_UNSIGNED_SHORT, 0);
  errorOut();

  // Add the white teapot relative to the planet - using the planet's coordinates
  // The teapot should not rotate but stay on top of the planet
  // Reverse frame (starting with the planet ending up in the teapot):
  // Rotate in reverse around around y of the planet coordinates
  // Rotate in reverse around the new z to compensate for the sun rotation
  // Translate up to get on top of the planet
  // Scale 1/3 to make the teapot height the same as the radius of the planet 
  ModelView = glm::rotate(ModelView, glm::radians(-g_steps.getRotPlanet(0)), glm::vec3(0, 1.0f, 0 ));
  ModelView = glm::rotate(ModelView, glm::radians(-g_steps.getRotSun()), glm::vec3(0, 0, 1.0f ));
  ModelView = glm::translate(ModelView, glm::vec3(0.0f,1.0f,0.0f));
  ModelView = glm::scale(ModelView, glm::vec3(0.33f,0.33f,0.33f)); // set teapot size
  // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locMV, 1, GL_FALSE, glm::value_ptr(ModelView));
  glUniform3f(g_color_loc, 0.9f, 0.9f, 0.9f);  // white
  glBindVertexArray(g_teapot_vao);
  glDrawElements(GL_TRIANGLES, g_teapotShape.getNIndices(),
		 GL_UNSIGNED_SHORT, 0);
  errorOut();

  // back to sun
  ModelView = ModelViewStack.top();
  ModelViewStack.pop();
  // Start saturn at 45 degree of the x-axis
  // Reverse frame (starting with the sun ending up in the saturn):
  // Rotate the frame Rz by 45 deg starting at the sun's coordinate system
  // Rotate the frame in y for the animation
  // Translate the frame by 5 in the new x to the location of saturn
  // Scale the frame to 0.5f to make saturn smaller
  ModelView = glm::rotate(ModelView, glm::radians(45.0f), glm::vec3(0, 0, 1.0f));
  ModelView = glm::rotate(ModelView, glm::radians(g_steps.getRotPlanet(1)), glm::vec3(0, 1.0f, 0));
  // position the planet
  ModelView = glm::translate(ModelView, glm::vec3(5.0f, 0.0, 0.0));
  ModelView = glm::scale(ModelView, glm::vec3( 0.5f, 0.5f, 0.5f));
  // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locMV, 1, GL_FALSE, glm::value_ptr(ModelView));
  glUniform3f(g_color_loc, 0.4f, 0.3f, 0.1f);  // saturn
  glBindVertexArray(g_sphere_vao);
  glDrawElements(GL_TRIANGLES, g_sphereShape.getNIndices(),
		 GL_UNSIGNED_SHORT, 0);
  errorOut();

  // Add the saturn-like ring -- rotate around the z-axis of saturn
  ModelView = glm::rotate(ModelView, glm::radians(45.0f), glm::vec3(1.0f, 0, 0));
  ModelView = glm::rotate(ModelView, glm::radians(g_steps.getRotRing()), glm::vec3(0, 0, 1.0f));
  ModelViewStack.push(ModelView); // save ring's rotation coordinate system

  // stretch the ring in x and z, and squish it in y
  ModelView = glm::scale(ModelView, glm::vec3(1.8f, 0.5f, 1.8f));

  // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locMV, 1, GL_FALSE, glm::value_ptr(ModelView));
  glUniform3f(g_color_loc, 0.4f, 0.4f, 0.4f);  // saturn ring color
  glBindVertexArray(g_torus_vao);
  glDrawElements(GL_TRIANGLES, g_torusShape.getNIndices(),
		 GL_UNSIGNED_SHORT, 0);
  errorOut();

  // back to saturn's ring (without scale)
  ModelView = ModelViewStack.top();
  ModelViewStack.pop();
  // Add the moon of saturn
  // Reverse frame rotation:
  // Rotate around z by 15 degree
  // Move the frame out to a bit beyond the ring
  // Scale the frame to 0.1 of saturn's frame
  ModelView = glm::rotate(ModelView, glm::radians(15.0f), glm::vec3(0, 0, 1.0f));
  ModelView = glm::translate(ModelView, glm::vec3( 2.0f, 0.0f, 0.0f )); 

  ModelView = glm::scale(ModelView, glm::vec3(0.1f, 0.1f, 0.1f));
  // Update uniform for this drawing
  glUniformMatrix4fv(g_tfm.locMV, 1, GL_FALSE, glm::value_ptr(ModelView));
  glUniform3f(g_color_loc, 0.8f, 0.7f, 0.2f);  // saturn moon color
  glBindVertexArray(g_sphere_vao);
  glDrawElements(GL_TRIANGLES, g_sphereShape.getNIndices(),
		 GL_UNSIGNED_SHORT, 0);
  errorOut();
  glFlush();
  glutSwapBuffers();
}


/**
 * OpenGL reshape function - main window
 */
void solarReshape( GLsizei _width, GLsizei _height ) {
  GLfloat minDim = std::min(g_winSize.d_width,g_winSize.d_height);
  // adjust the view volume to the correct aspect ratio
  if ( _width > _height ) {
    g_winSize.d_width = minDim  * (GLfloat)_width/(GLfloat)_height;
    g_winSize.d_height = minDim;
  } else {
    g_winSize.d_width = minDim;
    g_winSize.d_height = minDim * (GLfloat)_height/(GLfloat)_width;
  }
  glm::mat4 Projection = glm::ortho( -g_winSize.d_width/2.0f, g_winSize.d_width/2.0f, 
				     -g_winSize.d_height/2.0f, g_winSize.d_height/2.0f,
				     g_winSize.d_near, g_winSize.d_far ); 
  glUniformMatrix4fv(g_tfm.locP, 1, GL_FALSE, glm::value_ptr(Projection));
  g_winSize.d_widthPixel = _width;
  g_winSize.d_heightPixel = _height;
  // reshape our viewport
  glViewport( 0, 0, 
	      g_winSize.d_widthPixel,
	      g_winSize.d_heightPixel );
}


void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
  case 'q':
    exit(0);
    break;
  case '+':
    g_steps.faster();
    break;
  case '-':
    g_steps.slower();
    break;
  default:
    break;
  }
}

void idleFunc(void)
{
  g_steps.update();
  glutPostRedisplay();
}


}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 600); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   GLenum err = glewInit();
   if (GLEW_OK != err) {
     /* Problem: glewInit failed, something is seriously wrong. */
     cerr << "Error: " << glewGetErrorString(err) << endl;
     return -1;
   }
   cerr << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
   cerr << "Before init" << endl;
   init();
   cerr << "After init" << endl;
   glutReshapeFunc(solarReshape);
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idleFunc);
   glutMainLoop();
   return 0;
}

