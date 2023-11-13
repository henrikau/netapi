#pragma once

struct net_drv {
	int  ( *init)(struct net_drv *drv);
	void (*close)(struct net_drv *drv);
	int  ( *read)(struct net_drv *drv, void * buffer, int size);
	int  (*write)(struct net_drv *drv, void * buffer, int size);
};
