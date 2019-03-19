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
1320
%{
#include <Engine/Engine.h>
#include "DeclDllImport.h"
#include <GameMP/SessionProperties.h>
#include <EntitiesMP/Global.h>
#include <EntitiesMP/Common/Common.h>
#include <EntitiesMP/Common/Flags.h>
#include "GLModelHolderAnimationChanger.h"
#include "ModelMatrix.h"
#include "RenderSessionAPI.h"
#include "ShaderProfileAPI.h"
%}

uses "GLShaders/DeclDllImport";
uses "EntitiesMP/ModelDestruction";
uses "GLShaders/DeclDllExport";

enum SkaCustomShadingType {
  0 SCST_NONE             "Automatic shading",
  1 SCST_CONSTANT_SHADING "Constant shading",
  2 SCST_FULL_CUSTOMIZED  "Customized shading"
};

enum SkaShadowType {
  0 SST_NONE      "None",
  1 SST_CLUSTER   "Cluster shadows",
  2 SST_POLYGONAL "Polygonal" 
};

class GLModelHolder : CRationalEntity {
name      "GLModelHolder";
thumbnail "GLResources\\GLModelHolder.tbn";
features  "HasName", "HasDescription";

properties:
  1 CTFileName           m_fnModel                "Model file (.smc)"      'M' = CTFILENAME(""),
  2 FLOAT                m_fStretchAll            "StretchAll"             'S' = 1.0f,
  3 ANGLE3D              m_vStretchXYZ            "StretchXYZ"             'X' = FLOAT3D(1.0f, 1.0f, 1.0f),
  4 CTString             m_strName                "Name"                   'N' = "",
  5 CTString             m_strGLProfile           "GL Profile"                 = "GLShaders",
  6 BOOL                 m_bColliding             "Collision"              'L' = FALSE,
  7 BOOL                 m_bBackground            "Background"             'B' = FALSE,
  8 BOOL                 m_bTargetable            "Targetable"                 = FALSE,
  9 ANGLE3D              m_aShadingDirection      "Shade. Light direction" 'D' = ANGLE3D( AngleDeg(45.0f),AngleDeg(45.0f),AngleDeg(45.0f)),
 10 COLOR                m_colLight               "Shade. Light color"     'O' = C_WHITE,
 11 COLOR                m_colAmbient             "Shade. Ambient color"   'A' = C_BLACK,
 12 BOOL                 m_bActive                "Active"                     = TRUE,
 13 FLOAT                m_fClassificationStretch "Classification stretch"     = 1.0f,
 14 CTString             m_strDescription                                      = "",
 16 enum SkaShadowType        m_stClusterShadows  "Shadows"                'W' = SST_CLUSTER,
 17 enum SkaCustomShadingType m_cstCustomShading  "Shading mode"           'H' = SCST_NONE,

{
int m_renderSession;
int m_profile;
}

components:

functions:
  void GLModelHolder()
  {
    m_renderSession = -1;
    m_profile = -1;
  }

