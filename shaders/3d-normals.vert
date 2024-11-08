#version 330 core

layout (location = 0) in vec3 aPos;

out VS_OUT {
	vec3 WorldSpacePosition;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

const vec3 center = vec3(-0.016841, 0.110154, -0.001537);

vec4 multiplyQuaternions(vec4 a, vec4 b) {
	float s = a.w * b.w - dot(a.xyz, b.xyz);
	float x = a.w * b.x + b.w * a.x + a.y * b.z - a.z * b.y;
	float y = a.w * b.y + b.w * a.y + a.z * b.x - a.x * b.z;
	float z = a.w * b.z + b.w * a.z + a.x * b.y - a.y * b.x;
	return normalize(vec4(x, y, z, s));
}

vec4 multiplyQuaternions(vec4 a, vec3 b){
	return multiplyQuaternions(a, vec4(b, 1));
}

mat4 rotateX(float a){
	return mat4(1, 0, 0, 0,
				0, cos(a), -sin(a), 0,
				0, sin(a), cos(a), 0,
				0, 0, 0, 1);
}

mat4 rotateY(float a){
	return mat4(cos(a), 0, sin(a), 0,
				0, 1, 0, 0,
				-sin(a), 0, cos(a), 0,
				0, 0, 0, 1);
}

void main()
{
//	vec3 centered = 12 * (aPos - center);
//	vs_out.WorldSpacePosition = vec3(rotateY(time) * vec4(centered, 1.0) * rotateX(time / 10));
	vs_out.WorldSpacePosition = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(vs_out.WorldSpacePosition, 1.0);
}