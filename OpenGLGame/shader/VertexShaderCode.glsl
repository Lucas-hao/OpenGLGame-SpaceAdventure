#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec3 normal;
in layout(location=2) vec2 vertexUV;

out VS_OUT {
    vec3 FragPos;
	vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	// mvp transformation
	vec4 v = vec4(position, 1.0);
	// Texture Coordinate
	vs_out.FragPos = vec3(model * v);
	vs_out.Normal = normal;
	vs_out.TexCoords = vertexUV;

	gl_Position = proj * view * model * v;
}