  void ~GLModelHolder()
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
      m_profile = AlexShaders_CreateShaderProfile(m_strGLProfile.str_String);
    }
    AlexShaders_SetCurrentProfile(m_profile);
  }

  void Read_t(CTStream* istr)
  {
    istr->ExpectID_t("GLPR");
    char dummy[8];
    istr->Read_t(dummy, 8);
    int len;
    istr->Read_t(&len, sizeof(int));
    char* tmp = new char[len+1];
    istr->Read_t(tmp, len);
    tmp[len] = '\0';
    m_strGLProfile = tmp;
    delete[] tmp;
    SetProfile();
    // profile loaded first, now its safe to load other data
    // because CModelInstance will require proper shader
    CRationalEntity::Read_t(istr);
    m_profile = -1;
    m_renderSession = -1;
    AlexShaders_Finalize();
  }

  void Write_t(CTStream* ostr)
  {
    // it is mandatory to write shader profile before CModelInstance!
    ostr->WriteID_t("GLPR");
    char dummy[8];
    ostr->Write_t(dummy, 8);
    int len = m_strGLProfile.Length();
    ostr->Write_t(&len, sizeof(int));
    ostr->Write_t(m_strGLProfile.str_String, len);
    CRationalEntity::Write_t(ostr);
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

  BOOL FillEntityStatistics(EntityStats *pes)
  {
    pes->es_strName = m_fnModel.FileName();
    pes->es_ctCount = 1;
    pes->es_ctAmmount = 1;
    pes->es_fValue = 0;
    pes->es_iScore = 0;
    return TRUE;
  }

  FLOAT3D GetClassificationBoxStretch(void)
  {
    return FLOAT3D( m_fClassificationStretch, m_fClassificationStretch, m_fClassificationStretch);
  }

  void ReceiveDamage(
    CEntity *penInflictor,
    enum DamageType dmtType,
    FLOAT fDamageAmmount,
    const FLOAT3D &vHitPoint,
    const FLOAT3D &vDirection) 
  {
  };

  void *GetEntityInfo(void)
  {
    return CEntity::GetEntityInfo();
  };
  
  BOOL IsTargetable(void) const
  {
    return m_bTargetable;
  }

  BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
  {
    switch(m_cstCustomShading)
    {
    case SCST_FULL_CUSTOMIZED:
      {
        colLight   = m_colLight;
        colAmbient = m_colAmbient;

        AnglesToDirectionVector(m_aShadingDirection, vLightDirection);
        vLightDirection = -vLightDirection;
        break;
      }
    case SCST_CONSTANT_SHADING:
      {
        UBYTE lR,lG,lB,aR,aG,aB,rR,rG,rB;
        ColorToRGB( colLight,   lR, lG, lB);
        ColorToRGB( colAmbient, aR, aG, aB);
        colLight = 0;
        rR = (UBYTE) Clamp( (ULONG)lR+aR, (ULONG)0, (ULONG)255);
        rG = (UBYTE) Clamp( (ULONG)lG+aG, (ULONG)0, (ULONG)255);
        rB = (UBYTE) Clamp( (ULONG)lB+aB, (ULONG)0, (ULONG)255);
        colAmbient = RGBToColor( rR, rG, rB);
        break;
      }
    case SCST_NONE:
      {
        break;
      }
    }

    return m_stClusterShadows!=SST_NONE;
  };

  void MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX)
  {
    m_fStretchAll*=fStretch;
    if (bMirrorX) {
      m_vStretchXYZ(1)=-m_vStretchXYZ(1);
    }
  }

  void StretchModel(void)
  {
    // stretch factors must not have extreme values
    if (Abs(m_vStretchXYZ(1))  < 0.01f) { m_vStretchXYZ(1)   = 0.01f;  }
    if (Abs(m_vStretchXYZ(2))  < 0.01f) { m_vStretchXYZ(2)   = 0.01f;  }
    if (Abs(m_vStretchXYZ(3))  < 0.01f) { m_vStretchXYZ(3)   = 0.01f;  }
    if (m_fStretchAll< 0.01f) { m_fStretchAll = 0.01f;  }

    if (Abs(m_vStretchXYZ(1))  >1000.0f) { m_vStretchXYZ(1)   = 1000.0f*Sgn(m_vStretchXYZ(1)); }
    if (Abs(m_vStretchXYZ(2))  >1000.0f) { m_vStretchXYZ(2)   = 1000.0f*Sgn(m_vStretchXYZ(2)); }
    if (Abs(m_vStretchXYZ(3))  >1000.0f) { m_vStretchXYZ(3)   = 1000.0f*Sgn(m_vStretchXYZ(3)); }
    if (m_fStretchAll>1000.0f) { m_fStretchAll = 1000.0f; }

    GetModelInstance()->StretchModel( m_vStretchXYZ*m_fStretchAll );
    ModelChangeNotify();
  };

  void InitModelHolder(void)
  {
    if (m_fnModel=="")
    {
      m_fnModel=CTFILENAME("Models\\Editor\\Ska\\Axis.smc");
    }

    if (m_bActive)
    {
      InitAsSkaModel();
    }
    else
    {
      InitAsSkaEditorModel();
    }
   
    BOOL bLoadOK = TRUE;
    try
    {
      if (_bWorldEditorApp)
      {
        m_profile = -1;
      }
      SetProfile();
      SetSkaModel_t(m_fnModel);
    }
    catch(char *strError)
    {
      WarningMessage(TRANS("Cannot load ska model '%s':\n%s"), (CTString&)m_fnModel, strError);
      bLoadOK = FALSE;
    }
    if (!bLoadOK)
    {
      SetSkaModel(CTFILENAME("Models\\Editor\\Ska\\Axis.smc"));
    }
    
    StretchModel();
    ModelChangeNotify();

    if (m_bColliding&&m_bActive)
    {
      SetPhysicsFlags(EPF_MODEL_FIXED);
      SetCollisionFlags(ECF_MODEL_HOLDER);
    }
    else
    {
      SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
      SetCollisionFlags(ECF_IMMATERIAL);
    }

    switch(m_stClusterShadows)
    {
    case SST_NONE:
      {
        SetFlags(GetFlags()&~ENF_CLUSTERSHADOWS);
        break;
      }
    case SST_CLUSTER:
      {
        SetFlags(GetFlags()|ENF_CLUSTERSHADOWS);
        break;
      }
    case SST_POLYGONAL:
      {
        SetFlags(GetFlags()&~ENF_CLUSTERSHADOWS);
        break;
      }
    }

    if (m_bBackground)
    {
      SetFlags(GetFlags()|ENF_BACKGROUND);
    }
    else
    {
      SetFlags(GetFlags()&~ENF_BACKGROUND);
    }

    m_strDescription.PrintF("%s", (CTString&)m_fnModel.FileName());

    return;
  };

