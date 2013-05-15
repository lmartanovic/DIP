/*! \file Shader.h */
#ifndef SHADER_H
#define SHADER_H
//STL
#include <string>
//OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
/*!
 * \enum ShaderType
 * \brief Simplifies OpenGL shader type naming
*/
typedef enum ShaderType
{
	VS = GL_VERTEX_SHADER,
	FS = GL_FRAGMENT_SHADER,
	GS = GL_GEOMETRY_SHADER
} shaderType;

/*! 
 * \class Shader
 * \brief Shader Program (effect) class.
*/
class Shader
{
public:
	//! Constructor
	Shader();
	//! Destructor
	~Shader();
	//! Creates a shader program object
	void create();
	//! Creates a shader object of given type from given source file
	void addShader(shaderType type, const char * const file);
	//! Links all attached shader and creates a complete shader program
	void link();
	//! Set as active shader program
	void use();
	//! Uniform initialisation - program specific - needs to be overriden
	virtual void initUniforms() {};
	//! Shader program id getter
	GLuint getId();
	//! Gets shader program information log
	std::string getProgramInfoLog();
	//! Gets shader information log
	std::string getShaderInfoLog(GLuint shader);
protected:
	GLuint id;	/*!< GL object identifier */
};

/*! \fn setUniform
	\brief Various data type uniform variable setter.
	\param uni Uniform object identifier.
	\param value Value to be assigned.

	mat4, mat3, vec3, float and int implemented so far.
*/
void setUniform(GLint uni, glm::mat4 & value);
void setUniform(GLint uni, glm::mat3 & value);
void setUniform(GLint uni, glm::vec3 & value);
void setUniform(GLint uni, glm::vec2 & value);
void setUniform(GLint uni, GLint value);
void setUniform(GLint uni, GLfloat value);

#endif