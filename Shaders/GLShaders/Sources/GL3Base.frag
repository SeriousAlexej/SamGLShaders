#version 330
out vec4 color;

in vec2 UV;
in vec3 normal;

uniform vec3 colLight;
uniform vec3 colAmbient;
uniform sampler2D baseTexture;
uniform bool fullbright;
uniform bool smoothLight;
uniform vec4 baseColor;
uniform vec3 lightDir;

void main()
{
  vec4 colDiffuse = vec4(texture(baseTexture, UV).rgb, 1.0);
  colDiffuse = colDiffuse * baseColor;
  if (fullbright)
  {
    color = colDiffuse;
  }
  else
  {
    vec3 lightDirection = lightDir;
	if (smoothLight)
	{
	  lightDirection = normalize(lightDirection);
	}
    float dirCos = clamp(dot(normal, lightDirection), 0.0, 1.0);
    color = colDiffuse * vec4(colAmbient + dirCos*colLight, 1.0);
  }
}