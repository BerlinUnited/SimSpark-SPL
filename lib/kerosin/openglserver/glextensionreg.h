#ifndef GLEXTENSIONREG_H__
#define GLEXTENSIONREG_H__

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>

#if defined(_WIN32)
#include <GL/wglext.h>
#else
#include <GL/glx.h>
#include <GL/glxext.h>
#endif
struct GLExtGenFunctionPointers
{
#ifdef GL_ARB_multisample
	PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB;
#endif //GL_ARB_multisample

#ifdef GL_ARB_multitexture
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
	PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB;
	PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB;
	PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
	PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB;
	PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB;
	PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB;
	PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB;
	PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB;
	PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB;
	PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB;
	PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
	PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
	PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB;
	PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB;
	PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB;
	PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB;
	PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB;
	PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB;
	PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
	PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB;
	PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB;
	PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB;
	PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB;
	PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB;
	PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB;
	PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB;
	PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
	PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB;
	PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB;
	PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB;
	PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB;
	PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB;
#endif //GL_ARB_multitexture

#ifdef GL_ARB_point_parameters
	PFNGLPOINTPARAMETERFARBPROC glPointParameterfARB;
	PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB;
#endif //GL_ARB_point_parameters

#ifdef GL_ARB_texture_compression
	PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3DARB;
	PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
	PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1DARB;
	PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImageARB;
#endif //GL_ARB_texture_compression

#ifdef GL_ARB_transpose_matrix
	PFNGLLOADTRANSPOSEMATRIXFARBPROC glLoadTransposeMatrixfARB;
	PFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixdARB;
	PFNGLMULTTRANSPOSEMATRIXFARBPROC glMultTransposeMatrixfARB;
	PFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixdARB;
#endif //GL_ARB_transpose_matrix

#ifdef GL_ARB_vertex_buffer_object
	PFNGLBINDBUFFERARBPROC glBindBufferARB;
	PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
	PFNGLGENBUFFERSARBPROC glGenBuffersARB;
	PFNGLISBUFFERARBPROC glIsBufferARB;
	PFNGLBUFFERDATAARBPROC glBufferDataARB;
	PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
	PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
	PFNGLMAPBUFFERARBPROC glMapBufferARB;
	PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
	PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
	PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;
#endif //GL_ARB_vertex_buffer_object

#ifdef GL_ARB_vertex_program
	PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
	PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
	PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
	PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
	PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
	PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
	PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
	PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
	PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
	PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
	PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
	PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
	PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
	PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
	PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
	PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
	PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
	PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
	PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
	PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
	PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
	PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
	PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
	PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
	PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
	PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
	PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
	PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
	PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
	PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
	PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
	PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
	PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
	PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
	PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
	PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
	PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
	PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
	PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
	PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
	PFNGLBINDPROGRAMARBPROC glBindProgramARB;
	PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
	PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
	PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
	PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
	PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
	PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
	PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
	PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
	PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
	PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
	PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
	PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
	PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
	PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
	PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
	PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
	PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
	PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
	PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
	PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
	PFNGLISPROGRAMARBPROC glIsProgramARB;
#endif //GL_ARB_vertex_program

#ifdef GL_ARB_window_pos
	PFNGLWINDOWPOS2DARBPROC glWindowPos2dARB;
	PFNGLWINDOWPOS2FARBPROC glWindowPos2fARB;
	PFNGLWINDOWPOS2IARBPROC glWindowPos2iARB;
	PFNGLWINDOWPOS2SARBPROC glWindowPos2sARB;
	PFNGLWINDOWPOS2DVARBPROC glWindowPos2dvARB;
	PFNGLWINDOWPOS2FVARBPROC glWindowPos2fvARB;
	PFNGLWINDOWPOS2IVARBPROC glWindowPos2ivARB;
	PFNGLWINDOWPOS2SVARBPROC glWindowPos2svARB;
	PFNGLWINDOWPOS3DARBPROC glWindowPos3dARB;
	PFNGLWINDOWPOS3FARBPROC glWindowPos3fARB;
	PFNGLWINDOWPOS3IARBPROC glWindowPos3iARB;
	PFNGLWINDOWPOS3SARBPROC glWindowPos3sARB;
	PFNGLWINDOWPOS3DVARBPROC glWindowPos3dvARB;
	PFNGLWINDOWPOS3FVARBPROC glWindowPos3fvARB;
	PFNGLWINDOWPOS3IVARBPROC glWindowPos3ivARB;
	PFNGLWINDOWPOS3SVARBPROC glWindowPos3svARB;
#endif //GL_ARB_window_pos

#ifdef GL_EXT_blend_color
	PFNGLBLENDCOLOREXTPROC glBlendColorEXT;
#endif //GL_EXT_blend_color

#ifdef GL_EXT_blend_func_separate
	PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT;
#endif //GL_EXT_blend_func_separate

#ifdef GL_EXT_blend_minmax
	PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;
#endif //GL_EXT_blend_minmax

#ifdef GL_EXT_compiled_vertex_array
	PFNGLLOCKARRAYSEXTPROC glLockArraysEXT;
	PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT;
#endif //GL_EXT_compiled_vertex_array

#ifdef GL_EXT_draw_range_elements
	PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT;
#endif //GL_EXT_draw_range_elements

#ifdef GL_EXT_fog_coord
	PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
	PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT;
	PFNGLFOGCOORDDEXTPROC glFogCoorddEXT;
	PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT;
	PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT;
#endif //GL_EXT_fog_coord

#ifdef GL_EXT_multi_draw_arrays
	PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT;
	PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT;
#endif //GL_EXT_multi_draw_arrays

#ifdef GL_EXT_paletted_texture
	PFNGLCOLORTABLEEXTPROC glColorTableEXT;
	PFNGLCOLORSUBTABLEEXTPROC glColorSubTableEXT;
	PFNGLGETCOLORTABLEEXTPROC glGetColorTableEXT;
	PFNGLGETCOLORTABLEPARAMETERIVEXTPROC glGetColorTableParameterivEXT;
	PFNGLGETCOLORTABLEPARAMETERFVEXTPROC glGetColorTableParameterfvEXT;
#endif //GL_EXT_paletted_texture

#ifdef GL_EXT_point_parameters
	PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT;
	PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT;
	PFNGLPOINTPARAMETERFSGISPROC glPointParameterfSGIS;
	PFNGLPOINTPARAMETERFVSGISPROC glPointParameterfvSGIS;
#endif //GL_EXT_point_parameters

#ifdef GL_EXT_secondary_color
	PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT;
	PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT;
	PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT;
	PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT;
	PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT;
	PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT;
	PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT;
	PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT;
	PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT;
	PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT;
	PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT;
	PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT;
	PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT;
	PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT;
	PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT;
	PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT;
	PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;
#endif //GL_EXT_secondary_color

#ifdef GL_EXT_stencil_two_side
	PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;
#endif //GL_EXT_stencil_two_side

#ifdef GL_EXT_texture3D
	PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
	PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT;
#endif //GL_EXT_texture3D

#ifdef GL_EXT_texture_object
	PFNGLARETEXTURESRESIDENTEXTPROC glAreTexturesResidentEXT;
	PFNGLBINDTEXTUREEXTPROC glBindTextureEXT;
	PFNGLDELETETEXTURESEXTPROC glDeleteTexturesEXT;
	PFNGLGENTEXTURESEXTPROC glGenTexturesEXT;
	PFNGLISTEXTUREEXTPROC glIsTextureEXT;
	PFNGLPRIORITIZETEXTURESEXTPROC glPrioritizeTexturesEXT;
#endif //GL_EXT_texture_object

#ifdef GL_EXT_vertex_array
	PFNGLARRAYELEMENTEXTPROC glArrayElementEXT;
	PFNGLCOLORPOINTEREXTPROC glColorPointerEXT;
	PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT;
	PFNGLEDGEFLAGPOINTEREXTPROC glEdgeFlagPointerEXT;
	PFNGLGETPOINTERVEXTPROC glGetPointervEXT;
	PFNGLINDEXPOINTEREXTPROC glIndexPointerEXT;
	PFNGLNORMALPOINTEREXTPROC glNormalPointerEXT;
	PFNGLTEXCOORDPOINTEREXTPROC glTexCoordPointerEXT;
	PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT;
#endif //GL_EXT_vertex_array

#ifdef GL_EXT_vertex_weighting
	PFNGLVERTEXWEIGHTFEXTPROC glVertexWeightfEXT;
	PFNGLVERTEXWEIGHTFVEXTPROC glVertexWeightfvEXT;
	PFNGLVERTEXWEIGHTPOINTEREXTPROC glVertexWeightPointerEXT;
#endif //GL_EXT_vertex_weighting

#ifdef GL_NV_element_array
	PFNGLELEMENTPOINTERNVPROC glElementPointerNV;
	PFNGLDRAWELEMENTARRAYNVPROC glDrawElementArrayNV;
	PFNGLDRAWRANGEELEMENTARRAYNVPROC glDrawRangeElementArrayNV;
	PFNGLMULTIDRAWELEMENTARRAYNVPROC glMultiDrawElementArrayNV;
	PFNGLMULTIDRAWRANGEELEMENTARRAYNVPROC glMultiDrawRangeElementArrayNV;
#endif //GL_NV_element_array

#ifdef GL_NV_fence
	PFNGLDELETEFENCESNVPROC glDeleteFencesNV;
	PFNGLGENFENCESNVPROC glGenFencesNV;
	PFNGLISFENCENVPROC glIsFenceNV;
	PFNGLTESTFENCENVPROC glTestFenceNV;
	PFNGLGETFENCEIVNVPROC glGetFenceivNV;
	PFNGLFINISHFENCENVPROC glFinishFenceNV;
	PFNGLSETFENCENVPROC glSetFenceNV;
#endif //GL_NV_fence

#ifdef GL_NV_fragment_program
	PFNGLPROGRAMNAMEDPARAMETER4FNVPROC glProgramNamedParameter4fNV;
	PFNGLPROGRAMNAMEDPARAMETER4DNVPROC glProgramNamedParameter4dNV;
	PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC glProgramNamedParameter4fvNV;
	PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC glProgramNamedParameter4dvNV;
	PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC glGetProgramNamedParameterfvNV;
	PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC glGetProgramNamedParameterdvNV;
#endif //GL_NV_fragment_program

#ifdef GL_NV_half_float
	PFNGLVERTEX2HNVPROC glVertex2hNV;
	PFNGLVERTEX2HVNVPROC glVertex2hvNV;
	PFNGLVERTEX3HNVPROC glVertex3hNV;
	PFNGLVERTEX3HVNVPROC glVertex3hvNV;
	PFNGLVERTEX4HNVPROC glVertex4hNV;
	PFNGLVERTEX4HVNVPROC glVertex4hvNV;
	PFNGLNORMAL3HNVPROC glNormal3hNV;
	PFNGLNORMAL3HVNVPROC glNormal3hvNV;
	PFNGLCOLOR3HNVPROC glColor3hNV;
	PFNGLCOLOR3HVNVPROC glColor3hvNV;
	PFNGLCOLOR4HNVPROC glColor4hNV;
	PFNGLCOLOR4HVNVPROC glColor4hvNV;
	PFNGLTEXCOORD1HNVPROC glTexCoord1hNV;
	PFNGLTEXCOORD1HVNVPROC glTexCoord1hvNV;
	PFNGLTEXCOORD2HNVPROC glTexCoord2hNV;
	PFNGLTEXCOORD2HVNVPROC glTexCoord2hvNV;
	PFNGLTEXCOORD3HNVPROC glTexCoord3hNV;
	PFNGLTEXCOORD3HVNVPROC glTexCoord3hvNV;
	PFNGLTEXCOORD4HNVPROC glTexCoord4hNV;
	PFNGLTEXCOORD4HVNVPROC glTexCoord4hvNV;
	PFNGLMULTITEXCOORD1HNVPROC glMultiTexCoord1hNV;
	PFNGLMULTITEXCOORD1HVNVPROC glMultiTexCoord1hvNV;
	PFNGLMULTITEXCOORD2HNVPROC glMultiTexCoord2hNV;
	PFNGLMULTITEXCOORD2HVNVPROC glMultiTexCoord2hvNV;
	PFNGLMULTITEXCOORD3HNVPROC glMultiTexCoord3hNV;
	PFNGLMULTITEXCOORD3HVNVPROC glMultiTexCoord3hvNV;
	PFNGLMULTITEXCOORD4HNVPROC glMultiTexCoord4hNV;
	PFNGLMULTITEXCOORD4HVNVPROC glMultiTexCoord4hvNV;
	PFNGLFOGCOORDHNVPROC glFogCoordhNV;
	PFNGLFOGCOORDHVNVPROC glFogCoordhvNV;
	PFNGLSECONDARYCOLOR3HNVPROC glSecondaryColor3hNV;
	PFNGLSECONDARYCOLOR3HVNVPROC glSecondaryColor3hvNV;
	PFNGLVERTEXWEIGHTHNVPROC glVertexWeighthNV;
	PFNGLVERTEXWEIGHTHVNVPROC glVertexWeighthvNV;
	PFNGLVERTEXATTRIB1HNVPROC glVertexAttrib1hNV;
	PFNGLVERTEXATTRIB1HVNVPROC glVertexAttrib1hvNV;
	PFNGLVERTEXATTRIB2HNVPROC glVertexAttrib2hNV;
	PFNGLVERTEXATTRIB2HVNVPROC glVertexAttrib2hvNV;
	PFNGLVERTEXATTRIB3HNVPROC glVertexAttrib3hNV;
	PFNGLVERTEXATTRIB3HVNVPROC glVertexAttrib3hvNV;
	PFNGLVERTEXATTRIB4HNVPROC glVertexAttrib4hNV;
	PFNGLVERTEXATTRIB4HVNVPROC glVertexAttrib4hvNV;
	PFNGLVERTEXATTRIBS1HVNVPROC glVertexAttribs1hvNV;
	PFNGLVERTEXATTRIBS2HVNVPROC glVertexAttribs2hvNV;
	PFNGLVERTEXATTRIBS3HVNVPROC glVertexAttribs3hvNV;
	PFNGLVERTEXATTRIBS4HVNVPROC glVertexAttribs4hvNV;
#endif //GL_NV_half_float

#ifdef GL_NV_occlusion_query
	PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueriesNV;
	PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueriesNV;
	PFNGLISOCCLUSIONQUERYNVPROC glIsOcclusionQueryNV;
	PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQueryNV;
	PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQueryNV;
	PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryivNV;
	PFNGLGETOCCLUSIONQUERYUIVNVPROC glGetOcclusionQueryuivNV;
#endif //GL_NV_occlusion_query

#ifdef GL_NV_pixel_data_range
	PFNGLPIXELDATARANGENVPROC glPixelDataRangeNV;
	PFNGLFLUSHPIXELDATARANGENVPROC glFlushPixelDataRangeNV;
#endif //GL_NV_pixel_data_range

#ifdef GL_NV_point_sprite
	PFNGLPOINTPARAMETERINVPROC glPointParameteriNV;
	PFNGLPOINTPARAMETERIVNVPROC glPointParameterivNV;
#endif //GL_NV_point_sprite

#ifdef GL_NV_primitive_restart
	PFNGLPRIMITIVERESTARTNVPROC glPrimitiveRestartNV;
	PFNGLPRIMITIVERESTARTINDEXNVPROC glPrimitiveRestartIndexNV;
#endif //GL_NV_primitive_restart

#ifdef GL_NV_register_combiners
	PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;
	PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV;
	PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV;
	PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV;
	PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
	PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
	PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
	PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV;
	PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV;
	PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV;
	PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV;
	PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV;
	PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV;
#endif //GL_NV_register_combiners

#ifdef GL_NV_register_combiners2
	PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV;
	PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC glGetCombinerStageParameterfvNV;
#endif //GL_NV_register_combiners2

#ifdef GL_NV_stencil_two_side
	PFNGLACTIVESTENCILFACENVPROC glActiveStencilFaceNV;
#endif //GL_NV_stencil_two_side

#ifdef GL_NV_vertex_array_range
	PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV;
	PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV;
#endif //GL_NV_vertex_array_range

#ifdef GL_NV_vertex_program
	PFNGLBINDPROGRAMNVPROC glBindProgramNV;
	PFNGLDELETEPROGRAMSNVPROC glDeleteProgramsNV;
	PFNGLEXECUTEPROGRAMNVPROC glExecuteProgramNV;
	PFNGLGENPROGRAMSNVPROC glGenProgramsNV;
	PFNGLAREPROGRAMSRESIDENTNVPROC glAreProgramsResidentNV;
	PFNGLREQUESTRESIDENTPROGRAMSNVPROC glRequestResidentProgramsNV;
	PFNGLGETPROGRAMPARAMETERFVNVPROC glGetProgramParameterfvNV;
	PFNGLGETPROGRAMPARAMETERDVNVPROC glGetProgramParameterdvNV;
	PFNGLGETPROGRAMIVNVPROC glGetProgramivNV;
	PFNGLGETPROGRAMSTRINGNVPROC glGetProgramStringNV;
	PFNGLGETTRACKMATRIXIVNVPROC glGetTrackMatrixivNV;
	PFNGLGETVERTEXATTRIBDVNVPROC glGetVertexAttribdvNV;
	PFNGLGETVERTEXATTRIBFVNVPROC glGetVertexAttribfvNV;
	PFNGLGETVERTEXATTRIBIVNVPROC glGetVertexAttribivNV;
	PFNGLGETVERTEXATTRIBPOINTERVNVPROC glGetVertexAttribPointervNV;
	PFNGLISPROGRAMNVPROC glIsProgramNV;
	PFNGLLOADPROGRAMNVPROC glLoadProgramNV;
	PFNGLPROGRAMPARAMETER4FNVPROC glProgramParameter4fNV;
	PFNGLPROGRAMPARAMETER4DNVPROC glProgramParameter4dNV;
	PFNGLPROGRAMPARAMETER4DVNVPROC glProgramParameter4dvNV;
	PFNGLPROGRAMPARAMETER4FVNVPROC glProgramParameter4fvNV;
	PFNGLPROGRAMPARAMETERS4DVNVPROC glProgramParameters4dvNV;
	PFNGLPROGRAMPARAMETERS4FVNVPROC glProgramParameters4fvNV;
	PFNGLTRACKMATRIXNVPROC glTrackMatrixNV;
	PFNGLVERTEXATTRIBPOINTERNVPROC glVertexAttribPointerNV;
	PFNGLVERTEXATTRIB1SNVPROC glVertexAttrib1sNV;
	PFNGLVERTEXATTRIB1FNVPROC glVertexAttrib1fNV;
	PFNGLVERTEXATTRIB1DNVPROC glVertexAttrib1dNV;
	PFNGLVERTEXATTRIB2SNVPROC glVertexAttrib2sNV;
	PFNGLVERTEXATTRIB2FNVPROC glVertexAttrib2fNV;
	PFNGLVERTEXATTRIB2DNVPROC glVertexAttrib2dNV;
	PFNGLVERTEXATTRIB3SNVPROC glVertexAttrib3sNV;
	PFNGLVERTEXATTRIB3FNVPROC glVertexAttrib3fNV;
	PFNGLVERTEXATTRIB3DNVPROC glVertexAttrib3dNV;
	PFNGLVERTEXATTRIB4SNVPROC glVertexAttrib4sNV;
	PFNGLVERTEXATTRIB4FNVPROC glVertexAttrib4fNV;
	PFNGLVERTEXATTRIB4DNVPROC glVertexAttrib4dNV;
	PFNGLVERTEXATTRIB4UBNVPROC glVertexAttrib4ubNV;
	PFNGLVERTEXATTRIB1SVNVPROC glVertexAttrib1svNV;
	PFNGLVERTEXATTRIB1FVNVPROC glVertexAttrib1fvNV;
	PFNGLVERTEXATTRIB1DVNVPROC glVertexAttrib1dvNV;
	PFNGLVERTEXATTRIB2SVNVPROC glVertexAttrib2svNV;
	PFNGLVERTEXATTRIB2FVNVPROC glVertexAttrib2fvNV;
	PFNGLVERTEXATTRIB2DVNVPROC glVertexAttrib2dvNV;
	PFNGLVERTEXATTRIB3SVNVPROC glVertexAttrib3svNV;
	PFNGLVERTEXATTRIB3FVNVPROC glVertexAttrib3fvNV;
	PFNGLVERTEXATTRIB3DVNVPROC glVertexAttrib3dvNV;
	PFNGLVERTEXATTRIB4SVNVPROC glVertexAttrib4svNV;
	PFNGLVERTEXATTRIB4FVNVPROC glVertexAttrib4fvNV;
	PFNGLVERTEXATTRIB4DVNVPROC glVertexAttrib4dvNV;
	PFNGLVERTEXATTRIB4UBVNVPROC glVertexAttrib4ubvNV;
	PFNGLVERTEXATTRIBS1SVNVPROC glVertexAttribs1svNV;
	PFNGLVERTEXATTRIBS1FVNVPROC glVertexAttribs1fvNV;
	PFNGLVERTEXATTRIBS1DVNVPROC glVertexAttribs1dvNV;
	PFNGLVERTEXATTRIBS2SVNVPROC glVertexAttribs2svNV;
	PFNGLVERTEXATTRIBS2FVNVPROC glVertexAttribs2fvNV;
	PFNGLVERTEXATTRIBS2DVNVPROC glVertexAttribs2dvNV;
	PFNGLVERTEXATTRIBS3SVNVPROC glVertexAttribs3svNV;
	PFNGLVERTEXATTRIBS3FVNVPROC glVertexAttribs3fvNV;
	PFNGLVERTEXATTRIBS3DVNVPROC glVertexAttribs3dvNV;
	PFNGLVERTEXATTRIBS4SVNVPROC glVertexAttribs4svNV;
	PFNGLVERTEXATTRIBS4FVNVPROC glVertexAttribs4fvNV;
	PFNGLVERTEXATTRIBS4DVNVPROC glVertexAttribs4dvNV;
	PFNGLVERTEXATTRIBS4UBVNVPROC glVertexAttribs4ubvNV;
#endif //GL_NV_vertex_program

#ifdef GL_VERSION_1_2
	PFNGLBLENDCOLORPROC glBlendColor;
	PFNGLBLENDEQUATIONPROC glBlendEquation;
	PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
	PFNGLCOLORTABLEPROC glColorTable;
	PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
	PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
	PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
	PFNGLGETCOLORTABLEPROC glGetColorTable;
	PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
	PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
	PFNGLCOLORSUBTABLEPROC glColorSubTable;
	PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
	PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
	PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
	PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
	PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
	PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
	PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
	PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
	PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
	PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
	PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
	PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
	PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
	PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
	PFNGLGETHISTOGRAMPROC glGetHistogram;
	PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
	PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
	PFNGLGETMINMAXPROC glGetMinmax;
	PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
	PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
	PFNGLHISTOGRAMPROC glHistogram;
	PFNGLMINMAXPROC glMinmax;
	PFNGLRESETHISTOGRAMPROC glResetHistogram;
	PFNGLRESETMINMAXPROC glResetMinmax;
	PFNGLTEXIMAGE3DPROC glTexImage3D;
	PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
	PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
	PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d;
	PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv;
	PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f;
	PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv;
	PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i;
	PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv;
	PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s;
	PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv;
	PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d;
	PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
	PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
	PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;
	PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i;
	PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv;
	PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s;
	PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv;
	PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d;
	PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv;
	PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f;
	PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv;
	PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i;
	PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv;
	PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s;
	PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv;
	PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d;
	PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv;
	PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f;
	PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv;
	PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i;
	PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv;
	PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s;
	PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv;
#endif //GL_VERSION_1_2

#ifdef GL_VERSION_1_3
	PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
	PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
	PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
	PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
	PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
	PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
	PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
	PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
	PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
	PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
	PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
	PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
#endif //GL_VERSION_1_3

#ifdef GL_VERSION_1_4
	PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
	PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
	PFNGLPOINTPARAMETERFPROC glPointParameterf;
	PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
	PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
	PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv;
	PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
	PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv;
	PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
	PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv;
	PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
	PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv;
	PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
	PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv;
	PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
	PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
	PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
	PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
	PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
	PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
	PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
	PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
	PFNGLWINDOWPOS2DPROC glWindowPos2d;
	PFNGLWINDOWPOS2FPROC glWindowPos2f;
	PFNGLWINDOWPOS2IPROC glWindowPos2i;
	PFNGLWINDOWPOS2SPROC glWindowPos2s;
	PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
	PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
	PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
	PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
	PFNGLWINDOWPOS3DPROC glWindowPos3d;
	PFNGLWINDOWPOS3FPROC glWindowPos3f;
	PFNGLWINDOWPOS3IPROC glWindowPos3i;
	PFNGLWINDOWPOS3SPROC glWindowPos3s;
	PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
	PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
	PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
	PFNGLWINDOWPOS3SVPROC glWindowPos3sv;
#endif //GL_VERSION_1_4

