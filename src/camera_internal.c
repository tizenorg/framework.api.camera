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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mm.h>
#include <mm_camcorder.h>
#include <mm_types.h>
#include <camera.h>
#include <camera_internal.h>
#include <camera_private.h>
#include <glib.h>
#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_N_CAMERA"

static gboolean __mm_video_frame_render_error_callback(void *param, void *user_data);


static gboolean __mm_video_frame_render_error_callback(void *param, void *user_data)
{
	camera_s *handle = (camera_s*)user_data;

	if (handle == NULL) {
		LOGE("handle is NULL");
		return TRUE;
	}

	if (handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR]) {
		((camera_x11_pixmap_error_cb)handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR])((unsigned int *)param, handle->user_data[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR]);
	}

	return TRUE;
}


int camera_set_x11_display_rotation(camera_h camera, camera_rotation_e rotation)
{
	return camera_set_display_rotation(camera, rotation);
}

int camera_get_x11_display_rotation(camera_h camera, camera_rotation_e *rotation)
{
	return camera_get_display_rotation(camera, rotation);
}

int camera_set_x11_display_flip(camera_h camera, camera_flip_e flip)
{
	return camera_set_display_flip(camera, flip);
}

int camera_get_x11_display_flip(camera_h camera, camera_flip_e *flip)
{
	return camera_get_display_flip(camera, flip);
}

int camera_set_x11_display_visible(camera_h camera, bool visible)
{
	return camera_set_display_visible(camera, visible);
}

int camera_is_x11_display_visible(camera_h camera, bool* visible)
{
	return camera_is_display_visible(camera, visible);
}

int camera_set_x11_display_mode(camera_h camera, camera_display_mode_e mode)
{
	return camera_set_display_mode(camera, mode);
}

int camera_get_x11_display_mode(camera_h camera, camera_display_mode_e* mode)
{
	return camera_get_display_mode(camera, mode);
}

int camera_set_x11_display_pixmap(camera_h camera, camera_x11_pixmap_updated_cb callback, void *user_data)
{
	int ret;
	camera_s *handle = (camera_s*)camera;
	camera_state_e capi_state;

	if (handle == NULL || callback == NULL) {
		LOGE("INVALID_PARAMETER(handle:%p,callback:%p,user_data:%p)", handle, callback, user_data);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	camera_get_state(camera, &capi_state);
	if (capi_state > CAMERA_STATE_CREATED) {
		LOGE("INVALID STATE(state:%d)", capi_state);
		return CAMERA_ERROR_INVALID_STATE;
	}

	ret = mm_camcorder_set_attributes(handle->mm_handle, NULL,
	                                  MMCAM_DISPLAY_SURFACE, MM_DISPLAY_SURFACE_X_EXT,
	                                  MMCAM_DISPLAY_HANDLE, callback, sizeof(unsigned int (void *)),
	                                  MMCAM_DISPLAY_HANDLE_USER_DATA, user_data, sizeof(void *),
	                                  NULL);

	return __convert_camera_error_code(__func__, ret);
}

int camera_set_x11_display_pixmap_error_cb(camera_h camera, camera_x11_pixmap_error_cb callback, void *user_data)
{
	int ret;
	camera_s *handle = (camera_s*)camera;
	camera_state_e capi_state;

	if (handle == NULL || callback == NULL) {
		LOGE("INVALID_PARAMETER(handle:%p,callback:%p,user_data:%p)", handle, callback, user_data);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	camera_get_state(camera, &capi_state);
	if (capi_state > CAMERA_STATE_CREATED) {
		LOGE("INVALID STATE(state:%d)", capi_state);
		return CAMERA_ERROR_INVALID_STATE;
	}

	ret = mm_camcorder_set_video_frame_render_error_callback(handle->mm_handle, __mm_video_frame_render_error_callback, (void *)handle);
	if (ret == MM_ERROR_NONE) {
		LOGI("set pixmap_error_cb(0x%08x) and user_data(0x%8x)", callback, user_data);
		handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR] = callback;
		handle->user_data[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR] = user_data;
		LOGI("Event type : %d ", _CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR);
		return CAMERA_ERROR_NONE;
	}

	return __convert_camera_error_code(__func__, ret);
}

int camera_unset_x11_display_pixmap_error_cb(camera_h camera)
{
	camera_s *handle = (camera_s*)camera;
	camera_state_e capi_state;

	if (handle == NULL) {
		LOGE("INVALID_PARAMETER(handle:%p)", handle);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	camera_get_state(camera, &capi_state);
	if (capi_state > CAMERA_STATE_CREATED) {
		LOGE("INVALID STATE(state:%d)", capi_state);
		return CAMERA_ERROR_INVALID_STATE;
	}

	mm_camcorder_set_video_frame_render_error_callback(handle->mm_handle, (mm_camcorder_video_frame_render_error_callback)NULL, (void *)NULL);
	handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR] = NULL;
	handle->user_data[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR] = NULL;

	return CAMERA_ERROR_NONE;
}
