#include "TreeGenerator.h"

TreeGenerator::TreeGenerator(std::string barkTexFilepath, std::string leafTexFilepath)
{
	barkTextureFile = barkTexFilepath;
	leafTextureFile = leafTexFilepath;
	srand(time(NULL));
	branch.init("c:\\Users\\Wilson\\Documents\\School\\CSE167\\PA4\\CSE167StarterCode-master\\cylinder2.obj");
	branch.heap = false;
	branch.addTexture(barkTexFilepath);
	leaf.init("c:\\Users\\Wilson\\Documents\\School\\CSE167\\PA4\\CSE167StarterCode-master\\leaf2.obj");
	leaf.heap = false;
}

TreeGenerator::~TreeGenerator()
{
}

Transform* TreeGenerator::generateTree(int levels)
{
	if (levels)
	{
		return createTree(levels, levels);
	}
	return NULL;
}

Transform* TreeGenerator::createTree(int level, int maxlevel)
{
	Transform* newBranch = new Transform();
	Transform* branchTransform = new Transform();
	newBranch->translate(0, POTENTIAL(TRUNKHEIGHT, level, maxlevel), 0);
	branchTransform->scaleAbs(POTENTIAL(TRUNKWIDTH, level, maxlevel), POTENTIAL(TRUNKHEIGHT, level, maxlevel), POTENTIAL(TRUNKWIDTH, level, maxlevel));
	branchTransform->addChild(&branch);
	newBranch->addChild(branchTransform);
	if(--level)
	{
		//left branch
		Transform* subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
		Transform* subBranchRotate = new Transform();
		subBranchRotate->translate(0, POTENTIAL(TRUNKHEIGHT * 0.6f, level, maxlevel), 0);
		subBranchRotate->rotate(Z_AXIS_VEC, 40 * RANDOMIZE(70, 80));
		subBranchRotate->rotate(Y_AXIS_VEC, 30 * RANDOMIZE(0, 100));
		subBranchRotate->addChild(subBranch);
		newBranch->addChild(subBranchRotate);

		//middle branch
		subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKHEIGHT, level + 1, maxlevel) / 2.0f, 0);
		newBranch->addChild(subBranch);
		newBranch->addChild(createTree(level, maxlevel));

		//right branch
		subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
		subBranchRotate = new Transform();
		subBranchRotate->translate(0, POTENTIAL(TRUNKHEIGHT * 0.2f, level, maxlevel), 0);
		subBranchRotate->rotate(Z_AXIS_VEC, -40 * RANDOMIZE(70, 80));
		subBranchRotate->rotate(Y_AXIS_VEC, 30 + 30 * RANDOMIZE(0, 100));
		subBranchRotate->addChild(subBranch);
		newBranch->addChild(subBranchRotate);

		//front right branch
		subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
		subBranchRotate = new Transform();
		subBranchRotate->translate(0, POTENTIAL(TRUNKHEIGHT * 0.2f, level, maxlevel), 0);
		subBranchRotate->rotate(Z_AXIS_VEC, -40 * RANDOMIZE(70, 80));
		subBranchRotate->rotate(Y_AXIS_VEC, -40 - 30 * RANDOMIZE(0, 100));
		subBranchRotate->addChild(subBranch);
		newBranch->addChild(subBranchRotate);
	}
	else
	{
		Transform* leaves = new Transform;
		leaves->addChild(&leaf);
		leaves->translate(0, POTENTIAL(TRUNKHEIGHT, level + 1, maxlevel), 0);
		leaves->rotate(X_AXIS_VEC, 90);
		newBranch->addChild(leaves);
	}
	return newBranch;
}
