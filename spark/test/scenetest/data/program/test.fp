!!ARBfp1.0
# Simple program to show how to code up the default texture environment

ATTRIB col = fragment.color.primary; #diffuse interpolated color

ATTRIB tc0 = fragment.texcoord[0];      #first set of texture coordinates
ATTRIB L = fragment.texcoord[1];      #direction to light in tangent space

PARAM lightPos = program.local[0];      #world-space position of light
PARAM  red = { 1.0,  0.0,  0.0,  1.0 };

OUTPUT outColor = result.color;

TEMP matDiffuse;
TEMP normal;
TEMP matSpecular;
TEMP invLen, temp, normL;

TXP matDiffuse,		tc0, texture[0], 2D;           #sample the diffuse texture
TXP normal,			tc0, texture[1], 2D;           #sample the normalmap
TXP matSpecular,	tc0, texture[2], 2D;           #sample the specular texture

# vector to light
DP3		temp,   L, L;  # Normalize light vector
RSQ		invLen, temp.x;
MUL		normL,  L, invLen;

TEMP intensity;
DP3 intensity, normL, normal;

MUL temp, intensity, matDiffuse;
MOV outColor, temp;
    
END 
