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
#ifndef ALEX_GLM_VEC2_H
#define ALEX_GLM_VEC2_H

namespace glm
  {
  struct vec2
    {
  vec2()
  {
    x = 0.0f;
    y = 0.0f;
  }
  
  vec2(const vec2& v)
  {
    x = v.x;
    y = v.y;
  }
  
  vec2(float ix, float iy)
  {
    x = ix;
    y = iy;
  }
  
  vec2(float f)
  {
    x = f;
    y = f;
  }
  
  vec2& operator*=(float f)
  {
    x *= f;
    y *= f;
    return *this;
  }
  
  vec2& operator/=(float f)
  {
    x /= f;
    y /= f;
    return *this;
  }
  
  vec2& operator*=(const vec2& v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }
  
  vec2& operator/=(const vec2& v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }
  
  vec2& operator+=(const vec2& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
  
  vec2& operator-=(const vec2& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }
    
    float& operator[](size_t i)
      {
      return (&x)[i];
      }
    
    float operator[](size_t i) const
      {
      return (&x)[i];
      }
    
  static size_t length()
  {
    return 2;
  }

    float x, y;
    };
  
  static vec2 operator*(const vec2& a, float f)
  {
    return vec2(a) *= f;
  }
  
  static vec2 operator*(float f, const vec2& a)
  {
    return vec2(a) *= f;
  }
  
  static vec2 operator/(const vec2& a, float f)
  {
    return vec2(a) /= f;
  }
  
  static vec2 operator/(float f, const vec2& v)
  {
    return vec2(f) /= v;
  }
  
  static vec2 operator*(const vec2& a, const vec2& b)
  {
    return vec2(a) *= b;
  }
  
  static vec2 operator/(const vec2& a, const vec2& b)
  {
    return vec2(a) /= b;
  }
  
  static vec2 operator+(const vec2& a, const vec2& b)
  {
    return vec2(a) += b;
  }
  
  static vec2 operator-(const vec2& a, const vec2& b)
  {
    return vec2(a) -= b;
  }
  }

#endif
