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
#include <unistd.h>
#include <mm.h>
#include <mm_camcorder.h>
#include <mm_types.h>
#include <camera.h>
#include <camera_internal.h>
#include <camera_private.h>
#include <glib.h>
#include <dlog.h>
#include <vconf.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_N_CAMERA"

static gboolean __mm_video_frame_render_error_callback(void *param, void *user_data);


static gboolean __mm_video_frame_render_error_callback(void *param, void *user_data)
{
	camera_s *handle = (camera_s *)user_data;

	if (handle == NULL) {
		LOGE("handle is NULL");
		return true;
	}

	if (handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR]) {
		((camera_x11_pixmap_error_cb)handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR])((unsigned int *)param,
													   handle->user_data[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR]);
	}

	return true;
}


int camera_attr_get_flash_state(camera_attr_flash_state_e *state)
{
	int flash_state = VCONFKEY_CAMERA_FLASH_STATE_OFF;
	int camera_pid = -1;
	char process_path[20] = "";

	if (state == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)",CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	if (vconf_get_int(VCONFKEY_CAMERA_PID, &camera_pid)) {
		if (errno == EPERM || errno == EACCES) {
			LOGE("flash state error with permission");
			return CAMERA_ERROR_PERMISSION_DENIED;
		} else {
			LOGE("VCONFKEY_CAMERA_FLASH_STATE is not supported");
			return CAMERA_ERROR_NOT_SUPPORTED;
		}
	}

	if (vconf_get_int(VCONFKEY_CAMERA_FLASH_STATE, &flash_state)) {
		if (errno == EPERM || errno == EACCES) {
			LOGE("flash state error with permission");
			return CAMERA_ERROR_PERMISSION_DENIED;
		} else {
			LOGE("VCONFKEY_CAMERA_FLASH_STATE is not supported");
			return CAMERA_ERROR_NOT_SUPPORTED;
		}
	}

	if (camera_pid > -1) {
		snprintf(process_path, sizeof(process_path), "/proc/%d", camera_pid);

		LOGD("test path : [%s]", process_path);

		if (!access(process_path, F_OK)) {
			LOGD("flash state %d", flash_state);
			*state = flash_state;
		} else {
			LOGW("invalid camera_pid %d", camera_pid);
			*state = CAMERA_ATTR_FLASH_STATE_OFF;
		}
	} else {
		LOGW("invalid camera_pid %d", camera_pid);
		*state = CAMERA_ATTR_FLASH_STATE_OFF;
	}

	return CAMERA_ERROR_NONE;
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


int camera_is_x11_display_visible(camera_h camera, bool *visible)
{
	return camera_is_display_visible(camera, visible);
}


int camera_set_x11_display_mode(camera_h camera, camera_display_mode_e mode)
{
	return camera_set_display_mode(camera, mode);
}


int camera_get_x11_display_mode(camera_h camera, camera_display_mode_e *mode)
{
	return camera_get_display_mode(camera, mode);
}


