//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_NEWTONROOTFINDER_HPP
#define INCLUDE_EVOLUTION_MATH_NEWTONROOTFINDER_HPP

#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Matrix.hpp>

/**
 * @file    evolution/core/math/NewtonRootFinder.hpp
 * @brief   Class that implements Newton Raphson root finding.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::NewtonRootFinder
 * @see Evolution::Func
 */

namespace Evolution
{

    /**
     * @class NewtonRootFinder
     *
     * @brief Finds the roots of vector functions via the Newton Raphson method
     *
     * Basically calculates
     *   do {
     *          new_x = x - pinv(J'*J) * J' * f(x)
     *      } while (new_x - x < tol)
     *
     * to use:
     * 
     * create a class subclassed from NewtonRootFinder::Func and implement the
     * pure virtual function NewtonRootFinder::Func::apply.
     * NewtonRootFinder::Func::apply can take any rank Vector and return any
     * rank Vector. Then call NewtonRootFinder::find_root with the appropriate
     * inputs, and get the optimized x out.
     * 
     * Func &function is non-const in both functions because the
     * implementation of NewtonRootFinder::Func::apply might have
     * state.
     * 
     * @todo  heavy speed optimization.  This class is fairly slow.
     *
     * @ingroup core_math
     */
    class EVOLUTION_EXPORT_MATH NewtonRootFinder
    {
    public:

        /**
         * @class Func
         *
         * @brief Class that defines the function whose
         * roots will be find with the Newton Raphson method.
         *
         * @ingroup core_math
         */
        class Func
        {
        public:
            /**
             * @brief Computes the value of Func on the given input argument.
             *
             * @param in [in]; Input value
             * @param ret [out]; Output value resulting from Func(in)
             *
             * @return Result indicating success or failure
             */
            virtual Result apply(const Vector<double> &in,Vector<double> *ret) = 0;
            
            /**
             * @brief Standard destructor
             */
            virtual ~Func() {}
        }; // end class Func

    protected:

        /**
         * @brief calculates the jacobian for function at x.
         *
         * @param function       [in]; Function for which to find the jacobian.
         * @param x              [in]; X value at which to find the jacobian.
         * @param y              [out]; Y value at which the jacobian was computed.
         * @param jacobian       [out]; Jacobian value at x.
         * @param jacobian_delta [in]; Threshold parameter to twiddle (default = 1e-8).
         *
         * @return Result indicating success or failure
         */
        Result jacobian(Func &function,
                        const Vector<double> &x,
                        Vector<double> *y,
                        Matrix<double> *jacobian,
                        double jacobian_delta=1e-4);

    public:

        /**
         * @brief Finds the root of the function using the Newton
         * Raphson method.
         *
         * @param function      [in]; Function to optimize
         * @param initial_x     [in]; Initial guess
         * @param tol           [in]; Criteria for success
         * @param max_count     [in]; Maximum number of iterations to attempt (default = 100)
         * @param optimized_x   [out]; Optimized value of x (use NULL if not desired)
         * @param optimized_y   [out]; Optimized value of function(optimized_x) (use NULL if not desired)
         * @param last_jacobian [out]; Last jacobian calculated (use NULL if not desired)
         * @param it_count      [out]; Number of iterations executed till convergence (use NULL if not desired)
         *
         * @return Result indicating success or failure
         */
        Result find_root(Func &function,
                         const Vector<double> &initial_x,
                         const Vector<double> &tol,
                         unsigned int max_count=100,
                         Vector<double> *optimized_x = NULL,
                         Vector<double> *optimized_y = NULL,
                         Matrix<double> *last_jacobian = NULL,
                         unsigned int *it_count = NULL);

        /**
         * @brief Finds a rigid transformation between two matricies.
         *
         * @param start_in       [in]; Initial start matrix
         * @param end_in         [in]; Initial end matrix
         * @param tolerance      [in]; Criteria for success
         * @param max_iterations [in]; Maximum number of iterations to attempt (default = 100)
         * @param optimized_transform [out]; Optimized value of transformation btween the start and end matrices.
         *
         * @return Result indicating success or failure
         */
        static Result find_rigid_transform(const Matrix<double>& start_in, 
                                           const Matrix<double>& end_in, 
                                           const Vector<double> &tolerance,
                                           unsigned max_iterations=100,
                                           Vector<double> *optimized_transform=NULL);
    }; // end class NewtonRootFinder

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_MATH_NEWTONROOTFINDER_HPP
