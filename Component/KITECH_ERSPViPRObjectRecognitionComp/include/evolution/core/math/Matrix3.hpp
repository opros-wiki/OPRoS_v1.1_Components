//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_MATRIX3_HPP
#define INCLUDE_EVOLUTION_MATH_MATRIX3_HPP


/**
 * @file    evolution/core/math/Matrix3.hpp
 * @brief   Simple 3x3 matrix class.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Matrix3
 */

#include <math.h>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/EulerAngles.hpp>


namespace Evolution
{

    /**
     * @class Matrix3
     * @brief Class that implements a statically-allocated 3x3 matrix.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     **/
    class EVOLUTION_EXPORT_MATH Matrix3
    {
    public: // Logging

        /** 
         * @brief Declares the logging category for the Matrix3 class.
         */
        ERSP_LOG_CLASS_CATEGORY_DECLARE(Matrix3, "Evolution.Core.Math.Matrix3");

    public: // Structors

        /**
         * @brief Construct a matrix from 9 doubles
         *        The values are assumed to be ordered row-wise
         *
         * @param m00 [in]; Element 00 of the matrix
         * @param m01 [in]; Element 01 of the matrix
         * @param m02 [in]; Element 02 of the matrix
         * @param m10 [in]; Element 10 of the matrix
         * @param m11 [in]; Element 11 of the matrix
         * @param m12 [in]; Element 12 of the matrix
         * @param m20 [in]; Element 20 of the matrix
         * @param m21 [in]; Element 21 of the matrix
         * @param m22 [in]; Element 22 of the matrix
         */
        Matrix3 (const double m00=0, const double m01=0, const double m02=0,
                 const double m10=0, const double m11=0, const double m12=0,
                 const double m20=0, const double m21=0, const double m22=0)
        {
            init ();
            set(m00, m01, m02,
                m10, m11, m12,
                m20, m21, m22);
        }

        /**
         * @brief Copy constructor
         *
         * @param m [in]; Matrix to be copied.
         */
        Matrix3 (const Matrix3 & m)
        {
            init ();
            memcpy (_matrix, m._matrix, 9*sizeof(double));
        }

        /**
         * @brief Constructs a matrix from an array of 9 doubles
         *        The values would be stored row-wise
         *
         * @param array [in]; Array of doubles containing the values for filling the elements of the matrix.
         */
        Matrix3 (const double array[])
        {
            init ();
            memcpy (_matrix, array, 9*sizeof(double));
        }

        /**
         * @brief Construct a matrix from 3 vectors 
         *        (usually 3 orthogonal axies)
         *        Each vector should be stored as a column of the matrix
         *
         * @param m0 [in]; Column vector 0
         * @param m1 [in]; Column vector 1
         * @param m2 [in]; Column vector 2
         *
         * @see Vector3
         */
        Matrix3 (const Vector3& m0, const Vector3& m1, const Vector3& m2)
        {
            init ();
            set(m0, m1, m2);
        }

        /**
         * @brief Construct a matrix from a vector composed by Euler angles.  
         * The matrix is the transformation that would rotate a position 
         * vector by the angles described in the rotation.
         *
         * @param rotation [in]; Set of rotation angles
         *
         * @see EulerAngles
         */
        explicit Matrix3 (const EulerAngles& rotation)
        {
            init ();
            set(rotation);
        }

        /**
         * @brief Constructs a matrix from the given rotation vector,
         * using Rodrigues' formula.
         *
         * @param rotation [in]; Rotation vector
         **/
        explicit Matrix3 (const Vector3& rotation)
        {
            init ();
            set_rotation_vector(rotation);
        }

        /**
         * @brief *** DEPRECATED ***
         * Obtains the data as a single-dimensional array.  
         * Use get_linear_data() instead.
         *
         * @return Pointer to the internal data
         */
        const double* get_data () const { return get_linear_data(); }

        /**
         * @brief Obtains the data as a single-dimensional array.
         *
         * @return Pointer to the internal data
         */
        const double* get_linear_data () const { return _data[0]; }

        /// Obtains the data as a two-dimensional array.
        //const double** get_array_data () const { return _data; }

        /**
         * @brief Standard destructor
         */
        ~Matrix3 () {}

    public: // Information access
        
