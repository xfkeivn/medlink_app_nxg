#pragma once

#define CUSTOM_CAPTURE_USER_ID_START		10000

typedef enum 
{
	NO_DRAWING,
	REMOTE_DRAWING,
	LOCAL_DRAWING

}DRAWING_STATUS;

typedef enum
{
	DRAWING_MOUSE_MOVE,
	DRAWING_MOUSE_LEFT_DOWN,
	DRAWING_MOUSE_LEFT_UP,
	DRAWING_SCREEN_CLEAN,
	DRAWING_STATUS_START,
	DRAWING_STATUS_END,
}DRAWING_EVENT;


typedef struct
{
	DRAWING_EVENT drawing_event;
	int drawing_window_width;
	int drawing_window_height;
	int mouse_point_x;
	int mouse_point_y;
	int agora_uer_id;
}DrawingEvent;

class  IDrawingPanelObserver
{
public:
	virtual void on_drawing_panel_vkey_pressed() = 0;
	virtual void on_local_drawing_event(DrawingEvent localDrawingEvent) = 0;
	virtual void on_local_drawing_cleaned() = 0;

};

class   IAppEventObserver
{
public:

	virtual void on_remote_drawing_event(DrawingEvent remoteDrawingEvent) = 0;

};