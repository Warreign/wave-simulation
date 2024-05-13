#include "application/application.h"

Window* window = nullptr;

int main()
{
	Application app("Test Application");
	app.run();

	return 0;
}