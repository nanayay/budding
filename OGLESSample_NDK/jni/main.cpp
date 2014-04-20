#include "android_application.h"

void android_main(struct android_app* state)
{
    LOGD("Enter android_main() function");

	AndroidApplication app(state);

	if (app.Initialize())
	{
		app.Run();
	}

}

