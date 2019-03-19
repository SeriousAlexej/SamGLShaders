#version 330
layout(location = 0) in vec3 posModel;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normalModel;
layout(location = 3) in vec3 tangentModel;
layout(location = 4) in vec3 bitangentModel;
layout(location = 5) in vec2 maskUVMap;
out vec2 UV;
out vec2 maskUV;
out vec3 normal;
out mat3 TBN;
out vec3 eyePos;
out vec3 fragPos;
uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;
uniform vec3 lightDir;

void main()
{
  gl_Position = projMx * viewMx * vec4(posModel, 1.0);
  fragPos = vec3(posModel);
  UV = vertexUV;
  maskUV = maskUVMap;
  normal = normalModel;
  vec3 t = normalize((modelMx * vec4(tangentModel, 0.0)).xyz);
  vec3 b = normalize((modelMx * vec4(bitangentModel, 0.0)).xyz);
  vec3 n = normalize((modelMx * vec4(normalModel, 0.0)).xyz);
  TBN = mat3(t, b, n);
  eyePos = vec3(inverse(viewMx) * vec4(0.0, 0.0, 0.0, 1.0));
}