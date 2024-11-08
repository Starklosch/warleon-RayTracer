#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <glad/glad.h>
#include "Shader.h"

class ShaderProgram
{
	struct Properties {
		GLint type;
		GLint location;
		GLint size;
		GLint length;
	};

	static void PrintResource(Properties& props, std::string_view name);

public:
	ShaderProgram();
	ShaderProgram(const std::string& name);
	ShaderProgram(const ShaderProgram& program);
	ShaderProgram(ShaderProgram&& program) noexcept;
	~ShaderProgram();

	void Attach(GLenum type, std::string path);
	void Attach(std::shared_ptr<Shader> shader);
	//void Attach(Shader&& shader);
	void Detach(std::shared_ptr<Shader> shader);
	bool Link();
	void Use();
	void SetUBO(const std::string& name, int bindingPoint);

	void Uniform(const std::string& name, GLint value);
	void Uniform(const std::string& name, GLfloat value);
	void Uniform(const std::string& name, GLdouble value);

	void Print();

	operator GLuint() const;

	ShaderProgram& operator=(const ShaderProgram& other);
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

private:
	GLuint id;

	std::shared_ptr<Shader> vertShader;
	std::shared_ptr<Shader> fragShader;
	std::shared_ptr<Shader> geomShader;
	std::shared_ptr<Shader> compShader;
};

