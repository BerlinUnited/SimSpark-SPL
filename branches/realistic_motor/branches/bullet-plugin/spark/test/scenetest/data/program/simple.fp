!!ARBfp1.0

PARAM	constants = {2.0, 1.0, 32.0, 0.5 };
PARAM	attParams = { 1.0, 0.0, 0.000005, 0.0 };
PARAM	lightDiffuse = program.local[0];      #diffuse color of light
PARAM	lightRadius = program.local[1];      #radius of the light

ATTRIB tc0 = fragment.texcoord[0];      #first set of texture coordinates
ATTRIB toLight = fragment.texcoord[1];      #first set of texture coordinates
ATTRIB toView = fragment.texcoord[2];      #first set of texture coordinates

OUTPUT outColor = result.color;

TEMP diffuse, specular;
TEMP bump, L, R, V;
TEMP	invLen, temp;

# sample textures
	TXP diffuse,	tc0, texture[0], 2D;           #sample the diffuse texture
	TXP bump,		tc0, texture[1], 2D;           #sample the normal map
	TXP specular,	tc0, texture[2], 2D;           #sample the specular map

# unpack normal
	MAD		bump, bump, constants.x, -constants.y;                              # Scale and bias

# normalize normal
	DP3		temp,   bump, bump;
	RSQ		temp.w, temp.w;
	MUL		bump,   bump, temp.w;

# normalize toLight
TEMP distSq;
	DP3		distSq,   toLight, toLight; 
	RSQ		temp.w, distSq.w;
	MUL		L,  toLight, temp.w;

# normalize toView
	DP3		temp,   toView, toView; 
	RSQ		temp.w, temp.w;
	MUL		V,  toView, temp.w;

#reflection vector
	DP3		temp,	bump, L;
	MUL		temp,	temp, constants.x;
	MAD		R,		temp, bump, -L;

#specular exponent
	DP3_SAT	R,		R, V;
	LG2		temp,	R.x;
	MUL		temp,	temp, constants.z;
	EX2		R, temp.x;
	MUL		R, R, specular;

TEMP attenuation;
	MAD_SAT		attenuation, lightRadius.x, distSq.w, attParams.x;

TEMP intensity;
DP3_SAT  intensity, L, bump;

#M temp, intensity, diffuse;
MUL intensity, intensity, lightDiffuse;
MUL diffuse, diffuse, attenuation;
MUL R, R, attenuation;
MAD intensity, intensity, diffuse, R;
MOV outColor, intensity;
    
END 
