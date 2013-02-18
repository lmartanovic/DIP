#include <iostream>		//cerr, endl
#include <stdexcept>	//runtime_exception

#include "Shader.h"
#include "utilities.h"

//! Constructor
Shader::Shader() {}

//! Destructor
Shader::~Shader()
{
	GLsizei count;		//number of actually attached shaders
	GLuint shaders[3];	//array of potential shader identifiers (vs, fs, gs)

	// Get all attached shaders
	glGetAttachedShaders(id, 3, &count, shaders);
	// Detach and delete all shaders
	for(int i = 0; i < count; i++)
	{
		glDetachShader(id, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	// Delete the program itself
	glDeleteProgram(id);
}

//! Creates a shader program object
void Shader::create()
{
	if((id = glCreateProgram()) == 0)
		checkOpenGLError();
}

//! Creates a shader object of given type from given source file
void Shader::addShader(shaderType type, const char * const file)
{
	//create shader
	GLuint shader;
	if((shader = glCreateShader(type)) == 0)
	{
		checkOpenGLError();
		throw std::runtime_error("Unable to create shader.");
	}

	//load source
	std::string source;
	source = loadFile(file);

	//add source to shader object
	const char* s = source.c_str();
	glShaderSource(shader, 1, &s, NULL);

	//compile
	glCompileShader(shader);
	//check status
	int compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE)
	{
		std::cerr << getShaderInfoLog(shader) << std::endl;
		throw std::runtime_error("Unable to compile shader.");
	}

	//attach to program
	glAttachShader(id, shader);
}

//! Links all attached shader and creates a complete shader program
void Shader::link()
{
	int linkStatus;
	//link the program
	glLinkProgram(id);
	// Check validity
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
	if(linkStatus != GL_TRUE)
	{
		std::cerr << getProgramInfoLog() << std::endl;
		throw std::runtime_error("Unable to link shader program.");
	}
}

//! Set as active shader program
void Shader::use()
{
	glUseProgram(id);
}

//! Shader program id getter
GLuint Shader::getId()
{
	return id;
}

//! Gets shader program information log
std::string Shader::getProgramInfoLog()
{
	int length;	//length of the log
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
	std::string log(length, ' ');
	glGetProgramInfoLog(id, length, NULL, &log[0]);
	return log;
}

//! Gets shader information log
std::string Shader::getShaderInfoLog(GLuint shader)
{
	int length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	std::string log(length, ' ');
	glGetShaderInfoLog(shader, length, NULL, &log[0]);
	return log;
}

//================= Uniform setters ==========================================
void setUniform(GLint uni, glm::mat4 & value)
{
	glUniformMatrix4fv(uni, 1, GL_FALSE, glm::value_ptr(value));
}

void setUniform(GLint uni, glm::mat3 & value)
{
	glUniformMatrix3fv(uni, 1, GL_FALSE, glm::value_ptr(value));
}

void setUniform(GLint uni, glm::vec3 & value)
{
	glUniform3fv(uni, 1, glm::value_ptr(value));
}

void setUniform(GLint uni, GLint value)
{
	glUniform1i(uni, value);
}

void setUniform(GLint uni, GLfloat value)
{
	glUniform1f(uni, value);
}