	#if defined(_WIN32)
	#endif //_WIN32
};
struct GLExtGenExtensions
{
	bool mGL_APPLE_specular_vector;
	bool mGL_APPLE_transform_hint;
	bool mGL_ARB_depth_texture;
	bool mGL_ARB_fragment_program;
	bool mGL_ARB_multisample;
	bool mGL_ARB_multitexture;
	bool mGL_ARB_point_parameters;
	bool mGL_ARB_shadow;
	bool mGL_ARB_texture_border_clamp;
	bool mGL_ARB_texture_compression;
	bool mGL_ARB_texture_cube_map;
	bool mGL_ARB_texture_env_add;
	bool mGL_ARB_texture_env_combine;
	bool mGL_ARB_texture_env_dot3;
	bool mGL_ARB_texture_mirrored_repeat;
	bool mGL_ARB_transpose_matrix;
	bool mGL_ARB_vertex_buffer_object;
	bool mGL_ARB_vertex_program;
	bool mGL_ARB_window_pos;
	bool mGL_EXT_422_pixels;
	bool mGL_EXT_abgr;
	bool mGL_EXT_bgra;
	bool mGL_EXT_blend_color;
	bool mGL_EXT_blend_func_separate;
	bool mGL_EXT_blend_logic_op;
	bool mGL_EXT_blend_minmax;
	bool mGL_EXT_blend_subtract;
	bool mGL_EXT_clip_volume_hint;
	bool mGL_EXT_cmyka;
	bool mGL_EXT_color_subtable;
	bool mGL_EXT_compiled_vertex_array;
	bool mGL_EXT_convolution;
	bool mGL_EXT_coordinate_frame;
	bool mGL_EXT_copy_texture;
	bool mGL_EXT_cull_vertex;
	bool mGL_EXT_draw_range_elements;
	bool mGL_EXT_fog_coord;
	bool mGL_EXT_histogram;
	bool mGL_EXT_index_array_formats;
	bool mGL_EXT_index_func;
	bool mGL_EXT_index_material;
	bool mGL_EXT_index_texture;
	bool mGL_EXT_light_texture;
	bool mGL_EXT_misc_attribute;
	bool mGL_EXT_multi_draw_arrays;
	bool mGL_EXT_packed_pixels;
	bool mGL_EXT_paletted_texture;
	bool mGL_EXT_pixel_transform;
	bool mGL_EXT_pixel_transform_color_table;
	bool mGL_EXT_point_parameters;
	bool mGL_EXT_polygon_offset;
	bool mGL_EXT_rescale_normal;
	bool mGL_EXT_secondary_color;
	bool mGL_EXT_separate_specular_color;
	bool mGL_EXT_shadow_funcs;
	bool mGL_EXT_shared_texture_palette;
	bool mGL_EXT_stencil_two_side;
	bool mGL_EXT_stencil_wrap;
	bool mGL_EXT_subtexture;
	bool mGL_EXT_texture;
	bool mGL_EXT_texture3D;
	bool mGL_EXT_texture_compression_s3tc;
	bool mGL_EXT_texture_cube_map;
	bool mGL_EXT_texture_edge_clamp;
	bool mGL_EXT_texture_env_add;
	bool mGL_EXT_texture_env_combine;
	bool mGL_EXT_texture_env_dot3;
	bool mGL_EXT_texture_filter_anisotropic;
	bool mGL_EXT_texture_lod_bias;
	bool mGL_EXT_texture_object;
	bool mGL_EXT_texture_perturb_normal;
	bool mGL_EXT_vertex_array;
	bool mGL_EXT_vertex_weighting;
	bool mGL_HP_convolution_border_modes;
	bool mGL_HP_image_transform;
	bool mGL_HP_occlusion_test;
	bool mGL_HP_texture_lighting;
	bool mGL_IBM_cull_vertex;
	bool mGL_IBM_multimode_draw_arrays;
	bool mGL_IBM_rasterpos_clip;
	bool mGL_IBM_texture_mirrored_repeat;
	bool mGL_IBM_vertex_array_lists;
	bool mGL_INGR_color_clamp;
	bool mGL_INGR_interlace_read;
	bool mGL_INGR_palette_buffer;
	bool mGL_INTEL_parallel_arrays;
	bool mGL_INTEL_texture_scissor;
	bool mGL_MESA_resize_buffers;
	bool mGL_MESA_window_pos;
	bool mGL_NV_blend_square;
	bool mGL_NV_copy_depth_to_color;
	bool mGL_NV_depth_clamp;
	bool mGL_NV_element_array;
	bool mGL_NV_fence;
	bool mGL_NV_float_buffer;
	bool mGL_NV_fog_distance;
	bool mGL_NV_fragment_program;
	bool mGL_NV_half_float;
	bool mGL_NV_light_max_exponent;
	bool mGL_NV_multisample_filter_hint;
	bool mGL_NV_occlusion_query;
	bool mGL_NV_packed_depth_stencil;
	bool mGL_NV_pixel_data_range;
	bool mGL_NV_point_sprite;
	bool mGL_NV_primitive_restart;
	bool mGL_NV_register_combiners;
	bool mGL_NV_register_combiners2;
	bool mGL_NV_stencil_two_side;
	bool mGL_NV_texgen_emboss;
	bool mGL_NV_texgen_reflection;
	bool mGL_NV_texture_compression_vtc;
	bool mGL_NV_texture_env_combine4;
	bool mGL_NV_texture_rectangle;
	bool mGL_NV_texture_shader;
	bool mGL_NV_texture_shader2;
	bool mGL_NV_texture_shader3;
	bool mGL_NV_vertex_array_range;
	bool mGL_NV_vertex_array_range2;
	bool mGL_NV_vertex_program;
	bool mGL_NV_vertex_program1_1;
	bool mGL_NV_vertex_program2;
	bool mGL_PGI_misc_hints;
	bool mGL_PGI_vertex_hints;
	bool mGL_REND_screen_coordinates;
	bool mGL_SGIS_detail_texture;
	bool mGL_SGIS_fog_function;
	bool mGL_SGIS_generate_mipmap;
	bool mGL_SGIS_multisample;
	bool mGL_SGIS_pixel_texture;
	bool mGL_SGIS_sharpen_texture;
	bool mGL_SGIS_texture4D;
	bool mGL_SGIS_texture_border_clamp;
	bool mGL_SGIS_texture_color_mask;
	bool mGL_SGIS_texture_edge_clamp;
	bool mGL_SGIS_texture_filter4;
	bool mGL_SGIS_texture_lod;
	bool mGL_SGIS_texture_select;
	bool mGL_SGIX_blend_alpha_minmax;
	bool mGL_SGIX_clipmap;
	bool mGL_SGIX_depth_texture;
	bool mGL_SGIX_flush_raster;
	bool mGL_SGIX_fog_offset;
	bool mGL_SGIX_framezoom;
	bool mGL_SGIX_instruments;
	bool mGL_SGIX_interlace;
	bool mGL_SGIX_ir_instrument1;
	bool mGL_SGIX_list_priority;
	bool mGL_SGIX_pixel_texture;
	bool mGL_SGIX_reference_plane;
	bool mGL_SGIX_resample;
	bool mGL_SGIX_shadow;
	bool mGL_SGIX_shadow_ambient;
	bool mGL_SGIX_sprite;
	bool mGL_SGIX_tag_sample_buffer;
	bool mGL_SGIX_texture_add_env;
	bool mGL_SGIX_texture_lod_bias;
	bool mGL_SGIX_texture_multi_buffer;
	bool mGL_SGIX_texture_scale_bias;
	bool mGL_SGIX_vertex_preclip;
	bool mGL_SGIX_ycrcb;
	bool mGL_SGI_color_matrix;
	bool mGL_SGI_color_table;
	bool mGL_SGI_texture_color_table;
	bool mGL_SUNX_constant_data;
	bool mGL_SUN_convolution_border_modes;
	bool mGL_SUN_global_alpha;
	bool mGL_SUN_triangle_list;
	bool mGL_SUN_vertex;
	bool mGL_VERSION_1_2;
	bool mGL_VERSION_1_3;
	bool mGL_VERSION_1_4;
	bool mGL_WIN_phong_shading;
	bool mGL_WIN_specular_fog;
#if defined(_WIN32)
#endif //_WIN32
};

