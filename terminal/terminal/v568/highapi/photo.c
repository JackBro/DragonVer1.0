
#include "internal.h"
#ifndef	V578CMD_DISABLE

TPhotoContext gPhotoContext;

TPhotoContext * V5_PhotoGetContext(void)
{
	return &gPhotoContext;
}

void V5_PhotoOpen(void)
{
}

#endif //V578CMD_DISABLE
