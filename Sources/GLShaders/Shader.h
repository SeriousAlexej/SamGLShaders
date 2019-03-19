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
#ifndef SAM_GL_SHADERS_H
#define SAM_GL_SHADERS_H

#define ENGINE_API __declspec(dllimport)

#define SE_INCL_GL_TYPES_H
#define SE_INCL_ASSERT_H

typedef char* va_list;

#include <windows.h>

#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(x)

#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <Engine/Base/CTString.h>
#include <Engine/Math/Functions.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Graphics/Vertex.h>

extern __declspec(dllimport) CStaticStackArray<GFXVertex>   _avtxCommon;
extern __declspec(dllimport) CStaticStackArray<GFXTexCoord> _atexCommon;
extern __declspec(dllimport) CStaticStackArray<GFXColor>    _acolCommon;
extern __declspec(dllimport) CStaticStackArray<INDEX>       _aiCommonElements;
extern __declspec(dllimport) CStaticStackArray<INDEX>       _aiCommonQuads;

#define GLenum unsigned int
typedef int D3DFORMAT;
#include <Engine/Graphics/Gfx_wrapper.h>
#undef GLenum
#include <Engine/Graphics/Color.h>

struct ShaderDesc
{
  CStaticArray<class CTString> sd_astrTextureNames;
  CStaticArray<class CTString> sd_astrTexCoordNames;
  CStaticArray<class CTString> sd_astrColorNames;
  CStaticArray<class CTString> sd_astrFloatNames;
  CStaticArray<class CTString> sd_astrFlagNames;
  CTString sd_strShaderInfo;
};

typedef ULONG COLOR;

ENGINE_API void shaSetTexture(INDEX iTexture);
ENGINE_API void shaSetTextureWrapping(enum GfxWrap eWrapU, enum GfxWrap eWrapV);
ENGINE_API void shaSetUVMap(INDEX iUVMap);
ENGINE_API void shaSetColor(INDEX icolIndex);
ENGINE_API void shaEnableDepthTest(void);
ENGINE_API void shaDepthFunc(GfxComp eComp);
ENGINE_API COLOR &shaGetModelColor(void);
ENGINE_API COLOR &shaGetCurrentColor(void);
ENGINE_API ULONG &shaGetFlags();
ENGINE_API void shaCalculateLight(void);
ENGINE_API BOOL shaOverBrightningEnabled(void);
ENGINE_API void shaSetTextureModulation(INDEX iScale);
ENGINE_API void shaCullFace(GfxFace eFace);
ENGINE_API void shaDisableAlphaTest(void);
ENGINE_API void shaDisableBlend(void);
ENGINE_API void shaEnableDepthWrite(void);
ENGINE_API void shaBlendFunc(GfxBlend eSrc, GfxBlend eDst);
ENGINE_API void shaEnableBlend(void);
ENGINE_API void shaDisableDepthWrite(void);
ENGINE_API void shaModifyColorForFog(void);
ENGINE_API void shaRender(void);
ENGINE_API void shaDoFogPass(void);
ENGINE_API void shaDisableDepthTest(void);
ENGINE_API void shaEnableAlphaTest(void);
ENGINE_API FLOAT shaGetFloat(INDEX iFloatIndex);

#endif
