uniform float currentAngle;

void main( void )
{
	vec4 v = vec4( gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, 1.0 );

	//
	// First, use the sin() function to displace the vertices' Y value as a 
	// function of its X value summed with a uniform parameter which  
	// continuously grows from 0 to 360 and repeats. This will cause the 
	// geometry to ripple like a sine wave.
	//
	// Of course, we don't want our flag to look like a perfect sine wave, so
	// add in some variation by calling sin() again, but this time, use the Z 
	// value.
	//
	// Also, flag vertices, which are closer to the flagpole, shouldn't move  
	// much at all because they're attached to the pole, so use the X value  
	// again to diminish the movement of Y values which are closer to the  
	// flagpole. This becomes easy since X values start out at 0.0f near the   
	// flagpole and grow larger as they move away.
	//
	// Finally, I use a value of 0.08f to scale the effect for visual appeal.
	//

	v.y  = sin( gl_Vertex.x + currentAngle );
	v.y += sin( gl_Vertex.z + currentAngle );
	v.y *= gl_Vertex.x * 0.08;

	gl_Position = gl_ModelViewProjectionMatrix * v;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
