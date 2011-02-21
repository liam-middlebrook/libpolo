#include "shader.h"

#include <iostream>

namespace glWrap {

static glInterface *glsl;

bool initGlsl(glInterfaceMode mode){
	glsl = glGetInterface(mode);
	return glsl != NULL;
}

glShader::glShader() : handle(0) {
}

glShader::glShader(shaderType _type) : handle(0) {
	if(!glsl) return;
	
	type = _type;
	
	if(type == vertexShader)
		handle = glsl->createVertexShader();
	else
		handle = glsl->createFragmentShader();
	
	if(!handle) std::cerr << "! Couldn't create Shader" << std::endl;
}
void glShader::compile(const std::string &source){
	if(!glsl || !handle) return;
	
	if(type == vertexShader)
		std::cerr << "# Compiling Vertex shader " << handle << std::endl;
	else
		std::cerr << "# Compiling Fragment shader " << handle << std::endl;
	
	glsl->shaderSource(handle, source);
	glsl->compileShader(handle);
	
	std::string log = glsl->infoLog(handle);
	if(log != "")
		std::cerr << log << std::endl;
}
void glShader::compileFile(const std::string &file){
	compile(readFile(file));
}
glShader::~glShader(){
	/* Nothing right now */
}

glVertexShader::glVertexShader(const std::string &source) : glShader(vertexShader) {
	if(source != "" && handle != 0)
		glShader::compile(source);
}

glFragmentShader::glFragmentShader(const std::string &source) : glShader(fragmentShader) {
	if(source != "" && handle != 0)
		glShader::compile(source);
}

glProgram::glProgram() : isLinked(false), handle(0) {
	create();
}
glProgram::~glProgram(){
	/* Nothing right now */
}

void glProgram::create(){
	if(!glsl || handle) return;
	
	handle = glsl->createProgram();
	
	if(!handle) std::cerr << "! Couldn't create Program" << std::endl;
}

void glProgram::link(){
	if(!glsl || !handle) return;
	
	if(isLinked) return;
	
	std::cerr << "# Linking program " << handle << std::endl;
	glsl->linkProgram(handle);
	
	isLinked = true;
	
	std::string log = glsl->infoLog(handle);
	if(log != "")
		std::cerr << log << std::endl;
}

void glProgram::activate(){
	if(!glsl || !handle) return;
	
	if(!isLinked) link();
	
	glsl->useProgram(handle);
}
void glProgram::deactivate(){
	glsl->useProgram(0);
}

void glProgram::attach(const std::string &name, glShader &shader){
	if(!glsl || !handle) return;
	
	if(shaders.find(name) != shaders.end())
		return;
	
	isLinked = false;
	
	shaders[name] = shader;
	
	glsl->attachShader(this->handle,shader.handle);
}
void glProgram::attachVertexShaderFile(const std::string &name, const std::string &file){
	if(!glsl || !handle) return;
	
	glVertexShader newShader(readFile(file));
	attach(name, newShader);
}
void glProgram::attachFragmentShaderFile(const std::string &name, const std::string &file){
	if(!glsl || !handle) return;
	
	glFragmentShader newShader(readFile(file));
	attach(name, newShader);
}
//void deattach(const std::string &name);

glShader &glProgram::operator[](const std::string &name){
	isLinked = false;
	
	return shaders[name];
}

void glProgram::set(const std::string &name, float v0){
	if(!glsl || !handle) return;
	
	glsl->uniform(handle, name, v0);
}
void glProgram::set(const std::string &name, float v0, float v1){
	if(!glsl || !handle) return;
	
	glsl->uniform(handle, name, v0, v1);
}
void glProgram::set(const std::string &name, float v0, float v1, float v2){
	if(!glsl || !handle) return;
	
	glsl->uniform(handle, name, v0, v1, v2);
}
void glProgram::set(const std::string &name, float v0, float v1, float v2, float v3){
	if(!glsl || !handle) return;
	
	glsl->uniform(handle, name, v0, v1, v2, v3);
}
void glProgram::set(const std::string &name, const std::vector<float> &vect){
	if(!glsl || !handle) return;
	
	glsl->uniform(handle, name, vect);
}

}

