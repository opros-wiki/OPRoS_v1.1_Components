//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

/**
 * @file    evolution/core/math/PoseCov2D.hpp
 *
 * @brief A PoseCov2D is a position and orientation in 2-d space,
 * along with uncertainty information about that pose.
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @date    2004/08/12
 */

#ifndef INCLUDE__SRC_EVOLUTION_CORE_MATH_POSECOV2D_HPP
#define INCLUDE__SRC_EVOLUTION_CORE_MATH_POSECOV2D_HPP

#include <iostream>

#include <evolution/core/base/Export.hpp>
#include <evolution/core/math/Pose2D.hpp>

namespace Evolution
{

    /**
     * @class PoseCov2D
     *
     * @brief A 2-D pose, with covariance information.
     *
     * This adds uncertainty, represented as a covariance matrix, to Pose2D.
     *
     * @ingroup core_math
     */
    class EVOLUTION_EXPORT_MATH PoseCov2D : public Pose2D
    {
    public:

        /** @brief Default constructor creates an identity
         * transformation, with zero covariance. */
        PoseCov2D();

        /** @brief Copy contents of source pose into new PoseCov2D and leave
         * covariance with zeros.*/
        PoseCov2D( const Pose2D & source );

        void set_xyh_covariance(const double *new_covariance);
        const double *get_xyh_covariance() const;

        /**
         * @brief Treat the poses like homogeneous coordinate
         * transform matrices, and return a PoseCov2D of the product.
         *
         * This acts like a multiplication of homogeneous coordinate
         * transform matrices for planar position and rotation.  The
         * corresponding composition operation is performed on the
         * covariance matrix as well.
         *
         * Example:
         * PoseCov2D robot_rel_world = landmark_rel_world * robot_rel_landmark;
         *
         * Where robot_rel_landmark (for instance) stores the position
         * of the robot relative to a landmark's coordinate frame.
         *
         * This acts exactly the same as Pose2D::operator*(), with the
         * addition of the covariance matrix composition.
         */
        PoseCov2D operator*( const PoseCov2D& pose ) const;

        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::PoseCov2D& pose)
        {
            pose.print(out);
            return out; 
        }

        /**
         * @brief print out a matrix sub-block
         * @param out [in]; the stream to write to
         */
        void print(std::ostream& out) const;

    public: // Data

        double covariance[3 * 3];       /* symmetric */
    }; // end class PoseCov2D

} // end namespace Evolution

#endif /* INCLUDE__SRC_EVOLUTION_CORE_MATH_POSECOV2D_HPP */

