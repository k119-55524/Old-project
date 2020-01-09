#pragma once

#pragma unmanaged

#include "header.h"

class PhysXErrorCallback :
	public PxErrorCallback
{
public:
	PhysXErrorCallback(void);
	~PhysXErrorCallback(void);

	virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
};

#pragma managed
