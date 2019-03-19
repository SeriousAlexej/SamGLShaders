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
#ifndef ALEX_GLM_MAT3_H
#define ALEX_GLM_MAT3_H
#include "vec3.h"

namespace glm
  {
  struct mat3
    {
  mat3()
  {
  }
  
  mat3(float f)
  {
    for (size_t i = 0; i < 3; ++i)
      value[i][i] = f;
  }
    
    vec3& operator[](size_t i)
      {
      return value[i];
      }
    
  const vec3& operator[](size_t i) const
  {
    return value[i];
  }

    vec3 value[3];
    };
  
  static vec3 operator*(const mat3& m, const vec3& v)
  {
    return vec3(
      m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
      m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
      m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
  }
  
  static vec3 operator*(const vec3& v, const mat3& m)
  {
    return vec3(
      m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
      m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
      m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
  }
  
  static mat3 operator*(const mat3& m1, const mat3& m2)
  {
    float const SrcA00 = m1[0][0];
    float const SrcA01 = m1[0][1];
    float const SrcA02 = m1[0][2];
    float const SrcA10 = m1[1][0];
    float const SrcA11 = m1[1][1];
    float const SrcA12 = m1[1][2];
    float const SrcA20 = m1[2][0];
    float const SrcA21 = m1[2][1];
    float const SrcA22 = m1[2][2];

    float const SrcB00 = m2[0][0];
    float const SrcB01 = m2[0][1];
    float const SrcB02 = m2[0][2];
    float const SrcB10 = m2[1][0];
    float const SrcB11 = m2[1][1];
    float const SrcB12 = m2[1][2];
    float const SrcB20 = m2[2][0];
    float const SrcB21 = m2[2][1];
    float const SrcB22 = m2[2][2];

    mat3 Result(0.0f);
    Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
    Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
    Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
    Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
    Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
    Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
    Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
    Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
    Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
    return Result;
  }
  }

#endif
