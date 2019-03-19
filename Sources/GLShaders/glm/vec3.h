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
#ifndef ALEX_GLM_VEC3_H
#define ALEX_GLM_VEC3_H

namespace glm
  {
  struct vec3
    {
  vec3()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
  
  vec3(const vec3& v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  
  vec3(float ix, float iy, float iz)
  {
    x = ix;
    y = iy;
    z = iz;
  }

  vec3(float f)
  {
    x = f;
    y = f;
    z = f;
  }

  vec3& operator*=(float f)
  {
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  vec3& operator/=(float f)
  {
    x /= f;
    y /= f;
    z /= f;
    return *this;
  }

  vec3& operator*=(const vec3& v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  vec3& operator/=(const vec3& v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  vec3& operator+=(const vec3& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  vec3& operator-=(const vec3& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  float& operator[](size_t i)
    {
    return (&x)[i];
    }

  const float& operator[](size_t i) const
    {
    return (&x)[i];
    }

  static size_t length()
  {
    return 3;
  }

    float x, y, z;
    };
  
  static vec3 operator*(const vec3& a, float f)
  {
    return vec3(a) *= f;
  }
  
  static vec3 operator*(float f, const vec3& a)
  {
    return vec3(a) *= f;
  }
  
  static vec3 operator/(const vec3& a, float f)
  {
    return vec3(a) /= f;
  }
  
  static vec3 operator/(float f, const vec3& v)
  {
    return vec3(f) /= v;
  }
  
  static vec3 operator*(const vec3& a, const vec3& b)
  {
    return vec3(a) *= b;
  }
  
  static vec3 operator/(const vec3& a, const vec3& b)
  {
    return vec3(a) /= b;
  }
  
  static vec3 operator+(const vec3& a, const vec3& b)
  {
    return vec3(a) += b;
  }
  
  static vec3 operator-(const vec3& a, const vec3& b)
  {
    return vec3(a) -= b;
  }
  }

#endif
