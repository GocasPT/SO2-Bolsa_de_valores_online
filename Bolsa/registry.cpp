#include "registry.h"

void Registry::config(BOLSA& servidor) {
	/*TODO: read data from registry
	  - if not exist, create registry with default value and set it
	*/

	servidor.tData = { TRUE, &servidor.userList, &servidor.userQueue, 10 }; //TODO: change '10' to the read value or default value
}

void Registry::create() {
	/*TODO:
	  - create registry
	  - create key
	  - set value with default value
	*/
}