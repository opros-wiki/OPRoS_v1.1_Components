//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_OBJREC_POSIT_COPLANAR_HPP
#define INCLUDE_EVOLUTION_CORE_OBJREC_POSIT_COPLANAR_HPP

/**
 * @file evolution/core/objrec/PositCoplanar.hpp
 * @brief Iterative pose estimation algorithm.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::PositCoplanar
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Matrix3.hpp>
#include <evolution/core/math/Vector2.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Vector.hpp>

namespace Evolution
{
    /**
     * @class Pose
     * @brief Holds output of PositCoplanar::posit.
     *
     * R and T are the rigid transformation from the object reference
     * frame to the camera reference frame (X_c = R * X_w + T).
     *
     * @ingroup core_objrec
     * @ingroup vision_objrec
     */
    struct EVOLUTION_EXPORT_OBJREC Pose
    {
        Pose() : rotation(0,0,0), trans(0,0,0), its(0), error(HUGE_VAL) {};
        Matrix3 rotation; ///< 3x3 rotation matrix.
        Vector3 trans;    ///< Translation vector.
        unsigned its;     ///< Number of iterations till convergence.
        /**
         * Error at convergence (or on last iteration).  The error is
         * the RMS error (in pixels, according to focal length f)
         * between the projected points and the observed image coords.
         * If a solution is invalid (because the pose causes some
         * points to have negative depth), then E = HUGE_VAL.
         */
        double error;
    };
    
     /* @see
     * http://www.cfar.umd.edu/~daniel/daniel_papersfordownload/CoplanarPts.pdf
     */
    /**
     * @class PositCoplanar
     * @brief Iterative pose estimation algorithm for the coplanar case.
     *
     * Implementation of algorithm described in "Iterative Pose Estimation
     * using Coplanar Feature Points", by D. Oberkampf, D.F. DeMenthon, and
     * L.S. Davis.  CVGIP:Image Understanding, vol. 63, no. 3, May 1996.
     *
     * @ingroup core_objrec
     * @ingroup vision_objrec
     */
    class EVOLUTION_EXPORT_OBJREC PositCoplanar
    {
    public: // Constants
        /// Maximum number of refinement iterations
        static const unsigned int MAX_ITER = 60;

    public: // Structors
        /// Constructor
        PositCoplanar (const Matrix<double> &mi, const Matrix<double> &xy, 
                       const Vector2 &xy0, double focal_length, 
                       double pixel_noise, bool should_remove_outliers = false);

        /// Destructor
        ~PositCoplanar () {}

    public: // Accessors
        /// Set the coordinates of the points in the object reference frame
        /// Set the coordinates of the points in the image plane
        /// Set the image plane coordinates of the center of the coordinate
        /// frame of the object
        Result set_points(const Matrix<double> & mi, const Matrix<double> & xy,
                          const Vector2 &xy0)
        {
            if (mi.nrows () != xy.nrows ())
            {
                return RESULT_INVALID_ARGUMENT;
            }
            _npoints = mi.nrows ();
            _mi      = _mi;
            _xy      = _xy;
            _xy0     = _xy0;
            return RESULT_SUCCESS;
        }

        /// Set the error threshold
        Result set_pixel_noise (double pixel_noise)
        {
            _pixel_noise = pixel_noise;
            return RESULT_SUCCESS;
        }

        /// Set the focal length of the camera
        Result set_focal_length (double focal_length)
        {
            _f = focal_length;
            return RESULT_SUCCESS;
        }

        /// Set whether to represent the Pose in the camera reference frame
        Result set_camera_ref_frame (bool camera_ref_frame)
        {
            _camera_ref_frame = camera_ref_frame;
            return RESULT_SUCCESS;
        }

    public: // Interface
        /// Computation of the two possible solutions to the problem.
        /// Returned ordered so that sol1 has smaller error than sol2.
        Result compute_pose (Pose *sol1, Pose *sol2);

    protected:  // Helpers
        /// Compute initial pose estimation solutions
        Result compute_solutions (Vector<double> &epsi, 
                                  Vector3 *i1, Vector3 *j1, 
                                  Vector3 *i2, Vector3 *j2);

        /// Refine the solution
        Result refine_solution (Vector3 &i, Vector3 &j, Pose *sol);

        /// Compute the unit vectors that define the camera coordinates
        /// in the object reference frame
        Result compute_ijk (const Vector3 &i_est, const Vector3 &j_est, 
                            Vector3 *i, Vector3 *j, Vector3 *k, double *z0);

        /// Compute the mean square error of the pose iteration
        double compute_error (const Vector3 &i, const Vector3 &j, 
                              const Vector3 &k, double z0,
                              bool should_remove_outliers = false);

        /// Remove outliers form computation.  Will do nothing if
        /// _npoints <=3, since this is the minimum required for
        /// PositCoplanar to function.
        void remove_outliers(Vector<double> & proj_error);

    protected: // Implementation
        unsigned       _npoints;         ///< Number of object points.
        Matrix<double> _mi;              /**< (_npoints x 3) object points, 
                                          *    object ref frame. */
        Matrix<double> _mi_pinv;         ///< Pseudo-inverse of _mi.
        Vector2        _xy0;             /**< Image coordinates of the object 
                                          *   reference point. */ 
        Matrix<double> _xy;              /**< (_npoints x 2) xy coords of 
                                          *    observed projected points.  
                                          *    Usually homogenous coordinates.
                                          */
        double         _f;               ///< Focal length (in pixels) of camera (f = 1 for homogeneous
        double         _pixel_noise;     ///< Stop if RMS proj error is < pixel_noise
        Vector3        _u;               /**< Unit vector normal to the plane 
                                          *   containing points. */
        bool           _camera_ref_frame;/**< Choose whether the Pose will be 
                                          *   represented in the camera 
                                          *   reference frame (if false, the 
                                          *   Pose would be represented in 
                                          *   object reference frame). */
        bool           _remove_outliers; ///< Whether to attemp to remove outliers.
    }; // end class PositCoplanar

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_OBJREC_POSIT_COPLANAR_HPP
