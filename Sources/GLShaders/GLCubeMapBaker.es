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
1321
%{
#include <EngineGui/EngineGUI.h>
#include "DeclDllImport.h"
#include <GameMP/SessionProperties.h>
#include <EntitiesMP/Global.h>
#include <EntitiesMP/Common/Common.h>
#include <EntitiesMP/Common/Flags.h>
#include "ModelMatrix.h"
#include "RenderSessionAPI.h"
#include "ShaderProfileAPI.h"
#include "DeclDllExport.h"

#ifdef UNICODE
#define WIDE(x) L##x
#else
#define WIDE(x) x
#endif

static void AddAlphaChannel(UBYTE* pubSrcBitmap, ULONG* pulDstBitmap, PIX pixSize, UBYTE* pubAlphaBitmap)
{
  UBYTE ubR, ubG, ubB, ubA = 255;
  for (INDEX iPix = (pixSize - 1); iPix >= 0; iPix--)
  {
    ubR = pubSrcBitmap[iPix * 3 + 0];
    ubG = pubSrcBitmap[iPix * 3 + 1];
    ubB = pubSrcBitmap[iPix * 3 + 2];
    if (pubAlphaBitmap != NULL)
      ubA = pubAlphaBitmap[iPix];
    else
      ubA = 255;
    pulDstBitmap[iPix] = ByteSwap(RGBAToColor(ubR, ubG, ubB, ubA));
  }
}

static void RemoveAlphaChannel(ULONG* pulSrcBitmap, UBYTE* pubDstBitmap, PIX pixSize)
{
  UBYTE ubR, ubG, ubB;
  for (INDEX iPix = 0; iPix<pixSize; iPix++)
  {
    ColorToRGB(ByteSwap(pulSrcBitmap[iPix]), ubR, ubG, ubB);
    pubDstBitmap[iPix * 3 + 0] = ubR;
    pubDstBitmap[iPix * 3 + 1] = ubG;
    pubDstBitmap[iPix * 3 + 2] = ubB;
  }
}

static void FlipBitmap(UBYTE* pubSrc, PIX pixWidth, PIX pixHeight, BOOL bAlphaChannel)
{
  PIX pixSize = pixWidth * pixHeight;

  ULONG *pulNew = NULL;
  ULONG *pulNewSrc = (ULONG*)pubSrc;
  ULONG *pulNewDst = (ULONG*)pubSrc;
  if (!bAlphaChannel)
  {
    pulNew = (ULONG*)AllocMemory(pixSize *BYTES_PER_TEXEL);
    AddAlphaChannel(pubSrc, pulNew, pixSize);
    pulNewSrc = pulNew;
    pulNewDst = pulNew;
  }

  const PIX pixHalfWidth = (pixWidth + 1) / 2;
  const PIX pixHalfHeight = (pixHeight + 1) / 2;

  for (INDEX iRow = 0; iRow<pixHeight; iRow++)
  {
    PIX pixRowOffset = iRow * pixWidth;
    for (INDEX iPix = 0; iPix<pixHalfWidth; iPix++)
    {
      PIX pixBeg = pulNewSrc[pixRowOffset + iPix];
      PIX pixEnd = pulNewSrc[pixRowOffset + (pixWidth - 1 - iPix)];
      pulNewDst[pixRowOffset + iPix] = pixEnd;
      pulNewDst[pixRowOffset + (pixWidth - 1 - iPix)] = pixBeg;
    }
  }

  if (!bAlphaChannel)
  {
    RemoveAlphaChannel(pulNewDst, pubSrc, pixSize);
    if (pulNew != NULL)
      FreeMemory(pulNew);
  }
}

%}

uses "GLShaders/DeclDllExport";

class GLCubeMapBaker : CRationalEntity {
name      "GLCubeMapBaker";
thumbnail "GLResources\\GLCubeMapBaker.tbn";
features  "HasName";

properties:
  1 CTString m_strName       "Name"                  'N' = "GLCubeMapBaker",
  2 BOOL     m_bBake         "Press to bake"             = FALSE,
    
{
int m_renderSession;
int m_profile;
}

components:

functions:
  void GLCubeMapBaker()
  {
    m_renderSession = -1;
    m_profile = -1;
  }

  void ~GLCubeMapBaker()
  {
    AlexShaders_Finalize();
    m_profile = -1; // just in case
    m_renderSession = -1;
  }

  void SetProfile()
  {
    AlexShaders_Initialize();
    if (m_profile == -1)
    {
      m_profile = AlexShaders_CreateShaderProfile("GLShaders");
    }
    AlexShaders_SetCurrentProfile(m_profile);
  }

  void Read_t(CTStream* istr)
  {
    SetProfile();
    CRationalEntity::Read_t(istr);
    m_profile = -1;
    m_renderSession = -1;
    AlexShaders_Finalize();
  }

