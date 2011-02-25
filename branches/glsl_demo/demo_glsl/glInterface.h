#ifndef __GLINTERFACE_H__
#define __GLINTERFACE_H__

#include <string>
#include <vector>

namespace glWrap {

// glInterface
typedef unsigned int glHandle;

class glInterface {
	public:
		// shaders
		virtual glHandle    createVertexShader() = 0;
		virtual glHandle    createFragmentShader() = 0;
		virtual void        shaderSource(glHandle shader, const std::string &source) = 0;
		virtual void        compileShader(glHandle shader) = 0;
		
		// programs
		virtual glHandle    createProgram() = 0;
		virtual void        attachShader(glHandle program, glHandle shader) = 0;
		virtual void        linkProgram(glHandle program) = 0;
		virtual void        useProgram(glHandle program) = 0;
		
		// logging
		virtual std::string infoLog(glHandle program) = 0;
		
		// uniform variables
		virtual void        uniform(glHandle program, const std::string &name, float v0) = 0;
		virtual void        uniform(glHandle program, const std::string &name, float v0, float v1) = 0;
		virtual void        uniform(glHandle program, const std::string &name, float v0, float v1, float v2) = 0;
		virtual void        uniform(glHandle program, const std::string &name, float v0, float v1, float v2, float v3) = 0;
		virtual void        uniform(glHandle program, const std::string &name, const std::vector<float> &vect) = 0;
};

// glGetInterface & readFile
typedef enum {
	glModeARB = 1 << 0,
	glModeOGL = 1 << 1,
	glModeAny = glModeARB | glModeOGL
} glInterfaceMode;

glInterface *glGetInterface(glInterfaceMode mode = glModeAny);

std::string readFile(const std::string &file);

}

#endif // __GLINTERFACE_H__
