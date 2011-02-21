#include "glInterface.h"

#include <GL/glew.h>
#include <iostream>
#include <map>

#include <fstream>    // just for reading
#include <streambuf>  // text files

namespace glWrap {

class glInterfaceARB : public glInterface {
	private:
		// glHandle managing map
		typedef std::map<glHandle,GLhandleARB> handlesMap;
		handlesMap handles;
		
		glHandle getFreeHandle(){
			for(glHandle hnd = 1; hnd != 0; hnd++)
				if(!isHandle(hnd))
					return hnd;
			return 0;
		}
		bool isHandle(glHandle hnd){
			return handles.find(hnd) != handles.end();
		}
		void freeHandle(glHandle &hnd){
			handles.erase(hnd);
			hnd = 0;
		}
		bool glIsValidHandle(GLhandleARB hnd){
			return hnd != 0 && hnd != GL_INVALID_ENUM && hnd != GL_INVALID_OPERATION;
		}
		bool glIsValidLocation(GLint loc){
			return loc != GL_INVALID_VALUE && loc != GL_INVALID_OPERATION;
		}
		
	public:
		glHandle createVertexShader(){
			glHandle hnd = getFreeHandle();
			if(!hnd) return hnd;
			
			handles[hnd] = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
			
			if(!glIsValidHandle(handles[hnd])) freeHandle(hnd);
			return hnd;
		}
		glHandle createFragmentShader(){
			glHandle hnd = this->getFreeHandle();
			if(!hnd) return hnd;
			
			handles[hnd] = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
			
			if(!glIsValidHandle(handles[hnd])) freeHandle(hnd);
			return hnd;
		}
		void shaderSource(glHandle shader, const std::string &source){
			if(!isHandle(shader)) return;
			const char *src = &source[0];
			
			glShaderSourceARB(handles[shader], 1, &src,NULL);
		}
		void compileShader(glHandle shader){
			if(!isHandle(shader)) return;
			
			glCompileShaderARB(handles[shader]);
		}
		glHandle createProgram(){
			glHandle hnd = getFreeHandle();
			if(!hnd) return 0;
			
			handles[hnd] = glCreateProgramObjectARB();
			
			if(!glIsValidHandle(handles[hnd])) freeHandle(hnd);
			return hnd;
		}
		void attachShader(glHandle program, glHandle shader){
			if(!isHandle(program) || !isHandle(shader)) return;
			
			glAttachObjectARB(handles[program], handles[shader]);
		}
		void linkProgram(glHandle program){
			if(!isHandle(program)) return;
			
			glLinkProgramARB(handles[program]);
		}
		void useProgram(glHandle program){
			if(!isHandle(program)) return;
			
			glUseProgramObjectARB(handles[program]);
		}
		std::string infoLog(glHandle hnd){
			if(!isHandle(hnd)) return "";
			
			std::string log;
			int infologLength = 0;
			int charsWritten  = 0;
			char *infolog;
	
			glGetObjectParameterivARB(handles[hnd], GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);
	
			if (infologLength > 0){
				infolog = new char[infologLength];
				
				glGetInfoLogARB(handles[hnd], infologLength, &charsWritten, infolog);
				
				log = infolog;
				delete[] infolog;
			}
			
			return log;
		}
		void uniform(glHandle program, const std::string &name, float v0){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocationARB(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform1fARB(location, v0);
		}
		void uniform(glHandle program, const std::string &name, float v0, float v1){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocationARB(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform2fARB(location, v0, v1);
		}
		void uniform(glHandle program, const std::string &name, float v0, float v1, float v2){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocationARB(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform3fARB(location, v0, v1, v2);
		}
		void uniform(glHandle program, const std::string &name, float v0, float v1, float v2, float v3){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocationARB(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform4fARB(location, v0, v1, v2, v3);
		}
		void uniform(glHandle program, const std::string &name, const std::vector<float> &vect){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocationARB(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			GLfloat *vecf = new GLfloat[vect.size()];
			
			for(int i=0; i<vect.size(); i++)
				vecf[i] = vect[i];
			
			glUniform1fvARB(location, vect.size(), vecf);
			
			delete[] vecf;
		}
};

class glInterfaceOGL : public glInterface {
	private:
		typedef std::map<glHandle,GLuint> handlesMap;
		handlesMap handles;
		
		glHandle getFreeHandle(){
			for(glHandle hnd = 1; hnd != 0; hnd++)
				if(!isHandle(hnd))
					return hnd;
			return 0;
		}
		bool isHandle(glHandle hnd){
			return handles.find(hnd) != handles.end();
		}
		void freeHandle(glHandle &hnd){
			handles.erase(hnd);
			hnd = 0;
		}
		bool glIsValidLocation(GLint loc){
			return loc != GL_INVALID_VALUE && loc != GL_INVALID_OPERATION;
		}
		
	public:
		glHandle createVertexShader(){
			glHandle hnd = getFreeHandle();
			if(!hnd) return hnd;
			
			handles[hnd] = glCreateShader(GL_VERTEX_SHADER);
			
			if(glIsShader(handles[hnd]) == GL_FALSE) freeHandle(hnd);
			return hnd;
		}
		glHandle createFragmentShader(){
			glHandle hnd = this->getFreeHandle();
			if(!hnd) return hnd;
			
			handles[hnd] = glCreateShader(GL_FRAGMENT_SHADER);
			
			if(glIsShader(handles[hnd]) == GL_FALSE) freeHandle(hnd);
			return hnd;
		}
		void shaderSource(glHandle shader, const std::string &source){
			if(!isHandle(shader)) return;
			const char *src = &source[0];
			
			glShaderSource(handles[shader], 1, &src,NULL);
		}
		void compileShader(glHandle shader){
			if(!isHandle(shader)) return;
			
			glCompileShader(handles[shader]);
		}
		glHandle createProgram(){
			glHandle hnd = getFreeHandle();
			if(!hnd) return 0;
			
			handles[hnd] = glCreateProgram();
			
			if(glIsProgram(handles[hnd]) == GL_FALSE) freeHandle(hnd);
			return hnd;
		}
		void attachShader(glHandle program, glHandle shader){
			if(!isHandle(program) || !isHandle(shader)) return;
			
			glAttachShader(handles[program], handles[shader]);
		}
		void linkProgram(glHandle program){
			if(!isHandle(program)) return;
			
			glLinkProgram(handles[program]);
		}
		void useProgram(glHandle program){
			if(!isHandle(program)) return;
			
			glUseProgram(handles[program]);
		}
		std::string infoLog(glHandle hnd){
			if(!isHandle(hnd)) return "";
			
			std::string log;
			int infologLength = 0;
			int charsWritten  = 0;
			char *infolog;
			
			if(glIsProgram(handles[hnd]) != GL_FALSE)
				glGetProgramiv(handles[hnd], GL_INFO_LOG_LENGTH, &infologLength);
			else
				glGetShaderiv(handles[hnd], GL_INFO_LOG_LENGTH, &infologLength);
			
			if (infologLength > 0) {
				infolog = new char[infologLength];
				
				if(glIsProgram(handles[hnd]) != GL_FALSE)
					glGetProgramInfoLog(handles[hnd], infologLength, &charsWritten, infolog);
				else
					glGetShaderInfoLog(handles[hnd], infologLength, &charsWritten, infolog);
				
				log = infolog;
				delete[] infolog;
			}
			
			return log;
		}
		void uniform(glHandle program, const std::string &name, float v0){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocation(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform1f(location, v0);
		}
		void uniform(glHandle program, const std::string &name, float v0, float v1){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocation(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform2f(location, v0, v1);
		}
		void uniform(glHandle program, const std::string &name, float v0, float v1, float v2){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocation(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform3f(location, v0, v1, v2);
		}
		void uniform(glHandle program, const std::string &name, float v0, float v1, float v2, float v3){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocation(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			glUniform4f(location, v0, v1, v2, v3);
		}
		void uniform(glHandle program, const std::string &name, const std::vector<float> &vect){
			if(!isHandle(program)) return;
			
			GLint location = glGetUniformLocation(handles[program], name.c_str());
			
			if(!glIsValidLocation(location)) return;
			
			GLfloat *vecf = new GLfloat[vect.size()];
			
			for(int i=0; i<vect.size(); i++)
				vecf[i] = vect[i];
			
			glUniform1fv(location, vect.size(), vecf);
			
			delete[] vecf;
		}
};

static glInterfaceARB glARB;
static glInterfaceOGL glOGL;

glInterface *glGetInterface(glInterfaceMode mode){
	glewInit();
	
	if ((mode & glModeOGL) && glewIsSupported("GL_VERSION_2_0")){
		std::cerr << "# Using OpenGL 2.0" << std::endl;
		return &glOGL;
	}
	
	if ((mode & glModeARB) && GL_ARB_vertex_shader && GL_ARB_fragment_shader){
		std::cerr << "# Using OpenGL ARB" << std::endl;
		return &glARB;
	}
	
	std::cerr << "! OpenGL Shader Language Not Supported" << std::endl;
	return NULL;
}
                 
std::string readFile(const std::string &file){
	std::ifstream t(file.c_str());
	std::string str( (std::istreambuf_iterator<char>(t)) ,
                      std::istreambuf_iterator<char>( )  );
    return str;
}

}

