#include <socket_driver.h>
#include <netinet/ether.h>
#include <linux/if.h>		/* IFNAMSIZ */
#include <arpa/inet.h>		/* htons */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

struct socket_priv {
	int sock;
	int proto;
	char ifname[IFNAMSIZ+1];
	int timeout_us;
	bool rx;
};

static struct socket_priv *get_priv(struct net_drv *drv)
{
	if (!drv)
		return NULL;
	return (struct socket_priv *)(drv+1);
}

static int socket_init(struct net_drv *drv)
{
	struct socket_priv *priv = get_priv(drv);
	priv->sock = socket(AF_PACKET, SOCK_RAW, htons(priv->proto));
	if (priv->sock < 0)
		return -1;

	/* If Rx, timeout is a good thing */
	if (priv->timeout_us > 0) {
		struct timeval tv = {
			.tv_sec = priv->timeout_us / 1e6,
			.tv_usec = priv->timeout_us % 1000000,
		};
		if (setsockopt(priv->sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) == -1) {
			printf("%s(): Could not set timeout on socket (%d): %s\n",
				__func__, priv->sock, strerror(errno));
			close(priv->sock);
			return -1;
		}
	}
	/* bind to device, ... */

	printf("Socket created\n");
	return 0;
}

static void socket_close(struct net_drv *drv)
{
	if (!drv)
		return;
	struct socket_priv *priv = get_priv(drv);
	if (priv->sock > 0) {
		close(priv->sock);
		priv->sock = -1;
	}
}

static int socket_recv(struct net_drv *drv, void * buffer, int size)
{
	if (!drv)
		return -1;
	struct socket_priv *priv = get_priv(drv);
	if (priv->timeout_us > 0) {
		int res = -1;
		do {
			res = recv(priv->sock, buffer, size, MSG_TRUNC);
		} while (res > 0);
		return res;
	}
	return recv(priv->sock, buffer, size, MSG_TRUNC);
}

static int socket_send(struct net_drv *drv, void * buffer, int size)
{
	if (!drv)
		return -1;
	struct socket_priv *priv = get_priv(drv);
	return send(priv->sock, buffer, size, MSG_DONTWAIT);
}

struct net_drv * socket_create(int protocol, char *ifname, int timeout_us)
{
	struct net_drv *drv = malloc(sizeof(*drv) + sizeof(struct socket_priv));
	if (!drv)
		return NULL;
	memset(drv, 0, sizeof(*drv));

	struct socket_priv *priv = get_priv(drv);
	priv->proto = protocol;
	if (ifname)
		strncpy(priv->ifname, ifname, IFNAMSIZ);
	if (timeout_us > 0)
		priv->timeout_us = timeout_us;

	drv->init = socket_init;
	drv->close = socket_close;
	drv->read = socket_recv;
	drv->write = socket_send;
	return drv;
}
