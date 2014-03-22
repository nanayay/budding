#include "android_application.h"

void android_main(struct android_app* state)
{

	AndroidApplication app(state);

	if (app.Initialize())
	{
		app.Run();
	}

}

