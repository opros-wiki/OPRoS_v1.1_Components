//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH__KALMAN_FILTER_HPP
#define INCLUDE_EVOLUTION_MATH__KALMAN_FILTER_HPP
/**
 * @file  evolution/core/math/KalmanFilter.hpp
 * @brief Take in vector input data (measurements), and output filtered data 
 *        (estimates).
 *
 * Implements (Extended) Kalman filter on data.  By allowing the user
 * to over-ride the state transition, error, observation, and Jacobian
 * functions, an extended Kalman filter can be realized.
 *
 * @see Evolution::KalmanFilter
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Vector.hpp>

namespace Evolution 
{

    /**
     * @class KalmanFilter
     * @brief Does basic filtering for 2-D tracking.  Can be extended to n-D.
     * 
     * Takes a measurement, y_k+1, of a system of the form:
     *   x_k+1 = f(x_k) + q;  where q~N(0,_Q) (Gaussian, 0 mean, _Q cov)
     *   y_k = h(x_k)+ w;     where w~N(0,_R)
     *
     * and returns an estimate x_k+1 and the covariance of the estimate, P.
     *
     * You can set the functions f and h (and Jac_f and Jac_h, the
     * Jacobians of f and h, resp.) in the constructor of the filter.
     * Also, you can set the process and measurement noise covariances
     * and the initial conditions.
     *
     * By default, the system sets up a 2-D state and measurement, and simple
     * (identity) state transition and measurement functions.
     *
     * @ingroup core_math
     */
    class EVOLUTION_EXPORT_MATH KalmanFilter
    {

    public: // Types.

        // These are the types of maps that are used by the Kalman filter.

        /**
         * @brief Mapping for State Transition.
         *
         * Implements the function
         *     f(x_k, t, dt),
         * where the first argument is x_k, the current state vector;
         * the second argument is the current time, t;
         * and the third argument is the time difference from the last time. 
         * The return value is the vector output of the function, f().
         *
         * @param x_k [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @return Vector output of the function, f()
         *
         * @see Vector
         */
        typedef Vector<double> (*StateTransition)(Vector<double>& x_k, double t, double dt);

        /**
         * @brief Mapping for the observation.
         *
         * Implements the function
         *     h(x_k, t, dt),
         * where the first argument is x_k, the current state vector;
         * the second argument is the current time, t;
         * and the third argument is the time difference from the last time.
         *
         * The return value is the vector output of the function, h(),
         * which is the observation associated with the current state
         * vector x_k.
         *
         * @param x_k [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @param meas_dim [in]; Measurement dimensionality
         * @return Vector output of the function, h()
         *
         * @see Vector
         */
        typedef Vector<double> (*ObsMap)(Vector<double>& x_k, double t, double dt, unsigned meas_dim);

        /**
         * @brief The error function used to compare states.
         *
         * Returns a vector relating the difference between two
         * states.  By default, this is just a true difference (v_1 -
         * v_2), but this function can also be useful when working
         * with differences of rotations, which need to be returned
         * mod 2PI.
         *
         * @param v_1 [in]; First state vector
         * @param v_2 [in]; Second second vector
         * @return Difference error
         *
         * @see Vector
         */
        typedef Vector<double> (*ErrorFunction)(Vector<double>& v_1, Vector<double>& v_2);

        /**
         * @brief The Jacobian (derivative with respect to state
         * vector) of the state transition function.
         *
         * @param x_k [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @param jac [out]; Jacobian matrix (This matrix is assumed to be allocated by the caller of the function. The function is only in charge of filling in the entries of the matrix).
         * @return Boolean value: "true" if the Jacobian is the identity matrix.
         *
         * @see Vector, Matrix
         */
        typedef bool (*StateJacobian)(Vector<double>& x_k, double t, double dt, Matrix<double>* jac);

        /**
         * @brief The Jacobian (derivative with respect to state
         * vector) of the state transition function.
         *
         * @param x_k [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @param meas_dim [in]; Measurement dimensionality
         * @param jac [out]; Jacobian matrix (This matrix is assumed to be allocated by the caller of the function. The function is only in charge of filling in the entries of the matrix).
         * @return Boolean value: "true" if the Jacobian is the identity matrix.
         *
         * @see Vector, Matrix
         */
        typedef bool (*ObsJacobian)(Vector<double>& x_k, double t, double dt, unsigned meas_dim, Matrix<double>* jac);

    public: // Structors.
        
        // Constructor
        /**
         * @brief Constructor for a Kalman filter, with several
         * optional arguments.
         *
         * This constructor is intended to be used in one of three ways:
         * 1) Specify no arguments.
         *
         *    In this case, you get a system with a 2D state and 2D
         *    measurement, where all maps are just the identity, and
         *    the error function is the trivial one.
         *
         * 2) Specify the state dimensions and the f, h, Jac_f, and
         *    Jac_h functions.
         *
         *    In this case, the error is just defaulted to a simple 
         *    difference.

         * 3) Specify all the parameters. 
         *
         *    In this case, you have full control over the functions used 
         *    in the Kalman filter.
         *
         * @param state_dim [in]; Dimension of the state vector
         * @param measure_dim [in]; Dimension of the observation, or output, vector
         * @param f_in [in]; State transition function f()
         * @param h_in [in]; Observation map h()
         * @param Jac_f_in [in]; Jacobian of state transition function f()
         * @param Jac_h_in [in]; jacobian of observation map h()
         * @param error_fun_in [in]; Error function
         *
         */
        KalmanFilter (unsigned state_dim = 2, unsigned measure_dim = 2,
                      StateTransition f_in = f_fun_default, 
                      ObsMap h_in = h_fun_default, 
                      StateJacobian Jac_f_in = Jac_f_default, 
                      ObsJacobian Jac_h_in = Jac_h_default, 
                      ErrorFunction error_fun_in = error_fun_default)
        {
            init_dimensions(state_dim, measure_dim);

            _f_fun = f_in;
            _h_fun = h_in;
            _Jac_f = Jac_f_in;
            _Jac_h = Jac_h_in;
            _error_fun = error_fun_in;
            _is_Jac_f_identity = _is_Jac_h_identity = false;
        };

    public: 

        /**
         * @brief Initializes the dimensions of the Kalman filter.
         * 
         * @param state_dim [in]; Dimension of the state vector.
         * @param measure_dim [in]; Dimension of the observation, or output, vector.
         * 
         */
        void init_dimensions (unsigned state_dim, unsigned measure_dim)
        {
            _state_dim = state_dim;
            _measure_dim = measure_dim;
            _Q.eye(_state_dim);
            _R.eye(_measure_dim);

            _mu_ekf.fast_resize(_state_dim);
            _P_ekf.fast_resize(_state_dim, _state_dim);

            _mu_ekf_pred.fast_resize(_state_dim);
            _P_ekf_pred.fast_resize(_state_dim, _state_dim);
            _y_pred.fast_resize(_measure_dim);
        }
            
        /**
         * @brief Initializes the state of the Kalman filter.
         * 
         * @param mu_0 [in]; Initial value of the state vector
         * @param P_0 [in]; Initial value of the covariance of the estimate
         * @param t [in]; Initial time (optional); default is 0.0.
         */
        void init_filter(Vector<double>& mu_0, Matrix<double>& P_0, 
                         double t = 0.0)
        {
            PRECOND (mu_0.size ()  == _state_dim);
            PRECOND ((P_0.nrows () == _state_dim) && 
                     (P_0.ncols () == _state_dim));
            _mu_ekf = mu_0;
            _P_ekf = P_0;
            _t_old = t;
        }

        /**
         * @brief Computes mu and P for the next (extended) Kalman filter step.
         * 
         * @param y [in]; Measurement at current time
         * @param t [in]; Current time
         * @param mu_ekf_out [out]; Estimated mean from the filter
         * @param P_ekf_out [out]; Covariance of the estimate
         *
         * @return Result indicating success or failure
         */
        Result compute_stats(Vector<double>& y, double t,
                             Vector<double>* mu_ekf_out, 
                             Matrix<double>* P_ekf_out);

        /**
         * @brief Gets the predicted output from the last Kalman filter
         * transition. 
         * 
         * @param mu_ekf_out [out]; Current estimate of the state
         *
         * @return Result indicating success or failure
         */
        Result get_state(Vector<double>* mu_ekf_out) const;

    public: // Setting.

        /**
         * @brief Sets the state noise covariance matrix.
         * 
         * @param Q [in]; State noise covariance matrix
         */
        void set_Q(const Matrix<double>& Q) 
        {
            PRECOND ((Q.nrows () == _state_dim) && 
                     (Q.ncols () == _state_dim));
            _Q = Q;
        }

        /**
         * @brief Sets the measurement noise covariance matrix.
         * 
         * @param R [in]; The measurement noise covariance matrix
         */
        void set_R(const Matrix<double>& R) 
        {
            PRECOND ((R.nrows () == _measure_dim) && 
                     (R.ncols () == _measure_dim));
            _R = R;
        }

        /**
         * @brief Sets the state transition function.
         * 
         * @param f_in [in]; StateTransition function to be used in
         *     the filter.
         * @param state_dim [in]; Dimension of the state, used to
         *     verify correct state dimension (optional); defaults to 2.
         */
        void set_f_fun (StateTransition f_in, unsigned state_dim = 2)
        {
            PRECOND (state_dim == _state_dim);
            _f_fun = f_in;
        }

        /**
         * @brief Sets the observation map.
         * 
         * @param h_in [in]; Observation map to be used in the filter.
         * @param state_dim [in]; Dimension of the state, used to
         *     verify correct state dimension (optional); defaults to 2.
         * @param measure_dim [in]; Dimension of the measurement, used to
         *     verify correct measurement dimension (optional); defaults to 2.
         */
        void set_h_fun (ObsMap h_in, unsigned state_dim = 2, 
                        unsigned measure_dim = 2)
        {
            PRECOND ((state_dim == _state_dim) && 
                     (measure_dim == _measure_dim));
            _h_fun = h_in;
        }

        /**
         * @brief Sets the Jacobian (linearization) of f_fun.
         * 
         * @param Jac_f_in [in]; Jacobian of the StateTransition function
         * @param state_dim [in]; Dimension of the state, used to
         *     verify correct state dimension (optional); defaults to 2.
         */
        void set_Jac_f_fun (StateJacobian Jac_f_in, unsigned state_dim = 2)
        {
            PRECOND (state_dim == _state_dim);
            _Jac_f = Jac_f_in;
        }

        /**
         * @brief Sets the Jacobian (linearization) of h_fun.
         * 
         * @param Jac_h_in [in]; Jacobian of the Observation Map
         * @param state_dim [in]; Dimension of the state, used to
         *     verify correct state dimension (optional); defaults to 2.
         * @param measure_dim [in]; Dimension of the measurement, used to
         *     verify correct dimension (optional); defaults to 2.
         */
        void set_Jac_h_fun (ObsJacobian Jac_h_in, unsigned state_dim = 2, 
                            unsigned measure_dim = 2)
        {
            PRECOND ((state_dim == _state_dim) && 
                     (measure_dim == _measure_dim));
            _Jac_h = Jac_h_in;
        }

        /**
         * @brief Sets the ErrorFunction used in the filter.
         * 
         * @param error_fun_in [in]; ErrorFunction to be used by the filter.
         * @param measure_dim [in]; Dimension of the measurement, used to
         *     verify correct dimension (optional); defaults to 2.
         */
        void set_error_fun (ErrorFunction error_fun_in, 
                            unsigned measure_dim = 2)
        {
            PRECOND (measure_dim == _measure_dim);
            _error_fun = error_fun_in;
        }

    protected: 
        // These can all be set externally using the accessor methods
        // above, to allow arbitrary, time-dependent functions.

        StateTransition _f_fun; ///< One-step state transition map
        ObsMap _h_fun;          ///< Observation map
        StateJacobian _Jac_f;   ///< Jacobian linearization of f_fun
        ObsJacobian _Jac_h;     ///< Jacobian linearization of h_fun.
        ErrorFunction _error_fun; ///< Error function used in computing difference between y and y_pred.

    protected: // Internal default functions.
        /**
         * @brief Default one-step state transition map
         *
         * @param x [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @return Vector output of the function, f()
         */
        static Vector<double> f_fun_default(Vector<double>& x, double t, 
                                            double dt)
        {
            return (x);
        }

        /**
        * @brief Default observation map
        *
         * @param x [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @param measure_dim [in]; Measurement dimensionality
         * @return Vector output of the function, h()
         */
        static Vector<double> h_fun_default(Vector<double>& x, double t, 
                                            double dt, 
                                            unsigned measure_dim)
        {
            PRECOND (x.size () ==  measure_dim);
            return (x);
        }

        /** 
         * @brief Default error function
         *
         * @param y [in]; First vector
         * @param y_ref [in]; Second vector
         * @return Difference error
         */
        static Vector<double> error_fun_default(Vector<double>& y, 
                                                Vector<double>& y_ref)
        {
            PRECOND (y.size () == y_ref.size ());
            return (y-y_ref);
        }

        /**
         * @brief Jacobian linearization of the default f_fun.
         *
         * @param x [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @param Jac [out]; Jacobian matrix
         * @return Boolean value: "true" if the Jacobian is the identity matrix.
         */
        static bool Jac_f_default(Vector<double>& x, double t, double dt, 
                                  Matrix<double>* Jac)
        {
            PRECOND (Jac);
            Matrix<double> & mat = *Jac;
            mat.eye(x.size());
            return true;
        }

        /**
         * @brief Jacobian linearization of the default h_fun.
         *
         * @param x [in]; Current state vector
         * @param t [in]; Current time
         * @param dt [in]; Time difference from last time
         * @param measure_dim [in]; Measurement dimensionality
         * @param Jac [out]; Jacobian matrix
         * @return Boolean value: "true" if the Jacobian is the identity matrix.
         */
        static bool Jac_h_default(Vector<double>& x, double t, double dt, 
                                  unsigned measure_dim, 
                                  Matrix<double>* Jac)
        {
            PRECOND (Jac);
            Matrix<double> & mat = *Jac;
            mat.eye(x.size());
            return true;
        }    

    protected: // Implementation.
        Matrix<double> _Q;       ///< Variance of the process noise
        Matrix<double> _R;       ///< Variance of the measurement noise

        Vector<double> _mu_ekf;  ///< Estimate of the mean of the state
        Matrix<double> _P_ekf;   ///< Estimate of the variance of the state

        double _t_old;           ///< Last time

        Vector<double> _mu_ekf_pred; ///< Prediction of the mean of the state using f()
        Matrix<double> _P_ekf_pred;  ///< Prediction of the variance of the state
        Vector<double> _y_pred;  ///< Prediction of the output using h()

        bool _is_Jac_f_identity; ///< True when Jac_f is identity
        bool _is_Jac_h_identity; ///< True when Jac_h is identity

        unsigned   _state_dim;   ///< State dimensionality
        unsigned   _measure_dim; ///< Measurement dimensionality
    }; // End class KalmanFilter.

} // End namespace Evolution

#endif // INCLUDE_EVOLUTION_MATH__KALMAN_FILTER_HPP