class GLExtensionReg
{
private:
#if defined(_WIN32)
	HDC	mHDC;
#endif //_WIN32
	GLExtGenExtensions	mExtensions;

public:
	bool Has_GL_APPLE_specular_vector() const { return mExtensions.mGL_APPLE_specular_vector; }
	bool Has_GL_APPLE_transform_hint() const { return mExtensions.mGL_APPLE_transform_hint; }
	bool Has_GL_ARB_depth_texture() const { return mExtensions.mGL_ARB_depth_texture; }
	bool Has_GL_ARB_fragment_program() const { return mExtensions.mGL_ARB_fragment_program; }
	bool Has_GL_ARB_multisample() const { return mExtensions.mGL_ARB_multisample; }
	bool Has_GL_ARB_multitexture() const { return mExtensions.mGL_ARB_multitexture; }
	bool Has_GL_ARB_point_parameters() const { return mExtensions.mGL_ARB_point_parameters; }
	bool Has_GL_ARB_shadow() const { return mExtensions.mGL_ARB_shadow; }
	bool Has_GL_ARB_texture_border_clamp() const { return mExtensions.mGL_ARB_texture_border_clamp; }
	bool Has_GL_ARB_texture_compression() const { return mExtensions.mGL_ARB_texture_compression; }
	bool Has_GL_ARB_texture_cube_map() const { return mExtensions.mGL_ARB_texture_cube_map; }
	bool Has_GL_ARB_texture_env_add() const { return mExtensions.mGL_ARB_texture_env_add; }
	bool Has_GL_ARB_texture_env_combine() const { return mExtensions.mGL_ARB_texture_env_combine; }
	bool Has_GL_ARB_texture_env_dot3() const { return mExtensions.mGL_ARB_texture_env_dot3; }
	bool Has_GL_ARB_texture_mirrored_repeat() const { return mExtensions.mGL_ARB_texture_mirrored_repeat; }
	bool Has_GL_ARB_transpose_matrix() const { return mExtensions.mGL_ARB_transpose_matrix; }
	bool Has_GL_ARB_vertex_buffer_object() const { return mExtensions.mGL_ARB_vertex_buffer_object; }
	bool Has_GL_ARB_vertex_program() const { return mExtensions.mGL_ARB_vertex_program; }
	bool Has_GL_ARB_window_pos() const { return mExtensions.mGL_ARB_window_pos; }
	bool Has_GL_EXT_422_pixels() const { return mExtensions.mGL_EXT_422_pixels; }
	bool Has_GL_EXT_abgr() const { return mExtensions.mGL_EXT_abgr; }
	bool Has_GL_EXT_bgra() const { return mExtensions.mGL_EXT_bgra; }
	bool Has_GL_EXT_blend_color() const { return mExtensions.mGL_EXT_blend_color; }
	bool Has_GL_EXT_blend_func_separate() const { return mExtensions.mGL_EXT_blend_func_separate; }
	bool Has_GL_EXT_blend_logic_op() const { return mExtensions.mGL_EXT_blend_logic_op; }
	bool Has_GL_EXT_blend_minmax() const { return mExtensions.mGL_EXT_blend_minmax; }
	bool Has_GL_EXT_blend_subtract() const { return mExtensions.mGL_EXT_blend_subtract; }
	bool Has_GL_EXT_clip_volume_hint() const { return mExtensions.mGL_EXT_clip_volume_hint; }
	bool Has_GL_EXT_cmyka() const { return mExtensions.mGL_EXT_cmyka; }
	bool Has_GL_EXT_color_subtable() const { return mExtensions.mGL_EXT_color_subtable; }
	bool Has_GL_EXT_compiled_vertex_array() const { return mExtensions.mGL_EXT_compiled_vertex_array; }
	bool Has_GL_EXT_convolution() const { return mExtensions.mGL_EXT_convolution; }
	bool Has_GL_EXT_coordinate_frame() const { return mExtensions.mGL_EXT_coordinate_frame; }
	bool Has_GL_EXT_copy_texture() const { return mExtensions.mGL_EXT_copy_texture; }
	bool Has_GL_EXT_cull_vertex() const { return mExtensions.mGL_EXT_cull_vertex; }
	bool Has_GL_EXT_draw_range_elements() const { return mExtensions.mGL_EXT_draw_range_elements; }
	bool Has_GL_EXT_fog_coord() const { return mExtensions.mGL_EXT_fog_coord; }
	bool Has_GL_EXT_histogram() const { return mExtensions.mGL_EXT_histogram; }
	bool Has_GL_EXT_index_array_formats() const { return mExtensions.mGL_EXT_index_array_formats; }
	bool Has_GL_EXT_index_func() const { return mExtensions.mGL_EXT_index_func; }
	bool Has_GL_EXT_index_material() const { return mExtensions.mGL_EXT_index_material; }
	bool Has_GL_EXT_index_texture() const { return mExtensions.mGL_EXT_index_texture; }
	bool Has_GL_EXT_light_texture() const { return mExtensions.mGL_EXT_light_texture; }
	bool Has_GL_EXT_misc_attribute() const { return mExtensions.mGL_EXT_misc_attribute; }
	bool Has_GL_EXT_multi_draw_arrays() const { return mExtensions.mGL_EXT_multi_draw_arrays; }
	bool Has_GL_EXT_packed_pixels() const { return mExtensions.mGL_EXT_packed_pixels; }
	bool Has_GL_EXT_paletted_texture() const { return mExtensions.mGL_EXT_paletted_texture; }
	bool Has_GL_EXT_pixel_transform() const { return mExtensions.mGL_EXT_pixel_transform; }
	bool Has_GL_EXT_pixel_transform_color_table() const { return mExtensions.mGL_EXT_pixel_transform_color_table; }
	bool Has_GL_EXT_point_parameters() const { return mExtensions.mGL_EXT_point_parameters; }
	bool Has_GL_EXT_polygon_offset() const { return mExtensions.mGL_EXT_polygon_offset; }
	bool Has_GL_EXT_rescale_normal() const { return mExtensions.mGL_EXT_rescale_normal; }
	bool Has_GL_EXT_secondary_color() const { return mExtensions.mGL_EXT_secondary_color; }
	bool Has_GL_EXT_separate_specular_color() const { return mExtensions.mGL_EXT_separate_specular_color; }
	bool Has_GL_EXT_shadow_funcs() const { return mExtensions.mGL_EXT_shadow_funcs; }
	bool Has_GL_EXT_shared_texture_palette() const { return mExtensions.mGL_EXT_shared_texture_palette; }
	bool Has_GL_EXT_stencil_two_side() const { return mExtensions.mGL_EXT_stencil_two_side; }
	bool Has_GL_EXT_stencil_wrap() const { return mExtensions.mGL_EXT_stencil_wrap; }
	bool Has_GL_EXT_subtexture() const { return mExtensions.mGL_EXT_subtexture; }
	bool Has_GL_EXT_texture() const { return mExtensions.mGL_EXT_texture; }
	bool Has_GL_EXT_texture3D() const { return mExtensions.mGL_EXT_texture3D; }
	bool Has_GL_EXT_texture_compression_s3tc() const { return mExtensions.mGL_EXT_texture_compression_s3tc; }
	bool Has_GL_EXT_texture_cube_map() const { return mExtensions.mGL_EXT_texture_cube_map; }
	bool Has_GL_EXT_texture_edge_clamp() const { return mExtensions.mGL_EXT_texture_edge_clamp; }
	bool Has_GL_EXT_texture_env_add() const { return mExtensions.mGL_EXT_texture_env_add; }
	bool Has_GL_EXT_texture_env_combine() const { return mExtensions.mGL_EXT_texture_env_combine; }
	bool Has_GL_EXT_texture_env_dot3() const { return mExtensions.mGL_EXT_texture_env_dot3; }
	bool Has_GL_EXT_texture_filter_anisotropic() const { return mExtensions.mGL_EXT_texture_filter_anisotropic; }
	bool Has_GL_EXT_texture_lod_bias() const { return mExtensions.mGL_EXT_texture_lod_bias; }
	bool Has_GL_EXT_texture_object() const { return mExtensions.mGL_EXT_texture_object; }
	bool Has_GL_EXT_texture_perturb_normal() const { return mExtensions.mGL_EXT_texture_perturb_normal; }
	bool Has_GL_EXT_vertex_array() const { return mExtensions.mGL_EXT_vertex_array; }
	bool Has_GL_EXT_vertex_weighting() const { return mExtensions.mGL_EXT_vertex_weighting; }
	bool Has_GL_HP_convolution_border_modes() const { return mExtensions.mGL_HP_convolution_border_modes; }
	bool Has_GL_HP_image_transform() const { return mExtensions.mGL_HP_image_transform; }
	bool Has_GL_HP_occlusion_test() const { return mExtensions.mGL_HP_occlusion_test; }
	bool Has_GL_HP_texture_lighting() const { return mExtensions.mGL_HP_texture_lighting; }
	bool Has_GL_IBM_cull_vertex() const { return mExtensions.mGL_IBM_cull_vertex; }
	bool Has_GL_IBM_multimode_draw_arrays() const { return mExtensions.mGL_IBM_multimode_draw_arrays; }
	bool Has_GL_IBM_rasterpos_clip() const { return mExtensions.mGL_IBM_rasterpos_clip; }
	bool Has_GL_IBM_texture_mirrored_repeat() const { return mExtensions.mGL_IBM_texture_mirrored_repeat; }
	bool Has_GL_IBM_vertex_array_lists() const { return mExtensions.mGL_IBM_vertex_array_lists; }
	bool Has_GL_INGR_color_clamp() const { return mExtensions.mGL_INGR_color_clamp; }
	bool Has_GL_INGR_interlace_read() const { return mExtensions.mGL_INGR_interlace_read; }
	bool Has_GL_INGR_palette_buffer() const { return mExtensions.mGL_INGR_palette_buffer; }
	bool Has_GL_INTEL_parallel_arrays() const { return mExtensions.mGL_INTEL_parallel_arrays; }
	bool Has_GL_INTEL_texture_scissor() const { return mExtensions.mGL_INTEL_texture_scissor; }
	bool Has_GL_MESA_resize_buffers() const { return mExtensions.mGL_MESA_resize_buffers; }
	bool Has_GL_MESA_window_pos() const { return mExtensions.mGL_MESA_window_pos; }
	bool Has_GL_NV_blend_square() const { return mExtensions.mGL_NV_blend_square; }
	bool Has_GL_NV_copy_depth_to_color() const { return mExtensions.mGL_NV_copy_depth_to_color; }
	bool Has_GL_NV_depth_clamp() const { return mExtensions.mGL_NV_depth_clamp; }
	bool Has_GL_NV_element_array() const { return mExtensions.mGL_NV_element_array; }
	bool Has_GL_NV_fence() const { return mExtensions.mGL_NV_fence; }
	bool Has_GL_NV_float_buffer() const { return mExtensions.mGL_NV_float_buffer; }
	bool Has_GL_NV_fog_distance() const { return mExtensions.mGL_NV_fog_distance; }
	bool Has_GL_NV_fragment_program() const { return mExtensions.mGL_NV_fragment_program; }
	bool Has_GL_NV_half_float() const { return mExtensions.mGL_NV_half_float; }
	bool Has_GL_NV_light_max_exponent() const { return mExtensions.mGL_NV_light_max_exponent; }
	bool Has_GL_NV_multisample_filter_hint() const { return mExtensions.mGL_NV_multisample_filter_hint; }
	bool Has_GL_NV_occlusion_query() const { return mExtensions.mGL_NV_occlusion_query; }
	bool Has_GL_NV_packed_depth_stencil() const { return mExtensions.mGL_NV_packed_depth_stencil; }
	bool Has_GL_NV_pixel_data_range() const { return mExtensions.mGL_NV_pixel_data_range; }
	bool Has_GL_NV_point_sprite() const { return mExtensions.mGL_NV_point_sprite; }
	bool Has_GL_NV_primitive_restart() const { return mExtensions.mGL_NV_primitive_restart; }
	bool Has_GL_NV_register_combiners() const { return mExtensions.mGL_NV_register_combiners; }
	bool Has_GL_NV_register_combiners2() const { return mExtensions.mGL_NV_register_combiners2; }
	bool Has_GL_NV_stencil_two_side() const { return mExtensions.mGL_NV_stencil_two_side; }
	bool Has_GL_NV_texgen_emboss() const { return mExtensions.mGL_NV_texgen_emboss; }
	bool Has_GL_NV_texgen_reflection() const { return mExtensions.mGL_NV_texgen_reflection; }
	bool Has_GL_NV_texture_compression_vtc() const { return mExtensions.mGL_NV_texture_compression_vtc; }
	bool Has_GL_NV_texture_env_combine4() const { return mExtensions.mGL_NV_texture_env_combine4; }
	bool Has_GL_NV_texture_rectangle() const { return mExtensions.mGL_NV_texture_rectangle; }
	bool Has_GL_NV_texture_shader() const { return mExtensions.mGL_NV_texture_shader; }
	bool Has_GL_NV_texture_shader2() const { return mExtensions.mGL_NV_texture_shader2; }
	bool Has_GL_NV_texture_shader3() const { return mExtensions.mGL_NV_texture_shader3; }
	bool Has_GL_NV_vertex_array_range() const { return mExtensions.mGL_NV_vertex_array_range; }
	bool Has_GL_NV_vertex_array_range2() const { return mExtensions.mGL_NV_vertex_array_range2; }
	bool Has_GL_NV_vertex_program() const { return mExtensions.mGL_NV_vertex_program; }
	bool Has_GL_NV_vertex_program1_1() const { return mExtensions.mGL_NV_vertex_program1_1; }
	bool Has_GL_NV_vertex_program2() const { return mExtensions.mGL_NV_vertex_program2; }
	bool Has_GL_PGI_misc_hints() const { return mExtensions.mGL_PGI_misc_hints; }
	bool Has_GL_PGI_vertex_hints() const { return mExtensions.mGL_PGI_vertex_hints; }
	bool Has_GL_REND_screen_coordinates() const { return mExtensions.mGL_REND_screen_coordinates; }
	bool Has_GL_SGIS_detail_texture() const { return mExtensions.mGL_SGIS_detail_texture; }
	bool Has_GL_SGIS_fog_function() const { return mExtensions.mGL_SGIS_fog_function; }
	bool Has_GL_SGIS_generate_mipmap() const { return mExtensions.mGL_SGIS_generate_mipmap; }
	bool Has_GL_SGIS_multisample() const { return mExtensions.mGL_SGIS_multisample; }
	bool Has_GL_SGIS_pixel_texture() const { return mExtensions.mGL_SGIS_pixel_texture; }
	bool Has_GL_SGIS_sharpen_texture() const { return mExtensions.mGL_SGIS_sharpen_texture; }
	bool Has_GL_SGIS_texture4D() const { return mExtensions.mGL_SGIS_texture4D; }
	bool Has_GL_SGIS_texture_border_clamp() const { return mExtensions.mGL_SGIS_texture_border_clamp; }
	bool Has_GL_SGIS_texture_color_mask() const { return mExtensions.mGL_SGIS_texture_color_mask; }
	bool Has_GL_SGIS_texture_edge_clamp() const { return mExtensions.mGL_SGIS_texture_edge_clamp; }
	bool Has_GL_SGIS_texture_filter4() const { return mExtensions.mGL_SGIS_texture_filter4; }
	bool Has_GL_SGIS_texture_lod() const { return mExtensions.mGL_SGIS_texture_lod; }
	bool Has_GL_SGIS_texture_select() const { return mExtensions.mGL_SGIS_texture_select; }
	bool Has_GL_SGIX_blend_alpha_minmax() const { return mExtensions.mGL_SGIX_blend_alpha_minmax; }
	bool Has_GL_SGIX_clipmap() const { return mExtensions.mGL_SGIX_clipmap; }
	bool Has_GL_SGIX_depth_texture() const { return mExtensions.mGL_SGIX_depth_texture; }
	bool Has_GL_SGIX_flush_raster() const { return mExtensions.mGL_SGIX_flush_raster; }
	bool Has_GL_SGIX_fog_offset() const { return mExtensions.mGL_SGIX_fog_offset; }
	bool Has_GL_SGIX_framezoom() const { return mExtensions.mGL_SGIX_framezoom; }
	bool Has_GL_SGIX_instruments() const { return mExtensions.mGL_SGIX_instruments; }
	bool Has_GL_SGIX_interlace() const { return mExtensions.mGL_SGIX_interlace; }
	bool Has_GL_SGIX_ir_instrument1() const { return mExtensions.mGL_SGIX_ir_instrument1; }
	bool Has_GL_SGIX_list_priority() const { return mExtensions.mGL_SGIX_list_priority; }
	bool Has_GL_SGIX_pixel_texture() const { return mExtensions.mGL_SGIX_pixel_texture; }
	bool Has_GL_SGIX_reference_plane() const { return mExtensions.mGL_SGIX_reference_plane; }
	bool Has_GL_SGIX_resample() const { return mExtensions.mGL_SGIX_resample; }
	bool Has_GL_SGIX_shadow() const { return mExtensions.mGL_SGIX_shadow; }
	bool Has_GL_SGIX_shadow_ambient() const { return mExtensions.mGL_SGIX_shadow_ambient; }
	bool Has_GL_SGIX_sprite() const { return mExtensions.mGL_SGIX_sprite; }
	bool Has_GL_SGIX_tag_sample_buffer() const { return mExtensions.mGL_SGIX_tag_sample_buffer; }
	bool Has_GL_SGIX_texture_add_env() const { return mExtensions.mGL_SGIX_texture_add_env; }
	bool Has_GL_SGIX_texture_lod_bias() const { return mExtensions.mGL_SGIX_texture_lod_bias; }
	bool Has_GL_SGIX_texture_multi_buffer() const { return mExtensions.mGL_SGIX_texture_multi_buffer; }
	bool Has_GL_SGIX_texture_scale_bias() const { return mExtensions.mGL_SGIX_texture_scale_bias; }
	bool Has_GL_SGIX_vertex_preclip() const { return mExtensions.mGL_SGIX_vertex_preclip; }
	bool Has_GL_SGIX_ycrcb() const { return mExtensions.mGL_SGIX_ycrcb; }
	bool Has_GL_SGI_color_matrix() const { return mExtensions.mGL_SGI_color_matrix; }
	bool Has_GL_SGI_color_table() const { return mExtensions.mGL_SGI_color_table; }
	bool Has_GL_SGI_texture_color_table() const { return mExtensions.mGL_SGI_texture_color_table; }
	bool Has_GL_SUNX_constant_data() const { return mExtensions.mGL_SUNX_constant_data; }
	bool Has_GL_SUN_convolution_border_modes() const { return mExtensions.mGL_SUN_convolution_border_modes; }
	bool Has_GL_SUN_global_alpha() const { return mExtensions.mGL_SUN_global_alpha; }
	bool Has_GL_SUN_triangle_list() const { return mExtensions.mGL_SUN_triangle_list; }
	bool Has_GL_SUN_vertex() const { return mExtensions.mGL_SUN_vertex; }
	bool Has_GL_VERSION_1_2() const { return mExtensions.mGL_VERSION_1_2; }
	bool Has_GL_VERSION_1_3() const { return mExtensions.mGL_VERSION_1_3; }
	bool Has_GL_VERSION_1_4() const { return mExtensions.mGL_VERSION_1_4; }
	bool Has_GL_WIN_phong_shading() const { return mExtensions.mGL_WIN_phong_shading; }
	bool Has_GL_WIN_specular_fog() const { return mExtensions.mGL_WIN_specular_fog; }
#if defined(_WIN32)
#endif //_WIN32
	GLExtensionReg();
	~GLExtensionReg();
	void Init(GLExtGenFunctionPointers *funPtr = 0, GLExtGenExtensions *ext = 0);
	bool 	QueryGLExtension(const char *name);
#if defined(_WIN32)
	bool 	QueryWGLExtension(const char *name);
	HDC		GetHDC() const {	return mHDC;	}
	void	SetHDC(HDC hdc){	mHDC = hdc;		}
#endif //_WIN32
};

