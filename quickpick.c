#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void notify(const char* message) {
    char command[256];
    snprintf(command, 256, "notify-send \"%s\"", message);
    system(command);
}

int isXclipInstalled() {
    const char* command = "which xclip";
    FILE* file = popen(command, "r");
    if (file == NULL) {
        printf("Error executing the command\n");
        return 0;
    }
    char result[256];
    fgets(result, sizeof(result), file);
    pclose(file);
    return (strlen(result) > 0);
}

int main() {
    // Open a connection to the X server
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Failed to open display\n");
        return 1;
    }

    // Get the root window and its dimensions
    Window root = DefaultRootWindow(display);
    int screen_num = DefaultScreen(display);
    int screen_width = DisplayWidth(display, screen_num);
    int screen_height = DisplayHeight(display, screen_num);

    // Create a cursor to indicate the pixel we're selecting
    Cursor cursor = XCreateFontCursor(display, XC_crosshair);

    // Grab the pointer and make it invisible
    XGrabPointer(display, root, False, ButtonPressMask, GrabModeAsync, GrabModeAsync, root, cursor, CurrentTime);
    XDefineCursor(display, root, cursor);
    XFlush(display);

    // Wait for the user to click the mouse button
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == ButtonPress) {
            break;
        }
    }

    // Get the color of the pixel under the mouse cursor
    int x = event.xbutton.x;
    int y = event.xbutton.y;
    XImage* image = XGetImage(display, root, x, y, 1, 1, AllPlanes, ZPixmap);
    if (!image) {
        fprintf(stderr, "Failed to get pixel color\n");
        return 1;
    }
    unsigned long pixel = XGetPixel(image, 0, 0);
    XFree(image);

    // Convert the pixel color to a hex string
    char hexcode[8];
    snprintf(hexcode, 8, "#%06lX", pixel & 0xFFFFFF);

    // Copy the hex string to the clipboard
	if (isXclipInstalled()) {
		char clipboard_cmd[256];
		snprintf(clipboard_cmd, 256, "echo -n \"%s\" | xclip -selection clipboard", hexcode);
		system(clipboard_cmd);
	} else {
		printf("\nxclip is not installed, failed to copy hexcode!\nhttps://github.com/astrand/xclip/blob/master/INSTALL\n");
	}

    // Notify the user
    char message[256];
    snprintf(message, 256, "Copied Color | %s", hexcode);
    notify(message);

    // Clean up
    Cursor default_cursor = XCreateFontCursor(display, XC_left_ptr);
    XDefineCursor(display, root, default_cursor);
    XFlush(display);
    XFreeCursor(display, cursor);
    XUngrabPointer(display, CurrentTime);
    XCloseDisplay(display);
    return 0;
}

