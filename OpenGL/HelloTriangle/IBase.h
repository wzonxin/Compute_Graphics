#pragma once
#include "iostream"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stdio.h"
#include "Shader.h"


//#ifndef STB_IMAGE_IMPLEMENTATION
	//#define STB_IMAGE_IMPLEMENTATION
	//#include "stb_image.h"
//#endif //

class BaseRef
{
public:
	virtual void Init();
	virtual void MainLoop(GLFWwindow* windows);
};
