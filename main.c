#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

void open_gamepad_or_joystick(SDL_JoystickID instance_id) {
	if (SDL_IsGamepad(instance_id)) {
		SDL_Gamepad* gamepad = SDL_OpenGamepad(instance_id);
		if (gamepad) {
			SDL_SetGamepadSensorEnabled(gamepad, SDL_SENSOR_ACCEL, true);
			SDL_SetGamepadSensorEnabled(gamepad, SDL_SENSOR_GYRO, true);
		}
	} else {
		SDL_OpenJoystick(instance_id);
	}
}

int main(int argc, char* argv[]) {
	(void) argc;
	(void) argv;
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_SENSOR)) {
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Input Logger", 640, 480, 0);
	if (!window) {
		SDL_Quit();
		return 1;
	}

	int joystick_count = 0;
	SDL_JoystickID* joysticks = SDL_GetJoysticks(&joystick_count);
	if (joysticks) {
		for (int i = 0; i < joystick_count; ++i) {
			open_gamepad_or_joystick(joysticks[i]);
		}
		SDL_free(joysticks);
	}

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT:
					running = false;
					break;

				case SDL_EVENT_KEYBOARD_ADDED:
					printf("[Keyboard Device] Added: ID %u\n", event.kdevice.which);
					break;
				case SDL_EVENT_KEYBOARD_REMOVED:
					printf("[Keyboard Device] Removed: ID %u\n", event.kdevice.which);
					break;
				case SDL_EVENT_KEY_DOWN:
					printf("[Keyboard] Key Down: %s (Keycode: %d)\n", SDL_GetKeyName(event.key.key), event.key.key);
					break;
				case SDL_EVENT_KEY_UP:
					printf("[Keyboard] Key Up: %s (Keycode: %d)\n", SDL_GetKeyName(event.key.key), event.key.key);
					break;
				case SDL_EVENT_TEXT_INPUT:
					printf("[Keyboard] Text Input: %s\n", event.text.text);
					break;
				case SDL_EVENT_TEXT_EDITING:
					printf("[IME/Keyboard] Editing: %s\n", event.edit.text);
					break;

				case SDL_EVENT_MOUSE_ADDED:
					printf("[Mouse Device] Added: ID %u\n", event.mdevice.which);
					break;
				case SDL_EVENT_MOUSE_REMOVED:
					printf("[Mouse Device] Removed: ID %u\n", event.mdevice.which);
					break;
				case SDL_EVENT_MOUSE_MOTION:
					printf("[Mouse] Motion: Pos(%.1f, %.1f) Rel(%.1f, %.1f)\n", event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					printf("[Mouse] Button Down: %d at (%.1f, %.1f)\n", event.button.button, event.button.x, event.button.y);
					break;
				case SDL_EVENT_MOUSE_BUTTON_UP:
					printf("[Mouse] Button Up: %d at (%.1f, %.1f)\n", event.button.button, event.button.x, event.button.y);
					break;
				case SDL_EVENT_MOUSE_WHEEL:
					printf("[Mouse] Wheel: Scroll X: %.1f, Y: %.1f\n", event.wheel.x, event.wheel.y);
					break;

				case SDL_EVENT_GAMEPAD_ADDED:
					printf("[Gamepad Device] Added: ID %u\n", event.gdevice.which);
					open_gamepad_or_joystick(event.gdevice.which);
					break;
				case SDL_EVENT_GAMEPAD_REMOVED:
					printf("[Gamepad Device] Removed: ID %u\n", event.gdevice.which);
					break;
				case SDL_EVENT_GAMEPAD_AXIS_MOTION:
					printf("[Gamepad %u] Axis %d Value: %d\n", event.gaxis.which, event.gaxis.axis, event.gaxis.value);
					break;
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
					printf("[Gamepad %u] Button Down: %d\n", event.gbutton.which, event.gbutton.button);
					break;
				case SDL_EVENT_GAMEPAD_BUTTON_UP:
					printf("[Gamepad %u] Button Up: %d\n", event.gbutton.which, event.gbutton.button);
					break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
					printf("[Gamepad %u] Touchpad %d Down: Finger %d at (%.2f, %.2f)\n", event.gtouchpad.which, event.gtouchpad.touchpad, event.gtouchpad.finger, event.gtouchpad.x, event.gtouchpad.y);
					break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
					printf("[Gamepad %u] Touchpad %d Motion: Finger %d at (%.2f, %.2f)\n", event.gtouchpad.which, event.gtouchpad.touchpad, event.gtouchpad.finger, event.gtouchpad.x, event.gtouchpad.y);
					break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
					printf("[Gamepad %u] Touchpad %d Up: Finger %d at (%.2f, %.2f)\n", event.gtouchpad.which, event.gtouchpad.touchpad, event.gtouchpad.finger, event.gtouchpad.x, event.gtouchpad.y);
					break;
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
					printf("[Gamepad %u] Sensor %d Update: (%.2f, %.2f, %.2f)\n", event.gsensor.which, event.gsensor.sensor, event.gsensor.data[0], event.gsensor.data[1], event.gsensor.data[2]);
					break;

				case SDL_EVENT_JOYSTICK_AXIS_MOTION:
					printf("[Joystick %u] Axis %d Value: %d\n", event.jaxis.which, event.jaxis.axis, event.jaxis.value);
					break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
					printf("[Joystick %u] Button Down: %d\n", event.jbutton.which, event.jbutton.button);
					break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP:
					printf("[Joystick %u] Button Up: %d\n", event.jbutton.which, event.jbutton.button);
					break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION:
					printf("[Joystick %u] Hat %d Value: %d\n", event.jhat.which, event.jhat.hat, event.jhat.value);
					break;
				case SDL_EVENT_JOYSTICK_BALL_MOTION:
					printf("[Joystick %u] Ball %d Rel: (%d, %d)\n", event.jball.which, event.jball.ball, event.jball.xrel, event.jball.yrel);
					break;

				case SDL_EVENT_FINGER_DOWN:
					printf("[Touch] Finger Down: ID %llu at (%.2f, %.2f)\n", event.tfinger.fingerID, event.tfinger.x, event.tfinger.y);
					break;
				case SDL_EVENT_FINGER_UP:
					printf("[Touch] Finger Up: ID %llu at (%.2f, %.2f)\n", event.tfinger.fingerID, event.tfinger.x, event.tfinger.y);
					break;
				case SDL_EVENT_FINGER_MOTION:
					printf("[Touch] Finger Motion: ID %llu at (%.2f, %.2f)\n", event.tfinger.fingerID, event.tfinger.x, event.tfinger.y);
					break;

				case SDL_EVENT_PEN_DOWN:
					printf("[Pen] Down at (%.1f, %.1f)\n", event.ptouch.x, event.ptouch.y);
					break;
				case SDL_EVENT_PEN_UP:
					printf("[Pen] Up at (%.1f, %.1f)\n", event.ptouch.x, event.ptouch.y);
					break;
				case SDL_EVENT_PEN_MOTION:
					printf("[Pen] Motion at (%.1f, %.1f)\n", event.pmotion.x, event.pmotion.y);
					break;
				case SDL_EVENT_PEN_AXIS:
					printf("[Pen] Axis Update\n");
					break;

				case SDL_EVENT_SENSOR_UPDATE:
					printf("[System Sensor] Update: Type %d Data (%.2f, %.2f, %.2f)\n", event.sensor.which, event.sensor.data[0], event.sensor.data[1], event.sensor.data[2]);
					break;

				case SDL_EVENT_DROP_FILE:
					printf("[Drag&Drop] File Dropped: %s\n", event.drop.data);
					break;
				case SDL_EVENT_DROP_TEXT:
					printf("[Drag&Drop] Text Dropped: %s\n", event.drop.data);
					break;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
