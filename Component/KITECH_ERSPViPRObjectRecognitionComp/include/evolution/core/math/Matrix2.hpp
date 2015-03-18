//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_MATRIX2_HPP
#define INCLUDE_EVOLUTION_MATH_MATRIX2_HPP


/**
 * @file    evolution/core/math/Matrix2.hpp
 * @brief   Simple 2x2 matrix class.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Matrix2
 */

#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Vector2.hpp>


namespace Evolution
{

    /**
     * @class Matrix2
     * @brief Class that implements a statically-allocated 2x2 matrix.
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     **/
    class EVOLUTION_EXPORT_MATH Matrix2
    {
    public: // Logging
        /** 
         * @brief Declares the logging category for the Matrix2 class.
         */
        ERSP_LOG_CLASS_CATEGORY_DECLARE(Matrix2, "Evolution.Core.Math.Matrix2");

    public: // Structors
        /**
         * @brief Constructs a matrix from 4 doubles.
         *        The values are assumed to be ordered row-wise.
         *
         * @param m00 [in]; Element 00 of the matrix
         * @param m01 [in]; Element 01 of the matrix
         * @param m10 [in]; Element 10 of the matrix
         * @param m11 [in]; Element 11 of the matrix
         */
        Matrix2 (const double m00=0, const double m01=0,
                 const double m10=0, const double m11=0)
        {
            init ();
            set(m00, m01,
                m10, m11);
        }

        /**
         * @brief Copy constructor
         *
         * @param m [in]; Matrix to be copied.
         */
        Matrix2 (const Matrix2 & m)
        {
            init ();
            memcpy (_matrix, m._matrix, 4*sizeof(double));
        }

        /**
         * @brief Constructs a matrix from an array of 4 doubles
         *        The values would be stored row-wise
         *
         * @param array [in]; Array of doubles containing the values for filling the elements of the matrix.
         */
        Matrix2 (const double array[])
        {
            init ();
            memcpy (_matrix, array, 4*sizeof(double));
        }

        /**
         * @brief Constructs a matrix from 2 vectors 
         *        Each vector should be stored as a column of the matrix
         *
         * @param m0 [in]; Column vector 0
         * @param m1 [in]; Column vector 1
         *
         * @see Vector2
         */
        Matrix2 (const Vector2 m0, const Vector2 m1)
        {
            init ();
            set(m0, m1);
        }

        /**
         * @brief Standard destructor
         */
        ~Matrix2 () {}

    public: // Information access
        
        /**
         * @brief Retrieves element values.
         *
         * @param index [in]; Index of the element to be retrieved.
         * @return Element value
         */
        double * operator[] (const int index) 
        { 
            ASSERT (index >= 0 && index < 2);
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
            ASSERT (index >= 0 && index < 2);
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
            ASSERT (index >= 0 && index < 2);
            return _data[index]; 
        }

        /**
         * @brief Retrievs element values.
         *
         * @param index [in]; Index of the element to be retrieved.
         * @return Element value
         */
        const double * operator[] (const unsigned int index) const 
        { 
            ASSERT (index >= 0 && index < 2);
            return _data[index]; 
        }
        
    public: // Modifications
        /**
         * @brief Manually sets a matrix's contents with vectors
         *        Each vector would be stored in a column of the matrix.
         *
         *
         * @param m0 [in]; Column vector 0
         * @param m1 [in]; Column vector 1
         * @return A reference to this Matrix2
         *
         * @see Vector2
         */
        Matrix2& set (const Vector2 m0, const Vector2 m1)
        {
            _data[0][0] = m0[0];  _data[0][1] = m1[0];
            _data[1][0] = m0[1];  _data[1][1] = m1[1];;

            return (*this);
        }

        /**
         * @brief Manually sets a matrix's contents with doubles.
         *        The values are assumed to be ordered row-wise
         *
         * @param m00 [in]; Element 00 of the matrix
         * @param m01 [in]; Element 01 of the matrix
         * @param m10 [in]; Element 10 of the matrix
         * @param m11 [in]; Element 11 of the matrix
         * @return A reference to this Matrix2
         *
         * @see Vector2
         */
        Matrix2& set (const double m00, const double m01,
                      const double m10, const double m11)
        {
            _data[0][0] = m00;  _data[0][1] = m01;
            _data[1][0] = m10;  _data[1][1] = m11;

            return (*this);
        }

        /**
         * @brief Clears all the elements of the matrix.
         * @return A reference to this Matrix2
         */
        Matrix2& clear ()
        {
            return set(0,0,0,0);
        }

        /**
         * @brief Constructs the zero matrix
         */
        void zeros () { set(0,0,0,0);}

        /**
         * @brief Constructs the ones matrix.
         */
        void ones  () { set(1,1,1,1);}

        /**
         * @brief Constructs the identity matrix.
         */
        void eye   () { set(1,0,0,1);}

