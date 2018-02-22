// ==========================================================================
//
// ==========================================================================
#include "icosahedron.h"


Icosahedron::Icosahedron() : RenderShape() {
	// The coordinates of the 12 vertices can be defined by the vectors defined by all the possible cyclic permutations 
	// and sign-flips of coordinates of the form (2, 1, 0). 
	// https://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates
		d_vertex.insert(d_vertex.end(),
		{	2.0f, 1.0f, 0.0f,
			2.0f, -1.0f, 0.0f,
			-2.0f, 1.0f, 0.0f,
			-2.0f, -1.0f, 0.0f,

			0.0f, 2.0f, 1.0f, 
			0.0f, 2.0f, -1.0f,
			0.0f, -2.0f, 1.0f,
			0.0f, -2.0f, -1.0f,

			2.0f, 0.0f, 1.0f, 
			2.0f, 0.0f, -1.0f,
			-2.0f, 0.0f, 1.0f,
			-2.0f, 0.0f, -1.0f, 
		});
	// 20 faces
	d_index.insert(d_index.end(), {
		
	});
	// direct specification with all faces unrolled
	// - not supplied
}

