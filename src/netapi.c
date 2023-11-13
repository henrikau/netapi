#include <netapi.h>

int  netapi_init(struct net_drv *drv)
{
	if (!drv)
		return -1;
	return drv->init(drv);
}
void netapi_close(struct net_drv *drv)
{
	if (drv)
		drv->close(drv);
}

int netapi_read(struct net_drv *drv, void * buffer, int size)
{
	if (!drv)
		return -1;
	return drv->read(drv, buffer, size);
}

int  netapi_write(struct net_drv *drv, void * buffer, int size)
{
	if (!drv)
		return -1;
	return drv->write(drv, buffer, size);
}