#ifdef GL_ARB_multisample
extern PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB;
#endif //GL_ARB_multisample

#ifdef GL_ARB_multitexture
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB;
extern PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB;
extern PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB;
extern PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB;
extern PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB;
extern PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB;
extern PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB;
extern PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB;
extern PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
extern PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB;
extern PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB;
extern PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB;
extern PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB;
extern PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB;
extern PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB;
extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB;
extern PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB;
extern PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB;
extern PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB;
extern PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB;
extern PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB;
extern PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB;
extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
extern PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB;
extern PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB;
extern PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB;
extern PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB;
extern PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB;
#endif //GL_ARB_multitexture

#ifdef GL_ARB_point_parameters
extern PFNGLPOINTPARAMETERFARBPROC glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB;
#endif //GL_ARB_point_parameters

#ifdef GL_ARB_texture_compression
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC glCompressedTexImage3DARB;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC glCompressedTexImage1DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1DARB;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC glGetCompressedTexImageARB;
#endif //GL_ARB_texture_compression

#ifdef GL_ARB_transpose_matrix
extern PFNGLLOADTRANSPOSEMATRIXFARBPROC glLoadTransposeMatrixfARB;
extern PFNGLLOADTRANSPOSEMATRIXDARBPROC glLoadTransposeMatrixdARB;
extern PFNGLMULTTRANSPOSEMATRIXFARBPROC glMultTransposeMatrixfARB;
extern PFNGLMULTTRANSPOSEMATRIXDARBPROC glMultTransposeMatrixdARB;
#endif //GL_ARB_transpose_matrix

