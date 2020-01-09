#pragma once

#pragma unmanaged

#include "header.h"

class PhysXDefaultAllocator :
	public PxDefaultAllocator
{
public:
	PhysXDefaultAllocator( void );
	~PhysXDefaultAllocator( void );

	void* allocate(size_t size, const char*, const char*, int)
	{
		return _aligned_malloc(size, 16);
	}

	void deallocate(void* ptr)
	{
		_aligned_free(ptr);
	}

};

#pragma unmanaged