procedures:
  Die()
  {
    {FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
      itenChild->SendEvent(ERangeModelDestruction());
    }}

    Destroy();
    return;
  }

  Main()
  {
    InitModelHolder();

    wait() {
      on(EBegin): {
        resume;
      }
      on (EActivate):
      {
        SwitchToModel();
        m_bActive = TRUE;
        if (m_bColliding)
        {
          SetPhysicsFlags(EPF_MODEL_FIXED);
          SetCollisionFlags(ECF_MODEL_HOLDER);
        }
        resume;
      }
      on (EDeactivate):
      {
        SwitchToEditorModel();
        SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
        SetCollisionFlags(ECF_IMMATERIAL);
        m_bActive = FALSE;
        SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
        SetCollisionFlags(ECF_IMMATERIAL);
        resume;
      }
      on(ERangeModelDestruction):
      {
        {FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
          itenChild->SendEvent(ERangeModelDestruction());
        }}

        Destroy();
        resume;
      }
      on(EGLMHChangeAnim eCA) :
      {
        GetModelInstance()->AddAnimation(
          ska_GetIDFromStringTable(eCA.strAnim),
          eCA.dwFlags,
          eCA.fStrength,
          eCA.iGroupID,
          eCA.fSpeed
        );
        resume;
      }
      on(EGLMHRemoveAnim eRA) :
      {
        GetModelInstance()->RemAnimation(ska_GetIDFromStringTable(eRA.strAnim));
        resume;
      }
      on(EGLMHRemoveAnimsID eRAID) :
      {
        GetModelInstance()->RemAnimsWithID(eRAID.iGroupID);
        resume;
      }
      on(EGLMHStopAllAnims) :
      {
        GetModelInstance()->StopAllAnimations(0.1f);
        resume;
      }
      on(EDeath):
      {
        resume;
      }
      otherwise():
      {
        resume;
      }
    };
  }
};
