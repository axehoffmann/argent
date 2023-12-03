#include "archetype.h"

void ag::archetype::resolveBuffers()
{
	dyn_array<u64> toDelete(destroyBuffer.size());
	destroyBuffer.copy_to(toDelete.begin());

	for (auto& arr : dataArrays)
		arr->eraseIndices(toDelete);


}