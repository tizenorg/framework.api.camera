/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/





#ifndef __TIZEN_MULTIMEDIA_CAMERA_PRIVATE_H__
#define	__TIZEN_MULTIMEDIA_CAMERA_PRIVATE_H__
#include <camera.h>
#include <mm_camcorder.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DETECTED_FACE 20

typedef enum {
	_CAMERA_EVENT_TYPE_STATE_CHANGE,
	_CAMERA_EVENT_TYPE_FOCUS_CHANGE,
	_CAMERA_EVENT_TYPE_CAPTURE_COMPLETE,
	_CAMERA_EVENT_TYPE_PREVIEW,
	_CAMERA_EVENT_TYPE_CAPTURE,
	_CAMERA_EVENT_TYPE_ERROR,
	_CAMERA_EVENT_TYPE_HDR_PROGRESS,
	_CAMERA_EVENT_TYPE_INTERRUPTED,
	_CAMERA_EVENT_TYPE_FACE_DETECTION,
	_CAMERA_EVENT_TYPE_NUM
}_camera_event_e;

typedef struct _camera_s{
	MMHandleType mm_handle;

	void* user_cb[_CAMERA_EVENT_TYPE_NUM];
	void* user_data[_CAMERA_EVENT_TYPE_NUM];
	void* display_handle;
	camera_display_type_e display_type;
	int state;

	MMMessageCallback relay_message_callback;
	void* relay_user_data;
	int capture_count;
	int capture_width;
	int capture_height;
	bool is_continuous_shot_break;
	bool is_capture_completed;
	int current_capture_count;
	int current_capture_complete_count;
	bool capture_resolution_modified;
	camera_detected_face_s faceinfo[MAX_DETECTED_FACE];
	int num_of_faces;
	bool hdr_keep_mode;
	bool focus_area_valid;
} camera_s;

int _camera_get_mm_handle(camera_h camera , MMHandleType *handle);
int _camera_set_relay_mm_message_callback(camera_h camera, MMMessageCallback callback, void *user_data);
int __camera_start_continuous_focusing(camera_h camera);

#ifdef __cplusplus
}
#endif

#endif //__TIZEN_MULTIMEDIA_CAMERA_PRIVATE_H__

