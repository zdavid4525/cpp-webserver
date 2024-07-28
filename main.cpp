#include "Webserver.h"

int main() {
	Webserver ws("0.0.0.0", 8080);
	if (ws.init() != 0)
		return -1;
	ws.run();

	system("pause");
}
