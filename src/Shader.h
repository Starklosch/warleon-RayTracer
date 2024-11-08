#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <glad/glad.h>

class Shader
{
	GLuint id;
	GLenum type;

	bool Compile();

public:
	Shader(GLenum type, const std::string& path);
	Shader(GLuint id);
	~Shader();

	GLenum GetType() const;

	operator GLuint() const;

	static std::shared_ptr<Shader> Create(GLenum type, const std::string& path);
};

