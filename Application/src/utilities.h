/******************************************************************************
* DIP - Real-Time Illumination of a Scene - utilities.h                       *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Text file loading.                                                        *
* - OpenGL Error checking.                                                    *
*                                                                             *
*******************************************************************************
* Author                                                                      *
* ------                                                                      *
* Lukáš Martanovič (xmarta00@stud.fit.vutbr.cz)                               *
*                                                                             *
* 18.05.2013                                                                  *
*                                                                             *
*******************************************************************************
* This software is not copyrighted.                                           *
*                                                                             *
* This source code is offered for use in the public domain.                   *
* You may use, modify or distribute it freely.                                *
*                                                                             *
******************************************************************************/

/*! \file utilities.h */
#ifndef UTILITIES_H
#define UTILITIES_H
//STL
#include <fstream>
#include <iterator>
#include <string>

//! Text file loader
std::string loadFile(const char * const file)
{
    std::ifstream stream(file);	//input file stream
    //return a string filled by an iterator without skipping whites
    return std::string(std::istream_iterator<char>(stream >> std::noskipws),
                       std::istream_iterator<char>());
}

//! Error getter
void checkOpenGLError()
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
	    switch(err)
	    {
	    case GL_NO_ERROR:
	        std::cout <<
	                "GL_NO_ERROR" << std::endl <<
	                "No error has been recorded." << std::endl;
	        break;
	    case GL_INVALID_ENUM:
	        std::cerr <<
	                  "GL_INVALID_ENUM" << std::endl <<
	                  "An unacceptable value is specified for an enumerated argument." << std::endl <<
	                  "The offending command is ignored" << std::endl <<
	                  "and has no other side effect than to set the error flag." <<     std::endl;
	        break;
	    case GL_INVALID_VALUE:
	        std::cerr <<
	                  "GL_INVALID_VALUE" << std::endl <<
	                  "A numeric argument is out of range." << std::endl <<
	                  "The offending command is ignored" << std::endl <<
	                  "and has no other side effect than to set the error flag." << std::endl;
	        break;
	    case GL_INVALID_OPERATION:
	        std::cerr <<
	                  "GL_INVALID_OPERATION" << std::endl <<
	                  "The specified operation is not allowed in the current state." << std::endl <<
	                  "The offending command is ignored" << std::endl <<
	                  "and has no other side effect than to set the error flag." << std::endl;
	        break;
	    case GL_INVALID_FRAMEBUFFER_OPERATION:
	        std::cerr <<
	                  "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl <<
	                  "The framebuffer object is not complete. The offending command" << std::endl <<
	                  "is ignored and has no other side effect than to set the error flag." << std::endl;
	        break;
	    case GL_OUT_OF_MEMORY:
	        std::cerr <<
	                  "GL_OUT_OF_MEMORY" << std::endl <<
	                  "There is not enough memory left to execute the command." << std::endl <<
	                  "The state of the GL is undefined," << std::endl <<
	                  "except for the state of the error flags," << std::endl <<
	                  "after this error is recorded." << std::endl;
	        break;
	    }//switch
    }//while loop
}

#endif