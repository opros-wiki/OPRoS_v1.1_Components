//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__ICAMERA_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__ICAMERA_HPP


/**
 * @file    evolution/core/resource/ICamera.hpp
 * @brief   Interface for a camera
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::ICamera
 * @see Evolution::CameraResolution
 **/


#include <deque>

#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/EulerAngles.hpp>
#include <evolution/core/math/Matrix3.hpp>
#include <evolution/core/resource/Colorspace.hpp>
#include <evolution/core/resource/CameraParameters.hpp>



namespace Evolution
{


    class Image;
    class ICameraGroup;
    class IVideoStream;


    /**
     * @interface ICamera
     *
     * @brief This class provides an interface to a single camera.
     *
     * This is a standard interface class.  Camera drivers need to inherit this
     * interface (class CameraDriver : public ICamera) and implement all pure
     * virtual methods.
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ICamera
    {
    public: // Types

        /**
         * @brief This class encapsulates camera resolution.
         *
         * Each CameraResolution object simply encodes a height and a width,
         * and supports the operator== and operator!= operation.
         *
         * @ingroup core_resource
         */
        class EVOLUTION_EXPORT_RESOURCE CameraResolution
        {
        public:
            /// Constructor
            CameraResolution()
                : _x(0),
                  _y(0)
            {
            }

            /**
             * @brief Constructor taking x/y resolution values
             * @param  x [in]; x coordinate for resolution
             * @param  y [in]; y coordinate for resolution
             **/
            CameraResolution(unsigned int x, unsigned int y)
                : _x(x),
                  _y(y)
            {
            }

            /**
             * @brief Equality operator.
             *
             * This is the equality operator.
             * @param cam_res [in]; CameraResolution object to test against.
             * @return true if equal, false otherwise.
             */
            bool operator==(const CameraResolution &cam_res) const
            {
                return (_x == cam_res._x && _y == cam_res._y);
            }

            /**
             * @brief Inequality operator.
             *
             * This is the inequality operator.
             * @param cam_res [in]; CameraResolution object to test against.
             * @return true if not equal, false otherwise.
             */
            bool operator!=(const CameraResolution &cam_res) const
            {
                return (!operator==(cam_res));
            }

        public:

            unsigned int _x; ///< width of the camera resolution
            unsigned int _y; ///< height of the camera resolution
        };

        /**
         * @brief This is a list of camera reolutions.
         */
        typedef std::deque<CameraResolution> CameraResolutionList;


        /**
         * @brief This is a list of the modes of operation available to the camera.
         */
        enum CaptureMode
        {
            CAPTURE_SINGLE=0,          ///< Capture single frames.
            CAPTURE_STREAM             ///< Have camera stream frames to internal circular buffer.
        };

    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char* const INTERFACE_ID; // "Evolution.ICamera"

    public: // Structors

        /**
         * @brief Constructor
         */
        ICamera () {}

        /**
         * @brief Destructor
         */
        virtual ~ICamera () {}

    public: // Status

        /**
         * @brief This function obtains the device opened for video input.
         *
         * Obtains the identity string for the device opened for video input.
         *
         * @return String with ident string of video device.
         */
        virtual String get_video_device () const = 0;

        /**
         * @brief This function obtains the number of internal buffers to be used.
         *
         * Obtains the number of internal buffers to be used.
         *
         * @return number of used buffers.
         */
        virtual unsigned get_buffer_count () const = 0;

        /**
         * @brief This function obtains the frame rate in FPS.
         *
         * Obtains the frame rate in FPS.
         *
         * @return float value of FPS
         */
        virtual float get_frame_rate () const = 0;

        /** 
         * @brief This function obtains the horizontal and vertical resolution.
         *
         *  Obtains the horizontal and vertical resolution.
         *
         *  @param hres [out]; horizontal resolution.
         *  @param vres [out]; vertical resolution.
         */ 
        virtual void get_resolution (unsigned* hres, unsigned* vres) const = 0;

        /** 
         * @brief This function obtains the desired video format for capture.
         *
         * Obtains the desired video format for capture.
         *
         * @return ColorFormat of the video capture.
         */
        virtual ColorFormat get_video_format () const = 0;

