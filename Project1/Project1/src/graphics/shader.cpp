#include "shader.h"

Shader::Shader(){}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success;
	char infoLog[512];

	GLuint vertextShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
	
	id = glCreateProgram();
	glAttachShader(id, vertextShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Link error " << infoLog << std::endl;
	}

	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate()
{
	glUseProgram(id);
}

std::string Shader::loadShaderSrc(const char* filepatch)
{
	std::ifstream file;
	std::stringstream buf;
	std::string ret = "";

	file.open(filepatch);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
		std::cout << "Open " << filepatch << std::endl;
	}
	else
	{
		std::cout << "File " << filepatch << " can't open" << std::endl;
	}
	file.close();

	return ret;
}

GLuint Shader::compileShader(const char* filepath, GLenum type)
{
	int success;
	char infoLog[512];

	GLuint ret = glCreateShader(type);

	std::string shaderSrc = loadShaderSrc(filepath);
	const GLchar* shader = shaderSrc.c_str();
	glShaderSource(ret, 1, &shader, NULL);
	glCompileShader(ret);

	glGetShaderiv(ret, type, &success);

	if (!success)
	{
		glGetShaderInfoLog(ret, 512, NULL, infoLog);
		std::cout << "Shader error " << infoLog << std::endl;
	}

	return ret;
}

void Shader::setMat4(const std::string& name, glm::mat4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setInt(const std::string& name, int val)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), val);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set3Float(const std::string& name, glm::vec3 v)
{
	set3Float(name, v.x, v.y, v.z);
}

void Shader::set3Float(const std::string& name, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}