  BOOL RenderOneTexture(CDrawPort* pdp, ANGLE3D rotation, const CTFileName& fnTarget)
  {
    if (pdp->Lock())
    {
      pdp->FillZBuffer(ZBUF_BACK);

      CAnyProjection3D prProjection;
      CPerspectiveProjection3D prPerspectiveProjection;
      prPerspectiveProjection.FOVL() = AngleDeg(90.0f);
      prPerspectiveProjection.ScreenBBoxL() = FLOATaabbox2D(FLOAT2D(0.0f, 0.0f), FLOAT2D(256.0f, 256.0f));
      prPerspectiveProjection.FrontClipDistanceL() = 0.1f;
      prPerspectiveProjection.AspectRatioL() = 1.0f;
      prPerspectiveProjection.ObjectStretchL() = FLOAT3D(1.0f, 1.0f, 1.0f);
      prPerspectiveProjection.ViewerPlacementL() = CPlacement3D(en_plPlacement.pl_PositionVector, rotation);
      prPerspectiveProjection.ObjectPlacementL() = CPlacement3D(FLOAT3D(0.0f, 0.0f, 0.0f), ANGLE3D(0.0f, 0.0f, 0.0f));
      prProjection = prPerspectiveProjection;
      prProjection->FarClipDistanceL() = 1000.0f;
      prProjection->Prepare();

      ::RenderView(*GetWorld(), *(CEntity*)NULL, prProjection, *pdp);
      pdp->Unlock();
      try
      {
        CImageInfo imageInfo;
        pdp->GrabScreen(imageInfo);
        FlipBitmap(imageInfo.ii_Picture, imageInfo.ii_Width, imageInfo.ii_Height, imageInfo.ii_BitsPerPixel == 32);
        imageInfo.SaveTGA_t(fnTarget);
      }
      catch (char*)
      {
        return TRUE;
      }
      return FALSE;
    }
    return TRUE;
  }

  void BakeTexture()
  {
    CTFileName fnCubemap = _EngineGUI.FileRequester("Select name for cubemap",
      FILTER_TEX FILTER_END, "Save directory",
      "", "", NULL, FALSE);
    if (fnCubemap == "")
    {
      return;
    }
    fnCubemap = fnCubemap.NoExt();
    
    CDrawPort* pdp = NULL;
    // create canvas to render picture
    _pGfx->CreateWorkCanvas(256, 256, &pdp);
    if (pdp != NULL)
    {
      BOOL errMsgShown = FALSE;
      BOOL error = FALSE;
      error |= RenderOneTexture(pdp, ANGLE3D(0.0f, 0.0f, 0.0f), fnCubemap + "_back.tga");
      error |= RenderOneTexture(pdp, ANGLE3D(-90.0f, 0.0f, 0.0f), fnCubemap + "_right.tga");
      error |= RenderOneTexture(pdp, ANGLE3D(90.0f, 0.0f, 0.0f), fnCubemap + "_left.tga");
      error |= RenderOneTexture(pdp, ANGLE3D(180.0f, 90.0f, 0.0f), fnCubemap + "_top.tga");
      error |= RenderOneTexture(pdp, ANGLE3D(180.0f, -90.0f, 0.0f), fnCubemap + "_bottom.tga");
      error |= RenderOneTexture(pdp, ANGLE3D(180.0f, 0.0f, 0.0f), fnCubemap + "_front.tga");
      try
      {
        CreateTexture_t(fnCubemap + "_front.tga", fnCubemap + ".tex", 256, MAX_MEX_LOG2 + 1, FALSE);
      }
      catch (char*)
      {
        errMsgShown = TRUE;
        ::MessageBox(::GetActiveWindow(), WIDE("Failed to create placeholder texture!"), WIDE("Error"), MB_OK | MB_ICONERROR);
      }
      _pGfx->DestroyWorkCanvas(pdp);
      if (!error)
      {
        ::MessageBox(::GetActiveWindow(), WIDE("Cubemap has been exported!"), WIDE("Success"), MB_OK | MB_ICONINFORMATION);
      } else if (!errMsgShown) {
        ::MessageBox(::GetActiveWindow(), WIDE("Failed to create cubemap texture!"), WIDE("Error"), MB_OK | MB_ICONERROR);
      }

    } else {
      ::MessageBox(::GetActiveWindow(), WIDE("Could not create drawport for cubemap!"), WIDE("Error"), MB_OK | MB_ICONERROR);
    }
  }

  void AdjustShaderParams(INDEX iSurfaceID, CShader* pShader, ShaderParams& spParams)
  {
    SetProfile();

    if (m_renderSession == -1)
    {
      m_renderSession = AlexShaders_RegisterRenderSession();
    }

    AlexShaders_SetRenderSession(m_renderSession);

    // setup model matrix
    g_AlexShaders_modelMatrix_pos0 = en_plPlacement.pl_PositionVector(1);
    g_AlexShaders_modelMatrix_pos1 = en_plPlacement.pl_PositionVector(2);
    g_AlexShaders_modelMatrix_pos2 = en_plPlacement.pl_PositionVector(3);
    g_AlexShaders_modelMatrix_ang0 = en_plPlacement.pl_OrientationAngle(1);
    g_AlexShaders_modelMatrix_ang1 = en_plPlacement.pl_OrientationAngle(2);
    g_AlexShaders_modelMatrix_ang2 = en_plPlacement.pl_OrientationAngle(3);
  }

procedures:
  Main()
  {
    InitAsSkaEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);
    SetProfile();
    SetSkaModel(CTFILENAME("GLResources\\Models\\Sphere\\Sphere.smc"));

    if (m_bBake)
    {
      m_bBake = FALSE;
      if (_bWorldEditorApp)
      {
        BakeTexture();
      }
    }

    autowait(0.05f);

    wait()
    {
      on (EBegin) :
      {
        resume;
      }
    }
    return;
  };
};