#ifdef GL_ARB_vertex_buffer_object
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLISBUFFERARBPROC glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;
#endif //GL_ARB_vertex_buffer_object

#ifdef GL_ARB_vertex_program
extern PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
extern PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
extern PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
extern PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
extern PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
extern PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
extern PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
extern PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
extern PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
extern PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
extern PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
extern PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
extern PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
extern PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
extern PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
extern PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
extern PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
extern PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
extern PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
extern PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
extern PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
extern PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
extern PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
extern PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
extern PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
extern PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
extern PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
extern PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
extern PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
extern PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
extern PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
extern PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
extern PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
extern PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
extern PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
extern PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
extern PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
extern PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
extern PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
extern PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
extern PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
extern PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
extern PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
extern PFNGLISPROGRAMARBPROC glIsProgramARB;
#endif //GL_ARB_vertex_program

#ifdef GL_ARB_window_pos
extern PFNGLWINDOWPOS2DARBPROC glWindowPos2dARB;
extern PFNGLWINDOWPOS2FARBPROC glWindowPos2fARB;
extern PFNGLWINDOWPOS2IARBPROC glWindowPos2iARB;
extern PFNGLWINDOWPOS2SARBPROC glWindowPos2sARB;
extern PFNGLWINDOWPOS2DVARBPROC glWindowPos2dvARB;
extern PFNGLWINDOWPOS2FVARBPROC glWindowPos2fvARB;
extern PFNGLWINDOWPOS2IVARBPROC glWindowPos2ivARB;
extern PFNGLWINDOWPOS2SVARBPROC glWindowPos2svARB;
extern PFNGLWINDOWPOS3DARBPROC glWindowPos3dARB;
extern PFNGLWINDOWPOS3FARBPROC glWindowPos3fARB;
extern PFNGLWINDOWPOS3IARBPROC glWindowPos3iARB;
extern PFNGLWINDOWPOS3SARBPROC glWindowPos3sARB;
extern PFNGLWINDOWPOS3DVARBPROC glWindowPos3dvARB;
extern PFNGLWINDOWPOS3FVARBPROC glWindowPos3fvARB;
extern PFNGLWINDOWPOS3IVARBPROC glWindowPos3ivARB;
extern PFNGLWINDOWPOS3SVARBPROC glWindowPos3svARB;
#endif //GL_ARB_window_pos

