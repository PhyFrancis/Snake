#version 330

in vec4 Color;
uniform vec4 gColor;
out vec4 FragColor;

void main()
{
    /*FragColor = vec4(1.0, 0.0, 0.0, 1.0);*/
    FragColor = Color;
}