        /**
         * @brief This function obtains the list of color formats that the camera supports,
         * according to the specified criteria.
         *
         * @param ticket [in]; Security TicketId
         * @param search_format [in]; format to search
         * @param matching [in]; ColorSearchCriterion type
         * @param match_list [out]; List of ColorFormats this device supports
         * @see ColorspaceUtils::find_matching_format
         * @return Result indicating success or failure.
         **/
        virtual Result get_supported_formats (TicketId ticket,
                                              ColorFormat search_format,
                                              unsigned matching,
                                              ColorFormatList* match_list) = 0;

        /**
         * @brief This function obtains the list of camera resolutions 
         * the camera supports, according to the specified criteria.
         * @note This will return RESULT_NOT_IMPLEMENTED for LinuxCamera, but works for DirectXCamera (Windows only).
         * @param ticket [in]; Security TicketId
         * @param search_format [in]; format used for match
         * @param resolution_list [out]; list of CameraResolutions this camera supports
         * @return Result indicating success or failure.
         **/
        virtual Result get_supported_resolutions (TicketId ticket,
                                                  ColorFormat search_format,
                                                  CameraResolutionList* resolution_list) = 0;

        /**
         * @brief This function obtains the capture mode.
         *
         * Obtains the capture mode.
         * @return The CaptureMode of the device.
         **/
        virtual CaptureMode get_capture_mode () const = 0;

        /**
         * @brief This function obtains the current camera parameters.
         *
         * Obtains the current camera parameters.
         * @return The current device CameraParameters
         **/
        virtual const CameraParameters* get_parameters () const = 0;

        /**
         * @brief This function obtains the current camera exposure.
         *
         * Obtains the current camera exposure setting.
         * @param  exposure [out]; Exposure, measured in microseconds.
         * @return Result can be RESULT_NOT_IMPLEMENTED on some cameras.
         **/
        virtual Result get_exposure (long* exposure) = 0;

        /**
         * @brief This function obtains the current camera gain.
         *
         * Obtains the current camera gain setting.
         * @param  gain [out]; Gain, a multiplier.
         * @return Result can be RESULT_NOT_IMPLEMENTED on some cameras.
         **/
        virtual Result get_gain (float* gain) = 0;

        /**
         * @brief This function obtains the current camera exposure.
         *
         * Obtains the current camera gamma setting.
         * @param  gamma [out]; Gamma, a multiplier.
         * @return Result can be RESULT_NOT_IMPLEMENTED on some cameras.
         **/
        virtual Result get_gamma (float *gamma) = 0;

        /**
         * @brief Get the minimum value of exposure.
         * @return Minimum value of exposure.
         */
        virtual long get_min_exposure () = 0;
        /**
         * @brief Get the minimum value of the gain.
         * @return Minimum value of the gain.
         */
        virtual float get_min_gain () = 0;
        /**
         * @brief Get the minimum value of gamma.
         * @return Minimum value of gamma.
         */
        virtual float get_min_gamma () = 0;
        /**
         * @brief Get the maximum value of exposure.
         * @return Maximum value of exposure.
         */
        virtual long get_max_exposure () = 0;
        /**
         * @brief Get the maximum value of the gain.
         * @return Maximum value of the gain.
         */
        virtual float get_max_gain () = 0;
        /**
         * @brief Get the maximum value of gamma.
         * @return Maximum value of gamma.
         */
        virtual float get_max_gamma () = 0;

    public: // Configuration

        /**
         * @brief This function sets the device to be opened for video input.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  device [in]; Camera and OS dependent device to use for video input
         */
        virtual Result set_video_device (TicketId ticket, const char* device) = 0;

        /**
         * @brief This function sets the number of internal buffers to be used.
         *
         * @note Care must be taken to ensure that no writing to the buffers
         * occurs while they are being resized. Generally this means
         * deactivating the camera temporarily. The ERSP provided camera types will deactivate and reactivate automatically.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param   count [in]; The new buffer size
         **/
        virtual Result set_buffer_count (TicketId ticket, unsigned count) = 0;

        /**
         * @brief This function sets the frame rate in FPS.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  fps [in]; The new frame rate
         */
        virtual Result set_frame_rate (TicketId ticket, float fps) = 0;

        /**
         * @brief This function sets the horizontal and vertical resolution.
         *
         * @note Care must be taken to ensure that no writing to the buffers
         * occurs while they are being resized. Generally this means
         * deactivating the camera temporarily.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  hor [in]; The horizontal resolution
         * @param  ver [in]; The vertical resolution
         **/
        virtual Result set_resolution (TicketId ticket, unsigned hor, unsigned ver) = 0;

