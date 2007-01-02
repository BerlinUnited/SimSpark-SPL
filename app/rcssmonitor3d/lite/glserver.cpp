/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: glserver.cpp,v 1.12 2007/01/02 13:56:55 jamu Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "glserver.h"
#include <iostream>
#include <string>

using namespace salt;

GLServer::GLServer(int width, int height,
                   Vector3f camPos,
                   Vector3f lookAtPos,
                   Vector3f up, bool wire)
{
    mCamPos  = camPos;
    mLookAt  = lookAtPos;
    mUp      = up;

    mWidth     = width;
    mHeight    = height;
    mWireframe = wire;

    mCamera = Camera(camPos, lookAtPos, up);
}

// void
// GLServer::InitIcons
// {
//     mCamModeIcon[][][][eFree]
//     mCamModeIcon[][][][eFollowBall]
//     mCamModeIcon[][][][eCenterBall]
        
//         }

// void
// GLServer::InitTexture(const string &tFile)
// {

//     if (!ReadTexture(tFile))
//     {
//         mTextureWidth  = 128;
//         mTextureHeight = 128;

//         cout << "Texture: Loading " << tFile << " failed, using random image.\n";
    
//         int c;

//         for (int i = 0;i <  mTextureHeight; i++) {        
//             for (int j = 0;j <  mTextureWidth; j++) { 
                
//                 c = ((((i&0x10)==0)^((j&0x10))==0))*15;      
            
//                 mTexture[i][j][0] = (GLubyte) 55+c;
//                 mTexture[i][j][1] = (GLubyte) 156+c;
//                 mTexture[i][j][2] = (GLubyte) 55+c;
//                 mTexture[i][j][3] = (GLubyte) 255;
//             }
//         }
//     }
// }


// bool
// GLServer::ReadTexture(const string &tFile)
// {
//     // FIXME: This is a crude hack. No flexibility in the file allowed.
//     // rewrite with fileserver etc...
//     ifstream texturefile;

//     // absolute path to texturefile
//     string absTFile = "";
    
//     if (tFile[0] == '/')
//     {
//         // absolute path
//         absTFile = tFile;
        
//         texturefile.open(absTFile.c_str()); 
//     }
//     else
//     {
//         // relative path given, so we try ~/.rcssserver3d/ and
//         // $prefix/share/rcssserver3d/ as directories

//         char* home = getenv("HOME");

//         if (home)
//         {
//             absTFile = string(home)+"/.rcssserver3d/"+tFile;
//             texturefile.open(absTFile.c_str()); 
//         }

//         if (!texturefile)
//         {
//             // some macro magic (not at all)
//             absTFile = PREFIX "/share/" PACKAGE_NAME "/"+tFile;

//             texturefile.open(absTFile.c_str()); 
//         }
//     }
    
//     if (!texturefile) 
//     {
//         cout << "Texture: could not open texture file" << endl;
//         return false;
//     }

//     string line;
    
//     //read magic no.
//     texturefile >> line;
    
//     if (line != "P6")
//     {
//         cout << "Texture: wrong magic number. No raw PPM" << endl;
//         return false;
//     }
    
//     // comment by gimp
//     texturefile >> line;
//     texturefile.ignore(INT_MAX, '\n');
    
//     // read the size
//     texturefile >> mTextureWidth >> mTextureHeight;
    
//     if ((mTextureWidth != 128) || (mTextureHeight > 128))
//     {
//         // texture has the wrong size
//         cout << "Texture: Wrong Size" << endl;
//         return false;
//     }       
    
//     int numcolors;
//     texturefile >> numcolors;
//     if (numcolors > 255)
//     {
//         cout << "Texture: too many colors (" << numcolors << ")" << endl;
//         return false;
//     }
    
//     GLubyte r,g,b;
    
//     for (int i = 0;i <  mTextureHeight; i++)
//     {        
//         for (int j = 0;j <  mTextureWidth; j++)
//         { 
//             texturefile >> r >> g >> b;
            
//             mTexture[i][j][0] = (GLubyte) r;
//             mTexture[i][j][1] = (GLubyte) g;
//             mTexture[i][j][2] = (GLubyte) b;
//                 mTexture[i][j][3] = (GLubyte) 255;
//         }
//     }
//     texturefile.close();
    
//     return true;
    
// }




