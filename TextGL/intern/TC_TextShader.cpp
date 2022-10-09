#include "TC_TextShader.h"

std::string cms_verx = R"(

#version 330 compatibility

out vec2 tex_coord;

void main()
{
  tex_coord = gl_MultiTexCoord0.xy;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

)";

std::string cms_frag = R"(

#version 330 core

uniform sampler2D tex;

in vec2 tex_coord;

out vec4 f_color;

void main()
{
	vec4 clr = texture(tex, tex_coord);	
	f_color = vec4(1, 1, 1, clr.x);
}

)";
