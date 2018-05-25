#ifndef __CVK_DEFS_H
#define __CVK_DEFS_H

#include <vector>
#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_access.hpp>

// OpenGL
#define VERTICES      0
#define NORMALS       1
#define TEXTURECOORDS 2
#define TANGENTS      3

#define COLOR_TEXTURE_UNIT		GL_TEXTURE0
#define NORMAL_TEXTURE_UNIT		GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT		GL_TEXTURE2
#define CUBE_MAP_TEXTURE_UNIT	GL_TEXTURE3
#define METALLIC_TEXTURE_UNIT	GL_TEXTURE4
#define ROUGHNESS_TEXTURE_UNIT	GL_TEXTURE5
#define AO_TEXTURE_UNIT			GL_TEXTURE6

#define VERTEX_SHADER_BIT    1
#define TESS_CONTROL_BIT     2
#define TESS_EVAL_BIT        4
#define GEOMETRY_SHADER_BIT  8
#define FRAGMENT_SHADER_BIT 16
#define COMPUTE_SHADER_BIT  32

#define MAT_AMBIENT_BIT      1
#define MAT_DIFFUSE_BIT      2
#define MAT_SPECULAR_BIT     4
#define MAT_TRANSPARENCY_BIT 8

#define NO_FOG     0
#define FOG_LINEAR 1
#define FOG_EXP    2
#define FOG_EXP2   3

#define MAX_LIGHTS 8

#define INVALID_GL_VALUE 0xFFFFFFFF

// Geometry Objects
#define CVK_CONE     0
#define CVK_CUBE     1
#define CVK_GEOMETRY 2
#define CVK_PLANE    3
#define CVK_SPHERE   4
#define CVK_TEAPOT   5
#define CVK_TRIANGLE 6
#define CVK_WIRECUBE 7

// Ray Tracing
#define RAYEPS    1e-4f
#define MINWEIGHT 0.01f

