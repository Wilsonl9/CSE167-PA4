#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include<string>
#include<time.h>
#include<unordered_map>
#include<cmath>
#include"Transform.h"
#include"Geometry.h"

#define MAXHEIGHT 10
#define MINHEIGHT 0
#define VARIANCE 15.0f
#define RETAIN 0.48f
#define RANDOMIZE(min,max) (((float)(rand() % ((max) - (min) + 1))) + ((float)(min)))
#define PLUSMINUS(x) (((2.0f * ((float)(x))) * (((float)((rand() % 100) + 1.0f)) / 100.0f)) - ((float)(x)))
#define MEAN4(x1,x2,x3,x4) (((float)((x1) + (x2) + (x3) + (x4))) / 4.0f)
#define MEAN3(x1,x2,x3) (((float)((x1) + (x2) + (x3))) / 3.0f)

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();
	Geometry* generateTerrain(int N);

private:
	std::unordered_map<float, std::unordered_map<float, float>> mat;
	float* mat2;
	Geometry* createTerrain(int N);
	void calculateVerticesAndIndices(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, int N, int levels);
};

#endif
