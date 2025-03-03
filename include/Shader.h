#ifndef SHADER_H
#define SHADER_H

#include <string>

using namespace std;

#pragma once
class Shader
{
public:
	unsigned int programID; // program ID

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;

	int checkCompilation(unsigned int& id);
	int checkProgram(unsigned int& id);
};

#endif
