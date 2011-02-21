// gl_Vertex
// gl_ModelViewProjectionMatrix

void main()
{
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
