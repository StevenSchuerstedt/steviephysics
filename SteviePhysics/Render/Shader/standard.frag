#version 330 core


out vec4 FragColor;

in vec3 color_out;

void main(){


  FragColor = vec4(color_out, 1.0f);
  
}
