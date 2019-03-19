#version 330
out vec4 color;

in vec3 normal;
in vec3 eyePos;
in vec3 fragPos;

uniform samplerCube reflectionTexture;

void main()
{
  vec3 viewDir = normalize(fragPos - eyePos);
  vec3 reflected = reflect(viewDir, normal);
  vec3 colDiffuse = texture(reflectionTexture, reflected).rgb;
  color = vec4(colDiffuse, 1.0);
}