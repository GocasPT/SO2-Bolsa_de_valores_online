#pragma once
#ifndef REGISTRY_H
#define REGISTRY_H

#include "bolsa.h"
#include "SO2_REGISTRY.h"

namespace Registry {
	// TODO: read data from registry
	void config(BOLSA& servidor);
	void create();
}

#endif // !REGISTRY_H
