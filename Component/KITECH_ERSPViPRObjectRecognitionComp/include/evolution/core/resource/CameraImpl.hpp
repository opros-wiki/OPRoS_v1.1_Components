//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__CAMERA_IMPL_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__CAMERA_IMPL_HPP


/**
 * @file   evolution/core/resource/CameraImpl.hpp
 * @brief  Helper class for implementing ICamera
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::CameraImpl
 */


#include <limits.h>
#include <vector>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/resource/ICamera.hpp>
#include <evolution/core/resource/ResourceDriverImpl.hpp>
#include <evolution/core/resource/Image.hpp>


namespace Evolution
{

    /**
     * @interface CameraImpl
     *
     * @brief Helper class for implementing ICamera
     *
     * The CameraImpl class provides buffer management, simple accessors, and
     * stubs for optional methods.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE CameraImpl : public ICamera,
                                                 public ResourceDriverImpl
    {
    public: // Constants

        static const long INVALID_FRAME = -1;   ///< Index to indicate no valid frames.

    public: // Structors

        /**
         * @brief Constructor
         * 
         * @param  ticket [in]; The security ticket
         * @param  resource_config [in]; The camera's resource configuration 
         *                               data
         * @param  resource_container [in]; The resource container containing
         *                                  the camera
         */
        CameraImpl (TicketId ticket,
                    const ResourceConfig& resource_config,
                    IResourceContainer& resource_container)
            : ResourceDriverImpl (ticket, resource_config, resource_container),
              _camera_parameters (NULL),
              _current_frame (INVALID_FRAME),
              _capture_mode (CAPTURE_SINGLE),
              _hres (0),
              _vres (0),
              _video_format (COLOR_FORMAT_UNSPECIFIED),
              _frame_rate (0),
              _exposure (0),
              _gain (0.0),
              _gamma(0.0),
              /* these are overwritten on cameras that support reading the values */
              _min_exposure (0),
              _min_gain (0.0),
              _min_gamma(0.0),
              _max_exposure (16688), // The Lumenera has this as a max
              _max_gain (10.0),
              _max_gamma(5.0)
        {
            _camera_parameters = new CameraParameters((const DeviceConfig&)_resource_config);
        }

        /**
         * @brief Destructor
         */
        virtual ~CameraImpl () 
        {
            for (size_t i = 0; i < _camera_buffer.size (); ++i)
            {
                _camera_buffer[i]->remove_ref();
            }
            _camera_buffer.clear ();
            if (_camera_parameters != NULL)
            {
                _camera_parameters->remove_ref();
            }
        }

    public: // Status

        /**
         * @brief The get_video_device function obtains the device opened for
         * video input.
         *
         * @return The video device name.
         */
        virtual String get_video_device () const
        {
            return (_device_name);
        } // end get_video_device()

        /**
         * @brief The get_buffer_count function obtains the number of internal
         * buffers to be used.
         * 
         * @return The number of image buffers used by the driver.
         */
        virtual unsigned get_buffer_count () const { return (_camera_buffer.size ()); }

        /**
         * @brief The get_frame_rate function obtains the frame rate in FPS.
         *
         * @return The frame rate parameter.
         */
        virtual float get_frame_rate () const { return (_frame_rate); }

        /**
         * @brief The get_resolution function obtains the horizontal and
         * vertical resolution.
         * 
         * @param hor [out]; The horizontal resolution
         * @param ver [out]; The vertical resolution
         */
        virtual void get_resolution (unsigned* hor, unsigned* ver) const
        {
            ASSERT (hor && ver);
            *hor = _hres;
            *ver = _vres;
        }

        /**
         * @brief The get_video_format function obtains the desired video
         * format for capture.
         * 
         * @return The camera's current video format.
         */
        virtual ColorFormat get_video_format () const { return (_video_format); }

        /**
         * @brief The get_capture_mode function obtains the capture mode.
         *
         * @return The camera's current capture mode.
         */
        virtual CaptureMode get_capture_mode () const { return (_capture_mode); }

        /**
         * @brief Get the list of resolutions supported by the camera for a
         * particular video format.
         *
         * @param ticket [in]; The security ticket id
         * @param search_format [in]; The desired color format
         * @param resolution_list [out]; The list of resolutions supported for 
         *                               the specified format.
         *
         * @return RESULT_NOT_IMPLEMENTED.
         */
        virtual Result get_supported_resolutions (TicketId ticket,
                                                  ColorFormat search_format,
                                                  CameraResolutionList* resolution_list)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

        /**
         * @brief The get_parameters function obtains the current camera
         * parameters.
         *
         * @return The driver's camera parameters.
         */
        virtual const CameraParameters* get_parameters () const { return (_camera_parameters); }

        /**
         * @brief Get the current exposure setting.
         *
         * @param exposure [out]; The exposure
         *
         * @return RESULT_NOT_IMPLEMENTED
         */
        virtual Result get_exposure (long* exposure)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