        /**
         * @brief This function sets the desired video format for capture, changing the
         * resolution simultaneously if non-zero values are
         * given.
         *
         * @note If an exact match for the color format is not critical,
         * other criteria may be specified (OR'ed values of type
         * ColorMatchCriterion).
         *
         * Format and resolution are grouped together because they
         * both affect the size of the buffer, and changing them in
         * two steps would be inefficent.
         *
         * Care must be taken to ensure that no writing to the buffers
         * occurs while they are being resized. Generally this means
         * deactivating the camera temporarily.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  format [in]; The new format
         * @param  hor [in]; The new horizontal resolution
         * @param  ver [in]; The new vertical resolution
         * @param  match_criteria [in]; The ColorMatchCriterion used for the match
         **/
        virtual Result set_video_format (TicketId ticket, ColorFormat format,
                                         unsigned hor = 0, unsigned ver = 0,
                                         unsigned match_criteria = COLOR_MATCH_EXACT) = 0;

        /**
         * @brief This function sets the capture mode.
         *
         * @note Care must be taken to ensure that no writing to the buffers
         * occurs while they are being resized. Generally this means
         * deactivating the camera temporarily.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  mode [in]; The new capture mode
         **/
        virtual Result set_capture_mode (TicketId ticket, CaptureMode mode) = 0;
        
        /**
         * @brief This function sets the camera exposure.
         *
         * Obtains the current camera exposure setting.
         * @param  ticket [in]; The security ticket id
         * @param  exposure [in]; 
         * @return Result can be RESULT_NOT_IMPLEMENTED on some cameras.
         **/
        virtual Result set_exposure (TicketId ticket, long exposure) = 0;

        /**
         * @brief This function sets the camera gain.
         *
         * Obtains the current camera gain setting.
         * @param  ticket [in]; The security ticket id
         * @param  gain [in]; 
         * @return Result can be RESULT_NOT_IMPLEMENTED on some cameras.
         **/
        virtual Result set_gain (TicketId ticket, float gain) = 0;

        /**
         * @brief This function sets the camera exposure.
         *
         * Obtains the current camera gamma setting.
         * @param  ticket [in]; The security ticket id
         * @param  gamma [in]; 
         * @return Result can be RESULT_NOT_IMPLEMENTED on some cameras.
         **/
        virtual Result set_gamma (TicketId ticket, float gamma) = 0;

    public: // Movement (absolute in base frame)

        /**
         * @brief This function rotates the camera to the specified pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  rotation [in]; Desired rotation target specified as Matrix3
         */
        virtual Result rotate_to (TicketId ticket, const Matrix3& rotation) = 0;

        /**
         * @brief This function rotates the camera to the specified pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  rotation [in]; The desired rotation target specified as EulerAngles
         */
        virtual Result rotate_to (TicketId ticket, const EulerAngles& rotation) = 0;

        /**
         * @brief This function zooms the camera to the specified level.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  zoom [in]; The zoom level to use
         * @note This function is currently not implemented by drivers supplied with ERSP. A call will return RESULT_NOT_IMPLEMENTED.
         */
        virtual Result zoom_to (TicketId ticket, double zoom) = 0;

    public: // Movement (relative to current pose)

        /**
         * @brief This function pans the camera relative to the initial camera pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  dtheta [in]; The degree of pan desired
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result pan_rel_ZYX (TicketId ticket, double dtheta) = 0;

        /**
         * @brief This function tilts the camera relative to the initial camera pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  dphi [in]; The degree of tilt desired
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result tilt_rel_ZYX (TicketId ticket, double dphi) = 0;

        /**
         * @brief This function rolls the camera relative to the initial camera pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  dpsi [in]; The degree of roll desired
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result roll_rel_ZYX (TicketId ticket, double dpsi) = 0;
        
        /**
         * @brief This function pans the camera relative to the current pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  dtheta [in]; The degree of pan desired
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result pan_rel (TicketId ticket, double dtheta) = 0;

        /**
         * @brief This function tilts the camera relative to the current pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  dphi [in]; The degree of tilt desired
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result tilt_rel (TicketId ticket, double dphi) = 0;

        /**
         * @brief This function rolls the camera relative to the current relative pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  dpsi [in]; The degree of roll desired
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result roll_rel (TicketId ticket, double dpsi) = 0;

        /**
         * @brief This function rotates the camera relative to the current pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  rotation [in]; The desired rotation target as a Matrix3
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result rotate_rel (TicketId ticket, const Matrix3& rotation) = 0;

        /**
         * @brief This function rotates the camera relative to the current pose.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  rotation [in]; The desired rotation target as an EulerAngles
         * @note Only implemented by ERSP for PhidgetCamera, not currently available for Linux.
         */
        virtual Result rotate_rel (TicketId ticket, const EulerAngles& rotation) = 0;

