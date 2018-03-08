#include "OBJObject.h"

#include <errno.h>

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	parse(filepath);
	centerAndResize();
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
	glGenBuffers(1, &NBO);

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


	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

					 // We've sent the vertex data over to OpenGL, but there's still something missing.
					 // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
	totalRot = glm::mat4(1.0f);
	rotOffset = glm::mat4(1.0f);
	transFirst = false;
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	errno = 0;
	FILE* inFile = fopen(filepath, "r");
	char vertexType[3] = {0};
	float x = 0;
	float y = 0;
	float z = 0;
	float red = 0;
	float green = 0;
	float blue = 0;
	float vertex;
	float normal;
//	float vertex[3];
//	float normal[3];
	float notUsed = 0;

	if (inFile == NULL)
	{
		std::cerr << "Error loading file" << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	else
	{
		while (fscanf(inFile, "%s", vertexType) != EOF)
		{
			if (strncmp(vertexType, "vn", 2) == 0)
			{
				fscanf(inFile, "%f %f %f\n", &x, &y, &z);
				normals.push_back(glm::vec3(x, y, z));
			}
			else if (strncmp(vertexType, "v", 1) == 0)
			{
				fscanf(inFile, "%f %f %f %f %f %f\n", &x, &y, &z, &red, &green, &blue);
				vertices.push_back(glm::vec3(x, y, z));
			}
			else if (strncmp(vertexType, "f", 1) == 0)
			{
				for(int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						fscanf(inFile, "/");
						if(j == 0)
						{
//							fscanf(inFile, "%f", &vertex[i]);
							fscanf(inFile, "%f", &vertex);
							indices.push_back(vertex - 1);
						}
						if (j == 1)
						{
							fscanf(inFile, "%f", &notUsed);
						}
						if (j == 2)
						{
							//fscanf(inFile, "%f", &normal[i]);
							fscanf(inFile, "%f", &normal);
							nindices.push_back(normal - 1);
						}
					}
				}
				fscanf(inFile, "\n");
//				faces.push_back(face(vertex[0], vertex[1], vertex[2], 0, 0, 0, normal[0], normal[1], normal[2]));
			}
			else
			{
				fscanf(inFile, "%*[^\n]\n");
			}
		}
	}
}