//---------------------------------ApplyCamera--------------------------
//
//----------------------------------------------------------------------
void GLServer::ApplyCamera()
{
    //switch z-buffer on
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);

    //create a viewport
    glViewport(0,0,mWidth,mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //setup camera angle, ratio, near and far clipping plane
    gluPerspective(45.0f,(GLfloat)mWidth/(GLfloat)mHeight,0.1f,10000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mCamera.Look();
}

//---------------------------------initGL-------------------------------
//
// OpenGL initialisation function
//----------------------------------------------------------------------

void GLServer::InitGL (void)
{
    glDisable(GL_LIGHTING);     
        
    //JAN
    /*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &mTexNames);
    glBindTexture(GL_TEXTURE_2D, mTexNames);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexture); 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  
 	// 2d texture, 3 colors, width, height, RGB in that order, byte data, and the data.
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, mTextureWidth, mTextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, mTexture);*/         
}

void GLServer::DrawTextPix(const char* text, Vector2f pix, ETextAlign ta)
{

    int width = GetTextWidth(text);
    switch (ta)
    {
    case eCENTER:
        pix[0] = (mWidth - width)/ 2.0 + pix[0];
        break;
    case eRIGHT:
        pix[0] = mWidth - width + pix[0];
        break;
    default: ;
    }

    const Vector2f pos(pix[0] / (float)mWidth * 2 - 1.0f,
                       1.0f - (pix[1] / (float)mHeight * 2));
    DrawText(text,pos);
}

int
GLServer::GetTextHeight() const
{
    // currently only GLUT_BITMAP_HELVETICA_18 is used
    return 18;
}

int
GLServer::GetTextWidth(const char* text) const
{
    // currently GLUT_BITMAP_HELVETICA_18 is used
    int width = 0;
    for (const char* s = text; *s; ++s)
    {
        width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *s);
    }
    return width;
}

void
GLServer::DrawText3D(const char* text, const Vector3f& pos)
{
    glDisable (GL_DEPTH_TEST);
    //glDisable (GL_TEXTURE_2D);

    glRasterPos3f(pos[0],pos[1],pos[2]);
    for (const char* s = text; *s; ++s)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
    }

    //glEnable (GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
}

void
GLServer::DrawText3D(const std::string& text, const Vector3f& pos)
{
    glDisable (GL_DEPTH_TEST);
    //glDisable (GL_TEXTURE_2D);

    glRasterPos3f(pos[0],pos[1],pos[2]);
    for (unsigned int i = 0; i < text.length(); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    }

    //glEnable (GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
}

//--------------------------drawText-------------------------------------
//draws a given text string onto the screen at position pos;; (-1,1)
//is top left, (+1,-1) is bottom right of the viewport
//-----------------------------------------------------------------------
void GLServer::DrawText(const char* text, Vector2f pos)
{
    //glDisable (GL_TEXTURE_2D);
    glDisable (GL_DEPTH_TEST);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glOrtho (0.0,mWidth,0.0,mHeight,-0.0,0.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    glRasterPos2f(pos[0],pos[1]);
    for (const char* s = text; *s; ++s)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s);
        }
}


void GLServer::DrawGroundRectangle(Vector3f pos, float szX, float szY,
                                   float angleDeg, float height)
{
    //const int faceNum = 10;

    glPushMatrix();
    glRotatef(0,1,0,angleDeg);
    glTranslatef(pos[0],pos[1], pos[2]);
    glNormal3f(0,0,1);

    // store the sizes of our faces
//     GLfloat deltaX = szX/faceNum;
//     GLfloat deltaY = szY/faceNum;
    
    GLfloat x=0.0;
    GLfloat y=0.0;

    glBegin(GL_QUADS);
    glTexCoord2f(10.0, 10.0);
    glVertex3f(x,y,height);
    glTexCoord2f(0.0, 10.0);
    glVertex3f(x,y+szY,height);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3f(x+szX,y+szY,height);
    glTexCoord2f(10.0, 0.0);
    glVertex3f(x+szX,y,height);
    glEnd();
    glPopMatrix();
}

//====================================================================================

// void GLServer::DrawGroundRectangle(Vector3f pos, float szX, float szY,
//                                    float angleDeg, float height)
// {
//     const int faceNum = 2;

//     glPushMatrix();
//     glRotatef(0,1,0,angleDeg);
//     glTranslatef(pos[0],pos[1], pos[2]);
//     glNormal3f(0,0,1);

//     // store the sizes of our faces
//     GLfloat deltaX = szX/faceNum;
//     GLfloat deltaY = szY/faceNum;

//     GLfloat x,y;


//     glDisable(GL_LIGHTING);
    
        
//     for (int i=0; i<faceNum; i++)
//         {
//             y = i*deltaY;

//             //draw face as Quadric strip
//             glBegin(GL_QUAD_STRIP);
//             for (int j=0; j<faceNum; j++)
//                 {
//                     x = j*deltaX;
//                     glTexCoord2f(0.0, 0.0); glVertex3f(x,y,height);
//                     glTexCoord2f(0.0, 1.0); glVertex3f(x,y+deltaY,height);
//                 }
//             glTexCoord2f(1.0, 0.0); glVertex3f(x+deltaX,y,height);
//             glTexCoord2f(1.0, 1.0); glVertex3f(x+deltaX,y+deltaY,height);
//             glEnd();
//         }

