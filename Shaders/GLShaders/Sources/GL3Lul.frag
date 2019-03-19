#version 330
out vec4 color;

in vec2 UV;
in float viewerDistance;

uniform sampler2D lulTexture;
uniform float lulCenterX;
uniform float lulCenterY;

void main()
{
  float lulForce = 1.0 - clamp(viewerDistance / 10.0, 0.0, 1.0);
  vec2 uvLuld = UV;
  vec2 center = vec2(lulCenterX, lulCenterY);
  vec2 uvToCenter = center - uvLuld;
  float distanceToCenter = length(uvToCenter);
  uvLuld += lulForce * uvToCenter * 2.0 * pow(cos(distanceToCenter), 15.0);
  if (distance(uvLuld, UV) > distanceToCenter)
  {
    uvLuld = center;
  }
  color = texture(lulTexture, uvLuld).rgba;
}