// Colors
#define AQUAMARINE          glm::vec3(.439216f, .858824f, .576471f)
#define BLACK               glm::vec3(0.0f, 0.0f, 0.0f)
#define BLUE                glm::vec3(0.0f, 0.0f, 1.0f)
#define BLUEGREY            glm::vec3(.8f, .8f, 1.0f)
#define BLUEVIOLET          glm::vec3(.623529f, .372549f, .623529f)
#define BRONZE              glm::vec3(.59f, .35f, .22f)
#define BROWN               glm::vec3(.647059f, .164706f, .164706f)
#define CADETBLUE           glm::vec3(.372549f, .623529f, .623529f)
#define CORAL               glm::vec3(1.0f, .498039f, 0.0f)
#define CORNFLOWERBLUE      glm::vec3(.258824f, .258824f, .435294f)
#define CYAN                glm::vec3(0.0f, 1.0f, 1.0f)
#define DARKGREEN           glm::vec3(.184314f, .309804f, .184314f)
#define DARKGREY            glm::vec3(.2f, .2f, .2f)
#define DARKOLIVEGREEN      glm::vec3(.309804f, .309804f, .184314f)
#define DARKORCHID          glm::vec3(.6f, .196078f, .8f)
#define DARKSLATEBLUE       glm::vec3(.419608f, .137255f, .556863f)
#define DARKSLATEGRAY       glm::vec3(.184314f, .309804f, .309804f)
#define DARKSLATEGREY       glm::vec3(.184314f, .309804f, .309804f)
#define DARKTURQUOISE       glm::vec3(.439216f, .576471f, .858824f)
#define DIMGRAY             glm::vec3(.329412f, .329412f, .329412f)
#define DIMGREY             glm::vec3(.329412f, .329412f, .329412f)
#define FIREBRICK           glm::vec3(.556863f, .137255f, .137255f)
#define FORESTGREEN         glm::vec3(.137255f, .556863f, .137255f)
#define GOLD                glm::vec3(.8f, .498039f, .196078f)
#define GOLDENROD           glm::vec3(.858824f, .858824f, .439216f)
#define GRAY                glm::vec3(.752941f, .752941f, .752941f)
#define GREEN               glm::vec3(0.0f, 1.0f, 0.0f)
#define GREENYELLOW         glm::vec3(.576471f, .858824f, .439216f)
#define GREY                glm::vec3(.752941f, .752941f, .752941f)
#define INDIANRED           glm::vec3( .309804f, .184314f, .184314f)
#define KHAKI               glm::vec3(.623529f, .623529f, .372549f)
#define LIGHTBLUE           glm::vec3(.74902f, .847059f, .847059f)
#define LIGHTGRAY           glm::vec3(.658824f, .658824f, .658824f)
#define LIGHTGREY           glm::vec3(.658824f, .658824f, .658824f)
#define LIGHTSTEELBLUE      glm::vec3(.560784f, .560784f, .737255f)
#define LIMEGREEN           glm::vec3(.196078f, .8f, .196078f)
#define MAGENTA             glm::vec3(1.0f, 0.0f, 1.0f)
#define MAROON              glm::vec3(.556863f, .137255f, .419608f)
#define MEDIUMAQUAMARINE    glm::vec3(.196078f, .8f, .6f)
#define MEDIUMBLUE          glm::vec3(.196078f, .196078f, .8f)
#define MEDIUMFORESTGREEN   glm::vec3(.419608f, .556863f, .137255f)
#define MEDIUMGOLDENROD     glm::vec3(.917647f, .917647f, .678431f)
#define MEDIUMORCHID        glm::vec3(.576471f, .439216f, .858824f)
#define MEDIUMSEAGREEN      glm::vec3(.258824f, .435294f, .258824f)
#define MEDIUMSLATEBLUE     glm::vec3(.498039f, 0.0f, 1.0f)
#define MEDIUMSPRINGGREEN   glm::vec3(.498039f, 1.0f, 0.0f)
#define MEDIUMTURQUOISE     glm::vec3(.439216f, .858824f, .858824f)
#define MEDIUMVIOLETRED     glm::vec3(.858824f, .439216f, .576471f)
#define MIDNIGHTBLUE        glm::vec3(.184314f, .184314f, .309804f)
#define NAVY                glm::vec3(.137255f, .137255f, .556863f)
#define NAVYBLUE            glm::vec3(.137255f, .137255f, .556863f)
#define ORANGE              glm::vec3(.8f, .196078f, .196078f)
#define ORANGERED           glm::vec3(1.0f, 0.0f, .498039f)
#define ORCHID              glm::vec3(.858824f, .439216f, .858824f)
#define PALEGREEN           glm::vec3(.560784f, .737255f, .560784f)
#define PINK                glm::vec3(.737255f, .560784f, .560784f)
#define PLUM                glm::vec3(.917647f, .678431f, .917647f)
#define RED                 glm::vec3(1.0f, 0.0f, 0.0f)
#define SALMON              glm::vec3(.435294f, .258824f, .258824f)
#define SEAGREEN            glm::vec3(.137255f, .556863f, .419608f)
#define SIENNA              glm::vec3(.556863f, .419608f, .137255f)
#define SKYBLUE             glm::vec3(.196078f, .6f, .8f)
#define SLATEBLUE           glm::vec3(0.0f, .498039f, 1.0f)
#define SPRINGGREEN         glm::vec3(0.0f, 1.0f, .498039f)
#define STEELBLUE           glm::vec3(.137255f, .419608f, .556863f)
#define THISTLE             glm::vec3(.847059f, .74902f, .847059f)
#define TURQUOISE           glm::vec3(.678431f, .917647f, .917647f)
#define VIOLET              glm::vec3(.309804f, .184314f, .309804f)
#define VIOLETRED           glm::vec3(.8f, .196078f, .6f)
#define WHEAT               glm::vec3(.847059f, .847059f, .74902f)
#define WHITE               glm::vec3(.988235f, .988235f, .988235f)
#define YELLOW              glm::vec3(1.0f, 1.0f, 0.0f)
#define YELLOWGREEN         glm::vec3(.6f, .8f, .196078f)

enum TextureType
{
	COLOR_TEXTURE,
	NORMAL_TEXTURE,
	METALLIC_TEXTURE,
	ROUGHNESS_TEXTURE,
	AO_TEXTURE
};

#endif /* __CVK_DEFS_H */