#ifdef GL_EXT_blend_color
extern PFNGLBLENDCOLOREXTPROC glBlendColorEXT;
#endif //GL_EXT_blend_color

#ifdef GL_EXT_blend_func_separate
extern PFNGLBLENDFUNCSEPARATEEXTPROC glBlendFuncSeparateEXT;
#endif //GL_EXT_blend_func_separate

#ifdef GL_EXT_blend_minmax
extern PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT;
#endif //GL_EXT_blend_minmax

#ifdef GL_EXT_compiled_vertex_array
extern PFNGLLOCKARRAYSEXTPROC glLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT;
#endif //GL_EXT_compiled_vertex_array

#ifdef GL_EXT_draw_range_elements
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT;
#endif //GL_EXT_draw_range_elements

#ifdef GL_EXT_fog_coord
extern PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
extern PFNGLFOGCOORDFVEXTPROC glFogCoordfvEXT;
extern PFNGLFOGCOORDDEXTPROC glFogCoorddEXT;
extern PFNGLFOGCOORDDVEXTPROC glFogCoorddvEXT;
extern PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT;
#endif //GL_EXT_fog_coord

#ifdef GL_EXT_multi_draw_arrays
extern PFNGLMULTIDRAWARRAYSEXTPROC glMultiDrawArraysEXT;
extern PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT;
#endif //GL_EXT_multi_draw_arrays

#ifdef GL_EXT_paletted_texture
extern PFNGLCOLORTABLEEXTPROC glColorTableEXT;
extern PFNGLCOLORSUBTABLEEXTPROC glColorSubTableEXT;
extern PFNGLGETCOLORTABLEEXTPROC glGetColorTableEXT;
extern PFNGLGETCOLORTABLEPARAMETERIVEXTPROC glGetColorTableParameterivEXT;
extern PFNGLGETCOLORTABLEPARAMETERFVEXTPROC glGetColorTableParameterfvEXT;
#endif //GL_EXT_paletted_texture

#ifdef GL_EXT_point_parameters
extern PFNGLPOINTPARAMETERFEXTPROC glPointParameterfEXT;
extern PFNGLPOINTPARAMETERFVEXTPROC glPointParameterfvEXT;
extern PFNGLPOINTPARAMETERFSGISPROC glPointParameterfSGIS;
extern PFNGLPOINTPARAMETERFVSGISPROC glPointParameterfvSGIS;
#endif //GL_EXT_point_parameters

