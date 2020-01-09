#pragma once

#include "header.h"
#include "PhysXErrorCallback.h"
#include "PhysXDefaultAllocator.h"

CObjectBase* ObjRot = NULL;
CGameInterface* cFPS = NULL;

CGameInterface* cInfoObj = NULL;

HRESULT CALLBACK OnInicialise( void );
void CALLBACK OnPreFrameRender( float deltaTime );

static PhysXErrorCallback PhysXDefError;
static PhysXDefaultAllocator PhysXDefAlloc;