        /**
         * @brief Retrieves element values.
         *
         * @param index [in]; Index of the element to be retrieved.
         * @return Element value
         */
        double * operator[] (const int index) 
        { 
            ASSERT (index >= 0 && index < 3);
            return _data[index]; 
        }

        /**
         * @brief Retrieves element values.
         *
         * @param index [in]; Index of the element to be retrieved.
         * @return Element value
         */
        const double * operator[] (const int index) const 
        { 
            ASSERT (index >= 0 && index < 3);
            return _data[index]; 
        }
        
        /**
         * @brief Retrieves element values.
         *
         * @param index [in]; Index of the element to be retrieved.
         * @return Element value
         */
        double * operator[] (const unsigned int index) 
        { 
            ASSERT (index < 3);
            return _data[index]; 
        }

        /**
         * @brief Retrieves element values.
         *
         * @param index [in]; Index of the element to be retrieved.
         * @return Element value
         */
        const double * operator[] (const unsigned int index) const 
        { 
            ASSERT (index < 3);
            return _data[index]; 
        }
        
    public: // Modifications

        /**
         * @brief Manually sets a matrix's contents with vectors
         *        Each vector would be stored in a column of the matrix
         *
         * @param m0 [in]; Column vector 0
         * @param m1 [in]; Column vector 1
         * @param m2 [in]; Column vector 2
         * @return A reference to this Matrix3
         *
         * @see Vector3
         */
        Matrix3& set (const Vector3& m0, const Vector3& m1, const Vector3& m2)
        {
            _data[0][0] = m0[0];  _data[0][1] = m1[0];  _data[0][2] = m2[0];
            _data[1][0] = m0[1];  _data[1][1] = m1[1];  _data[1][2] = m2[1];
            _data[2][0] = m0[2];  _data[2][1] = m1[2];  _data[2][2] = m2[2];

            return *this;
        }

        /**
         * @brief Manually set a matrix's contents with doubles.
         *        The values are assumed to be ordered row-wise
         *
         * @param m00 [in]; Element 00 of the matrix
         * @param m01 [in]; Element 01 of the matrix
         * @param m02 [in]; Element 02 of the matrix
         * @param m10 [in]; Element 10 of the matrix
         * @param m11 [in]; Element 11 of the matrix
         * @param m12 [in]; Element 12 of the matrix
         * @param m20 [in]; Element 20 of the matrix
         * @param m21 [in]; Element 21 of the matrix
         * @param m22 [in]; Element 22 of the matrix
         * @return A reference to this Matrix3
         *
         * @see Vector3
         */
        Matrix3& set (const double m00, const double m01, const double m02,
                      const double m10, const double m11, const double m12,
                      const double m20, const double m21, const double m22)
        {
            _data[0][0] = m00;  _data[0][1] = m01;  _data[0][2] = m02;
            _data[1][0] = m10;  _data[1][1] = m11;  _data[1][2] = m12;
            _data[2][0] = m20;  _data[2][1] = m21;  _data[2][2] = m22;

            return *this;
        }

        /**
         * @brief Manually composes a rotation matrix as given by the
         * Euler Angles. The transformation maps points in the rotated
         * frame back to the original frame. This corresponds to a ZYX
         * parameterization of Euler Angles (Fick angles).
         *
         * @param rotation [in]; Set of rotation angles
         * @return A reference to this Matrix3
         *
         * @see EulerAngles
         **/
        Matrix3& set (const EulerAngles& rotation)
        {
            // Precompute the sines and cosines
            double sy = sin(rotation[EulerAngles::YAW  ]);
            double cy = cos(rotation[EulerAngles::YAW  ]);
            double sp = sin(rotation[EulerAngles::PITCH]);
            double cp = cos(rotation[EulerAngles::PITCH]);
            double sr = sin(rotation[EulerAngles::ROLL ]);
            double cr = cos(rotation[EulerAngles::ROLL ]);

            // Transformation matrix that rotates positions by these angles
            set( cy*cp,    cy*sp*sr - sy*cr,   cy*sp*cr + sy*sr,
                 sy*cp,    sy*sp*sr + cy*cr,   sy*sp*cr - cy*sr,
                 -sp,               cp*sr,              cp*cr );

            return *this;
        }

