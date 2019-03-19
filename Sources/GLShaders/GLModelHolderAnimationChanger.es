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
1322
%{
#include <Engine/Engine.h>
#include "DeclDllImport.h"
#include <GameMP/SessionProperties.h>
#include <EntitiesMP/Global.h>
#include <EntitiesMP/Common/Common.h>
#include <EntitiesMP/Common/Flags.h>
#include "DeclDllExport.h"
%}

uses "GLShaders/DeclDllExport";

enum ActionType {
  0 AT_STARTANIM "Add Animation",
  1 AT_REMANIM   "Remove Animation",
  2 AT_REMANIMID "Remove Animations by ID",
  3 AT_STOPANIMS "Stop all animations",
};

event EGLMHChangeAnim {
  CTString strAnim,
  ULONG dwFlags,
  FLOAT fStrength,
  INDEX iGroupID,
  FLOAT fSpeed,
};

event EGLMHRemoveAnim {
  CTString strAnim,
};

event EGLMHRemoveAnimsID {
  INDEX iGroupID,
};

event EGLMHStopAllAnims {};

class GLModelHolderAnimationChanger : CRationalEntity {
name      "GLModelHolderAnimationChanger";
thumbnail "GLResources\\GLModelHolderAnimationChanger.tbn";
features  "HasName", "HasTarget", "IsTargetable";

properties:
  1 CTString m_strName         "Name"           'N' = "GLModelHolderAnimationChanger",
  2 CEntityPointer m_penTarget "GLModelHolder"  'T' COLOR(C_RED|0xFF),
  3 enum ActionType m_penEvent "Action"         'G' = AT_STARTANIM,
  4 CTString m_strAnim         "Animation Name"     = "",
  5 FLOAT m_fStrength          "Strength"           = 1.0f,
  6 INDEX m_iGroupID           "Group ID"           = 0,
  7 FLOAT m_fSpeed             "Speed"              = 1.0f,
  8 BOOL m_bLooping            "Looping"            = FALSE,
  9 BOOL m_bNoRestart          "No restart"         = FALSE,
 10 BOOL m_bClear              "New clear state"    = FALSE,
 11 BOOL m_bClone              "New cloned state"   = FALSE,

components:
  1 model   MODEL_MARKER     "Models\\Editor\\AnimationChanger.mdl",
  2 texture TEXTURE_MARKER   "Models\\Editor\\AnimationChanger.tex",

functions:
  SLONG GetUsedMemory(void)
  {
    SLONG slUsedMemory = sizeof(GLModelHolderAnimationChanger) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
    slUsedMemory += m_strName.Length();
    return slUsedMemory;
  }

procedures:
  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    autowait(0.1f);

    wait()
    {
      on (ETrigger eTrigger) :
      {
        if(IsDerivedFromClass(m_penTarget, "GLModelHolder"))
        {
          switch(m_penEvent)
          {
          case AT_STARTANIM:
          {
            ULONG m_dwFlags = AN_LOOPING;
            if(!m_bLooping) { m_dwFlags &= ~AN_LOOPING; }
            if(m_bClear) { m_dwFlags |= AN_CLEAR; }
            if(m_bClone) { m_dwFlags |= AN_CLONE; }
            if(m_bNoRestart) { m_dwFlags |= AN_NORESTART; }

            EGLMHChangeAnim eCA;
            eCA.strAnim = m_strAnim;
            eCA.dwFlags = m_dwFlags;
            eCA.fStrength = m_fStrength;
            eCA.iGroupID = m_iGroupID;
            eCA.fSpeed = m_fSpeed;
            m_penTarget->SendEvent(eCA);
            break;
          }
          case AT_REMANIM:
          {
            EGLMHRemoveAnim eRA;
            eRA.strAnim = m_strAnim;
            m_penTarget->SendEvent(eRA);
            break;
          }
          case AT_REMANIMID:
          {
            EGLMHRemoveAnimsID eRAID;
            eRAID.iGroupID = m_iGroupID;
            m_penTarget->SendEvent(eRAID);
            break;
          }
          case AT_STOPANIMS:
          {
           m_penTarget->SendEvent(EGLMHStopAllAnims());
           break;
          }
          default:
          {
            CPrintF("Unknown GLModelHolderAnimationChanger action.\n");
            break;
          }
          }
        }
        resume;
      }
    }
    Destroy();
    return;
  };
};