#ifdef GL_EXT_secondary_color
extern PFNGLSECONDARYCOLOR3BEXTPROC glSecondaryColor3bEXT;
extern PFNGLSECONDARYCOLOR3BVEXTPROC glSecondaryColor3bvEXT;
extern PFNGLSECONDARYCOLOR3DEXTPROC glSecondaryColor3dEXT;
extern PFNGLSECONDARYCOLOR3DVEXTPROC glSecondaryColor3dvEXT;
extern PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT;
extern PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT;
extern PFNGLSECONDARYCOLOR3IEXTPROC glSecondaryColor3iEXT;
extern PFNGLSECONDARYCOLOR3IVEXTPROC glSecondaryColor3ivEXT;
extern PFNGLSECONDARYCOLOR3SEXTPROC glSecondaryColor3sEXT;
extern PFNGLSECONDARYCOLOR3SVEXTPROC glSecondaryColor3svEXT;
extern PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT;
extern PFNGLSECONDARYCOLOR3UIEXTPROC glSecondaryColor3uiEXT;
extern PFNGLSECONDARYCOLOR3UIVEXTPROC glSecondaryColor3uivEXT;
extern PFNGLSECONDARYCOLOR3USEXTPROC glSecondaryColor3usEXT;
extern PFNGLSECONDARYCOLOR3USVEXTPROC glSecondaryColor3usvEXT;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT;
#endif //GL_EXT_secondary_color

#ifdef GL_EXT_stencil_two_side
extern PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;
#endif //GL_EXT_stencil_two_side

#ifdef GL_EXT_texture3D
extern PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
extern PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT;
#endif //GL_EXT_texture3D

#ifdef GL_EXT_texture_object
extern PFNGLARETEXTURESRESIDENTEXTPROC glAreTexturesResidentEXT;
extern PFNGLBINDTEXTUREEXTPROC glBindTextureEXT;
extern PFNGLDELETETEXTURESEXTPROC glDeleteTexturesEXT;
extern PFNGLGENTEXTURESEXTPROC glGenTexturesEXT;
extern PFNGLISTEXTUREEXTPROC glIsTextureEXT;
extern PFNGLPRIORITIZETEXTURESEXTPROC glPrioritizeTexturesEXT;
#endif //GL_EXT_texture_object

#ifdef GL_EXT_vertex_array
extern PFNGLARRAYELEMENTEXTPROC glArrayElementEXT;
extern PFNGLCOLORPOINTEREXTPROC glColorPointerEXT;
extern PFNGLDRAWARRAYSEXTPROC glDrawArraysEXT;
extern PFNGLEDGEFLAGPOINTEREXTPROC glEdgeFlagPointerEXT;
extern PFNGLGETPOINTERVEXTPROC glGetPointervEXT;
extern PFNGLINDEXPOINTEREXTPROC glIndexPointerEXT;
extern PFNGLNORMALPOINTEREXTPROC glNormalPointerEXT;
extern PFNGLTEXCOORDPOINTEREXTPROC glTexCoordPointerEXT;
extern PFNGLVERTEXPOINTEREXTPROC glVertexPointerEXT;
#endif //GL_EXT_vertex_array

#ifdef GL_EXT_vertex_weighting
extern PFNGLVERTEXWEIGHTFEXTPROC glVertexWeightfEXT;
extern PFNGLVERTEXWEIGHTFVEXTPROC glVertexWeightfvEXT;
extern PFNGLVERTEXWEIGHTPOINTEREXTPROC glVertexWeightPointerEXT;
#endif //GL_EXT_vertex_weighting

#ifdef GL_NV_element_array
extern PFNGLELEMENTPOINTERNVPROC glElementPointerNV;
extern PFNGLDRAWELEMENTARRAYNVPROC glDrawElementArrayNV;
extern PFNGLDRAWRANGEELEMENTARRAYNVPROC glDrawRangeElementArrayNV;
extern PFNGLMULTIDRAWELEMENTARRAYNVPROC glMultiDrawElementArrayNV;
extern PFNGLMULTIDRAWRANGEELEMENTARRAYNVPROC glMultiDrawRangeElementArrayNV;
#endif //GL_NV_element_array

#ifdef GL_NV_fence
extern PFNGLDELETEFENCESNVPROC glDeleteFencesNV;
extern PFNGLGENFENCESNVPROC glGenFencesNV;
extern PFNGLISFENCENVPROC glIsFenceNV;
extern PFNGLTESTFENCENVPROC glTestFenceNV;
extern PFNGLGETFENCEIVNVPROC glGetFenceivNV;
extern PFNGLFINISHFENCENVPROC glFinishFenceNV;
extern PFNGLSETFENCENVPROC glSetFenceNV;
#endif //GL_NV_fence

#ifdef GL_NV_fragment_program
extern PFNGLPROGRAMNAMEDPARAMETER4FNVPROC glProgramNamedParameter4fNV;
extern PFNGLPROGRAMNAMEDPARAMETER4DNVPROC glProgramNamedParameter4dNV;
extern PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC glProgramNamedParameter4fvNV;
extern PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC glProgramNamedParameter4dvNV;
extern PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC glGetProgramNamedParameterfvNV;
extern PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC glGetProgramNamedParameterdvNV;
#endif //GL_NV_fragment_program

#ifdef GL_NV_half_float
extern PFNGLVERTEX2HNVPROC glVertex2hNV;
extern PFNGLVERTEX2HVNVPROC glVertex2hvNV;
extern PFNGLVERTEX3HNVPROC glVertex3hNV;
extern PFNGLVERTEX3HVNVPROC glVertex3hvNV;
extern PFNGLVERTEX4HNVPROC glVertex4hNV;
extern PFNGLVERTEX4HVNVPROC glVertex4hvNV;
extern PFNGLNORMAL3HNVPROC glNormal3hNV;
extern PFNGLNORMAL3HVNVPROC glNormal3hvNV;
extern PFNGLCOLOR3HNVPROC glColor3hNV;
extern PFNGLCOLOR3HVNVPROC glColor3hvNV;
extern PFNGLCOLOR4HNVPROC glColor4hNV;
extern PFNGLCOLOR4HVNVPROC glColor4hvNV;
extern PFNGLTEXCOORD1HNVPROC glTexCoord1hNV;
extern PFNGLTEXCOORD1HVNVPROC glTexCoord1hvNV;
extern PFNGLTEXCOORD2HNVPROC glTexCoord2hNV;
extern PFNGLTEXCOORD2HVNVPROC glTexCoord2hvNV;
extern PFNGLTEXCOORD3HNVPROC glTexCoord3hNV;
extern PFNGLTEXCOORD3HVNVPROC glTexCoord3hvNV;
extern PFNGLTEXCOORD4HNVPROC glTexCoord4hNV;
extern PFNGLTEXCOORD4HVNVPROC glTexCoord4hvNV;
extern PFNGLMULTITEXCOORD1HNVPROC glMultiTexCoord1hNV;
extern PFNGLMULTITEXCOORD1HVNVPROC glMultiTexCoord1hvNV;
extern PFNGLMULTITEXCOORD2HNVPROC glMultiTexCoord2hNV;
extern PFNGLMULTITEXCOORD2HVNVPROC glMultiTexCoord2hvNV;
extern PFNGLMULTITEXCOORD3HNVPROC glMultiTexCoord3hNV;
extern PFNGLMULTITEXCOORD3HVNVPROC glMultiTexCoord3hvNV;
extern PFNGLMULTITEXCOORD4HNVPROC glMultiTexCoord4hNV;
extern PFNGLMULTITEXCOORD4HVNVPROC glMultiTexCoord4hvNV;
extern PFNGLFOGCOORDHNVPROC glFogCoordhNV;
extern PFNGLFOGCOORDHVNVPROC glFogCoordhvNV;
extern PFNGLSECONDARYCOLOR3HNVPROC glSecondaryColor3hNV;
extern PFNGLSECONDARYCOLOR3HVNVPROC glSecondaryColor3hvNV;
extern PFNGLVERTEXWEIGHTHNVPROC glVertexWeighthNV;
extern PFNGLVERTEXWEIGHTHVNVPROC glVertexWeighthvNV;
extern PFNGLVERTEXATTRIB1HNVPROC glVertexAttrib1hNV;
extern PFNGLVERTEXATTRIB1HVNVPROC glVertexAttrib1hvNV;
extern PFNGLVERTEXATTRIB2HNVPROC glVertexAttrib2hNV;
extern PFNGLVERTEXATTRIB2HVNVPROC glVertexAttrib2hvNV;
extern PFNGLVERTEXATTRIB3HNVPROC glVertexAttrib3hNV;
extern PFNGLVERTEXATTRIB3HVNVPROC glVertexAttrib3hvNV;
extern PFNGLVERTEXATTRIB4HNVPROC glVertexAttrib4hNV;
extern PFNGLVERTEXATTRIB4HVNVPROC glVertexAttrib4hvNV;
extern PFNGLVERTEXATTRIBS1HVNVPROC glVertexAttribs1hvNV;
extern PFNGLVERTEXATTRIBS2HVNVPROC glVertexAttribs2hvNV;
extern PFNGLVERTEXATTRIBS3HVNVPROC glVertexAttribs3hvNV;
extern PFNGLVERTEXATTRIBS4HVNVPROC glVertexAttribs4hvNV;
#endif //GL_NV_half_float

#ifdef GL_NV_occlusion_query
extern PFNGLGENOCCLUSIONQUERIESNVPROC glGenOcclusionQueriesNV;
extern PFNGLDELETEOCCLUSIONQUERIESNVPROC glDeleteOcclusionQueriesNV;
extern PFNGLISOCCLUSIONQUERYNVPROC glIsOcclusionQueryNV;
extern PFNGLBEGINOCCLUSIONQUERYNVPROC glBeginOcclusionQueryNV;
extern PFNGLENDOCCLUSIONQUERYNVPROC glEndOcclusionQueryNV;
extern PFNGLGETOCCLUSIONQUERYIVNVPROC glGetOcclusionQueryivNV;
extern PFNGLGETOCCLUSIONQUERYUIVNVPROC glGetOcclusionQueryuivNV;
#endif //GL_NV_occlusion_query

#ifdef GL_NV_pixel_data_range
extern PFNGLPIXELDATARANGENVPROC glPixelDataRangeNV;
extern PFNGLFLUSHPIXELDATARANGENVPROC glFlushPixelDataRangeNV;
#endif //GL_NV_pixel_data_range

#ifdef GL_NV_point_sprite
extern PFNGLPOINTPARAMETERINVPROC glPointParameteriNV;
extern PFNGLPOINTPARAMETERIVNVPROC glPointParameterivNV;
#endif //GL_NV_point_sprite