//     glPopMatrix();
// }

//====================================================================================
void
GLServer::DrawCircle(const salt::Vector3f& pos, float radius,
                     float /*start_angle*/, float /*end_angle*/)
{
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    int num_lines = 60;

    for(int i =0;i<num_lines;i++)
    {
        double angle = i*2*M_PI/num_lines;
        glVertex3f(pos[0] + radius*cos(angle), pos[1] + radius*sin(angle), pos[2]);
    }
    glEnd();
    glPopMatrix();
}
void
GLServer::DrawArbitraryLine(const salt::Vector3f& startPos, const salt::Vector3f& endPos)
{
    glPushMatrix();
    glBegin(GL_LINES);

    glVertex3f(startPos[0],startPos[1],startPos[2]);
    glVertex3f(endPos[0], endPos[1],endPos[2]);

    glEnd();
    glPopMatrix();
}

//------------------------drawWireBox-------------------------------------
//
// draws a wireframbox with given dimensions to position 'boxPos'
//-----------------------------------------------------------------------
void GLServer::DrawWireBox(Vector3f boxPos, Vector3f sz)
{
    glPushMatrix();
    glTranslatef(boxPos[0],boxPos[1], boxPos[2]);

    // ground plane
    glBegin(GL_LINE_LOOP);
    glVertex3f(0    ,0,0);
    glVertex3f(sz[0],0,0);
    glVertex3f(sz[0],0,sz[2]);
    glVertex3f(0    ,0,sz[2]);
    glEnd();

    // top plane
    glBegin(GL_LINE_LOOP);
    glVertex3f(0    ,sz[1],0);
    glVertex3f(sz[0],sz[1],0);
    glVertex3f(sz[0],sz[1],sz[2]);
    glVertex3f(0    ,sz[1],sz[2]);
    glEnd();

    // sides
    glBegin(GL_LINES);
    glVertex3f(0    ,0,0);
    glVertex3f(0    ,sz[1],0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(sz[0],0,0);
    glVertex3f(sz[0],sz[1],0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(sz[0],0,sz[2]);
    glVertex3f(sz[0],sz[1],sz[2]);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0    ,0,sz[2]);
    glVertex3f(0    ,sz[1],sz[2]);
    glEnd();

    glPopMatrix();
}

//------------------------drawGoal-------------------------------------
//
// draws a goal with given dimensions to position 'goalPos'
//-----------------------------------------------------------------------
void GLServer::DrawGoal(Vector3f goalPos, Vector3f sz, float barRadius)
{
    GLUquadricObj *cyl;
    cyl = gluNewQuadric();

    glPushMatrix();
    glTranslatef(goalPos[0]-gSign(goalPos[0])*barRadius, goalPos[1], goalPos[2]);


    // draw goal sides as cylinders
    glPushMatrix();
    glTranslatef(0, 0, sz[2] + barRadius);
    glRotatef(-90.0f, 1,0,0);
    gluCylinder(cyl,barRadius,barRadius,sz[1],15,15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, sz[1] + barRadius, 0);
    gluCylinder(cyl,barRadius,barRadius,sz[2],15,15);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -barRadius, 0);
    gluCylinder(cyl,barRadius,barRadius,sz[2],15,15);
    glPopMatrix();

    glPopMatrix();

    gluDeleteQuadric(cyl);
}

//------------------------drawSphere-------------------------------------
//
// draws a solid sphere with given radius to position 'spherePos'
//-----------------------------------------------------------------------
void GLServer::DrawSphere(Vector3f spherePos,float radius, int res, bool wireframe)
{
    glLineWidth(2);

    glPushMatrix();
    glTranslatef(spherePos[0],spherePos[1], spherePos[2]);
    //if(!mWireframe) 
    if(!wireframe){
        glutSolidSphere(radius, res, res); // WAS res
    }else{ 
        glutWireSphere(radius, 8, 8); //WAS res
    }
    glPopMatrix();

    glLineWidth(1);
}


//-----------------------DrawShadowOfSphere------------------------------
//
// draws the shadow of a sphere with given radius by scaling its size onto
// the ground (no y component)
//-----------------------------------------------------------------------
void GLServer::DrawShadowOfSphere(Vector3f spherePos,float radius)
{
    // distance between ground and shadows
    const float delta = 0.001f;

    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);

    glTranslatef(spherePos[0], spherePos[1],delta);
    glScalef(1.0f, 1.0f, 0.0f);
    glutSolidSphere(radius, 10, 10);

    glPopMatrix();
}


