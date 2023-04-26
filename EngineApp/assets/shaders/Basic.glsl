#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 vertexColor;

//out vec2 v_TexCoord;
out vec3 fragmentColor;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position, 1);
	//v_TexCoord = texCoord;
	fragmentColor = vertexColor;
};

#shader fragment
#version 330 core

//layout(location = 0) out vec4 color;
in vec3 fragmentColor;
out vec4 color;
//in vec2 v_TexCoord;

uniform vec4 u_Color; // "u_"´ú±ú’niform±äÁ¿
uniform sampler2D u_Texture;

void main()
{
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	//color = texColor;
	//color = u_Color;
	color = vec4(fragmentColor, 1);
};