void OBJObject::draw(GLuint shaderProgram, glm::vec3 viewPos, bool phong, struct DirLight* dirLight, struct PointLight* pointLight, int pointCount, struct SpotLight* spotLight, int spotCount)
{
/*	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glColor3f(normals[i].x, normals[i].y, normals[i].z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();*/

	// Calculate the combination of the model and view (camera inverse) matrices
//	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uView = glGetUniformLocation(shaderProgram, "view");
	uModel = glGetUniformLocation(shaderProgram, "model");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);

	uPhong = glGetUniformLocation(shaderProgram, "phong");
	glUniform1i(uPhong, (int)phong);
	if(phong)
	{
		//send more values to shader
		/*uLightColor = glGetUniformLocation(shaderProgram, "lightColor");
		glUniform3f(uLightColor, 1.0f, 1.0f, 1.0f);
		uLightPos = glGetUniformLocation(shaderProgram, "lightPos");
		glUniform3f(uLightPos, 10.0f, 10.0f, 15.0f);*/
		uViewPos = glGetUniformLocation(shaderProgram, "viewPos");
		glUniform3f(uViewPos, viewPos.x, viewPos.y, viewPos.z);

		//direction light
		clearLight = glGetUniformLocation(shaderProgram, "dirCount");
		glUniform1i(clearLight, dirLight != NULL);
		if(dirLight)
		{
			dirLight->uDirection = glGetUniformLocation(shaderProgram, "dirLight.direction");
			glUniform3f(dirLight->uDirection, dirLight->direction.x, dirLight->direction.y, dirLight->direction.z);
			dirLight->uAmbient = glGetUniformLocation(shaderProgram, "dirLight.ambient");
			glUniform3f(dirLight->uAmbient, dirLight->ambient.x, dirLight->ambient.y, dirLight->ambient.z);
			dirLight->uDiffuse = glGetUniformLocation(shaderProgram, "dirLight.diffuse");
			glUniform3f(dirLight->uDiffuse, dirLight->diffuse.x, dirLight->diffuse.y, dirLight->diffuse.z);
			dirLight->uSpecular = glGetUniformLocation(shaderProgram, "dirLight.specular");
			glUniform3f(dirLight->uSpecular, dirLight->specular.x, dirLight->specular.y, dirLight->specular.z);
		}
		/*else
		{
			clearLight = glGetUniformLocation(shaderProgram, "dirLight.direction");
			glUniform3f(clearLight, 0.0f, 0.0f, 0.0f);
			clearLight = glGetUniformLocation(shaderProgram, "dirLight.ambient");
			glUniform3f(clearLight, 0.2f, 0.2f, 0.2f);
			clearLight = glGetUniformLocation(shaderProgram, "dirLight.diffuse");
			glUniform3f(clearLight, 0.0f, 0.0f, 0.0f);
			clearLight = glGetUniformLocation(shaderProgram, "dirLight.specular");
			glUniform3f(clearLight, 0.0f, 0.0f, 0.0f);
		}*/

		material.uAmbient = glGetUniformLocation(shaderProgram, "material.ambient");
		glUniform3f(material.uAmbient, material.ambient.x, material.ambient.y, material.ambient.z);
		material.uDiffuse = glGetUniformLocation(shaderProgram, "material.diffuse");
		glUniform3f(material.uDiffuse, material.diffuse.x, material.diffuse.y, material.diffuse.z);
		material.uSpecular = glGetUniformLocation(shaderProgram, "material.specular");
		glUniform3f(material.uSpecular, material.specular.x, material.specular.y, material.specular.z);
		material.uShininess = glGetUniformLocation(shaderProgram, "material.shininess");
		glUniform1f(material.uShininess, material.shininess);

		//point light
		uPointCount = glGetUniformLocation(shaderProgram, "pointCount");
		glUniform1i(uPointCount, pointCount);
		if(pointLight)
		{
/*		std::cout << pointLight[0].position.x << " " << pointLight[0].position.y << " " << pointLight[0].position.z << std::endl;
		std::cout << "A:" << pointLight[0].ambient.x << " " << pointLight[0].ambient.y << " " << pointLight[0].ambient.z << std::endl;
		std::cout << "B:" << pointLight[0].diffuse.x << " " << pointLight[0].diffuse.y << " " << pointLight[0].diffuse.z << std::endl;
		std::cout << "C:" << pointLight[0].specular.x << " " << pointLight[0].specular.y << " " << pointLight[0].specular.z << std::endl;*/
			pointLight->uPosition = glGetUniformLocation(shaderProgram, "pointLight.position");
			glUniform3f(pointLight->uPosition, pointLight->position.x, pointLight->position.y, pointLight->position.z);
			pointLight->uAmbient = glGetUniformLocation(shaderProgram, "pointLight.ambient");
			glUniform3f(pointLight->uAmbient, pointLight->ambient.x, pointLight->ambient.y, pointLight->ambient.z);
			pointLight->uDiffuse = glGetUniformLocation(shaderProgram, "pointLight.diffuse");
			glUniform3f(pointLight->uDiffuse, pointLight->diffuse.x, pointLight->diffuse.y, pointLight->diffuse.z);
			pointLight->uSpecular = glGetUniformLocation(shaderProgram, "pointLight.specular");
			glUniform3f(pointLight->uSpecular, pointLight->specular.x, pointLight->specular.y, pointLight->specular.z);
			pointLight->uConstant = glGetUniformLocation(shaderProgram, "pointLight.constant");
			glUniform1f(pointLight->uConstant, pointLight->constant);
			pointLight->uLinear = glGetUniformLocation(shaderProgram, "pointLight.linear");
			glUniform1f(pointLight->uLinear, pointLight->linear);
			pointLight->uQuadratic = glGetUniformLocation(shaderProgram, "pointLight.quadratic");
			glUniform1f(pointLight->uQuadratic, pointLight->quadratic);
		}

		//spotlight
		uSpotCount = glGetUniformLocation(shaderProgram, "spotCount");
		glUniform1i(uSpotCount, spotCount);
		if (spotLight)
		{
			spotLight->uPosition = glGetUniformLocation(shaderProgram, "spotLight.position");
			glUniform3f(spotLight->uPosition, spotLight->position.x, spotLight->position.y, spotLight->position.z);
			spotLight->uDirection = glGetUniformLocation(shaderProgram, "spotLight.direction");
			glUniform3f(spotLight->uDirection, spotLight->direction.x, spotLight->direction.y, spotLight->direction.z);
			spotLight->uPosition = glGetUniformLocation(shaderProgram, "spotLight.cutoff");
			glUniform1f(spotLight->uPosition, spotLight->cutoff);
			spotLight->uAmbient = glGetUniformLocation(shaderProgram, "spotLight.ambient");
			glUniform3f(spotLight->uAmbient, spotLight->ambient.x, spotLight->ambient.y, spotLight->ambient.z);
			spotLight->uDiffuse = glGetUniformLocation(shaderProgram, "spotLight.diffuse");
			glUniform3f(spotLight->uDiffuse, spotLight->diffuse.x, spotLight->diffuse.y, spotLight->diffuse.z);
			spotLight->uSpecular = glGetUniformLocation(shaderProgram, "spotLight.specular");
			glUniform3f(spotLight->uSpecular, spotLight->specular.x, spotLight->specular.y, spotLight->specular.z);
			spotLight->uConstant = glGetUniformLocation(shaderProgram, "spotLight.constant");
			glUniform1f(spotLight->uConstant, spotLight->constant);
			spotLight->uLinear = glGetUniformLocation(shaderProgram, "spotLight.linear");
			glUniform1f(spotLight->uLinear, spotLight->linear);
			spotLight->uQuadratic = glGetUniformLocation(shaderProgram, "spotLight.quadratic");
			glUniform1f(spotLight->uQuadratic, spotLight->quadratic);
		}
		/*for(int i = 0; i < num; ++i)
		{
			std::string name = "pointLight[" + std::to_string(i) + "].";
			pointLight[i].uPosition = glGetUniformLocation(shaderProgram, (name + "position").c_str());
			glUniform3f(pointLight[i].uPosition, pointLight[i].position.x, pointLight[i].position.y, pointLight[i].position.z);
			pointLight[i].uAmbient = glGetUniformLocation(shaderProgram, (name + "ambient").c_str());
			glUniform3f(pointLight[i].uAmbient, pointLight[i].ambient.x, pointLight[i].ambient.y, pointLight[i].ambient.z);
			pointLight[i].uDiffuse = glGetUniformLocation(shaderProgram, (name + "diffuse").c_str());
			glUniform3f(pointLight[i].uDiffuse, pointLight[i].diffuse.x, pointLight[i].diffuse.y, pointLight[i].diffuse.z);
			pointLight[i].uSpecular = glGetUniformLocation(shaderProgram, (name + "specular").c_str());
			glUniform3f(pointLight[i].uSpecular, pointLight[i].specular.x, pointLight[i].specular.y, pointLight[i].specular.z);
			pointLight[i].uConstant = glGetUniformLocation(shaderProgram, (name + "constant").c_str());
			glUniform1f(pointLight[i].uConstant, pointLight[i].constant);
			pointLight[i].uLinear = glGetUniformLocation(shaderProgram, (name + "linear").c_str());
			glUniform1f(pointLight[i].uLinear, pointLight[i].linear);
			pointLight[i].uQuadratic = glGetUniformLocation(shaderProgram, (name + "quadratic").c_str());
			glUniform1f(pointLight[i].uQuadratic, pointLight[i].quadratic);
		}*/
	}

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::update()
{
	toWorld = glm::mat4(1.0f);
	if(!transFirst)
	{
		applyRotate();
//		spin(1.0f);
		applyTranslate();
	}
	else
	{
		applyTranslate();
		applyRotate();
	}
	applyScale();
}

