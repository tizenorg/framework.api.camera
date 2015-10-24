/*
* Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __TIZEN_MULTIMEDIA_CAMERA_INTERNAL_H__
#define	__TIZEN_MULTIMEDIA_CAMERA_INTERNAL_H__
#include <camera.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @file camera_internal.h
  * @brief This file contains the Camera Product-internal API for framework, related structures and enumerations.
  */


/**
 * @internal
 * @addtogroup CAPI_MEDIA_CAMERA_ATTRIBUTES_MODULE_INTERNAL
 * @{
 */

/**
 * @brief Enumeration for the flash state.
 * @since_tizen 2.4
 */
typedef enum
{
    CAMERA_ATTR_FLASH_STATE_OFF = 0,          /**< Off state */
    CAMERA_ATTR_FLASH_STATE_ON,               /**< On state */
} camera_attr_flash_state_e;

/**
 * @}
 */


/**
 * @internal
 * @addtogroup CAPI_MEDIA_CAMERA_DISPLAY_MODULE_INTERNAL
 * @{
 */


/**
 * @brief Enumerations of the camera display plane type.
 * @since_tizen 2.4
 */
typedef enum
{
    CAMERA_DISPLAY_SCALER_MAIN = 0,       /**< Main video plane */
    CAMERA_DISPLAY_SCALER_SUB = 1         /**< Sub scaler plane */
} camera_display_scaler_e;

/**
 * @}
 */


/**
 * @internal
 * @addtogroup CAPI_MEDIA_CAMERA_ATTRIBUTES_MODULE_INTERNAL
 * @{
 */


/**
 * @brief Gets the flash state.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This API returns state of flash controlled by camera API.
 *          It will return CAMERA_ATTR_FLASH_STATE_OFF if flash turned on by device API.
 * @param[out] state The flash state
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @retval #CAMERA_ERROR_NOT_SUPPORTED The feature is not supported
 * @see camera_attr_foreach_supported_flash_mode()
 * @see camera_attr_set_flash_mode()
 * @see camera_attr_get_flash_mode()
 */
int camera_attr_get_flash_state(camera_attr_flash_state_e *state);

/**
 * @}
 */


/**
 * @internal
 * @addtogroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @{
 */

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Called when the media camera needs updated xid.
 * @remarks If current display type is not #CAMERA_DISPLAY_TYPE_OVERLAY, no operation is performed.
 * @param[in] user_data The user data passed from the callback registration function
 * @return The updated xid
 * @pre It will be invoked when camera needs updated xid and if this callback is registered using camera_set_x11_display_pixmap().
 * @see	camera_set_x11_display_pixmap()
 */
typedef unsigned int (*camera_x11_pixmap_updated_cb)(void *user_data);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Called when the media camera needs to inform about rendering error.
 * @remarks If current display type is not #CAMERA_DISPLAY_TYPE_OVERLAY, no operation is performed.
 * @param[in] pixmap_id The pixmap_id where the rendering error occurred
 * @param[in] user_data The user data passed from the callback registration function
 * @see	camera_set_x11_display_pixmap_error_cb()
 */
typedef void (*camera_x11_pixmap_error_cb)(unsigned int *pixmap_id, void *user_data);


/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Sets the display rotation.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function should be called before previewing (see camera_start_preview())\n
 *          This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[in] rotation The display rotation
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_INVALID_OPERATION Display type is not X11
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_start_preview()
 * @see	camera_get_x11_display_rotation()
 */
int camera_set_x11_display_rotation(camera_h camera, camera_rotation_e rotation);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Gets the display rotation.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[out] rotation The display rotation
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_set_x11_display_rotation()
 */
int camera_get_x11_display_rotation(camera_h camera, camera_rotation_e *rotation);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Sets the display flip.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[in] flip The display flip
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @retval #CAMERA_ERROR_INVALID_OPERATION Display type is not X11
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see	camera_get_x11_display_flip()
 */
int camera_set_x11_display_flip(camera_h camera, camera_flip_e flip);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Gets the display flip.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[out] flip The display flip
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_set_x11_display_flip()
 */
int camera_get_x11_display_flip(camera_h camera, camera_flip_e *flip);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Sets the visible property for X11 display.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[in] visible The display visibility property
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_is_x11_display_visible()
 */
int camera_set_x11_display_visible(camera_h camera, bool visible);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Gets the visible property of X11 display.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[out] visible If @c true the camera display is visible, otherwise @c false
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_set_x11_display_visible()
 */
int camera_is_x11_display_visible(camera_h camera, bool *visible);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Sets the X11 display mode.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[in] mode The display mode
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_get_x11_display_mode()
 */
int camera_set_x11_display_mode(camera_h camera , camera_display_mode_e mode);

/**
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @brief Gets the X11 display mode.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/camera
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[out] mode The display mode
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_PERMISSION_DENIED The access to the resources can not be granted
 * @see camera_set_x11_display_mode()
 */
int camera_get_x11_display_mode(camera_h camera, camera_display_mode_e *mode);

/**
 * @brief Registers a callback function to be invoked when camera needs updated xid.
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[in] callback The callback function to register
 * @param[in] user_data	The user data to be passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_OPERATION Invalid operation
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @pre	The camera state must be #CAMERA_STATE_CREATED by camera_create().
 * @post camera_set_x11_display_pixmap() will be invoked.
 *
 * @see camera_set_x11_display_pixmap()
 */
int camera_set_x11_display_pixmap(camera_h camera, camera_x11_pixmap_updated_cb callback, void *user_data);

