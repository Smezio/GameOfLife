#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	/*
	*	1. Extract shaders from files
	*/
	string vertexSource, fragmentSource;
	ifstream vShaderFile, fShaderFile;

	// ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}

	const char* vShaderSource = vertexSource.c_str();
	const char* fShaderSource = fragmentSource.c_str();

	/*
	*	2. Create and compile shaders
	*/
	unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexID, 1, &vShaderSource, NULL);
	glCompileShader(vertexID);
	checkCompilation(vertexID);
	glShaderSource(fragmentID, 1, &fShaderSource, NULL);
	glCompileShader(fragmentID);
	checkCompilation(fragmentID);

	/*
	*	3. Attach shaders and link program
	*/
	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);
	checkProgram(programID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void Shader::use()
{
	glUseProgram(programID);
}

int Shader::checkCompilation(unsigned int& shader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::COMPILER\n" << infoLog << endl;
		return -1;
	}

	return 0;
}

int Shader::checkProgram(unsigned int& program)
{
	int success;
	char infoLog[512];
	glGetShaderiv(program, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::PROGRAM::LINKING\n" << infoLog << endl;
		return -1;
	}

	return 0;
}