        /**
         * @brief
         * Computes the rotation matrix from the given vector using
         * Rodrigues' formula.
         *
         * @param rin [in]; Rotation vector
         * @return A reference to this Matrix3
         **/
        Matrix3& set_rotation_vector (const Vector3& rin)
        {
            double theta = ersp_hypot (ersp_hypot (rin[0], rin[1]), rin[2]);
            double s = sin (theta);
            double c = cos (theta);
            double w1, w2, w3;
            double w11, w12, w13, w22, w23, w33;

            // rotation too small to hunt down its axis
            if (theta < 1E-8)
            {
                _matrix[0] = _matrix[4] = _matrix[8] = 1.0;
                _matrix[1] = _matrix[2] = _matrix[3] = 0.0;
                _matrix[5] = _matrix[6] = _matrix[7] = 0.0;
                return (*this);
            }
            w1 = rin[0] / theta;
            w2 = rin[1] / theta;
            w3 = rin[2] / theta;
            w11 = w1 * w1;
            w12 = w1 * w2;
            w13 = w1 * w3;
            w22 = w2 * w2;
            w23 = w2 * w3;
            w33 = w3 * w3;
            
            _matrix[0] = c + (1.0 - c) * w11;
            _matrix[4] = c + (1.0 - c) * w22;
            _matrix[8] = c + (1.0 - c) * w33;

            _matrix[1] = s * (-w3) + (1.0 - c) * w12;
            _matrix[2] = s * w2    + (1.0 - c) * w13;
            _matrix[3] = s * w3    + (1.0 - c) * w12;
            _matrix[5] = s * (-w1) + (1.0 - c) * w23;
            _matrix[6] = s * (-w2) + (1.0 - c) * w13;
            _matrix[7] = s * w1    + (1.0 - c) * w23;
            return (*this);
        } // end set_rotation_vector

        /**
         * @brief Clears all the elements of the matrix.
         * @return A reference to this Matrix3
         */
        Matrix3& clear ()
        {
            return set(0,0,0,0,0,0,0,0,0);
        }

        /**
         * @brief Constructs the zero matrix
         */
        void zeros () { set(0,0,0,0,0,0,0,0,0);}

        /**
         * @brief Constructs the ones matrix.
         */
        void ones  () { set(1,1,1,1,1,1,1,1,1);}

        /**
         * @brief Constructs the identity matrix.
         */
        void eye   () { set(1,0,0,0,1,0,0,0,1);}

        /**
         * @brief Operator = : copies one matrix to the current matrix.
         *
         * @param m [in]; Matrix to be copied.
         * @return A reference to this Matrix3
         */
        Matrix3& operator= (const Matrix3& m)
        {
            memcpy (_matrix, m._matrix, 9 * sizeof(double));

            return *this;
        }

        /**
         * @brief Operator = : assigns an array of doubles to the
         * current matrix.
         *
         * @param array [in]; Array of doubles containing the values for filling the elements of the matrix.
         * @return A reference to this Matrix3
         */
        Matrix3& operator= (const double array[])
        {
            memcpy (_matrix, array, 9 * sizeof(double));

            return *this;
        }

        /**
         * @brief Operator += : adds one matrix to the current matrix
         * and stores the sum in the current matrix..
         *
         * @param m [in]; Matrix to be added to the this Matrix3.
         * @return A reference to this Matrix3
         */
        Matrix3& operator+= (const Matrix3& m)
        {
            for (int i = 0; i < 3;  i++)
            {
                for (int j = 0; j < 3;  j++)
                {
                    _data[i][j] += m[i][j];
                }
            }

            return *this;
        }

        /**
         * @brief Operator += : substracts one matrix from the current matrix
         * and stores the substraction in the current matrix.
         *
         * @param m [in]; Matrix to be substracted from the this Matrix3.
         * @return A reference to this Matrix3
         */
        Matrix3& operator-= (const Matrix3& m)
        {
            for (int i = 0; i < 3;  i++)
            {
                for (int j = 0; j < 3;  j++)
                {
                    _data[i][j] -= m[i][j];
                }
            }

            return *this;
        }

        /**
         * @brief Operator *= : scales a matrix.
         *
         * @param scale [in]; Value to be used to scale each element of the matrix.
         * @return A reference to this Matrix3
         */
        Matrix3& operator*= (double scale)
        {
            for (int i = 0; i < 9;  i++)
            {
                _matrix[i] *= scale;
            }

            return *this;
        }

