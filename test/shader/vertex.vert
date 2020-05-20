#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float time;

out vec2 v_texCoord;
void main()
{    
    v_texCoord = aTexCoord;
    gl_Position = vec4(aPos,1.0f);
}