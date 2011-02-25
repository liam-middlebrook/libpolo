#ifndef __SHADER_H__
#define __SHADER_H__

#include "glInterface.h"
#include <map>

namespace glWrap {

bool initGlsl(glInterfaceMode mode = glModeAny);

class glShader {
	public:
		typedef enum {
			vertexShader = 0,
			fragmentShader
		} shaderType;
	
	protected:
		glHandle handle;
		shaderType type;
	
	public:
		glShader();
		glShader(shaderType type);
		~glShader();
		
		void compile(const std::string &source);
		void compileFile(const std::string &file);
		
	friend class glProgram;
};

class glVertexShader : public glShader {
	public:
		glVertexShader(const std::string &source = "");
};

class glFragmentShader : public glShader {
	public:
		glFragmentShader(const std::string &source = "");
};

class glProgram {
	protected:
		bool isLinked;
		glHandle handle;
		
		std::map<std::string, glShader> shaders;
	
	public:
		glProgram();
		~glProgram();
		
		// handling
		void create();
		void link();
		
		// activation
		void activate();
		void deactivate();
		
		// shaders interaction
		void attach(const std::string &name, glShader &shader);
		void attachVertexShaderFile(const std::string &name, const std::string &file);
		void attachFragmentShaderFile(const std::string &name, const std::string &file);
		//void deattach(const std::string &name);
		glShader &operator[](const std::string &name);
		
		// uniform variables
		void set(const std::string &name, float v0);
		void set(const std::string &name, float v0, float v1);
		void set(const std::string &name, float v0, float v1, float v2);
		void set(const std::string &name, float v0, float v1, float v2, float v3);
		void set(const std::string &name, const std::vector<float> &vect);
};

}

#endif // __SHADER_H__
