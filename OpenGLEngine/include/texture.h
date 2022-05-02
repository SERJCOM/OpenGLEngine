#pragma once
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <GL/glew.h>
#include "object.h"

class Texture {
public:
	unsigned int texture;
	unsigned int skyboxID = 0;

	void LoadSkyBox() {
		sf::Image skybox[6];
		glGenTextures(1, &skyboxID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);

		if (!skybox[0].loadFromFile("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/skybox1.jpg")) {
			std::cout << "failed to load the texture" << std::endl;
		}
		skybox[1].loadFromFile("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/skybox2.jpg");
		skybox[2].loadFromFile("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/skybox3.jpg");
		skybox[3].loadFromFile("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/skybox4.jpg");
		skybox[4].loadFromFile("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/skybox5.jpg");
		skybox[5].loadFromFile("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/skybox6.jpg");

		for (int i = 0; i < 6; i++) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, skybox[i].getSize().x, skybox[i].getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox[i].getPixelsPtr()
			);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

	void LoadTexture(std::string name) {
		sf::Image texture_im;
		if (!texture_im.loadFromFile(("D:/prog/������� VISUAL STUDIO/OpenGLEngine/OpenGLEngine/textures/" + name).c_str())) {
			std::cout << "failed to load the texture" << std::endl;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_im.getSize().x, texture_im.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_im.getPixelsPtr());
	}

};
