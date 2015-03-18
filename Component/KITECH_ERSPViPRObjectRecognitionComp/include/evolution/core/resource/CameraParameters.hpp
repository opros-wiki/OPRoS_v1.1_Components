//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE__CAMERA_PARAMETERS_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE__CAMERA_PARAMETERS_HPP


/**
 * @file   evolution/core/resource/CameraParameters.hpp
 * @brief  This class defines intrinsic and extrinsic camera parameters.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::CameraParameters
 */


#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/math/Pose3D.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Vector2.hpp>

#if !EVOLUTION_PLATFORM_AIBO
#include <evolution/core/resource/ResourceConfigTypes.hpp>
#endif

namespace Evolution
{

// FIXME: update class description comments here

    /**
     * @class CameraParameters 
     *
     * @brief This class defines all intrinsic and extrinsic parameters for a
     * given camera, with operations to modify their values.
     *
     * @note Where coordinate transformations are concerned, a little
     * clarity goes a long way, so let us enumerate the conventions
     * used here for rotations (extrinsic parameters).
     *
     * For the various transformations on the camera pose, we speak of
     * two basic coordinate frames:<br>
     *   * the base robot frame B<br>
     *   * the current camera frame C.
     *
     * In addition, we use certain subscripts to further specify the
     * camera frame being used.  Unfortunately, there are two
     * different conventions that can be followed, with equally strong
     * motivations for each, and so we will attempt to support both.
     * The two conventions come from sensor configurations (robotics)
     * and computer vision (cv).  In configuring sensors, it is
     * convenient to use a coordinate frame that has the x-axis
     * pointing forward, similar to the robot's base frame.  In
     * computer vision, the camera's focal axis (i.e., the part of the
     * sensor that points forward) is the z-axis
     *
     * To try to keep these straight, we will denote by subscripts on
     * the camera frames either "x", for the sensor-centric view, which
     * has the x-axis forward, or "z" for the computer-vision-centric
     * view, which has the z-axis forward.
     *
     * The base frame, and those labeled with a subscript "x", follow
     * robotics conventions of axis labeling with positive x forward,
     * y left, and z up. The frames using CV conventions positive z
     * forward, y down, and x right.
     *
     * We use the term "absolute rotation" to denote transformations
     * from the base robot frame to the current camera frame.  Note
     * that these transformations represent rotations between frames.
     * The same transformation rotates a vector in the opposite way
     * (e.g., from the camera frame to the base frame).
     *
     * To denote a rotation, we use the "standard" notation of
     * rot(A,B) to denote a coordinate transformation that rotates a
     * vector, vB, specified in frame B, to a vector, vA, specified in
     * frame A.  In other words<br>
     *    vA = rot(A,B) vB
     *
     * Alternatively, this rotation provides a rotation of the
     * coordinate frame A into coordinate frame B.
     *
     * We then provide four transformations to be retrieved.  Two that
     * are referenced to the base frame, and two that are referenced
     * to the camera's initial frame:
     *
     *   rot_abs = rot(B, Cx) = rot(B, Cx0) rot(Cx0, Cx)
     *   rot_abs_cv = rot(B, Cz) = rot(B, Cx0) rot(Cx0, Cx) rot(Cx, Cz)
     *   rot_rel = rot(Cx0, Cx)
     *   rot_rel_cv = rot(Cz0, Cz) = rot(Cz0, Cx0) rot(Cx0, Cx) rot(Cx, Cz)
     *
     * Note that we also provide transformations from "robot/sensor"
     * to "cv" frames, which are just the rotations between the two
     * frames specified as Z_FWD_TO_X_FWD (rot(Cx0, Cz0)) and
     * X_FWD_TO_Z_FWD (rot(Cz0, Cx0)).
     *
     * WARNING!  We have chosen to store internally only the above
     * rotations, and all other are computed from them.  In terms of
     * actually tracking the motion of the camera, the only one that
     * varies with time is rot_rel = rot(Cx0, Cx).  This rotation is 
     * very useful when trying to drive the camera around on a pan-tilt head.
     * 
     * The alternative, to store rot_rel_cv = rot(Cz0, Cz) was also
     * considered, since it is used very frequently in computer vision
     * calculations.
     *
     * In the end, we chose rot_rel, since the computer vision
     * calculations of rotation are likely to require many fewer
     * computations compared to the overall computational load (for
     * computer vision) than the relative rotation for driving the
     * servos.  No doubt, we will regret this later, and want to
     * change over to storing rot_rel_cv, but that will have to wait.
     *
     * @ingroup core_vision
     * @ingroup core_vision_aibo
     */
    class EVOLUTION_EXPORT_RESOURCE CameraParameters : public ObjectImpl
    {
    public: // Constants
        static const Matrix3 X_FWD_REL_Z_FWD;   ///< x points forward -> z points forward
        static const Matrix3 Z_FWD_REL_X_FWD;   ///< z points forward -> x points forward
    public: // Structors

#if !EVOLUTION_PLATFORM_AIBO
        /**
         * @brief A constructor that takes the configuration of the associated
         * camera device.
         *
         * @param camera_config [in]; The resource configuration data of the
         *      associated camera device
         */
        CameraParameters (const DeviceConfig& camera_config);
#endif        

