#version 460

layout(location=0) in vec3 position;
layout(location=1) in float density;

uniform mat4 view;
uniform mat4 projection;

out vec3 color;

vec3 ValueToColor(float value)
{
	vec3 color;
	if (3*value<1)
	{
		color.r=0.0f;
		color.g=3.0f*value;
		color.b=1.0f;
	}
	else if (3*value<2)
	{
		color.r=3.0f*value-1.0f;
		color.g=1.0f;
		color.b=-3.0f*value+2.0f;
	}
	else
	{
		color.r=1.0f;
		color.g=-3.0f*value+3.0f;
		color.b=0.0f;
	}
	return color;
}

void main()
{
	gl_PointSize=4;
	gl_Position=projection*view*vec4(position,1.0f);
	color=ValueToColor((density-500.0f)/2000.0f);
}