void OBJObject::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the cube
	if (direction == X_AXIS)
	{
		this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * toWorld;
	}
	else if (direction == Y_AXIS)
	{
		this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) * toWorld;
	}
	else if (direction == Z_AXIS)
	{
		this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * toWorld;
	}
}

void OBJObject::setBaseCoordinate(float x, float y, float z)
{
	baseX = x;
	baseY = y;
	baseZ = z;
}

void OBJObject::setDirection(int direction)
{
	this->direction = direction;
}

void OBJObject::scalePoint(float amount)
{
	scalePointAmount += amount;
	if (scalePointAmount < 1)
	{
		scalePointAmount = 1;
	}
	glPointSize(scalePointAmount);
}

void OBJObject::resetScalePoint()
{
	scalePointAmount = 1;
	glPointSize(scalePointAmount);
}

void OBJObject::translate(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void OBJObject::resetTranslate()
{
	x = 0;
	y = 0;
	z = 0;
}

void OBJObject::applyTranslate()
{
	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(this->x + baseX, this->y + baseY, this->z + baseZ)) * toWorld;

}

void OBJObject::scale(float amount)
{
	scaleAmount += amount;
}

void OBJObject::resetScale()
{
	scaleAmount = 0;
}

void OBJObject::applyScale()
{
	toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(pow(1.2f, scaleAmount))) * toWorld;
}