        /**
         * @brief Default constructor.
         */
        CameraParameters ();

        /**
         * @brief Copy constructor.
         *
         * @param other [in]; The CameraParameters instance to copy
         */
        CameraParameters (const CameraParameters& other); 

        /**
         * @brief Assignment operator.
         *
         * @param other [in]; The CameraParameters instance to assign
         *
         * @return A reference to the newly assigned object.
         */
        CameraParameters& operator=(const CameraParameters& other);

   protected:
        /**
         * @brief Destructor. Protected because you shouldn't allocate 
         * these on the stack. 
         * @see ObjectImpl
         */
        virtual ~CameraParameters ();

    public: // Information

        /**
         * @brief Interface ID for IObject
         */
        static const char * const INTERFACE_ID;

#if !EVOLUTION_PLATFORM_AIBO
        /**
         * @brief Obtains the camera ID.
         *
         * @return The camera ID.
         */
        const char* get_camera_id () const;

        // FIXME: this function is only used externally by the
        // SLAMOdometry behavior.  I think it should be a private
        // function.  The only reason it is called by SLAMOdometry is
        // because there's a set_camera_params() function for a
        // "camera_params" behavior parameter.  I don't think this
        // should be allowed... who will need to change the camera
        // params other than to set them in the config file to the
        // correct values?
        /**
         * @brief Loads the camera calibration file.
         *
         * @param name [in]; The name of the calibration file, minus the
         *                   -intrinsic and -extrinsic parts. This parameter
         *                   can be NULL or an empty string, in which case the
         *                   path to the default calibration file will be
         *                   returned
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result load_camera_calibration(const char* name);

        // FIXME: load_intrinsic_and_extrinsic_parameters() is only
        // public to allow for unit testing.  Specifically
        // core/vision/tests/vision_utils.cpp needs it public.
        /**
         * @brief Loads parameters from the camera calibration files.
         * This is called internally after it does all the
         * path-searching.  It requires that the directory is
         * already part of the prefix.
         *
         * @param prefix [in]; The name of the calibration file, minus the
         *                     -intrinsic and -extrinsic parts
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result load_intrinsic_and_extrinsic_parameters( const char *prefix );
#endif

    public: // Parameter access

        /**
         * @brief This obtains the camera pose relative to the robot.
         *
         * It is important to note that this function returns the pose
         * of the *image plane* relative to the robot.  In the image
         * plane frame, +Z points out of the camera lens, +X points to
         * the right within the image, and +Y points down within the
         * image.
         *
         * On the robot however, +Z points up, +X points forward, and
         * +Y points to the left.
         *
         * @return The position and rotation of the camera relative to
         * the robot.
         */
        Pose3D get_camera_pose_rel_robot() const
        {
            return _camera_rel_robot;
        }

        /**
         * @brief This obtains the camera center (in pixels).
         *
         * @pre cx, and cy are valid pointers.
         * 
         * @param cx [out]; The x coordinate of the camera center (in pixels)
         * @param cy [out]; The y coordinate of the camera center (in pixels)
         */
        void get_camera_center (double* cx, double* cy) const
        {
            PRECOND (cx && cy);
            *cx = _cx;
            *cy = _cy;
        }

        /**
         * @brief Obtains the x and y focal lengths, in pixels.
         *
         * @pre fx, and fy are valid pointers.
         * 
         * @param fx [out]; The x focal length
         * @param fy [out]; The y focal length
         */
        void get_focal_length_xy_pixels (double* fx, double* fy) const
        {
            PRECOND (fx && fy);

            *fx = _fx;
            *fy = _fy;
        }

