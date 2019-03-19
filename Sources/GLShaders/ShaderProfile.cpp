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
#include <fstream>
#include "ShaderProfile.h"
#include "ShaderProfileAPI.h"
#include "ShaderSettings.h"
#include "RenderSessionAPI.h"
#define ASSERTALWAYS(x)
#include "Shader.h"
#include <Engine/Base/Stream.h>

static std::string g_defaultProfile = "";
static std::vector<ShaderProfile> g_shaderProfiles;
static int g_currentProfile = 0;

namespace
{
  class DefaultProfileLoader
  {
  public:
    DefaultProfileLoader()
    {
      CTFileName defaultProfilePath;
      ExpandFilePath(0, CTFileName("EFNMShaders\\GLDefaultProfile", 4), defaultProfilePath);
      std::ifstream f;
      f.open(static_cast<const char*>(defaultProfilePath));
      if (f.is_open())
      {
        f >> g_defaultProfile;
        f.close();
      }
      if (g_defaultProfile.empty())
        g_defaultProfile = "GLShaders";

      g_shaderProfiles.push_back(ShaderProfile());
    }
  };
  static DefaultProfileLoader g_loader;
}

ShaderProfile::ShaderProfile()
  : loadingDir(g_defaultProfile)
{
  Clear();
}

void ShaderProfile::Clear()
{
  shaders.clear();
  shaders.resize(20, ShaderSettings());
}

int AlexShaders_CreateShaderProfile(const char* loadingDir)
{
  {for (int i = 0; i < g_shaderProfiles.size(); ++i)
  {
    if (g_shaderProfiles[i].loadingDir == loadingDir)
      return i;
  }}

  g_shaderProfiles.push_back(ShaderProfile());
  if (loadingDir)
    g_shaderProfiles.back().loadingDir = loadingDir;
  return g_shaderProfiles.size() - 1;
}

int AlexShaders_SetCurrentProfile(int newProfile)
{
  int prevProfile = g_currentProfile;
  g_currentProfile = newProfile;
  return prevProfile;
}

int AlexShaders_GetCurrentProfile()
{
  return g_currentProfile;
}

void ClearAllProfiles()
{
  for (unsigned int i = 0; i < g_shaderProfiles.size(); ++i)
    g_shaderProfiles[i].Clear();
}

const std::string& GetCurrentShaderLoadingDir()
{
  return g_shaderProfiles[g_currentProfile].loadingDir;
}

ShaderSettings& GetOrCreateShader(int index)
{
  ShaderSettings& shader = g_shaderProfiles[g_currentProfile].shaders[index - 1];
  if (shader.loaded)
  {
    if (!shader.compiled && AlexShaders_Initialized())
      shader.CompileShader();
    return shader;
  }

  AlexShaders_Initialize();
  std::string shaderPath = std::string("EFNMShaders\\") + GetCurrentShaderLoadingDir() + "\\";
  std::string shaderFile = "GLShader_";
  if (index > 9)
    shaderFile += static_cast<char>((index / 10) + '0');
  shaderFile += static_cast<char>((index % 10) + '0');
  shaderFile += ".json";
  CTFileName absShaderPath;
  ExpandFilePath(0, CTFileName(shaderPath.c_str(), 4), absShaderPath);
  LoadShader(static_cast<const char*>(absShaderPath), shaderFile, shader);
  return shader;
}
