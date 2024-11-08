#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 WorldSpacePosition;
} gs_in[];

out vec3 WorldSpacePosition;
out vec3 Normal;

uniform mat4 model;

void main()
{
	vec3 ab = normalize(gs_in[1].WorldSpacePosition - gs_in[0].WorldSpacePosition).xyz;
	vec3 ac = normalize(gs_in[2].WorldSpacePosition - gs_in[0].WorldSpacePosition).xyz;
	vec3 normal = cross(ab, ac);

	Normal = mat3(transpose(inverse(model))) * normal;

    // mat4 viewProjection = projection * view;

	gl_Position = gl_in[0].gl_Position;
	WorldSpacePosition = gs_in[0].WorldSpacePosition;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	WorldSpacePosition = gs_in[1].WorldSpacePosition;
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	WorldSpacePosition = gs_in[2].WorldSpacePosition;
	EmitVertex();

	EndPrimitive();
}