        /**
         * @brief This obtains the radial distortion parameters.
         *
         * @pre k1, and k2 are valid pointers.
         * 
         * @param k1 [out]; The first radial distortion parameter
         * @param k2 [out]; The second radial distortion parameter
         */
        void get_radial_distortion (double* k1, double* k2) const
        {
            PRECOND (k1 && k2);

            *k1 = _k[0];
            *k2 = _k[1];
        }

        /**
         * @brief Returns the five radial and tangential distortion
         * parameters in a five-element array (quintic).
         *
         * @param kc_out [out]; Points to a 5-element array that
         *      will contain the radial and tangential distortion
         *      parameters
         */
        void get_radial_distortion_quintic (double *kc_out) const
        {
            memcpy(kc_out, _k, sizeof (_k));
        }

        /**
         * @brief Returns the shear on the CCD imager.
         *
         * @return The shear on the CCD imager.
         */
        double get_alpha_c () const
        {
            return _alpha_c;
        }

#if !EVOLUTION_PLATFORM_AIBO
        /**
         * @brief This obtains the value of an arbitrary camera configuaration
         * parameter.
         *
         * @pre name, and value are valid pointers.
         * 
         * @param name  [in]; The name of the paramter
         * @param value [in]; The value of the specified parameter
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_additional_parameter (const char* name,
                                         String*     value) const
        {
            PRECOND (name && value);

            return (_camera_config->get_parameter (name, value));
        }
#endif

        /**
         * @brief 
         * Get the intrinsic parameters in a standard matrix format.
         *
         * This obtains the camera intrinsic parameters in the following
         * standard format:<br>
         * <br>
         *  ./ fx 0  cx \  <br>
         *  (  0  fy cy  ) <br>
         *  .\ 0  0  1  /  <br>
         * <br>
         * where fx = -f/Sx and fy = -f/Sy, f = focal length in mm,
         * and (Sx, Sy) = the effective size of a pixel in mm.
         *
         * @pre A valid Matrix3 pointer.
         * 
         * @param params [out]; The intrinsic parameters in matrix form
         */
        void get_intrinsic_parameters (Matrix3* params) const
        {
            PRECOND (params);

            (*params)[0][0] = _fx;
            (*params)[0][1] =   0;
            (*params)[0][2] = _cx;
            (*params)[1][0] =   0;
            (*params)[1][1] = _fy;
            (*params)[1][2] = _cy;
            (*params)[2][0] =   0;
            (*params)[2][1] =   0;
            (*params)[2][2] =   1;
        } // end get_intrinsic_parameters()

        /**
         * @brief Helper for printing intrisic parameters to an output stream.
         *
         * @param stream [in]; Stream where the intrinsic parameters will be printed out.
         */
        void print_intrinsics( std::ostream & stream);

        /**
         * @brief Transform pixel (image) x,y coordinates to idealized
         * image-plane u,v coordinates.
         *
         * @param pixel_x [in] input X image coordinate
         * @param pixel_y [in] input Y image coordinate
         * @param u [out] the resulting X coordinate, range: -1 to 1
         * @param v [out] the resulting Y coordinate, range: -1 to 1
         */
        void pixel_to_uv(double pixel_x, double pixel_y, double *u, double *v);

        /**
         * @brief 
         * Obtains the matrix that transform from the coordinates
         * of a plane defined by two axes to world coordinates.
         *
         * Given a plane parameterized by axes u,v,
         * let the map from plane coords to world coords be given:
         * P(a,b) = origin + a * u + b * v,
         * find the 3x3 matrix which transforms P(a,b)
         * into a point (x,y,z) with x/z=a and y/z=b.
         *
         *  z will be >0 if the intersection is in front of the camera.
         *  
         *  @param plane_origin [in]; The origin of the plane.
         *  @param plane_u [in]; The u axes defining the plane.
         *  @param plane_v [in]; The v axes defining the plane.
         *  @param matrix_out [out]; The matrix that transform plane
         *      coordinates to world coordinates.
         *
         *  @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result get_homography( const Vector3 &plane_origin,
                               const Vector3 &plane_u,
                               const Vector3 &plane_v,
                               Matrix3       *matrix_out );

        /** 
         * @brief Convert a camera-relative 3-d coordinate to a
         * projected 2d point.
         *
         * @param point_rel_camera_cv [in]; The 3-D camera-relative coordinate of a point.
         * @param image_coordinates_out [out]; The 2D projection image coordinates of the given point. 
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result project( const Vector3 & point_rel_camera,
                        Vector2       * image_coordinates_out );

        /**
         * @brief Transform robot-relative coordinates to
         * camera-relative coordinates.
         */
        Vector3 convert_robot_coords_to_camera( Vector3 point_rel_robot )
        {
            return _camera_rel_robot.inv() * point_rel_robot;
        }

