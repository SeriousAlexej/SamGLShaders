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
#ifndef ALEX_GLM_MATH_H
#define ALEX_GLM_MATH_H
#include <math.h>

namespace glm
  {
  static float atan(float f)
    {
    return ::atan(f);
    }

  static float tan(float f)
    {
    return ::tan(f);
    }

  static float radians(float f)
    {
    return f * static_cast<float>(0.01745329251994329576923690768489);
    }
  
  static float degrees(float f)
  {
    return f * static_cast<float>(57.295779513082320876798154814105);
  }
  }

#endif
