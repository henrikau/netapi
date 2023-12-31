#include <netapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <socket_driver.h>
#include <stdint.h>

/* gcc -o build/main src/main.c src/socket_driver.c -I include/
 */
int main(int argc, char *argv[])
{
	struct net_drv *drv = NULL;
	unsigned char buffer[1500] = {0};


	drv = socket_create(0x22f0, "lo", -1);
	/* drv = udp_create(...); */
	/* drv = tcp_create(...) */

	if (netapi_init(drv) < 0) {
		perror("Failed creating socket (run with sudo?)");
		return -1;
	}
	if (netapi_read(drv, buffer, 1500) > 0) {
		printf("Got data!\n");
	}
}
