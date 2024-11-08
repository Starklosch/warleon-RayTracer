#include <Camera.hpp>
#include <Canvas.hpp>
#include <Mesh.hpp>
#include <Renderer.hpp>
#include <Scene.hpp>
#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

#include <tiny_obj_loader.h>
#include "Grid.hpp"
#include "ShaderProgram.h"

GLuint fullVao = 0;
int fullVertexCount = 0;

void loadFullMesh() {
	tinyobj::ObjReaderConfig reader_config;
	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile("data/bunny.obj", reader_config)) {
		return;
	}

	std::vector<glm::vec3> vertices;
	vertices.reserve(10000);

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;

		// 3 vertices for each face
		for (auto index : shapes[s].mesh.indices) {
			float x = attrib.vertices[3 * index.vertex_index + 0];
			float y = attrib.vertices[3 * index.vertex_index + 1];
			float z = attrib.vertices[3 * index.vertex_index + 2];
			vertices.emplace_back(x, y, z);
		}
	}

	glGenVertexArrays(1, &fullVao);
	glBindVertexArray(fullVao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	fullVertexCount = vertices.size();
}

void drawFullMesh(ShaderProgram& program, glm::mat4& model, glm::mat4& view) {
	if (fullVao == 0) {
		std::cerr << "Full mesh not loaded\n";
		return;
	}

	static glm::vec3 color(1);

	program.Use();
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(program, "objectColor"), 1, glm::value_ptr(color));

	glBindVertexArray(fullVao);
	glDrawArrays(GL_TRIANGLES, 0, fullVertexCount);
	glBindVertexArray(0);
}

