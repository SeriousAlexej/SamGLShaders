#version 330
out vec4 color;
in vec2 UV;
in vec2 maskUV;
in vec3 normal;
in mat3 TBN;
in vec3 eyePos;
in vec3 fragPos;
uniform float time;
uniform float speed;
uniform vec3 colLight;
uniform float specularStrength;
uniform float reflectiveStrength;
uniform float dudvStretchU;
uniform float dudvStretchV;
uniform float dudvStrength;
uniform float opacity;
uniform samplerCube reflectionTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalTexture;
uniform sampler2D maskTexture;
uniform vec3 lightDir;

void main()
{
  vec3 L = normalize(lightDir);
  vec3 viewDir = normalize(fragPos - eyePos);
  vec2 distortedUV = texture(dudvTexture, vec2((UV.x + time*speed)*dudvStretchU, UV.y*dudvStretchV)).xy * 0.1;
  distortedUV = vec2(UV.x*dudvStretchU, UV.y*dudvStretchV) + vec2(distortedUV.x * dudvStretchU, (distortedUV.y + time * speed)*dudvStretchV);
  vec3 distortion = (texture(dudvTexture, distortedUV).xyz * 2.0 - 1.0) * dudvStrength;
  vec3 normalSample = texture(normalTexture, distortedUV).xyz;
  vec3 n = TBN * (2.0 * normalSample - 1.0);
  n = normalize(2.0 * normal - n);
  float dirCos = clamp(dot(n, L), 0.0, 1.0);
  vec3 specular = colLight * specularStrength * pow(max(0.0, dot(reflect(L, n), viewDir)), 20.0);
  vec3 reflected = reflect(viewDir, normal + distortion);
  vec3 refracted = refract(viewDir, normal + distortion, 0.8);
  float refractionFactor = pow(dot(-viewDir, normal), reflectiveStrength); 
  vec3 colDiffuse = mix(texture(reflectionTexture, reflected).rgb, texture(reflectionTexture, refracted).rgb, refractionFactor);
  float alpha = opacity * (1.0 - refractionFactor / 10.0);
  color = vec4(colDiffuse + dirCos * specular, alpha * texture(maskTexture, maskUV).r);
}