#include <stdio.h>
#include <stdlib.h>

#include "4002.h"

struct ram_4002 * create_ram()
{
	struct ram_4002 *ram = malloc(sizeof(struct ram_4002));
	return ram;
}