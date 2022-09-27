﻿#pragma once
#include <iostream>
#include "window.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <SFML/Graphics/Image.hpp>



class Engine
{
public:
	Engine();

	void ClearBuffers();

	void Drawning(int x, int y){
		glViewport(0, 0, x, y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};