/**
 * @brief Registers a callback function to be invoked when failure of rendering preview frame happens.
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @remarks This function is valid only for #CAMERA_DISPLAY_TYPE_OVERLAY.
 * @param[in] camera The handle to the camera
 * @param[in] callback The callback function to register
 * @param[in] user_data	The user data to be passed to the callback function
 *
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @pre	The camera state must be #CAMERA_STATE_CREATED by camera_create().
 * @post camera_set_x11_display_pixmap_error_cb() will be invoked.
 *
 * @see camera_set_x11_display_pixmap_error_cb()
 */
int camera_set_x11_display_pixmap_error_cb(camera_h camera, camera_x11_pixmap_error_cb callback, void *user_data);

/**
 * @brief Unregisters the display pixmap error callback function.
 * @ingroup CAPI_MEDIA_CAMERA_X11_DISPLAY_MODULE
 * @param[in] camera The handle to the camera
 * @return @c 0 on success, otherwise a negative error value
 * @retval #CAMERA_ERROR_NONE Successful
 * @retval #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAMERA_ERROR_INVALID_STATE Invalid state
 * @see camera_set_x11_display_pixmap_error_cb()
 */
int camera_unset_x11_display_pixmap_error_cb(camera_h camera);

/**
 * @}
 */


/**
 * @internal
 * @addtogroup CAPI_MEDIA_CAMERA_DISPLAY_MODULE_INTERNAL
 * @{
 */

/**
 * @brief Sets the display scaler
 *
 * @param[in] camera The handle to the camera
 * @param[in] plane The display plane
 * @return	    0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int camera_set_display_scaler(camera_h camera, camera_display_scaler_e plane);

/**
 * @brief Gets the display scaler
 *
 * @param[in] camera The handle to the camera
 * @param[out] plane The display plane
 * @return	    0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 */
int camera_get_display_scaler(camera_h camera, camera_display_scaler_e *plane);

/**
 * @}
 */

/**
 * @internal
 * @addtogroup CAPI_MEDIA_CAMERA_ATTRIBUTES_MODULE_INTERNAL
 * @{
 */

/**
 * @brief Sets the pan level.
 * @details The range for pan level is getting from camera_attr_get_pan_range(). If @a pan is out of range, #CAMERA_ERROR_INVALID_PARAMETER error occurred.
 *
 * @param[in] camera    The handle to the camera
 * @param[in] pan      The pan level
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_get_pan()
 * @see camera_attr_get_pan_range()
 */
int camera_attr_set_pan(camera_h camera, int pan);

/**
 * @brief Sets the tilt level.
 * @details The range for tilt level is getting from camera_attr_get_tilt_range(). If @a tilt is out of range, #CAMERA_ERROR_INVALID_PARAMETER error occurred.
 *
 * @param[in] camera    The handle to the camera
 * @param[in] tilt      The tilt level
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_get_tilt()
 * @see camera_attr_get_tilt_range()
 */
int camera_attr_set_tilt(camera_h camera, int tilt);

/**
 * @brief Gets the pan level.
 *
 * @param[in] camera    The handle to the camera
 * @param[out] pan     The pan level
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_set_pan()
 * @see camera_attr_get_pan_range()
 */
int camera_attr_get_pan(camera_h camera, int *pan);

/**
 * @brief Gets the tilt level.
 *
 * @param[in] camera    The handle to the camera
 * @param[out] tilt     The tilt level
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_set_tilt()
 * @see camera_attr_get_tilt_range()
 */
int camera_attr_get_tilt(camera_h camera, int *tilt);

/**
 * @brief Gets the available pan level.
 *
 * @param[in] camera    The handle to the camera
 * @param[out] min      The minimum pan level
 * @param[out] max      The maximum pan level
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_set_pan()
 * @see camera_attr_get_pan()
 */
int camera_attr_get_pan_range(camera_h camera, int *min, int *max);

/**
 * @brief Gets the available tilt level.
 *
 * @param[in] camera    The handle to the camera
 * @param[out] min      The minimum tilt level
 * @param[out] max      The maximum tilt level
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_set_tilt()
 * @see camera_attr_get_tilt()
 */
int camera_attr_get_tilt_range(camera_h camera, int *min, int *max);

/**
 * @brief Sets the bitrate for encoded preview stream.
 *
 * @param[in] camera    The handle to the camera
 * @param[in] bitrate   The bps of encoded preview stream
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_get_encoded_preview_bitrate()
 */
int camera_attr_set_encoded_preview_bitrate(camera_h camera, int bitrate);

/**
 * @brief Gets the bitrate for encoded preview stream.
 *
 * @param[in] camera    The handle to the camera
 * @param[out] bitrate  The bps of encoded preview stream
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_set_encoded_preview_bitrate()
 */
int camera_attr_get_encoded_preview_bitrate(camera_h camera, int *bitrate);

/**
 * @brief Sets the I-frame interval for encoded preview stream.
 *
 * @param[in] camera    The handle to the camera
 * @param[in] interval  The I-frame interval of encoded preview stream (milisecond)
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_get_encoded_preview_iframe_interval()
 */
int camera_attr_set_encoded_preview_iframe_interval(camera_h camera, int interval);

/**
 * @brief Gets the I-frame interval for encoded preview stream.
 *
 * @param[in] camera    The handle to the camera
 * @param[out] interval  The I-frame interval of encoded preview stream (milisecond)
 * @return        0 on success, otherwise a negative error value.
 * @retval      #CAMERA_ERROR_NONE Successful
 * @retval      #CAMERA_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see camera_attr_set_encoded_preview_iframe_interval()
 */
int camera_attr_get_encoded_preview_iframe_interval(camera_h camera, int *interval);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__TIZEN_MULTIMEDIA_CAMERA_INTERNAL_H__