void OBJObject::centerAndResize()
{
	int size = vertices.size();
	glm::vec3 center;
	float xMin = FLT_MAX;
	float xMax = FLT_MIN;
	float yMin = FLT_MAX;
	float yMax = FLT_MIN;
	float zMin = FLT_MAX;
	float zMax = FLT_MIN;
	float scaleDownAmount = 0;
	glm::vec4 intermVal;
	for (int i = 0; i < size; ++i)
	{
		xMin = (vertices[i].x < xMin) ? vertices[i].x : xMin;
		xMax = (vertices[i].x > xMax) ? vertices[i].x : xMax;
		yMin = (vertices[i].y < yMin) ? vertices[i].y : yMin;
		yMax = (vertices[i].y > yMax) ? vertices[i].y : yMax;
		zMin = (vertices[i].z < zMin) ? vertices[i].z : zMin;
		zMax = (vertices[i].z > zMax) ? vertices[i].z : zMax;
	}
	center = glm::vec3((xMax + xMin) / 2.0f, (yMax + yMin) / 2.0f, (zMax + zMin) / 2.0f);
	scaleDownAmount = 2 / std::max(std::max(xMax - xMin, yMax - yMin), zMax - zMin);
	for (int i = 0; i < vertices.size(); ++i)
	{
		intermVal = (glm::translate(glm::mat4(1.0f), (center * -1.0f)) * glm::vec4(vertices[i], 1));
		vertices[i] = glm::vec3(glm::scale(glm::mat4(1.0f), glm::vec3(scaleDownAmount)) * intermVal);
	}
}

void OBJObject::applyRotate()
{
	toWorld = totalRot * rotOffset * toWorld;
}

glm::vec3 OBJObject::getVertex(int ver)
{
	return vertices[ver];
}

glm::vec3 OBJObject::getNormal(int norm)
{
	return normals[norm];
}

float OBJObject::getScaleAmount()
{
	return scalePointAmount;
}

void OBJObject::rasterize(int width, int height)
{
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it

	// Loop through all the vertices of this OBJ Object and render them
	glm::vec4 tempVec;
	glm::mat4 worldToCamera = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 normalizePerspective = glm::perspective(60.0f / 180.0f * glm::pi<float>(), (float)width / (float)height, 1.0f, 1000.0f);
	glm::mat4 viewportTrans = glm::mat4((width / 2),	0.0f,			0.0f,	0.0f,
										0.0f,			(height / 2),	0.0f,	0.0f,
										0.0f,			0.0f,			0.5f,	0.0f,
										(width / 2),	(height / 2),	0.5f,	1.0f);
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		tempVec = toWorld * glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1);
		tempVec = worldToCamera * tempVec;
		tempVec = normalizePerspective * tempVec;
		tempVec = viewportTrans * tempVec;
		tempVec = tempVec / tempVec[3];
		Window::drawPoint(tempVec[0], tempVec[1], tempVec[2], (int)scalePointAmount, normals[i].x, normals[i].y, normals[i].z);
	}
}

void OBJObject::rotate(glm::vec3 rotAxis, float deg)
{
	totalRot = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), rotAxis) * totalRot;
}

void OBJObject::setRotOffset()
{
	transFirst = !transFirst;
}

void OBJObject::setMatrial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	material.ambient = ambient;
	material.diffuse = diffuse;
	material.specular = specular;
	material.shininess = shininess;

}