#ifdef GL_NV_primitive_restart
extern PFNGLPRIMITIVERESTARTNVPROC glPrimitiveRestartNV;
extern PFNGLPRIMITIVERESTARTINDEXNVPROC glPrimitiveRestartIndexNV;
#endif //GL_NV_primitive_restart

#ifdef GL_NV_register_combiners
extern PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;
extern PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV;
extern PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV;
extern PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV;
extern PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
extern PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
extern PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV;
extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV;
#endif //GL_NV_register_combiners

#ifdef GL_NV_register_combiners2
extern PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV;
extern PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC glGetCombinerStageParameterfvNV;
#endif //GL_NV_register_combiners2

#ifdef GL_NV_stencil_two_side
extern PFNGLACTIVESTENCILFACENVPROC glActiveStencilFaceNV;
#endif //GL_NV_stencil_two_side

#ifdef GL_NV_vertex_array_range
extern PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV;
extern PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV;
#endif //GL_NV_vertex_array_range

#ifdef GL_NV_vertex_program
extern PFNGLBINDPROGRAMNVPROC glBindProgramNV;
extern PFNGLDELETEPROGRAMSNVPROC glDeleteProgramsNV;
extern PFNGLEXECUTEPROGRAMNVPROC glExecuteProgramNV;
extern PFNGLGENPROGRAMSNVPROC glGenProgramsNV;
extern PFNGLAREPROGRAMSRESIDENTNVPROC glAreProgramsResidentNV;
extern PFNGLREQUESTRESIDENTPROGRAMSNVPROC glRequestResidentProgramsNV;
extern PFNGLGETPROGRAMPARAMETERFVNVPROC glGetProgramParameterfvNV;
extern PFNGLGETPROGRAMPARAMETERDVNVPROC glGetProgramParameterdvNV;
extern PFNGLGETPROGRAMIVNVPROC glGetProgramivNV;
extern PFNGLGETPROGRAMSTRINGNVPROC glGetProgramStringNV;
extern PFNGLGETTRACKMATRIXIVNVPROC glGetTrackMatrixivNV;
extern PFNGLGETVERTEXATTRIBDVNVPROC glGetVertexAttribdvNV;
extern PFNGLGETVERTEXATTRIBFVNVPROC glGetVertexAttribfvNV;
extern PFNGLGETVERTEXATTRIBIVNVPROC glGetVertexAttribivNV;
extern PFNGLGETVERTEXATTRIBPOINTERVNVPROC glGetVertexAttribPointervNV;
extern PFNGLISPROGRAMNVPROC glIsProgramNV;
extern PFNGLLOADPROGRAMNVPROC glLoadProgramNV;
extern PFNGLPROGRAMPARAMETER4FNVPROC glProgramParameter4fNV;
extern PFNGLPROGRAMPARAMETER4DNVPROC glProgramParameter4dNV;
extern PFNGLPROGRAMPARAMETER4DVNVPROC glProgramParameter4dvNV;
extern PFNGLPROGRAMPARAMETER4FVNVPROC glProgramParameter4fvNV;
extern PFNGLPROGRAMPARAMETERS4DVNVPROC glProgramParameters4dvNV;
extern PFNGLPROGRAMPARAMETERS4FVNVPROC glProgramParameters4fvNV;
extern PFNGLTRACKMATRIXNVPROC glTrackMatrixNV;
extern PFNGLVERTEXATTRIBPOINTERNVPROC glVertexAttribPointerNV;
extern PFNGLVERTEXATTRIB1SNVPROC glVertexAttrib1sNV;
extern PFNGLVERTEXATTRIB1FNVPROC glVertexAttrib1fNV;
extern PFNGLVERTEXATTRIB1DNVPROC glVertexAttrib1dNV;
extern PFNGLVERTEXATTRIB2SNVPROC glVertexAttrib2sNV;
extern PFNGLVERTEXATTRIB2FNVPROC glVertexAttrib2fNV;
extern PFNGLVERTEXATTRIB2DNVPROC glVertexAttrib2dNV;
extern PFNGLVERTEXATTRIB3SNVPROC glVertexAttrib3sNV;
extern PFNGLVERTEXATTRIB3FNVPROC glVertexAttrib3fNV;
extern PFNGLVERTEXATTRIB3DNVPROC glVertexAttrib3dNV;
extern PFNGLVERTEXATTRIB4SNVPROC glVertexAttrib4sNV;
extern PFNGLVERTEXATTRIB4FNVPROC glVertexAttrib4fNV;
extern PFNGLVERTEXATTRIB4DNVPROC glVertexAttrib4dNV;
extern PFNGLVERTEXATTRIB4UBNVPROC glVertexAttrib4ubNV;
extern PFNGLVERTEXATTRIB1SVNVPROC glVertexAttrib1svNV;
extern PFNGLVERTEXATTRIB1FVNVPROC glVertexAttrib1fvNV;
extern PFNGLVERTEXATTRIB1DVNVPROC glVertexAttrib1dvNV;
extern PFNGLVERTEXATTRIB2SVNVPROC glVertexAttrib2svNV;
extern PFNGLVERTEXATTRIB2FVNVPROC glVertexAttrib2fvNV;
extern PFNGLVERTEXATTRIB2DVNVPROC glVertexAttrib2dvNV;
extern PFNGLVERTEXATTRIB3SVNVPROC glVertexAttrib3svNV;
extern PFNGLVERTEXATTRIB3FVNVPROC glVertexAttrib3fvNV;
extern PFNGLVERTEXATTRIB3DVNVPROC glVertexAttrib3dvNV;
extern PFNGLVERTEXATTRIB4SVNVPROC glVertexAttrib4svNV;
extern PFNGLVERTEXATTRIB4FVNVPROC glVertexAttrib4fvNV;
extern PFNGLVERTEXATTRIB4DVNVPROC glVertexAttrib4dvNV;
extern PFNGLVERTEXATTRIB4UBVNVPROC glVertexAttrib4ubvNV;
extern PFNGLVERTEXATTRIBS1SVNVPROC glVertexAttribs1svNV;
extern PFNGLVERTEXATTRIBS1FVNVPROC glVertexAttribs1fvNV;
extern PFNGLVERTEXATTRIBS1DVNVPROC glVertexAttribs1dvNV;
extern PFNGLVERTEXATTRIBS2SVNVPROC glVertexAttribs2svNV;
extern PFNGLVERTEXATTRIBS2FVNVPROC glVertexAttribs2fvNV;
extern PFNGLVERTEXATTRIBS2DVNVPROC glVertexAttribs2dvNV;
extern PFNGLVERTEXATTRIBS3SVNVPROC glVertexAttribs3svNV;
extern PFNGLVERTEXATTRIBS3FVNVPROC glVertexAttribs3fvNV;
extern PFNGLVERTEXATTRIBS3DVNVPROC glVertexAttribs3dvNV;
extern PFNGLVERTEXATTRIBS4SVNVPROC glVertexAttribs4svNV;
extern PFNGLVERTEXATTRIBS4FVNVPROC glVertexAttribs4fvNV;
extern PFNGLVERTEXATTRIBS4DVNVPROC glVertexAttribs4dvNV;
extern PFNGLVERTEXATTRIBS4UBVNVPROC glVertexAttribs4ubvNV;
#endif //GL_NV_vertex_program

#ifdef GL_VERSION_1_2
extern PFNGLBLENDCOLORPROC glBlendColor;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLCOLORTABLEPROC glColorTable;
extern PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
extern PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
extern PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
extern PFNGLGETCOLORTABLEPROC glGetColorTable;
extern PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
extern PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
extern PFNGLCOLORSUBTABLEPROC glColorSubTable;
extern PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
extern PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
extern PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
extern PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
extern PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
extern PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
extern PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
extern PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
extern PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
extern PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
extern PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
extern PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
extern PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
extern PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
extern PFNGLGETHISTOGRAMPROC glGetHistogram;
extern PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
extern PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
extern PFNGLGETMINMAXPROC glGetMinmax;
extern PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
extern PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
extern PFNGLHISTOGRAMPROC glHistogram;
extern PFNGLMINMAXPROC glMinmax;
extern PFNGLRESETHISTOGRAMPROC glResetHistogram;
extern PFNGLRESETMINMAXPROC glResetMinmax;
extern PFNGLTEXIMAGE3DPROC glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d;
extern PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv;
extern PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f;
extern PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv;
extern PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i;
extern PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv;
extern PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s;
extern PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv;
extern PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d;
extern PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
extern PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i;
extern PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv;
extern PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s;
extern PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv;
extern PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d;
extern PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv;
extern PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv;
extern PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i;
extern PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv;
extern PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s;
extern PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv;
extern PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d;
extern PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv;
extern PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f;
extern PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv;
extern PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i;
extern PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv;
extern PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s;
extern PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv;
#endif //GL_VERSION_1_2

#ifdef GL_VERSION_1_3
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
extern PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
extern PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
extern PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
extern PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
extern PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
#endif //GL_VERSION_1_3

#ifdef GL_VERSION_1_4
extern PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC glPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
extern PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
extern PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv;
extern PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
extern PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv;
extern PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
extern PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv;
extern PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
extern PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv;
extern PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
extern PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv;
extern PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
extern PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
extern PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
extern PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
extern PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
extern PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
extern PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLWINDOWPOS2DPROC glWindowPos2d;
extern PFNGLWINDOWPOS2FPROC glWindowPos2f;
extern PFNGLWINDOWPOS2IPROC glWindowPos2i;
extern PFNGLWINDOWPOS2SPROC glWindowPos2s;
extern PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
extern PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
extern PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
extern PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
extern PFNGLWINDOWPOS3DPROC glWindowPos3d;
extern PFNGLWINDOWPOS3FPROC glWindowPos3f;
extern PFNGLWINDOWPOS3IPROC glWindowPos3i;
extern PFNGLWINDOWPOS3SPROC glWindowPos3s;
extern PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
extern PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
extern PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
extern PFNGLWINDOWPOS3SVPROC glWindowPos3sv;
#endif //GL_VERSION_1_4

#if defined(_WIN32)
#endif //_WIN32
#endif //GLEXTENSIONREG_H__