        /**
         * @brief Operator *= : multiplies the current matrix with
         * another matrix and stores the result in the current matrix.
         *
         * @param m [in]; Matrix to be multiplied with the this Matrix3.
         * @return A reference to this Matrix3
         */
        Matrix3& operator*= (const Matrix3& m)
        {
            *this = *this * m;
            return *this; 
        }

        /**
         * @brief Sets the row vector at a specified location.
         *
         * @param vec [in]; Vector containing the new values for the row elements.
         * @param row [in]; Index of the row to be modified.
         * @return A reference to this Matrix3
         */
        Matrix3& set_row (Vector3 & vec, int row)
        {
            ASSERT (row >= 0 && row < 3);
            
            for (int i = 0; i < 3; i++)
            {
                _data[row][i] = vec[i];
            }
            return *this;
        }

        /**
         * @brief Sets the col vector at a specified location.
         *
         * @param vec [in]; Vector containing the new values for the col elements.
         * @param col [in]; Index of the col to be modified.
         * @return A reference to this Matrix3
         */
        Matrix3& set_col (Vector3 & vec, int col)
        {
            ASSERT (col >= 0 && col < 3);
            
            for (int i = 0; i < 3; i++)
            {
                _data[i][col] = vec[i];
            }
            return *this;
        }
        /**
         * @brief Computes the transpose of the matrix in-place.
         * @return A reference to this Matrix3
         */
        Matrix3& transpose ()
        {
            double old_data[9];
            memcpy (old_data, _matrix, 9*sizeof(double));
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    _data[j][i] = old_data[i*3+j];
                }
            }
            return *this;
        }

    public: // Binary operators

        /**
         * @brief Operator == : tests if two matrices are equal.
         *
         * @param m [in]; Matrix to be compared with the this Matrix3.
         * @return Boolean value: "true" if the two matricies are equal.
         */
        bool operator== (const Matrix3& m) const 
        {
            for (int i = 0; i < 3;  i++)
            {
                for (int j = 0; j < 3;  j++)
                {
                    if (double_not_equal(_data[i][j], m[i][j]))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        /**
         * @brief Operator != : tests if two matrices are not equal.
         *
         * @param m [in]; Matrix to be compared with the this Matrix3.
         * @return Boolean value: "true" if the two matricies are not equal.
         */
        bool operator!= (const Matrix3& m) const
        {
            return !(*this == m);
        }

        /**
         * @brief Checks if the matrix is the zero matrix.
         *
         * @return Boolean value: "true" if the matrix is the zero matrix.
         */
        bool is_zeros () const { return quick_check(0,0,0,0,0,0,0,0,0);}

        /**
         * @brief Checks if the matrix is the ones matrix.
         *
         * @return Boolean value: "true" if the matrix is the one matrix.
         */
        bool is_ones  () const { return quick_check(1,1,1,1,1,1,1,1,1);}

        /**
         * @brief Checks if the matrix is the identity matrix.
         *
         * @return Boolean value: "true" if the matrix is the identity matrix.
         */
        bool is_eye   () const { return quick_check(1,0,0,0,1,0,0,0,1);}

    public: // Arithmetic operators

        /**
         * @brief Operator + : returns the sum of two matrices.
         *
         * @param m [in]; Matrix to be added to the this Matrix3.
         * @return Matrix containing the sum.
         */
        Matrix3 operator+ (const Matrix3& m) const
        {
            Matrix3 r = (*this);
            return (r += m);
        }

        /**
         * @brief Operator - : returns the difference of two matrices.
         *
         * @param m [in]; Matrix to be added to the this Matrix3.
         * @return Matrix containing the difference.
         */
        Matrix3 operator- (const Matrix3& m) const
        {
            Matrix3 r = (*this);
            return (r -= m);
        }

        /**
         * @brief Operator - : returns the negative of the matrix.
         *
         * @return Matrix containing the negative of the this Matrix3.
         */
        Matrix3 operator- () {return (*this * (-1.0));}

        /**
         * @brief Operator * : scales a matrix.
         *
         * @param scale [in]; Value to be used to scale each element of the matrix.
         * @return Matrix containing the scaled version of the this Matrix3.
         */
        Matrix3 operator* (double scale) const 
        {
            Matrix3 r = (*this);
            return (r *= scale);
        }

        /**
         * @brief Operator * : transforms a vector by a matrix.
         *
         * @param v [in]; Vector that will be multiplied with the this Matrix3.
         * @return Vector containing the result of the multiplication of the input vector and the this Matrix3.
         */
        Vector3 operator* (const Vector3& v) const
        {
            // Apply matrix
            Vector3 r( _data[0][0]*v[0] + _data[0][1]*v[1] + _data[0][2]*v[2],
                       _data[1][0]*v[0] + _data[1][1]*v[1] + _data[1][2]*v[2],
                       _data[2][0]*v[0] + _data[2][1]*v[1] + _data[2][2]*v[2] );
            return r;
        }

        /**
         * @brief Operator * : returns the multiplication of two matrices.
         *
         * @param m [in]; Matrix to be multiplied with this Matrix3.
         * @return Matrix containing the multiplication.
         */
        Matrix3 operator* (const Matrix3& m) const
        {
            Matrix3 r;
            for (int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        r[i][j] += _data[i][k] * m[k][j];
                    }
                }
            } 
            return r;
        }

        /**
         * @brief Computes the determinant of the matrix.
         *
         * @return Value of the determinant of the matrix.
         */
        double determinant() const
        {
            return (+_data[0][0]*(_data[1][1]*_data[2][2]-_data[2][1]*_data[1][2])
                    -_data[1][0]*(_data[0][1]*_data[2][2]-_data[2][1]*_data[0][2])
                    +_data[2][0]*(_data[0][1]*_data[1][2]-_data[1][1]*_data[0][2]));
        }

        /**
         * @brief Computes the determinant of the matrix.
         *
         * @return Value of the determinant of the matrix.
         */
        double det() const {return determinant();}

        /**
         * @brief Computes the inverse of the matrix.
         *
         * @param inv [out]; Placeholder for the inverse of the this Matrix3.
         * @return Result indicating success or failure
         *
         * @pre The inv pointer should not be NULL.
         */
        Result inverse(Matrix3 * inv) const
        {
            PRECOND_RETURN_VALUE (inv, RESULT_PRECONDITION_ERROR);
            Matrix3 &m = *inv;
            double d = determinant();
            if (double_equal (d, 0.0))
            {
                ERSP_LOG_CLASS_WARN (Matrix3) ("The matrix is not invertible");
                return RESULT_PRECONDITION_ERROR;
            }

            d = 1.0 / d;
            m[0][0] =  (_data[1][1]*_data[2][2]-_data[2][1]*_data[1][2]);
            m[0][1] = -(_data[0][1]*_data[2][2]-_data[2][1]*_data[0][2]);
            m[0][2] =  (_data[0][1]*_data[1][2]-_data[1][1]*_data[0][2]);
            m[1][0] = -(_data[1][0]*_data[2][2]-_data[2][0]*_data[1][2]);
            m[1][1] =  (_data[0][0]*_data[2][2]-_data[2][0]*_data[0][2]);
            m[1][2] = -(_data[0][0]*_data[1][2]-_data[1][0]*_data[0][2]);
            m[2][0] =  (_data[1][0]*_data[2][1]-_data[2][0]*_data[1][1]);
            m[2][1] = -(_data[0][0]*_data[2][1]-_data[2][0]*_data[0][1]);
            m[2][2] =  (_data[0][0]*_data[1][1]-_data[1][0]*_data[0][1]);

            m *= d;
            return RESULT_SUCCESS;
        }

        /**
         * @brief Computes the inverse of the matrix.
         *
         * @param inv [out]; Placeholder for the inverse of the this Matrix3.
         * @return Result indicating success or failure
         *
         * @pre The inv pointer should not be NULL.
         */
        Result inv(Matrix3 * inv) const { return inverse(inv);}

        /**
         * @brief Computes the rotation vector from the matrix,
         * using Rodrigues' formula.
         * @param rout_ptr [out]; Placeholder for rotation vector.
         *
         * @see Vector3
         **/
        void get_rotation_vector (Vector3* rout_ptr) const
        {
            PRECOND (rout_ptr);
            Vector3& rout = *rout_ptr;
            double trace = _matrix[0] + _matrix[4] + _matrix[8];
            double tr = (trace - 1) * 0.5;
            double theta = acos (tr);
            double sin_theta = sin (theta);
            if (sin_theta >= 1e-5)
            {
                double factor = 0.5 / sin_theta * theta;
                
                rout[0] = (_matrix[2 * 3 + 1] - _matrix[1 * 3 + 2]) * factor;
                rout[1] = (_matrix[0 * 3 + 2] - _matrix[2 * 3 + 0]) * factor;
                rout[2] = (_matrix[1 * 3 + 0] - _matrix[0 * 3 + 1]) * factor;
            }
            else if (tr > 0)
            {
                rout[0] = rout[1] = rout[2] = 0.0;
            }
            else
            {
                rout[0] = theta * sqrt (_matrix[0] * 0.5 + 0.5);
                rout[1] = theta * sqrt (_matrix[4] * 0.5 + 0.5);
                if (_matrix[0 * 3 + 1] < 0)
                    rout[1] = -rout[1];
                rout[2] = theta * sqrt (_matrix[8] * 0.5 + 0.5);
                if (_matrix[0 * 3 + 2] < 0)
                    rout[2] = -rout[2];
            }
        } // end get_rotation_vector()

        /**
         * @brief Operator * : transforms a vector by the transpose of a matrix.
         *
         * @param v [in]; Vector that will be multiplied with the matrix.
         * @param m [in]; Matrix to be transposed before being multiplied with the vector.
         * @return Vector containing the result of the multiplication of the input vector and the matrix.
         */
        friend EVOLUTION_EXPORT_MATH Vector3 operator* (const Vector3& v, 
                                                        const Matrix3 &m)
        {
            // Apply matrix's transpose to v
            Vector3 r( m[0][0]*v[0] + m[1][0]*v[1] + m[2][0]*v[2],
                       m[0][1]*v[0] + m[1][1]*v[1] + m[2][1]*v[2],
                       m[0][2]*v[0] + m[1][2]*v[1] + m[2][2]*v[2] );
            return r;
        }

    public: // Output
        /**
         * @brief Print the contents of a Matrix3 to the specified stream.
         * @param out [in]; Stream to which to print the contents of Matrix3.
         * @param mat [in]; Matrix3 whose contents will be printed to the stream.
         *
         * @return Stream reference given in the out parameter.
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::Matrix3 & mat)
        {
            out << "[" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << "\n"
                << " " << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << "\n"
                << " " << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << "]";
            return out << "\n";
        }

    protected: // Helpers

        /**
         * @brief Checks if the contents of the matrix are equal to the given values.
         *
         * @param m00 [in]; Input value to compare with element 00 of the matrix
         * @param m01 [in]; Input value to compare with element 01 of the matrix
         * @param m02 [in]; Input value to compare with element 02 of the matrix
         * @param m10 [in]; Input value to compare with element 10 of the matrix
         * @param m11 [in]; Input value to compare with element 11 of the matrix
         * @param m12 [in]; Input value to compare with element 12 of the matrix
         * @param m20 [in]; Input value to compare with element 20 of the matrix
         * @param m21 [in]; Input value to compare with element 21 of the matrix
         * @param m22 [in]; Input value to compare with element 22 of the matrix
         * @return Boolean value: "true" if equality is reached.
         */
        bool quick_check(double m00, double m01, double m02,
                         double m10, double m11, double m12,
                         double m20, double m21, double m22) const
        {
            return ( double_equal(m00, _data[0][0]) && 
                     double_equal(m01, _data[0][1]) &&
                     double_equal(m02, _data[0][2]) &&
                     double_equal(m10, _data[1][0]) &&
                     double_equal(m11, _data[1][1]) &&
                     double_equal(m12, _data[1][2]) &&
                     double_equal(m20, _data[2][0]) &&
                     double_equal(m21, _data[2][1]) &&
                     double_equal(m22, _data[2][2]));
        }

        /**
         * @brief Internal pointers initialization
         */
        void init ()
        {
            _data[0] = _matrix;
            _data[1] = _data[0] + 3;
            _data[2] = _data[1] + 3;
        }

    protected: // Implementation

        /*
         * The internal matrix data.  It is represented as 3
         * pointers.  Each pointer points to a separate row of the matrix,
         * with matrix[0] being the top row.
         */
        double   _matrix[9]; /*< The internal matrix data. */
        double * _data[3];   /**< Three additional pointers used for
                              * fast access. Each pointer points to a
                              * separate row of the matrix, with
                              * matrix[0] being the top row. */
    }; // end class Matrix3

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_MATRIX3_HPP
