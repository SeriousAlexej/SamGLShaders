#version 330
out vec4 color;

in vec2 UV;
in vec3 normal;
in mat3 TBN;
in vec3 refl;
in mat4 viewMatrix;
in vec3 viewDir;

uniform vec3 colLight;
uniform vec3 colAmbient;
uniform float normalStrength;
uniform float reflectionStrength;
uniform float specularStrength;
uniform float specularFactor;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D reflectionTexture;
uniform vec3 lightDir;

void main()
{
  vec3 L = normalize(lightDir);
  vec3 colN = texture(normalTexture, UV).xyz * 2.0 - vec3(1.0);
  vec3 n = normalize((1.0 + normalStrength) * normal - TBN * colN * normalStrength);
  
  vec3 reflPos = reflect(refl, vec3(viewMatrix * vec4(n, 0.0)));
  float m = 2.0 * sqrt(pow(reflPos.x, 2.0) + pow(reflPos.y, 2.0) + pow(reflPos.z + 1.0, 2.0));
  vec2 reflUV = reflPos.xy / m + 0.5;
  float dirCos = clamp(dot(n, L), 0.0, 1.0);
  vec3 reflectionColor = reflectionStrength * texture2D(reflectionTexture, reflUV).rgb;
  reflectionColor = 0.3*colAmbient*reflectionColor + 0.7*reflectionColor*dirCos;
  
  vec3 specularReflection = colLight * specularStrength * pow(max(0.0, dot(reflect(L, n), viewDir)), specularFactor);
  
  vec4 colDiffuse = texture(diffuseTexture, UV).rgba;
  color = vec4(reflectionColor + colDiffuse.rgb*(colAmbient + dirCos*(colLight + specularReflection)), colDiffuse.a);
}