    public: // Capture

        /**
         * @brief This function obtains a pointer to the internal Image at the specified
         * index in the buffer (idx=0 is the most recent frame, idx=1
         * the second most recent, etc.).
         *
         * As this method provides access to the raw buffer, the image
         * data may be changed. It is the responsibility of the caller
         * to be done with the image before it is overwritten, either
         * by adjusting the frame rate or buffer count, or by making a
         * copy of the image (in which case the get_image_copy() function is a
         * better choice).
         *
         * In any case, the caller MUST call remove_ref() on the image
         * pointer when done!
         *
         * @note While the image data may change or be resized, the driver
         * must NOT delete the image itself, but rather call
         * remove_ref() on it. The caller of get_image() must be
         * guaranteed a valid image pointer until it calls
         * remove_ref().
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param img [in/out]; This will assign the pointer into the internal Image buffer for the specified index. Do not free this memory directly, but instead call the remove_ref() function on the returned Image when finished.
         * @param idx [in];  The index into the buffer from 0=most-recent up to n=oldest.
         **/
        virtual Result get_image (TicketId ticket, const Image** img, unsigned idx = 0) = 0;

        /**
         * @brief This function copies the image at the specified index in the buffer
         * (idx=0 is the most recent frame, idx=1 is the second most
         * recent, etc.) to img_copy, possibly performing a format
         * conversion.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param img_copy [out]; This is a pointer to an allocated copy of the specified Image from the internal buffer located by index. Caller must free.
         * @param idx [in];  The index into the buffer from 0=most-recent up to n=oldest.
         * @param dst_format [in]; Optional argument to alter ColorFormat for Image copy
         **/
        virtual Result get_image_copy (TicketId ticket, Image* img_copy, unsigned idx = 0,
                                       ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED) = 0;

        /**
         * @brief Copy the image with the best matching timestamp from the camera's ring buffer.
         *
         * The point of this function is to try to recover an image "indexed"
         * by it's timestamp.  The lowest delta image will always be selected,
         * and if all deltas are greater than the delta parameter, a
         * RESULT_NOT_FOUND will be returned.
         * 
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param img_copy [out]; This is a pointer to an allocated copy of the specified Image from the internal buffer located by index. Caller must free.
         * @param timestamp [in];  Timestamp to try to get matching image for.
         * @param delta [in];  Give up if an image can't be found with delta < abs(image.timestamp - timestamp)
         * @param dst_format [in]; Optional argument to alter ColorFormat for Image copy
         **/
        virtual Result get_image_copy (TicketId ticket, Image* img_copy, Timestamp timestamp, Timestamp delta=200,
                                       ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED) = 0;

        /**
         * @brief Retrieves the timestamp of an image in the ring buffer.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param timestamp [out]; This is a pointer to the image timestamp
         *    being retrieved.
         * @param idx [in];  The index into the buffer from 0=most-recent up to n=oldest.
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_image_timestamp (TicketId ticket, Timestamp* timestamp, unsigned idx = 0) = 0;
        
    public: // Streaming

        /**
         * @brief This function adds the camera to the given video stream, to which it will
         * push captured frames.
         * @param  ticket [in]; The (TicketId) for security purposes
         * @param  stream_id [in]; The string label for the video stream
         * @param  stream [in]; The target stream for the camera feed
         * @note Not currently implemented by ERSP devices.
         **/
        virtual Result add_to_stream (TicketId ticket, const char* stream_id, 
                                      const IVideoStream& stream) = 0; 

        /**
         * @brief This function removes the camera from the specified video stream.
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param  stream_id [in]; The string label for the video stream
         * @note Not currently implemented by ERSP devices.
         */
        virtual Result remove_from_stream (TicketId ticket, const char* stream_id) = 0;

    }; // end class ICamera


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_CORE_RESOURCE__ICAMERA_HPP