int camera_set_x11_display_pixmap(camera_h camera, camera_x11_pixmap_updated_cb callback, void *user_data)
{
	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;
	camera_state_e capi_state = CAMERA_STATE_NONE;

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
					  MMCAM_DISPLAY_HANDLE, callback, sizeof(void *),
					  MMCAM_DISPLAY_HANDLE_USER_DATA, user_data, sizeof(void *),
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_set_x11_display_pixmap_error_cb(camera_h camera, camera_x11_pixmap_error_cb callback, void *user_data)
{
	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;
	camera_state_e capi_state = CAMERA_STATE_NONE;

	if (handle == NULL || callback == NULL) {
		LOGE("INVALID_PARAMETER(handle:%p,callback:%p,user_data:%p)", handle, callback, user_data);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	camera_get_state(camera, &capi_state);
	if (capi_state > CAMERA_STATE_CREATED) {
		LOGE("INVALID STATE(state:%d)", capi_state);
		return CAMERA_ERROR_INVALID_STATE;
	}

	ret = mm_camcorder_set_video_frame_render_error_callback(handle->mm_handle,
								 __mm_video_frame_render_error_callback,
								 (void *)handle);
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
	camera_s *handle = (camera_s *)camera;
	camera_state_e capi_state = CAMERA_STATE_NONE;

	if (handle == NULL) {
		LOGE("INVALID_PARAMETER(handle:%p)", handle);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	camera_get_state(camera, &capi_state);
	if (capi_state > CAMERA_STATE_CREATED) {
		LOGE("INVALID STATE(state:%d)", capi_state);
		return CAMERA_ERROR_INVALID_STATE;
	}

	mm_camcorder_set_video_frame_render_error_callback(handle->mm_handle,
							   (mm_camcorder_video_frame_render_error_callback)NULL,
							   (void *)NULL);

	handle->user_cb[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR] = NULL;
	handle->user_data[_CAMERA_EVENT_TYPE_VIDEO_FRAME_RENDER_ERROR] = NULL;

	return CAMERA_ERROR_NONE;
}


int camera_set_display_scaler(camera_h camera, camera_display_scaler_e plane)
{
	if (camera == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;

	ret = mm_camcorder_set_attributes(handle->mm_handle, NULL,
					  MMCAM_DISPLAY_SCALER, plane,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_get_display_scaler(camera_h camera, camera_display_scaler_e *plane)
{
	if (camera == NULL || plane == NULL) {
		LOGE("INVALID_PARAMETER camera %p, plane %p", camera, plane);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;

	ret = mm_camcorder_get_attributes(handle->mm_handle, NULL,
					  MMCAM_DISPLAY_SCALER, plane,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_set_pan(camera_h camera, int pan)
{
	if (camera == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;

	ret = mm_camcorder_set_attributes(handle->mm_handle, NULL,
					  MMCAM_CAMERA_PAN, pan,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_set_tilt(camera_h camera, int tilt)
{
	if (camera == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;

	ret = mm_camcorder_set_attributes(handle->mm_handle, NULL,
					  MMCAM_CAMERA_TILT, tilt,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_get_pan(camera_h camera, int *pan)
{
	if (camera == NULL || pan == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;

        ret = mm_camcorder_get_attributes(handle->mm_handle, NULL,
					  MMCAM_CAMERA_PAN, pan,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_get_tilt(camera_h camera, int *tilt)
{
	if (camera == NULL || tilt == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;

	ret = mm_camcorder_get_attributes(handle->mm_handle, NULL,
					  MMCAM_CAMERA_TILT, tilt,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_get_pan_range(camera_h camera, int *min, int *max)
{
	if (camera == NULL || min == NULL || max == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;
	MMCamAttrsInfo ainfo;

	ret = mm_camcorder_get_attribute_info(handle->mm_handle, MMCAM_CAMERA_PAN, &ainfo);
	if (ret == MM_ERROR_NONE) {
		*min = ainfo.int_range.min;
		*max = ainfo.int_range.max;
	}

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_get_tilt_range(camera_h camera, int *min, int *max)
{
	if (camera == NULL || min == NULL || max == NULL) {
		LOGE("INVALID_PARAMETER(0x%08x)", CAMERA_ERROR_INVALID_PARAMETER);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	int ret = MM_ERROR_NONE;
	camera_s *handle = (camera_s *)camera;
	MMCamAttrsInfo ainfo;

	ret = mm_camcorder_get_attribute_info(handle->mm_handle, MMCAM_CAMERA_TILT, &ainfo);
	if (ret == MM_ERROR_NONE) {
		*min = ainfo.int_range.min;
		*max = ainfo.int_range.max;
	}

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_set_encoded_preview_bitrate(camera_h camera, int bitrate)
{
	camera_s *handle = (camera_s *)camera;
	int ret = MM_ERROR_NONE;

	if (camera == NULL || bitrate <= 0) {
		LOGE("invalid param : handle %p, bitrate %d", camera, bitrate);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	ret = mm_camcorder_set_attributes(handle->mm_handle, NULL,
					  MMCAM_ENCODED_PREVIEW_BITRATE, bitrate,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_get_encoded_preview_bitrate(camera_h camera, int *bitrate)
{
	camera_s *handle = (camera_s *)camera;
	int ret = MM_ERROR_NONE;

	if (camera == NULL || bitrate == NULL) {
		LOGE("invalid param : handle %p, bitrate %p", camera, bitrate);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	ret = mm_camcorder_get_attributes(handle->mm_handle, NULL,
					  MMCAM_ENCODED_PREVIEW_BITRATE, bitrate,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_set_encoded_preview_iframe_interval(camera_h camera, int interval)
{
	camera_s *handle = (camera_s *)camera;
	int ret = MM_ERROR_NONE;

	if (camera == NULL || interval <= 0) {
		LOGE("invalid param : handle %p, interval %d", camera, interval);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	ret = mm_camcorder_set_attributes(handle->mm_handle, NULL,
					  MMCAM_ENCODED_PREVIEW_IFRAME_INTERVAL, interval,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}


int camera_attr_get_encoded_preview_iframe_interval(camera_h camera, int *interval)
{
	camera_s *handle = (camera_s *)camera;
	int ret = MM_ERROR_NONE;

	if (camera == NULL || interval == NULL) {
		LOGE("invalid param : handle %p, interval %p", camera, interval);
		return CAMERA_ERROR_INVALID_PARAMETER;
	}

	ret = mm_camcorder_get_attributes(handle->mm_handle, NULL,
					  MMCAM_ENCODED_PREVIEW_IFRAME_INTERVAL, interval,
					  NULL);

	return __convert_camera_error_code(__func__, ret);
}