        /**
         * @brief This obtains the zoom factor.
         *
         * @return The zoom factor.
         */
        double get_zoom () const
        {
            return (_zoom);
        }

    public: // Parameter setting functions

        /**
         * @brief This obtains the camera pose relative to the robot.
         *
         * It is important to note that this function returns the pose
         * of the *image plane* relative to the robot.  In the image
         * plane frame, +Z points out of the camera lens, +X points to
         * the right within the image, and +Y points down within the
         * image.
         *
         * On the robot however, +Z points up, +X points forward, and
         * +Y points to the left.
         *
         * @return The position and rotation of the camera relative to
         * the robot.
         */
        void set_camera_pose_rel_robot(const Pose3D & camera_rel_robot)
        {
            _camera_rel_robot = camera_rel_robot;
        }

        /**
         * @brief This sets the camera center.
         *
         * @param cx [out]; The x coordinate of the camera center
         * @param cy [out]; The y coordinate of the camera center
         */
        void set_camera_center (double cx, double cy)
        {
            _cx = cx;
            _cy = cy;
        } // end set_camera_center()

        /**
         * @brief This sets the horizontal and vertical focal lengths, in pixels.
         *
         * @pre fx and fy are not equal to 0.
         * 
         * @param fx [in]; The x focal length, in pixels
         * @param fy [in]; The y focal length, in pixels
         */
        void set_focal_length_xy_pixels (double fx, double fy)
        {
            PRECOND ((fx != 0) && (fy != 0));

            _fx = fx;
            _fy = fy;
        }

        /**
         * @brief This sets the zoom factor.
         *
         * @param zoom [in]; The zoom factor
         */
        void zoom_to (double zoom)
        {
            _zoom = zoom;
        }

    public: // IObject methods

        /**
         * @brief Obtains the interface for this object.
         *
         * @param ticket        [in];  Security ticket
         * @param interface_id  [in];  The name of the interface to obtain
         * @param interface_ptr [out]; The interface
         *
         * @see IResourceDriver::obtain_interface()
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result obtain_interface (TicketId ticket,
                                 const char* interface_id,
                                 void** interface_ptr)
        {
            if (strcmp (interface_id, INTERFACE_ID) != 0)
            {
                *interface_ptr = NULL;
                return RESULT_NOT_FOUND;
            }

            add_ref();
            *interface_ptr = (void*) this;
            return RESULT_SUCCESS;
        }

    protected: // Helpers

        /**
         * @brief This function loads parameters from the camera configuration.
         */
        void load_config();

    private: // private helpers (shh!)

        void init_empty(); ///< Clear intrinsics to initial values which make sense.
        void copy_from(const CameraParameters& other); ///< copy constructor helper function.
        void warn_about_calibration_parameter(const char *param_name);

    protected: // Implementation

#if !EVOLUTION_PLATFORM_AIBO
        String               _camera_id;     ///< String identifying the associated camera.
        const DeviceConfig*  _camera_config; ///< Configuration of the associated camera.
#endif

        // Oulu intrinsic camera model values
        double   _cx;                    ///< Oulu model camera center (x-coordinate).
        double   _cy;                    ///< Oulu model camera center (y-coordinate).
        double   _fx;                    ///< Oulu model X focal length, in pixels.
        double   _fy;                    ///< Oulu model Y focal length, in pixels.
        double   _k[5];                  ///< Oulu model distortion parameters (a quintic: with no constant term).
        double   _alpha_c;               ///< Oulu model skew parameter.

        // Extrinsic parameters.
        Pose3D   _camera_rel_robot;      ///< Pose of the camera image plane relative to the robot.

        double   _zoom;                  ///< Zoom factor.
    }; // end class CameraParameters


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_RESOURCE__CAMERA_PARAMETERS_HPP