//-----------------------------------------------------------------------------
// work in n progress
//-----------------------------------------------------------------------------
// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
// if mesa ever gets glaux, let me know.
bool
GLServer::BMPImageLoad(const char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // used to convert bgr to rgb color.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
	printf("File Not Found : %s\n",filename);
        printf("\nIf you don't have the texture files, please download them at\n");
        printf("http://www.uni-koblenz.de/~murray/robocup/rcssmonitor-lite-textures.tgz\n");
        printf("and unpack them to your $HOME/.rcssserver3d directory.\n\n");
        
	return 0;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
	printf("Error reading width from %s.\n", filename);
	return 0;
    }
        
    // read the height 
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
	printf("Error reading height from %s.\n", filename);
	return 0;
    }
        
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
	printf("Error reading planes from %s.\n", filename);
	return 0;
    }
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
	printf("Error reading bpp from %s.\n", filename);
	return 0;
    }
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }
	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data. 
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;	
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }

    // we're done.
    return 1;
}

// Load Bitmaps And Convert To Textures
GLvoid 
GLServer::InitTexture(const string &tFile)
{
    // Load Texture
    Image *image1;

    string pkgname("." PACKAGE_NAME);
    string path = string(getenv("HOME")) + "/" + pkgname + "/"; 
    
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!BMPImageLoad((path + string("grass.bmp")).c_str(), image1)) {
        
        exit(1);
    }        

	// Load skymap
    Image *skytex[6];
    
    // allocate space for texture
    for(int i = 0; i < 6 ; i++)
	{
		skytex[i] = (Image *) malloc(sizeof(Image));
    	if (skytex[i] == NULL) {
		printf("Error allocating space for image");
		exit(0);
    	}
	}

    if (!BMPImageLoad((path + string("bottom.bmp")).c_str(), skytex[0]) ||
	!BMPImageLoad((path + string("top.bmp")).c_str(), skytex[1])    ||
	!BMPImageLoad((path + string("left.bmp")).c_str(), skytex[2])   ||
	!BMPImageLoad((path + string("right.bmp")).c_str(), skytex[3])  ||
	!BMPImageLoad((path + string("front.bmp")).c_str(), skytex[4])  ||
	!BMPImageLoad((path + string("back.bmp")).c_str(), skytex[5])) 
    {
       	exit(1);
    } 
       
    // Create Textures	
    glGenTextures(7, &mTexNames[0]);

    // texture 1 (poor quality scaling)
    glBindTexture(GL_TEXTURE_2D, mTexNames[0]);   // 2d texture (x and y size)

    // cheap scaling when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
    // cheap scaling when image smalled than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data); 

	for(int i = 0; i < 6; i++)
	{
            // texture 2 (mipmapped scaling)
            glBindTexture(GL_TEXTURE_2D, mTexNames[i+1]);   // 2d texture (x and y size)
            // scale linearly when image bigger than texture
            //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
            //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); 
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE); 
            //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            // scale linearly + mipmap when image smalled than texture
            glTexImage2D(GL_TEXTURE_2D, 0, 3, skytex[i]->sizeX, skytex[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skytex[i]->data);

            // 2d texture, 3 colors, width, height, RGB in that order, byte data, and the data.
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skytex[i]->sizeX, skytex[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, skytex[i]->data); 
	}
}

void 
GLServer::DrawSkyBackground(float x, float y, float z, float width, float height, float length)
{
	x = x - width  / 2;  
	y = y - height / 2;
	z = z - length / 2;

	glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glBindTexture(GL_TEXTURE_2D, mTexNames[1]);
	glBegin(GL_TRIANGLE_STRIP);                                   
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y + height, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
	glEnd();

	glBindTexture(GL_TEXTURE_2D, mTexNames[2]);
	glBegin(GL_TRIANGLE_STRIP);                   
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y,z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y + height, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);        
	glEnd();

	glBindTexture(GL_TEXTURE_2D, mTexNames[3]);
	glBegin(GL_TRIANGLE_STRIP);                           
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y,       z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,z + length);  
	glEnd();

	glBindTexture(GL_TEXTURE_2D, mTexNames[4]);
	glBegin(GL_TRIANGLE_STRIP);                                   
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y + height,z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);        
	glEnd();


	glBindTexture(GL_TEXTURE_2D, mTexNames[5]);
	glBegin(GL_TRIANGLE_STRIP);                           
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length); 
	glEnd();

	glBindTexture(GL_TEXTURE_2D, mTexNames[6]);
	glBegin(GL_TRIANGLE_STRIP);                           
		glTexCoord2f(1.0f, -1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(0.0f, -1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
	glEnd();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, mTexNames[0]);
}
