#include <iostream>
#include "polo.h"
#include "shader.h"

using namespace glWrap;
using namespace std;

typedef struct {
	Image image;
	glProgram program;
} demoData;

void draw(void *userData)
{	
	demoData *data = (demoData *)userData;
	
	clearScreen();
	
	if( getKey() == POLO_ESC )
		exitPolo();
	
	setPenColor(POLO_TRANSPARENT);
	setFillColor(POLO_WHITE);
	setTexture(data->image);
	drawQuad( 10, 10,
	          -10 + getScreenWidth(), 10,
	          -10 + getScreenWidth(), -10 + getScreenHeight(),
	          10, -10 + getScreenHeight() );
	
	data->program.set("resolution", getScreenWidth(), getScreenHeight());
	data->program.set("time", getTime());
	
	if(isMouseButtonPressed(0))
		data->program.set("mouse", getMouseX(), getMouseY());
}

void loadShaders(glProgram &program){
	program.create();
	
	program.attachVertexShaderFile("v", "shaders/shader.vert");
	program.attachFragmentShaderFile("f", "shaders/shader.frag");
	
	program.activate();
}

int main(int argc, char *argv[])
{
	demoData data;
	
	initPolo(512, 384, 0, "Shader Example");
	
	if(!initGlsl()) return 1;
	
	data.image = loadImage("text4.bmp");
	loadShaders(data.program);
	
	//hideMousePointer();
	
	setDrawCallback(draw);
	setPoloUserData(&data);
	
	runPolo();
}
