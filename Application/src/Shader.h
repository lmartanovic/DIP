/*! \file Shader.h */
//STL
#include <string>
//OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
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
	//! Creates a shader object of given type from given source file
	void addShader(shaderType type, const char * const file);
	//! Links all attached shader and creates a complete shader program
	void link();
	//! Set as active shader program
	void use();
	//! Uniform initialisation - shader program specific
	virtual void initUniforms() = 0;
	//! Shader program id getter
	GLuint getId();
	//! Gets shader program information log
	std::string getProgramInfoLog();
	//! Gets shader information log
	std::string getShaderInfoLog(GLuint shader);
private:
	GLuint id;	/*!< GL object identifier */
};