        /**
         * @brief Operator = : copies one matrix to the current matrix.
         *
         * @param m [in]; Matrix to be copied.
         * @return A reference to this Matrix2
         */
        Matrix2& operator= (const Matrix2& m)
        {
            memcpy (_matrix, m._matrix, 4 * sizeof(double));

            return *this;
        }

        /**
         * @brief Operator = : assigns an array of doubles to the
         * current matrix.
         *
         * @param array [in]; Array of doubles containing the values for filling the elements of the matrix.
         * @return A reference to this Matrix2
         */
        Matrix2& operator= (const double array[])
        {
            memcpy (_matrix, array, 4 * sizeof(double));

            return *this;
        }

        /**
         * @brief Operator += : adds one matrix to the current matrix
         * and stores the sum in the current matrix.
         *
         * @param m [in]; Matrix to be added to the this Matrix2.
         * @return A reference to this Matrix2
         */
        Matrix2& operator+= (const Matrix2& m)
        {
            for (int i = 0; i < 2;  i++)
            {
                for (int j = 0; j < 2;  j++)
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
         * @param m [in]; Matrix to be substracted from the this Matrix2.
         * @return A reference to this Matrix2
         */
        Matrix2& operator-= (const Matrix2& m)
        {
            for (int i = 0; i < 2;  i++)
            {
                for (int j = 0; j < 2;  j++)
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
         * @return A reference to this Matrix2
         */
        Matrix2& operator*= (double scale)
        {
            for (int i = 0; i < 4;  i++)
            {
                _matrix[i] *= scale;
            }

            return *this;
        }

        /**
         * @brief Operator *= : multiplies the current matrix with
         * another matrix and stores the result in the current matrix.
         *
         * @param m [in]; Matrix to be multiplied with the this Matrix2.
         * @return A reference to this Matrix2
         */
        Matrix2& operator*= (const Matrix2& m)
        {
            *this = *this * m;
            return *this; 
        }

        /**
         * @brief Sets the row vector at a specified location.
         *
         * @param vec [in]; Vector containing the new values for the row elements.
         * @param row [in]; Index of the row to be modified.
         * @return A reference to this Matrix2
         */
        Matrix2& set_row (Vector2 & vec, int row)
        {
            ASSERT (row >= 0 && row < 2);
            
            for (int i = 0; i < 2; i++)
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
         * @return A reference to this Matrix2
         */
        Matrix2& set_col (Vector2 & vec, int col)
        {
            ASSERT (col >= 0 && col < 2);
            
            for (int i = 0; i < 2; i++)
            {
                _data[i][col] = vec[i];
            }
            return *this;
        }

        /**
         * @brief Computes the transpose of the matrix in-place.
         * @return A reference to this Matrix2
         */
        Matrix2& transpose ()
        {
            double old_data[4];
            memcpy (old_data, _matrix, 4*sizeof(double));
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    _data[j][i] = old_data[i*2+j];
                }
            }
            return *this;
        }

    public: // Binary operators

        /**
         * @brief Operator == : tests if two matrices are equal.
         *
         * @param m [in]; Matrix to be compared with the this Matrix2.
         * @return Boolean value: "true" if the two matricies are equal.
         */
        bool operator== (const Matrix2& m) const 
        {
            for (int i = 0; i < 2;  i++)
            {
                for (int j = 0; j < 2;  j++)
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
         * @param m [in]; Matrix to be compared with the this Matrix2.
         * @return Boolean value: "true" if the two matricies are not equal.
         */
        bool operator!= (const Matrix2& m) const
        {
            return !(*this == m);
        }

        /**
         * @brief Checks if the matrix is the zero matrix.
         *
         * @return Boolean value: "true" if the matrix is the zero matrix.
         */
        bool is_zeros () { return quick_check(0,0,0,0);}

        /**
         * @brief Checks if the matrix is the ones matrix.
         *
         * @return Boolean value: "true" if the matrix is the one matrix.
         */
        bool is_ones  () { return quick_check(1,1,1,1);}

        /**
         * @brief Checks if the matrix is the identity matrix.
         *
         * @return Boolean value: "true" if the matrix is the identity matrix.
         */
        bool is_eye   () { return quick_check(1,0,0,1);}

    public: // Arithmetic operators
        /**
         * @brief Operator + : returns the sum of two matrices.
         *
         * @param m [in]; Matrix to be added to the this Matrix2.
         * @return Matrix containing the sum.
         */
        Matrix2 operator+ (const Matrix2& m) const
        {
            Matrix2 r = (*this);
            return (r += m);
        }

        /**
         * @brief Operator - : returns the difference of two matrices.
         *
         * @param m [in]; Matrix to be added to the this Matrix2.
         * @return Matrix containing the difference.
         */
        Matrix2 operator- (const Matrix2& m) const
        {
            Matrix2 r = (*this);
            return (r -= m);
        }

        /**
         * @brief Operator - : returns the negative of the matrix.
         *
         * @return Matrix containing the negative of the this Matrix2.
         */
        Matrix2 operator- () {return (*this * (-1.0));}

        /**
         * @brief Operator * : scales a matrix.
         *
         * @param scale [in]; Value to be used to scale each element of the matrix.
         * @return Matrix containing the scaled version of the this Matrix2.
         */
        Matrix2 operator* (double scale) const 
        {
            Matrix2 r = (*this);
            return (r *= scale);
        }

        /**
         * @brief Operator * : transforms a vector by a matrix.
         *
         * @param v [in]; Vector that will be multiplied with the this Matrix2.
         * @return Vector containing the result of the multiplication of the input vector and the this Matrix2.
         */
        Vector2 operator* (const Vector2& v) const
        {
            // Apply matrix
            Vector2 r( _data[0][0]*v[0] + _data[0][1]*v[1],
                       _data[1][0]*v[0] + _data[1][1]*v[1]);
            return r;
        }

        /**
         * @brief Operator * : returns the multiplication of two matrices.
         *
         * @param m [in]; Matrix to be multiplied with the this Matrix2.
         * @return Matrix containing the multiplication.
         */
        Matrix2 operator* (const Matrix2& m) const
        {
            Matrix2 r;
            for (int i = 0; i < 2; i++)
            {
                for(int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
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
            return (+_data[0][0]*_data[1][1]
                    -_data[1][0]*_data[0][1]);
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
         * @param inv [out]; Placeholder for the inverse of the this Matrix2.
         * @return Result indicating success or failure
         *
         * @pre The inv pointer should not be NULL.
         */
        Result inverse(Matrix2 * inv) const
        {
            PRECOND_RETURN_VALUE (inv, RESULT_PRECONDITION_ERROR);
            Matrix2 &m = *inv;
            double d = determinant();
            if (double_equal (d, 0.0))
            {
                ERSP_LOG_CLASS_WARN (Matrix2)("The matrix is not invertible");
                return RESULT_PRECONDITION_ERROR;
            }

            d = 1.0 / d;
            m[0][0] =  _data[1][1] * d;
            m[0][1] = -_data[0][1] * d;
            m[1][0] = -_data[1][0] * d;
            m[1][1] =  _data[0][0] * d;

            return RESULT_SUCCESS;
        }

        /**
         * @brief Computes the inverse of the matrix.
         *
         * @param inv [out]; Placeholder for the inverse of the this Matrix2.
         * @return Result indicating success or failure
         *
         * @pre The inv pointer should not be NULL.
         */
        Result inv(Matrix2 * inv) const { return inverse(inv);}

        /**
         * @brief Operator * : transforms a vector by the transpose of a matrix.
         *
         * @param v [in]; Vector that will be multiplied with the matrix.
         * @param m [in]; Matrix to be transposed before being multiplied with the vector.
         * @return Vector containing the result of the multiplication of the input vector and the matrix.
         */
        friend Vector2 operator* (const Vector2& v, 
                                  const Matrix2 &m)
        {
            // Apply matrix's transpose to v
            Vector2 r( m[0][0]*v[0] + m[1][0]*v[1],
                       m[0][1]*v[0] + m[1][1]*v[1] );
            return r;
        }

    public: // Output
        /**
         * @brief Print the contents of a Matrix2 to the specified stream.
         * @param out [in]; Stream to which to print the contents of Matrix2.
         * @param mat [in]; Matrix2 whose contents will be printed to the stream.
         *
         * @return Stream reference given in the out parameter.
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::Matrix2 & mat)
        {
            out << "[" << mat[0][0] << ", " << mat[0][1] << "\n"
                << " " << mat[1][0] << ", " << mat[1][1] << "]";
            return out << "\n";
        }

    protected: // Helpers
        /**
         * @brief Checks if the contents of the matrix are equal to the given values.
         *
         * @param m00 [in]; Input value to compare with element 00 of the matrix
         * @param m01 [in]; Input value to compare with element 01 of the matrix
         * @param m10 [in]; Input value to compare with element 10 of the matrix
         * @param m11 [in]; Input value to compare with element 11 of the matrix
         * @return Boolean value: "true" if equality is reached.
         * @internal
         */
        bool quick_check(double m00, double m01,
                         double m10, double m11)
        {
            return ( double_equal(m00, _data[0][0]) && 
                     double_equal(m01, _data[0][1]) &&
                     double_equal(m10, _data[1][0]) &&
                     double_equal(m11, _data[1][1]));
        }

        /**
         * @brief Internal pointers initialization
         * @internal
         */
        void init ()
        {
            _data[0] = _matrix;
            _data[1] = _data[0] + 2;
        }

    protected: // Implementation
        /*
         * The internal matrix data.  It is represented as 2
         * pointers.  Each pointer points to a separate row of the matrix,
         * with matrix[0] being the top row.
         */
        double   _matrix[4]; /**< The internal matrix data. */
        double * _data[2];  /**< Two additional pointers used for fast
                             * access. Each pointer points to a
                             * separate row of the matrix, with
                             * matrix[0] being the top row. */
    }; // end class Matrix2
    
} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_MATRIX2_HPP
