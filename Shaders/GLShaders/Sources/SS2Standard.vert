#version 330
layout(location = 0) in vec3 posModel;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normalModel;
layout(location = 3) in vec3 tangentModel;
layout(location = 4) in vec3 bitangentModel;

out vec2 UV;
out vec3 normal;
out mat3 TBN;
out vec3 refl;
out mat4 viewMatrix;
out vec3 viewDir;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

void main()
{
  gl_Position = projMx * viewMx * vec4(posModel, 1.0);
  UV = vertexUV;
  
  normal = (modelMx * vec4(normalModel, 0.0)).xyz;
  vec3 t = normalize((modelMx * vec4(tangentModel, 0.0)).xyz);
  vec3 b = normalize((modelMx * vec4(bitangentModel, 0.0)).xyz);
  vec3 n = normalize((modelMx * vec4(normalModel, 0.0)).xyz);
  TBN = mat3(t, b, n);
  
  refl = normalize(vec3(viewMx * vec4(posModel, 1.0)));
  viewMatrix = viewMx;
  viewDir = normalize((inverse(viewMx) * vec4(0.0, 0.0, -1.0, 0.0)).xyz);
}