        /**
         * @brief Get the current gain setting.
         *
         * @param gain [out]; The gain
         *
         * @return RESULT_NOT_IMPLEMENTED
         */
        virtual Result get_gain (float* gain)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

        /**
         * @brief Get the current gamma setting.
         *
         * @param gamma [out]; The gamma
         *
         * @return RESULT_NOT_IMPLEMENTED
         */
        virtual Result get_gamma (float* gamma)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

        /**
         * @brief Get the minimum value of exposure.
         * @return Minimum value of exposure.
         */
        long get_min_exposure() { return _min_exposure; }
        /**
         * @brief Get the minimum value of the gain.
         * @return Minimum value of the gain.
         */
        float get_min_gain() { return _min_gain; }
        /**
         * @brief Get the minimum value of gamma.
         * @return Minimum value of gamma.
         */
        float get_min_gamma() { return _min_gamma; }
        /**
         * @brief Get the maximum value of exposure.
         * @return Maximum value of exposure.
         */
        long get_max_exposure() { return _max_exposure; }
        /**
         * @brief Get the maximum value of the gain.
         * @return Maximum value of the gain.
         */
        float get_max_gain() { return _max_gain; }
        /**
         * @brief Get the maximum value of gamma.
         * @return Maximum value of gamma.
         */
        float get_max_gamma() { return _max_gamma; }

    public: // Configuration

        /**
         * @brief The set_buffer_count function sets the number of internal
         * buffers to be used.
         *
         * @param ticket [in]; The security ticket
         * @param count [in]; The desired buffer count
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result set_buffer_count (TicketId ticket, unsigned count);

        /**
         * @brief Set the exposure setting.
         *
         * @param ticket [in]; The security ticket id
         * @param exposure [in]; The exposure
         *
         * @return RESULT_NOT_IMPLEMENTED
         */
        virtual Result set_exposure (TicketId ticket, long exposure)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

        /**
         * @brief Set the gain setting.
         *
         * @param ticket [in]; The security ticket id
         * @param gain [in]; The gain
         *
         * @return RESULT_NOT_IMPLEMENTED
         */
        virtual Result set_gain (TicketId ticket, float gain)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

