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
#include "ShaderProfile.h"
#include "RenderSessionAPI.h"
#include "Shader.h"

#define GL_SHADER_MAIN(shaderName, index) \
extern "C" void __declspec(dllexport) Shader_##shaderName(void) \
{ \
  if (!AlexShaders_Initialized()) \
    AlexShaders_Initialize(); \
  ShaderSettings& shader = GetOrCreateShader(index); \
  if (!shader.compiled) \
    return; \
  shaSetTextureWrapping((GfxWrap)shader.textureWrappingX, (GfxWrap)shader.textureWrappingY); \
  if (shader.depthTest) \
  { \
    shaEnableDepthTest(); \
    shaDepthFunc((GfxComp)shader.depthFunc); \
  } else { \
    shaDisableDepthTest(); \
  } \
  if (shader.depthWrite) \
    shaEnableDepthWrite(); \
  else \
    shaDisableDepthWrite(); \
  if (shader.alphaTest) \
    shaEnableAlphaTest(); \
  else \
    shaDisableAlphaTest(); \
  if (shader.blending) \
  { \
    shaEnableBlend(); \
    shaBlendFunc((GfxBlend)shader.blendSrcMode, (GfxBlend)shader.blendDstMode); \
  } else { \
    shaDisableBlend(); \
  } \
  shaCullFace((GfxFace)shader.culling); \
  AlexRenderWithShader(shader); \
  shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT); \
}

#define GL_SHADER_DESC(shaderNameInput, index) \
extern "C" void __declspec(dllexport) Shader_Desc_##shaderNameInput(ShaderDesc& shDesc) \
{ \
  ShaderSettings& shader = GetOrCreateShader(index); \
  if (!shader.valid) \
  { \
    shDesc.sd_strShaderInfo = "GLShader_" #index "_invalid"; \
    return; \
  } \
  shDesc.sd_strShaderInfo = shader.name.c_str(); \
  shDesc.sd_astrTexCoordNames.New(shader.numUVMaps); \
  shDesc.sd_astrTextureNames.New(shader.textures.size()); \
  shDesc.sd_astrFloatNames.New(shader.floats.size()); \
  shDesc.sd_astrColorNames.New(shader.colors.size()); \
  shDesc.sd_astrFlagNames.New(shader.flags.size()); \
  if (!shader.uvMap1.shaderName.empty()) \
    shDesc.sd_astrTexCoordNames[0] = shader.uvMap1.name.c_str(); \
  if (!shader.uvMap2.shaderName.empty()) \
  { \
    if (shader.numUVMaps == 1) \
      shDesc.sd_astrTexCoordNames[0] = shader.uvMap2.name.c_str(); \
    else \
      shDesc.sd_astrTexCoordNames[1] = shader.uvMap2.name.c_str(); \
  } \
  {for (int i = 0; i < shader.textures.size(); ++i) \
  { \
    shDesc.sd_astrTextureNames[i] = shader.textures[i].name.c_str(); \
  }} \
  {for (int i = 0; i < shader.floats.size(); ++i) \
  { \
    shDesc.sd_astrFloatNames[i] = shader.floats[i].name.c_str(); \
  }} \
  {for (int i = 0; i < shader.colors.size(); ++i) \
  { \
    shDesc.sd_astrColorNames[i] = shader.colors[i].name.c_str(); \
  }} \
  {for (int i = 0; i < shader.flags.size(); ++i) \
  { \
    shDesc.sd_astrFlagNames[i] = shader.flags[i].name.c_str(); \
  }} \
}

#define GL_SHADER_DEFINE(index) \
GL_SHADER_MAIN(GLShader_##index, index) \
GL_SHADER_DESC(GLShader_##index, index)

GL_SHADER_DEFINE(1)
GL_SHADER_DEFINE(2)
GL_SHADER_DEFINE(3)
GL_SHADER_DEFINE(4)
GL_SHADER_DEFINE(5)
GL_SHADER_DEFINE(6)
GL_SHADER_DEFINE(7)
GL_SHADER_DEFINE(8)
GL_SHADER_DEFINE(9)
GL_SHADER_DEFINE(10)
GL_SHADER_DEFINE(11)
GL_SHADER_DEFINE(12)
GL_SHADER_DEFINE(13)
GL_SHADER_DEFINE(14)
GL_SHADER_DEFINE(15)
GL_SHADER_DEFINE(16)
GL_SHADER_DEFINE(17)
GL_SHADER_DEFINE(18)
GL_SHADER_DEFINE(19)
GL_SHADER_DEFINE(20)
