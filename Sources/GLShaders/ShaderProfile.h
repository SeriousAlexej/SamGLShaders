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
#ifndef SAM_GL_SHADERS_SHADER_PROFILE_H
#define SAM_GL_SHADERS_SHADER_PROFILE_H
#include "AlexShaders.h"
#include "ShaderSettings.h"

struct ShaderProfile
{
  ShaderProfile();

  void Clear();

  std::string loadingDir;
  std::vector<ShaderSettings> shaders;
};

void ClearAllProfiles();
const std::string& GetCurrentShaderLoadingDir();
ShaderSettings& GetOrCreateShader(int index);

#endif