using namespace war;
int main() {

	std::cout << std::filesystem::current_path() << std::endl;

	glm::vec3 center(-0.016841, 0.110154, -0.001537);

	Mesh::Loader load;
	std::string name("data/bunny.obj");
	load.OBJ(name);
	Ray cameraRay(point_t(-0.02, 0.11, -0.5), vec_t(0.0, 0.0, 1.0));
	// Renderer renderer(Canvas(1024, 1024), Camera(cameraRay, 0.4, 0.15, 0.15,
	// 0.0),Scene());
	Scene scene;
	scene.addMesh(load.getMesh());

	Renderer renderer(Canvas(32, 32), Camera(cameraRay, 0.4, 0.15, 0.15, 15.0), scene);
	renderer.render();
	//renderer.canvas.exportImage("render.png");

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	struct BoxInfo {
		GLuint vao;
		GLuint count;

		glm::vec3 color;
	};

	std::vector<int> colores = {
		0xFFB6C1, 0xFF7F50, 0x98FB98, 0xFFD700, 0xDDA0DD, 0xFFC0CB, 0x20B2AA, 0xF0E68C, 0xAFEEEE, 0xF8BBD0, 0xADD8E6, 0xFF6347, 0xE6E6FA, 0xFF1493, 0xB0E0E6, 0xFF69B4, 0xD8BFD8, 0xF5DEB3, 0xC0C0C0, 0xFFB347,
		0xFFE4E1, 0xFAEBD7, 0xD8BFD8, 0xF0FFF0, 0xE0FFFF, 0xF5F5DC, 0xD3D3D3, 0xFFF5EE, 0xFFE4B5, 0xE6B8B7, 0xFFE4C4, 0xFFF0F5, 0xFFDAB9, 0xF4A460, 0xF0E68C, 0xFFD700, 0xFFE4E1, 0xFF6347, 0xFAF0E6, 0xE4E1D5,
		0xFFF8DC, 0xF0F8FF, 0xD2B48C, 0xFF4500, 0xFF8C00, 0xDAA520, 0x32CD32, 0xADFF2F, 0x98FB98, 0x00FF7F, 0x7CFC00, 0x228B22, 0x008000, 0x006400, 0x2E8B57, 0x3CB371, 0x20B2AA, 0x66CDAA, 0x8FBC8F, 0x9ACD32
	};
;

	std::vector<BoxInfo> boxesInfo;
	std::cout << "The mesh has " << load.getMesh()->triangles.size() * 3 << " vertices in total\n";

	int total = 0;
	for (const auto& [idx, box] : load.getMesh()->getGrid()->data)
	{
		GLuint boxVao;
		glGenVertexArrays(1, &boxVao);
		glBindVertexArray(boxVao);

		GLuint boxVbo;
		glGenBuffers(1, &boxVbo);

		glBindBuffer(GL_ARRAY_BUFFER, boxVbo);

		// Convert triangles to vertices

		std::vector<glm::vec3> vertices;
		for (auto& triangle : box) {
			vertices.push_back(triangle->A);
			vertices.push_back(triangle->A + triangle->AB);
			vertices.push_back(triangle->A + triangle->AC);
		}
		std::cout << vertices.size() << " vertices for " << idx << '\n';

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
		glEnableVertexAttribArray(0);

		int color = colores.back();
		colores.pop_back();

		BoxInfo info;
		info.vao = boxVao;
		info.count = vertices.size();
		info.color = glm::vec3((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF) / 255.f;
		boxesInfo.emplace_back(info);

		total += vertices.size();
	}
	std::cout << "Total vertices " << total << '\n';

	// loadFullMesh();

	ShaderProgram program("shaders/3d-normals");
	ShaderProgram sprite("shaders/sprite");

	glEnable(GL_DEPTH_TEST);

	GLuint emptyVao;
	glGenVertexArrays(1, &emptyVao);
	glBindVertexArray(emptyVao);

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderer.canvas.width, renderer.canvas.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, renderer.canvas.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glm::vec3 modelPos(-center);
	glm::vec3 modelScale(12);
	glm::quat modelRot = glm::quatLookAt(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));

	glm::mat4 model = glm::mat4(1);
	model = glm::scale(model, modelScale);
	model = glm::translate(model, modelPos);

	glm::quat cameraRotation = glm::normalize(glm::quatLookAt(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));

	glm::vec3 camPosition = glm::vec3(0, 0, -5);
	glm::mat4 orientation = glm::mat4_cast(cameraRotation);
	glm::mat4 view = orientation * glm::translate(glm::mat4(1), camPosition);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	program.Use();
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	double fixedX = 0;
	double fixedY = 0;
	glfwSetCursorPos(window, fixedX, fixedY);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, fixedX, fixedY);

	glfwSwapInterval(1);

	glm::quat q(1, 0, 0, 0);

	glClearColor(0.2, 0.3, 0.3, 1.0);

	while (!glfwWindowShouldClose(window)) {
		orientation = glm::mat4_cast(cameraRotation);
		view = orientation * glm::translate(glm::mat4(1), camPosition);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.Use();
		glm::mat4 model = glm::mat4(1);
		model = glm::scale(model, modelScale);
		model *= glm::mat4_cast(q);
		model = glm::translate(model, modelPos);
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		for (const auto& box : boxesInfo) {
			glUniform3fv(glGetUniformLocation(program, "objectColor"), 1, glm::value_ptr(box.color));
			glBindVertexArray(box.vao);
			glDrawArrays(GL_TRIANGLES, 0, box.count);
		}

		// drawFullMesh(program, model, view);

		// Draw renderer result
		//sprite.Use();
		//glBindVertexArray(emptyVao);
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glfwSetCursorPos(window, fixedX, fixedY);

		if (y != fixedY) {
			glm::quat r = glm::rotate(glm::quat(1, 0, 0, 0), glm::radians(float(y) * 0.1f), glm::vec3(1, 0, 0));
			q = r * q;
		}
		if (x != fixedX) {
			glm::quat r = glm::rotate(glm::quat(1, 0, 0, 0), glm::radians(float(x) * 0.1f), glm::vec3(0, 1, 0));
			q = q * r;
		}
	}

	glfwTerminate();

	return 0;
}
