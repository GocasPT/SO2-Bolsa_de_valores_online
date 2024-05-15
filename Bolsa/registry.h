#pragma once
#ifndef REGISTRY_H
#define REGISTRY_H

#include "bolsa.h"
#include "SO2_REGISTRY.h"

#define DEFAULT_MAX_USERS 5
#define VALUE_MAX_USERS _T("maxUsers")

namespace Registry {
	void config(BOLSA& servidor);
}

#endif // !REGISTRY_H