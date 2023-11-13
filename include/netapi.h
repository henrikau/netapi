#pragma once

struct net_drv {
	int  ( *init)(struct net_drv *drv);
	void (*close)(struct net_drv *drv);
	int  ( *read)(struct net_drv *drv, void * buffer, int size);
	int  (*write)(struct net_drv *drv, void * buffer, int size);
};

int  netapi_init(struct net_drv *drv);
void netapi_close(struct net_drv *drv);
int  netapi_read(struct net_drv *drv, void * buffer, int size);
int  netapi_write(struct net_drv *drv, void * buffer, int size);
