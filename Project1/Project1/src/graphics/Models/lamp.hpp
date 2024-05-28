#ifndef LAMP_HPP
#define LAMP_HPP
#include "cube.hpp"


class Lamp : public Cube
{
public :
	glm::vec3 lightColor;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Lamp(glm::vec3 lightColor, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, glm::vec3 size)
		: lightColor(lightColor), ambient(ambient), diffuse(diffuse), specular(specular),
		Cube(pos, size, Material::white_plastic)
	{
		
	}

	void render(Shader shader)
	{
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}
};

#endif // !LAMP_HPP
