#include "Curve.h"

Curve::Curve()
{
	initialized = false;
}

Curve::~Curve()
{
	if (initialized)
	{
		// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
		// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
}

void Curve::init()
{
	for (unsigned int i = 1; i < vertices.size(); ++i)
	{
		indices.push_back(i - 1);
		indices.push_back(i);
	}
	initialized = true;
	toWorld = glm::mat4(1.0f);
	angle = 0;
	direction = 0;
	scalePointAmount = 1;
	x = 0;
	y = 0;
	z = 0;
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
	totalRot = glm::mat4(1.0f);
	rotOffset = glm::mat4(1.0f);
	transFirst = false;
}

void Curve::update()
{
}

void Curve::draw(glm::mat4 C, Shader* shaderProgram, Camera camera, glm::mat4 projection, bool phong)
{
	// Now send these values to the shader program
	shaderProgram->use();
	shaderProgram->setMat4("projection", projection);
	glm::mat4 view = camera.GetViewMatrix();
	float z = view[3][2];
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	view[3][2] = z;
	shaderProgram->setMat4("view", view);
//	shaderProgram->setMat4("model", C * toWorld);
//	shaderProgram->setBool("phong", phong);
	// Now draw. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

std::vector<glm::vec3>* Curve::getVertices()
{
	return &vertices;
}
