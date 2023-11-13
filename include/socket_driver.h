#pragma once
#include <netapi.h>

struct net_drv * socket_create(int protocol, char *ifname, int timeout_us);
