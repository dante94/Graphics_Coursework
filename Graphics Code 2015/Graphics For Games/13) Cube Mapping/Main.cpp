#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("Cube Mapping! sky textures courtesy of http://www.hazelwhorley.com", 1920,1080,true);
	if(!w.HasInitialised()) {
		return -1;
	}
	float counter = 0;
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}
	bool toggle = false;
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT) && renderer.scene == 2) {
			renderer.scene = 3;
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT) && renderer.scene == 1) {
			renderer.scene = 2;
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT) && renderer.scene == 2) {
			renderer.scene = 1;
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT) && renderer.scene == 3) {
			renderer.scene = 2;
		}
		float msec = w.GetTimer()->GetTimedMS();

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_X)) {
			renderer.CameraForward();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_PAUSE) ) {
			toggle = !toggle;
		}
		if (toggle)
		{
			counter += msec;
			if (counter > 1000) {
				if (renderer.scene == 1)
					renderer.scene = 2;
				else if (renderer.scene == 2)
					renderer.scene = 3;
				else if (renderer.scene == 3)
					renderer.scene = 1;

				counter = 0;
			}
		}
		renderer.SetMsec(msec);
		renderer.UpdateScene(msec);
		renderer.RenderScene();
	}

	return 0;
}