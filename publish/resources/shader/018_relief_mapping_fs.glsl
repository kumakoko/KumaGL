uniform sampler2D testTexture;
uniform sampler2D HeightTexture;
varying vec2 v_texCoord;

varying vec3 EyeDir;

// A function prototype? In a shader?
// Yes that’s right. Its actually much better to
// do it this way rather than inject the code
// into the 'main' function. Primarily because
// if you plan to add self-shadowing later.
// It makes the code much nicer to read and
// it’s easier to do, as it is only an extra ~2
// lines.
vec2 RayCast(sampler2D HMap, vec2 tc, vec3 ray);

void main( void )
{
    // Simply call the RayCast function, giving it
    // the hightmap, the current texture-coords and
    // the Eye Direction. Simple.
    vec2 TexCoords =RayCast(HeightTexture, gl_TexCoord[0].xy, EyeDir);

    // This also works extremely with Bump Mapping.
    // just remember to use the new tex-coords when
    // getting the normal.

    // It’s also possible to use self shadowing.
    // which is simply ray tracing the light.

    gl_FragColor =texture2D( testTexture, TexCoords ); 
}

// Ok, the pride of this sample. This function is
// Simple. You give it the heightmap, the current
// Texture coords and the 'ray' (eyeDir).
// It will raytrace the heightmap using 'ray'.
// And give back the coords of the point where
// the ray intersects the heightmap.
vec2 RayCast(sampler2D HMap, vec2 tc, vec3 ray)
{
    // Firstly, some constants. You could change
    // these to uniforms, which would let you increase/
    // decrease detail depending on the situation.
    // It would, however, force the compiler to not
    // unroll the loops, causing the shader to require
    // SM3.0 hardware.
    
    const int numsteps = 10;
    const int substeps = 5;
    
    // These are essentially the variables used.
    // The 'height' is the height of the current 'point'
    // the 'step' is the change in 'height' each linear step.
    // the 'currStep' is the current distance away from 
    // the origin of the 'ray'.
    // 'tHeight' is the height in the heightmap.
    // and 'delta' is the ray direction, used to return the tex-coords.
    
    float height = 1.0;
    float step= 1.0/float(numsteps);
    int currStep;
    vec4 tHeight= texture2D(HMap,tc);
    vec2 delta = vec2(-ray.x,-ray.y) * 0.05/ (ray.z);

    
    // The ray tracing is split up into 2 
    // parts. A linear test and a binary test.
    // The linear test simply splits the 'ray' 
    // into 'points' ('numsteps' is the total number of 'points')
    // and tests very 'point' along the ray.

    for(int i=1; i<=numsteps;i++)
    {      
        // Test the test height (tHeight) by the
        // current height (height). If tHeight is above
        // height then we have found the first point
        // that is below the heightmap.
        // If height is above tHeight, just continue the test.
        if(tHeight.x <= height)     
        {
            height -=step; 
            //height = 1.0 - step*(i);
            currStep=i;
            tHeight= texture2D(HMap,tc-delta*height);
        }
    }

    // The linear test ends, and should fill the currStep
    // and height vars with the data of the first 'point' that’s
    // under the heightmap. We don’t want this, we want the point before
    // it, the last point above the height map. Which is simply the 
    // last point before this one. So backtrack by one point.
    
    currStep-=1;
    height +=step; 

    // Next is the binary test. This is more complicated.
    // What it does is it has a 'test area' which begins simply
    // at the current point and ends with the next point (under the heightmap).
    // It divides the 'test area' in half, and tests the middle point.
    // The middle point becomes the new start point if it is above
    // the heightmap, or it becomes the endpoint if it is below the
    // heightmap. All the binary test does is split the area in half
    // 'substep' times. Producing very nice accuracy for the speed
    // compared to using just linear testing.
    
    for(int i=0; i<substeps;i++)
    {     
        step*=0.5;        
        height -=step; 
        tHeight= texture2D(HMap,tc-delta*height); 
    
        if(tHeight.x >= height)
        {   
            height+=step;              
        }
    }

    // Finally, calculate the new texture coords by simply
    // multiplying the current height by the 'delta' value.
    // and subtract it from the old tex coords.
    
    return tc-delta*height;    
}