        /**
         * @brief Set the gamma setting.
         *
         * @param ticket [in]; The security ticket id
         * @param gamma [in]; The gamma
         *
         * @return RESULT_NOT_IMPLEMENTED
         */
        virtual Result set_gamma (TicketId ticket, float gamma)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }


    public: // Movement (absolute)

        /**
         * @brief The rotate_to function rotates the camera to the specified
         * pose
         * 
         * @param  ticket [in]; The security ticket
         * @param  rotation [in]; The rotation to rotate to
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result rotate_to (TicketId ticket, const Matrix3& rotation) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The rotate_to function rotates the camera to the specified
         * pose
         *
         * @param  ticket [in]; The security ticket
         * @param  orientation [in]; The orientation in Euler angles to
         *                           rotate to
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result rotate_to (TicketId ticket, const EulerAngles& orientation) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The zoom_to function sets Zoom to the specified level.
         * 
         * @param  ticket [in]; The security ticket
         * @param  zoom [in]; The desired zoom level
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result zoom_to (TicketId ticket, double zoom) { return (RESULT_NOT_IMPLEMENTED); }

    public: // Movement (relative to the initial camera  pose.

        /**
         * @brief The pan_rel_ZYX function pans the camera relative to the
         * initial camera pose.  
         *
         * @param ticket [in]; The security ticket
         * @param dtheta [in]; The relative angular distance to pan in radians
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result pan_rel_ZYX (TicketId ticket, double dtheta) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The tilt_rel_ZYX function tilts the camera relative to the
         * initial camera pose.  
         *
         * @param ticket [in]; The security ticket
         * @param dphi [in]; The relative angular distance to tilt in radians
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result tilt_rel_ZYX (TicketId ticket, double dphi) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The roll_rel_ZYX function rolls the camera relative to the
         * initial camera pose.
         * 
         * @param  ticket [in]; The security ticket
         * @param  dpsi [in]; The relative angular distance to roll, in radians
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result roll_rel_ZYX (TicketId ticket, double dpsi) { return (RESULT_NOT_IMPLEMENTED); }

    public: // Movement (relative to current pose)

        /**
         * @brief The pan_rel function pans the camera relative to the current
         * pose.
         *
         * @param  ticket [in]; The security ticket
         * @param dtheta [in]; The relative angular distance to pan in radians
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result pan_rel (TicketId ticket, double dtheta) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The tilt_rel function tilts the camera relative to the
         * current pose.
         *
         * @param  ticket [in]; The security ticket
         * @param dphi [in]; The relative angular distance to tilt in radians
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result tilt_rel (TicketId ticket, double dphi) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The roll_rel function rolls the camera relative to the
         * current pose.
         * 
         * @param  ticket [in]; The security ticket
         * @param  dpsi [in]; The relative angular distance to roll, in radians
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result roll_rel (TicketId ticket, double dpsi) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The rotate_rel function rotates the camera relative to the
         * current pose.
         * 
         * @param  ticket [in]; The security ticket
         * @param  rotation [in]; The relative rotation distance to rotate,
         *                        in Euler angles
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result rotate_rel (TicketId ticket, const EulerAngles& rotation) { return (RESULT_NOT_IMPLEMENTED); }

        /**
         * @brief The rotate_rel function rotates the camera relative to the
         * current pose.
         * 
         * @param  ticket [in]; The security ticket
         * @param  rotation [in]; The relative rotation to rotate, as a
         *                        rotation matrix
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result rotate_rel (TicketId ticket, const Matrix3& rotation) { return (RESULT_NOT_IMPLEMENTED); }

    public: // Capture

        /**
         * @brief The get_image function obtains the image at the specified
         * index in the buffer (0 = most recent, 1 = second most recent).
         *
         * In capture mode CAPTURE_STREAM, it is assumed that another
         * thread is writing to the buffer. 
         *
         * In capture mode CAPTURE_SINGLE, however, the frame needs to
         * be acquired, and thus get_single_frame() is called, which
         * should be implemented by the driver.
         *
         * The returned image is allocated on the heap and has a 
         * reference count.  The Image::remove_ref() method should 
         * be called to free up the image when it is no longer needed. 
         * 
         * @param ticket [in]; The security ticket
         * @param img [out]; The pointer to the acquired image
         * @param idx [in]; The index into the internal image ringer buffer
         *                  where the image should be obtained.  Defaults to 0,
         *                  which is the latest acquired image
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_image (TicketId ticket, const Image** img, unsigned idx = 0);

        /**
         * @brief The get_image function obtains the image at the specified
         * timestamp or within the specified timestamp delta.
         *
         * In capture mode CAPTURE_STREAM, it is assumed that another
         * thread is writing to the buffer. 
         *
         * In capture mode CAPTURE_SINGLE, however, the frame needs to
         * be acquired, and thus get_single_frame() is called, which
         * should be implemented by the driver.
         *
         * The returned image is allocated on the heap and has a 
         * reference count.  The Image::remove_ref() method should 
         * be called to free up the image when it is no longer needed. 
         * 
         * @param ticket [in]; The security ticket
         * @param img [out]; The pointer to the acquired image
         * @param timestamp [in]; Timestamp at which the output image should have been taken.
         * @param delta [in]; Interval around the provided timestamp that will be acceptable for selecting the image to be returned.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_image (TicketId ticket, const Image** img, Timestamp timestamp, Timestamp delta);

        /**
         * @brief The get_image_copy function copies the image at the specified
         * index in the buffer (idx=0 is the most recent frame) to img_copy,
         * possibly performing a format conversion.  The caller is responsible
         * for calling remove_ref() on the copy to free it when the image 
         * copy is no longer needed.
         *
         * @see  get_image() for capture information.
         *
         * @param  ticket   [in];  The security ticket
         * @param  img_copy [out]; The pointer to the acquired image copy
         * @param  idx      [in, opt];  The index into the internal image ringer buffer
         *                         where the image should be obtained.  Defaults
         *                         to 0, which is the latest acquired image
         * @param  dst_format [in, opt]; An optional color format.  If different
         *                          from the capture format, the copy will be
         *                          converted to the dst_format if possible
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_image_copy (TicketId ticket,
                                       Image* img_copy, 
                                       unsigned idx = 0,
                                       ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED);

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
        virtual Result get_image_copy (TicketId ticket, Image* img_copy, Timestamp timestamp, Timestamp delta,
                                       ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED);
        
        /**
         * @brief Retrieves the timestamp of an image in the ring buffer.
         *
         * @param  ticket [in]; The (TicketId) for security purposes.
         * @param timestamp [out]; This is a pointer to the image timestamp
         *    being retrieved.
         * @param idx [in];  The index into the buffer from 0=most-recent up to n=oldest.
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_image_timestamp (TicketId ticket, Timestamp* timestamp, unsigned idx = 0);
        
    public: // Streaming

        /**
         * @brief The add_to_stream function adds the camera to the given video
         * stream, to which it will push captured frames.
         *
         * @param  ticket [in]; The security ticket
         * @param  stream_id [in]; The id of the stream to add the camera to
         * @param  stream [in]; The video stream to add the camera to
         * 
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result add_to_stream (TicketId ticket, const char* stream_id, 
                                      const IVideoStream& stream)
        {
            return (RESULT_NOT_IMPLEMENTED);
        } 

        /**
         * @brief The remove_from_stream function removes the camera from the
         * specified video stream.
         *
         * @param  ticket [in]; The security ticket
         * @param  stream_id [in]; The stream id of the video stream
         *                         to remove the camera from
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result remove_from_stream (TicketId ticket, const char* stream_id)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

    protected: // Internal types

        /**
         * @brief Array of images making up the camera's image buffer.
         */
        typedef std::vector<Image*> CameraBuffer;

    protected: // Overrides

        /**
         * @brief The get_single_frame funciton acquires a single frame from
         * the camera immediately, writing the data to the given image.
         *
         * This method is called by get_image() and get_image_copy()
         * in capture mode CAPTURE_SINGLE. It is responsible for
         * advancing the _current_frame.
         *
         * @note The output_image is assumed to be sized correctly for the
         * capture data!
         *
         * @param output_image [out]; The image obtained from the camera
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        virtual Result get_single_frame (Image* output_image)
        {
            return (RESULT_NOT_IMPLEMENTED);
        }

    protected: // Helpers

        /** 
         * @brief The logical_to_physical_index function converts a logical
         * (historical: 0 = most recent) to physical buffer index.
         *
         * @param logical [in]; The logical index
         * @param physical [out]; The corresponding physical index
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result logical_to_physical_index (unsigned logical, unsigned* physical) const
        {
            ASSERT (physical);
            size_t count = get_buffer_count ();
            if (logical >= count)
            {
                return (RESULT_OUT_OF_BOUND);
            }
            size_t idx = _current_frame + count - logical;
            ASSERT (idx >= 0);
            *physical = idx % count;
            return (RESULT_SUCCESS);
        } // end logical_to_physical_index()

        /**
         * @brief The get_next_frame function obtains the next frame.
         *
         * @return The index of the next frame in the image buffer.
         */
        unsigned get_next_frame () const
        {
            unsigned count = get_buffer_count ();
            PRECOND (count > 0);
            return ((_current_frame + 1) % count);
        }

        /**
         * @brief The set_buffer_resolution function sets the horizontal and
         * vertical resolution on all internal buffers, and also updates the
         * appropriate member variables.
         *
         * @param hor [in]; The horizontal resolution to set
         * @param ver [in]; The vertical resolution to set
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result set_buffer_resolution (unsigned hor, unsigned ver);

        /**
         * @brief The set_buffer_format function sets the video format and
         * resolution resolution simultaneously (for effienciency) on all
         * internal buffers. This function also updates the appropriate member
         * variables.
         *
         * @param format [in]; The new video format
         * @param hor [in, opt]; The horizontal resolution to set
         * @param ver [in, opt]; The vertical resolution to set
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result set_buffer_format (ColorFormat format,
                                  unsigned hor = 0, unsigned ver = 0);

    protected: // Assignment

        /**
         * @brief Copy constructor
         * This constructor is hidden, not implemented.
         * 
         * @param other [in]; The instance to be copied
         */
        CameraImpl (const CameraImpl& other)
            : ResourceDriverImpl (other._resource_ticket,
                                  other._resource_config, 
                                  other._resource_container),
                                  _camera_parameters (NULL)
        {
            _camera_parameters = new CameraParameters((const DeviceConfig&)_resource_config);
        }

        /**
         * @brief This assignment operator is hidden. Not implemented.
         * 
         * @param other [in]; The other instance to be assigned to this 
         *                    instance
         *
         * @return A reference to the newly assigned object.
         */
        CameraImpl& operator= (const CameraImpl& other)
        {
            return (*this);
        }

    protected: // Implementation

        // Device information.
        String            _device_name;       ///< The camera device name.
        CameraParameters  *_camera_parameters; ///< The camera parameters.
        // Storage.
        CameraBuffer _camera_buffer; ///< The camera driver's image buffer.
        long         _current_frame; ///< The index of the current frame in the image buffer.
        // Capture configuration.
        CaptureMode _capture_mode; ///< The current capture mode (single or stream).
        unsigned    _hres;         ///< The horizontal resolution.
        unsigned    _vres;         ///< The vertical resolution.
        ColorFormat _video_format; ///< The video format.
        float       _frame_rate;   ///< The current frame rate for stream capture mode.
        long        _exposure;     ///< The camera exposure, measured in microseconds.
        float       _gain;         ///< The camera gain.
        float       _gamma;        ///< The camera gamma.
        long        _min_exposure; ///< Min camera exposure, measured in microseconds.
        float       _min_gain;     ///< Min camera gain.
        float       _min_gamma;    ///< Min camera gamma.
        long        _max_exposure; ///< Max camera exposure, measured in microseconds.
        float       _max_gain;     ///< Max camera gain.
        float       _max_gamma;    ///< Max camera gamma.
    }; // end class CameraImpl

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_RESOURCE__CAMERA_IMPL_HPP
