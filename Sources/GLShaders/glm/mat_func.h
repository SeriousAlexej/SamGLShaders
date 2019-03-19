/*
    SamGLShaders - a free software for rendering Serious Sam models with OpenGL 3.3 shaders
    Copyright (C) 2019 Oleksii Sierov (seriousalexej@gmail.com)
	
    SamGLShaders is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    SamGLShaders is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with SamGLShaders.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ALEX_GLM_MATFUNCS_H
#define ALEX_GLM_MATFUNCS_H
#include <math.h>
#include "mat3.h"
#include "mat4.h"
#include "vec3.h"

namespace glm
{
  static mat3 transpose(const mat3& m)
  {
    mat3 result(0.0f);
    result[0][0] = m[0][0];
    result[0][1] = m[1][0];
    result[0][2] = m[2][0];

    result[1][0] = m[0][1];
    result[1][1] = m[1][1];
    result[1][2] = m[2][1];

    result[2][0] = m[0][2];
    result[2][1] = m[1][2];
    result[2][2] = m[2][2];
    return result;
  }
  
  static mat4 transpose(const mat4& m)
  {
    mat4 result(0.0f);
    result[0][0] = m[0][0];
    result[0][1] = m[1][0];
    result[0][2] = m[2][0];
    result[0][3] = m[3][0];

    result[1][0] = m[0][1];
    result[1][1] = m[1][1];
    result[1][2] = m[2][1];
    result[1][3] = m[3][1];

    result[2][0] = m[0][2];
    result[2][1] = m[1][2];
    result[2][2] = m[2][2];
    result[2][3] = m[3][2];

    result[3][0] = m[0][3];
    result[3][1] = m[1][3];
    result[3][2] = m[2][3];
    result[3][3] = m[3][3];
    return result;
  }
  
  static float clamp(float x, float minVal, float maxVal)
  {
    return min(max(x, minVal), maxVal);
  }
  
  static float dot(const vec3& x, const vec3& y)
  {
    vec3 tmp(x * y);
    return tmp.x + tmp.y + tmp.z;
  }
  
  static float dot(const vec4& x, const vec4& y)
  {
    vec4 tmp(x * y);
    return (tmp.x + tmp.y) + (tmp.z + tmp.w);
  }
    
  static vec3 normalize(const vec3& v)
  {
    return v / ::sqrtf(dot(v, v));
  }
  
  static vec4 normalize(const vec4& v)
  {
    return v / ::sqrtf(dot(v, v));
  }
  
  static vec3 cross(const vec3& x, const vec3& y)
  {
    return vec3(
      x.y * y.z - y.y * x.z,
      x.z * y.x - y.z * x.x,
      x.x * y.y - y.x * x.y);
  }

  static mat4 inverse(const mat4& m)
    {
      float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
      float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
      float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

      float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
      float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
      float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

      float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
      float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
      float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

      float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
      float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
      float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

      float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
      float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
      float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

      float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
      float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
      float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

      vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
      vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
      vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
      vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
      vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
      vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

      vec4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
      vec4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
      vec4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
      vec4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

      vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
      vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
      vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
      vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

      vec4 SignA(+1.0f, -1.0f, +1.0f, -1.0f);
      vec4 SignB(-1.0f, +1.0f, -1.0f, +1.0f);
      mat4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

      vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

      vec4 Dot0(m[0] * Row0);
      float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

      float OneOverDeterminant = 1.0f / Dot1;

      return Inverse * OneOverDeterminant;
    }
  
  static mat4 perspective(float fovy, float aspect, float zNear, float zFar)
  {
    float const tanHalfFovy = ::tan(fovy * 0.5f);

    mat4 Result(0.0f);
    Result[0][0] = 1.0f / (aspect * tanHalfFovy);
    Result[1][1] = 1.0f / (tanHalfFovy);
    Result[2][3] = -1.0f;
    Result[2][2] = - (zFar + zNear) / (zFar - zNear);
    Result[3][2] = - (2.0f * zFar * zNear) / (zFar - zNear);

    return Result;
  }
}

#endif
