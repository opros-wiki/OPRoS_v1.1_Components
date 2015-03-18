//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_MATRIX_HPP
#define INCLUDE_EVOLUTION_MATH_MATRIX_HPP


/**
 * @file    evolution/core/math/Matrix.hpp
 * @brief   Matrix template class.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */

#include <math.h>
#include <time.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/Matrix3.hpp>
#include <evolution/core/math/MathUtils.hpp>

namespace Evolution
{

    /**
     * @class Matrix
     *
     * @brief  Class that implements an MxN matrix.
     *
     * Convention for using blocks of matricies: upper left
     * of block, and width/height of block are passed in
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    template <class Type>
    class Matrix
    {
    public: // Structors
        /**
         * @brief Construct a blank matrix
         */
        Matrix () : _rows(0), _cols(0), _data(NULL) {}

        /**
         * @brief Construct a zero-based matrix
         * @param rows [in]; number of rows for the matrix
         * @param cols [in]; number of columns for the matrix
         */
        Matrix (unsigned rows, unsigned cols)
            : _rows(0), _cols(0), _data(NULL)
        {
            init (rows, cols);
        } // end constructor

        /**
         * @brief Construct a matrix with all elements equal to a constant
         * @param value [in]; value to initialize all matrix elements to
         * @param rows [in]; number of rows for the matrix
         * @param cols [in]; number of columns for the matrix
         */
        Matrix (const Type & value, unsigned rows, unsigned cols)
            : _rows(0), _cols(0), _data(NULL)
        {
            init (rows, cols);
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = value;
                    }
                }
            }
        } // end constructor

        /**
         * @brief Construct a matrix and initalize it with an array
         * @param array [in]; data to fill matrix elements with, size rows * cols
         * @param rows [in]; number of rows for the matrix
         * @param cols [in]; number of columns for the matrix
         */
        Matrix (const Type * array, unsigned rows, unsigned cols)
            : _rows(0), _cols(0), _data(NULL)
        {
            init (rows, cols);
            if (_data != NULL)
            {
                memcpy (_data[0], array, _rows*_cols*sizeof(Type));
            }
        } // end constructor


        /**
         * @brief Copy constructor
         * @param mat [in]; Matrix to copy
         */
        Matrix (const Matrix<Type> & mat)
            : _rows(0), _cols(0), _data(NULL)
        {
            init (mat.nrows (), mat.ncols ());
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = mat[i][j];
                    }
                }
            }
        } // end copy constructor

        /**
         * @brief Creates a Matrix<double> from a Matrix3
         * @param mat [in]; Matrix to copy
         */
        Matrix (const Matrix3 & mat)
            : _rows(0), _cols(0), _data(NULL)
        {
            init (3,3);
            if (_data != NULL)
            {
                memcpy ( _data, mat.get_data(), sizeof(double) * 9 );
            }
        }

        /**
         * @brief Destructor
         */
        ~Matrix ()
        {
            clear ();
        }

    public: // Assignment
        /**
         * @brief Assign one matrix to another matrix
         * @param mat [in]; matrix to copy
         * @return reference to *this
         */
        Matrix & operator= (const Matrix<Type> & mat)
        {
            if (this == &mat)
            {
                return *this;
            }

            //clear (); //don't clear before fast_resize.
            fast_resize (mat.nrows (), mat.ncols ());
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = mat[i][j];
                    }
                }
            }
            return *this;
        }

        /**
         * @brief Assign a value to all the elements of a matrix
         * @param value [in]; value to set all elements of matrix to
         * @return reference to *this
         */
        Matrix & operator= (const Type & value)
        {
            if ((_data != NULL) && (_data[0] != NULL))
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = value;
                    }
                }
            }
            return *this;
        }

    public: // Data access
        /**
         * @brief Retrieve element value
         * @param row [in]; row to access
         * @return the Type* pointing to the row'th row
         */
        Type * operator[] (const unsigned row)
        {
            ASSERT ( row < _rows);
            return _data[row];
        }

        /**
         * @brief Constant retrieve of element value
         * @param row [in]; row to access
         * @return the Type* pointing to the row'th row
         */
        const Type * operator[] (const unsigned row) const
        {
            ASSERT ( row < _rows);
            return _data[row];
        }

        /**
         * @brief Retrieve element value
         * @param row [in]; row to access
         * @return the Type* pointing to the row'th row
         */
        Type * operator[] (const int row)
        {
            ASSERT (row >= 0 );
            ASSERT ( (unsigned) row < _rows);
            return _data[row];
        }

        /**
         * @brief Constant retrieve of element value
         * @param row [in]; row to access
         * @return the Type* pointing to the row'th row
         */
        const Type * operator[] (const int row) const
        {
            ASSERT (row >= 0 );
            ASSERT ( (unsigned) row < _rows);
            return _data[row];
        }

        /**
         * @brief Number of rows
         * @return number of rows in matrix
         */
        unsigned nrows() const { return _rows; }

        /**
         * @brief Number of rows
         * @return number of rows in matrix
         */
        unsigned rsize() const { return _rows; }

        /**
         * @brief Number of columns
         * @return number of columns in matrix
         */
        unsigned ncols() const { return _cols; }

        /**
         * @brief Number of columns
         * @return number of columns in matrix
         */
        unsigned csize() const { return _cols; }

        /**
         * @brief Obtains the data as a single-dimensional array.
         * @return a single dimensional array of the matrix elements
         */
        const Type* get_linear_data () const { return _data[0]; }

        /**
         * @brief Obtains the data as a single-dimensional array.
         * @return a single dimensional array of the matrix elements
         */
        Type* get_linear_data () { return _data[0]; }

        /**
         * @brief Obtains the data as a two-dimensional array.
         * @return a two dimensional array of the matrix elements
         */
        const Type** get_array_data () const { return _data; }

        /**
         * @brief Obtains the data as a two-dimensional array.
         * @return a two dimensional array of the matrix elements
         */
        Type** get_array_data () { return _data; }

    public: // Modification
        /**
         * @brief Manually set a matrix's contents
         * @param array [in]; array to fill the matrix with
         * @param rows [in]; number of rows for the matrix
         * @param cols [in]; number of columns for the matrix
         * @return reference to *this
         */
        Matrix& set (const Type *array, unsigned rows, unsigned cols)
        {
            PRECOND (array);
            if ((_data != NULL) && (array == _data[0]))
            {
                return *this;
            }

            if ((cols != _cols) || (rows != _rows) || (_data == NULL))
            {
                clear ();
                init (rows, cols);
            }
            if (_data != NULL)
            {
                memcpy (_data[0], array, _rows*_cols*sizeof(Type));
            }
            return *this;
        }  // end set ()

        /**
         * @brief Copy a submatrix of the given matrix into the current
         *        object
         *        *this -> would have size height x width
         * @param m [in]; matrix to take data from.
         * @param row [in]; number of rows for the matrix
         * @param col [in]; number of columns for the matrix
         * @param height [in]; number of columns for the matrix
         * @param width [in]; number of columns for the matrix
         * @return reference to *this
         */
        Matrix& set (const Matrix<Type>& m, unsigned row, unsigned col,
                     unsigned height, unsigned width)
        {
            //check preconditions
            PRECOND (*this != m);
            PRECOND (col+width  <= m.ncols());
            PRECOND (row+height <= m.nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            init(height, width);//get the right data size, don't set with zeros
            unsigned oi=0,oj=0, i=0, j=0; //iteration variables
            unsigned lr=row+height, lc=col+width;
            for(j=0,oj=row;oj<lr;oj++, j++)
            {
                for(i=0,oi=col;oi<lc;oi++, i++)
                {
                    _data[j][i] = m[oj][oi];
                }

            }
            return (*this);
        }

        /**
         * @brief Copies the contents of the vector at a specified row
         * @param vec [in]; vector to fill the row with
         * @param row [in]; row to fill
         * @return reference to *this
         */
        Matrix& set_row (const Vector<Type> & vec, unsigned row)
        {
            ASSERT (row >= 0 && row < nrows());
            ASSERT (vec.size () > 0 && vec.size () <=  ncols());

            for (unsigned i = 0; i < vec.size (); i++)
            {
                _data[row][i] = vec[i];
            }

            return *this;
        }

        /**
         * @brief Copies the contents of the vector at specified row
         *        Vector3 is a vector of 3 doubles, so this function should
         *        not be used when the matrix is not a Matrix<double>
         * @param vec [in]; vector to fill the row with
         * @param row [in]; row to fill
         * @return reference to *this
         * @pre this->csize() == 3
         */
        Matrix& set_row (const Vector3& vec, unsigned row)
        {
            ASSERT (row >= 0 && row < nrows());
            ASSERT (ncols() >= 3);

            for (unsigned i = 0; i < 3; i++)
            {
                _data[row][i] = vec[i];
            }

            return *this;
        }

        /**
         * @brief Copies the contents of the vector at a specified column
         * @param vec [in]; vector to fill the column with
         * @param col [in]; column to fill
         * @return reference to *this
         */
        Matrix& set_col (const Vector<Type> & vec, unsigned col)
        {
            ASSERT (col >= 0 && col < ncols());
            ASSERT (vec.size () > 0 && vec.size () <=  nrows());

            for (unsigned i = 0; i < vec.size (); i++)
            {
                _data[i][col] = vec[i];
            }

            return *this;
        }

        /**
         * @brief Copies the contents of the vector at a specified column
         *        Vector3 is a vector of 3 doubles, so this function should
         *        not be used when the matrix is not a Matrix<double>
         * @param vec [in]; vector to fill the column with
         * @param col [in]; column to fill
         * @return reference to *this
         * @pre this->rsize() == 3
         */
        Matrix& set_col (const Vector3& vec, unsigned col)
        {
            ASSERT (col >= 0 && col < ncols());
            ASSERT (nrows() >= 3);

            for (unsigned i = 0; i < 3; i++)
            {
                _data[i][col] = vec[i];
            }

            return *this;
        }

        /**
         * @brief Copies the contents of the ind'th row of *this into row
         * @param ind [in]; row to copy to vector
         * @param row [out]; vector to write row to
         * @return Result indicating success or failure.
         */
        Result get_row (unsigned ind, Vector<Type>* row)  const
        {
            if(row == NULL) return RESULT_INVALID_ARGUMENT;
            if(row->size() != csize()) row->resize(0.0, csize());
            row->set(_data[ind], csize());
            return RESULT_SUCCESS;
        }

        /**
         * @brief Copies the contents of the ind'th row of *this into row
         * @param ind [in]; row to copy to vector
         * @param row [out]; vector to write row to
         * @pre this->csize() == 3
         * @return Result indicating success or failure.
         */
        Result get_row (unsigned ind, Vector3* row)  const
        {
            if(row == NULL) return RESULT_INVALID_ARGUMENT;
            if(csize() != 3) return RESULT_PRECONDITION_ERROR;
            row->set(_data[ind]);
            return RESULT_SUCCESS;
        }

        /**
         * @brief Copies the contents of the ind'th column of *this into col
         * @param ind [in]; col to copy to vector
         * @param col [out]; vector to write col to
         * @return Result indicating success or failure.
         */
        Result get_col (unsigned ind, Vector<Type>* col) const
        {
            if(col == NULL) return RESULT_INVALID_ARGUMENT;
            if(col->size() != rsize()) col->resize(0.0, rsize());
            for(unsigned i=0; i<rsize(); i++)
            {
                (*col)[i] = _data[i][ind];
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief Copies the contents of the ind'th column of *this into col
         * @param ind [in]; col to copy to vector
         * @param col [out]; vector to write col to
         * @pre this->csize() == 3
         * @return Result indicating success or failure.
         */
        Result get_col (unsigned ind, Vector3* col) const
        {
            if(col == NULL) return RESULT_INVALID_ARGUMENT;
            if(rsize() != 3) return RESULT_PRECONDITION_ERROR;
            unsigned i=0;
            for(unsigned i=0; i<rsize(); i++)
            {
                (*col)[i] = _data[i][ind];
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief return a matrix that containst the block at r,c h,w
         * @param r [in]; upper coordinate of block
         * @param c [in]; left coordinate of block
         * @param h [in]; height of block
         * @param w [in]; width of block
         * @return Matrix that contains the data in the specified block.
         */
        Matrix get_block (unsigned r, unsigned c, unsigned h, unsigned w)
        {
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            PRECOND (_rows >= r + h);
            PRECOND (_cols >= c + w);
            Matrix<Type> retval;
            retval.zeros(h, w);
            for(unsigned i = 0; i < h ; i++)
            {
                for(unsigned j = 0; j < w ; j++)
                {
                    retval[i][j] = _data[i+r][j+c];
                }
            }
            return retval;
        }

        /**
         * @brief insert the given matrix at location r,c
         * @param input [in]; matrix to set block with
         * @param r [in]; row coordinate to insert block at
         * @param c [in]; col coordinate to insert block at
         */
        void set_block (const Matrix<Type>& input, unsigned r, unsigned c)
        {
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            PRECOND (_rows >= r + input.nrows());
            PRECOND (_cols >= c + input.ncols());

            for(unsigned i = 0; i < input.nrows() ; i++)
            {
                for(unsigned j = 0; j < input.ncols() ; j++)
                {
                    _data[i+r][j+c] = input[i][j];
                }
            }
        }

        /**
         * copies block from ior,ioc ih,iw in input to itr,itc ih,iw in *this
         * @param input [in]; matrix to set block with
         * @param itr [in]; this row index
         * @param itc [in]; this column index
         * @param ih [in]; height of block
         * @param iw [in]; width of block
         * @param ior [in]; other row index
         * @param ioc [in]; other column index
         */
        void set_block (const Matrix<Type>& input, unsigned itr, unsigned itc,
                        unsigned ih, unsigned iw, unsigned ior=0,
                        unsigned ioc=0)
        {
            //detect and compensate for overrun bounds.
            if(itr+ih > _rows)
            {
                ih -= itr + ih - _rows;
            }
            if(itc + iw > _cols)
            {
                iw -= itc + iw - _cols;
            }
            if(itr < 0)
            {
                //ih +=itr;  //itr is negative. subtracts.
                itr = 0;
            }
            if(itc < 0)
            {
                //iw += itc;
                itc = 0;
            }

            PRECOND(ior+ih <= input.rsize());
            PRECOND(ioc+iw <= input.csize());
            PRECOND(ior   >= 0);
            PRECOND(ioc   >= 0);

            for(unsigned r=0;r<ih;r++)
            {
                for(unsigned c=0;c<iw;c++)
                {
                    _data[r+itr][c+itc] = input[r+ior][c+ioc];
                }
            }
        }

        /**
         * Clears out the matrix.
         * @return reference to *this
         */
        Matrix& clear ()
        {
            if (_data != NULL && _cols > 0 && _rows > 0)
            {
                if ((_data[0]) != NULL)
                {
                    delete [] (_data[0]);
                }
                delete [] _data;
                _data = NULL;
            }
            _rows = _cols = 0;
            return *this;
        }

        /**
         * @brief Resize the matrix and initialize its contents
         * @param value [in]; value to set each element to
         * @param rows [in]; number of rows for the resized matrix
         * @param cols [in]; number of columns for the resized matrix
         * @return reference to *this
         */
        Matrix& resize (const Type & value, unsigned rows, unsigned cols)
        {
            fast_resize(rows, cols);
            //clear ();
            //init (rows, cols);
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = value;
                    }
                }
            }
            return *this;
        }

        /**
         * @brief Resize the matrix, copy the contents if the new matrix
         *        is bigger than the old one
         *
         * example:  resize [1 1 1]
         *                  [1 1 1]
         *                  [1 1 1]
         * to 5 x 5, and the result is:
         *    [1 1 1 0 0]
         *    [1 1 1 0 0]
         *    [1 1 1 0 0]
         *    [0 0 0 0 0]
         *    [0 0 0 0 0]
         *
         * @param rows [in]; number of rows for the resized matrix
         * @param cols [in]; number of columns for the resized matrix
         * @return reference to *this
         */
        Matrix& resize (unsigned rows, unsigned cols)
        {
            if (_data != NULL)
            {
                if ((_rows == rows) && (_cols == cols))// have the right size
                {
                    return *this;
                } //else we need to find the bounds for data copy/init
                // 0 -> rcopy, rcopy -> rinit
                unsigned rcopy = MIN(rows, _rows); //if rows > _rows, need init.
                //unsigned rinit = rows;
                // 0 -> ccopy, ccopy -> cinit
                unsigned ccopy = MIN(cols, _cols); //if cols > _cols, need init.
                //unsigned cinit = cols;
                unsigned ocols = _cols;
                unsigned i=0, j=0;
                //Type * new_data = new Type[rows*cols];
                Type * old_data = _data[0];
                Type ** old_data_p = _data;
                _data[0] = NULL;
                _data = NULL;
                zeros (rows, cols);

                for (i = 0; i < rcopy; i++)
                {
                    for (j = 0; j < ccopy; j++)
                    {
                        _data[i][j] = old_data[i*ocols+j];
                    }
                }

                delete[] old_data;
                delete[] old_data_p;
            }
            else
            {
                zeros (rows, cols);
            }
            return *this;
        }

        /**
         * @brief Resize the matrix, do absolutely _no_ data initialization.
         * @param rows [in]; number of rows for the resized matrix
         * @param cols [in]; number of columns for the resized matrix
         * @return reference to *this
         */
        Matrix& fast_resize (unsigned rows, unsigned cols)
        {
            if (_data != NULL)
            {
                if ((_rows == rows) && (_cols == cols))// have the right size
                {
                    return *this;
                }
                clear ();
            }
            init (rows, cols);
            return *this;
        }

    public: // Special matrices
        /**
         * @brief Set the matrix to identity of size x size
         * @param size [in]; width and height of identity matrix
         */
        void eye (unsigned size)
        {
            resize ((Type) 0, size, size);
            if (_data != NULL)
            {
                for (unsigned i = 0; i < size; i++)
                {
                    _data [i][i] = (Type) 1;
                }
            }
        }

        /**
         * @brief eye(diagonal_values.size()) * diagonal_values
         * @param diagonal_values [in]; diagonal values to fill the diagonal with
         */
        void diagonal ( Vector<Type> diagonal_values)
        {
            if(diagonal_values.size() == 0) return;
            resize ((Type) 0, diagonal_values.size(), diagonal_values.size());
            if (_data != NULL)
            {
                for (unsigned ii = 0; ii < rsize(); ii++)
                {
                    _data [ii][ii] = diagonal_values[ii];
                }
            }
        }

        /**
         * @brief Set the matrix to a rows by cols ones matrix
         * @param rows [in]; number of rows
         * @param cols [in]; number of columns
         */
        void ones (unsigned rows, unsigned cols)
        {
            resize ((Type) 1, rows, cols);
        }

        /**
         * @brief Set the matrix to a rows by cols zeros matrix
         * @param rows [in]; number of rows
         * @param cols [in]; number of columns
         */
        void zeros (unsigned rows, unsigned cols)
        {
            resize ((Type) 0, rows, cols);
        }

        /**
         * @brief Set the matrix to a rows by cols random(range) matrix
         * @param rows [in]; number of rows
         * @param cols [in]; number of columns
         * @param range [in]; Maximum value for the random matrix to have.
         */
        void random (unsigned rows, unsigned cols, Type range)
        {
            init ( rows, cols);
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = (Type) (range * ((double) std::rand()) / RAND_MAX);
                    }
                }
            }
        }


        /**
         * @brief Generate a gradient matrix.
         *
         * Encode matrix with i * row_range/rows + j * col_range/cols
         *        for each i, j
         * @param rows [in]; number of rows
         * @param cols [in]; number of columns
         * @param row_range [in]; range of the row contribution
         * @param col_range [in]; range of the column contribution
         */
        void generate_gradient ( unsigned rows, unsigned cols,
                                 double row_range=0.5, double col_range=0.5)
        {
            double rgrad = row_range/(double)(rows-1.0); //set row gradient
            double cgrad = col_range/(double)(cols-1.0); //set col gradient

            if(_rows != rows || _cols != cols) //resize if needed.
            {
                resize ((Type) 0, rows, cols);
            }

            for (unsigned i = 0; i < _rows; i++) //iterate down the rows
            {
                for (unsigned j = 0; j < _cols; j++) //iterate down the cols
                {
                    _data[i][j] = i * rgrad + j * cgrad;
                }
            }
        }

    public:
        // Pointwise functions and other destructive operations (convolutions)
        typedef Type (*DfuncType)(Type); ///< Typedef for a function that transforms a double.

        /**
         * @brief applies an arbitrary double (*)(double) function
         *        to each element.
         * @param dfunc [in]; function to apply
         * @return true if successful, false otherwise
         */
        bool pointwise_function(DfuncType dfunc)
        {
            for (unsigned i = 0; i < _rows; i++) //iterate down the rows
            {
                for (unsigned j = 0; j < _cols; j++) //iterate down the cols
                {
                    _data[i][j] = dfunc(_data[i][j]);
                }
            }
            return true;
        }

        /**
         * @brief applies an arbitrary double (*)(double) function
         *        to a submatrix.
         * @param r [in]; row coordinate to start applying function
         * @param c [in]; col coordinate to start applying function
         * @param h [in]; height of submatrix
         * @param w [in]; width of submatrix
         * @param dfunc [in]; function to apply
         * @return true if successful, false otherwise
         */
        bool pointwise_function(DfuncType dfunc, unsigned r, unsigned c,
                                unsigned h, unsigned w)
        {
            if(r+h > _rows || c+w > _cols)
            {
                return false;
            }
            for (unsigned i = r; i < r+h; i++) //iterate down the rows
            {
                for (unsigned j = c; j < c+w; j++) //iterate down the cols
                {
                    _data[i][j] = dfunc(_data[i][j]);
                }
            }
            return true;
        }

        /**
         * @brief Shifts the matrix by rshift down and cshift right.
         *
         * Shifts the matrix by rshift down and cshift right.  Negative shifts
         * are supported, and simply shift the matrix left or up.
         * @param rshift [in]; Amount to shift down
         * @param cshift [in]; Amount to shift right
         */
        bool shift(int rshift, int cshift)
        {

            unsigned rstart=0, rend=0, rfinal=0;
            unsigned cstart=0, cend=0, cfinal=0;
            unsigned rincrement=0, cincrement=0;
            unsigned i=0, j=0;
            if(rshift < 0)
            {
                rstart = - rshift;
                rend   = _rows - 1;
                rfinal = _rows - rshift;
                rincrement = 1;
            }
            else
            {
                rstart = _rows - rshift - 1;
                rend   = -1; //end at the beginning!
                rfinal = - rshift - 1;
                rincrement = -1;
            }
            if(cshift < 0)
            {
                cstart = -cshift;
                cend   = _cols - 1;
                cfinal = _cols - cshift;
                cincrement =  1;
            }
            else
            {
                cstart = _cols - cshift - 1;
                cend   = -1; //end at the beginning!
                cfinal = - cshift - 1;
                cincrement = -1;
            }
            for(i=rstart; i != rend ; i += rincrement)
            {
                for(j=cstart; j != cend ; j += cincrement)
                {
                    _data[i+rshift][j+cshift] = _data[i][j];
                }
            }

            for(i=0; i < rsize() ; i ++)
            {
                for(j=cend; j != cfinal ; j += cincrement)
                {
                    _data[i][j+cshift] = 0;
                }
            }

            //FIXME: don't overwrite the third block with zeros twice. adjust upper/lower bounds
            for(i=rend; i != rfinal ; i += rincrement)
            {
                for(j=0; j < csize() ; j ++)
                {
                    _data[i+rshift][j] = 0;
                }
            }

            return true;
        }

        /**
         * @brief Shears the matrix by rshift down and cshift right.
         *
         * Shears the matrix by rshift down and cshift right.  Negative shears
         * are supported.  May not be entirely correct.
         *
         * The idea for this function is to shift the elements vertically, and
         * then shift them horizontally.  The shift distance is
         * (index * shear) / maxindex.
         * @param rshear [in]; Amount to shear down
         * @param cshear [in]; Amount to shear right
         * @return true if successful, false otherwise
         */
        bool shear(int rshear, int cshear)
        {
            Matrix<Type> temp = *this;

            double sfract=0; //fraction of shear.

            //widthwise
            int jadj=0, i=0, j=0;
            for(i=0; i != rsize() ; i ++)
            {
                for(j=0; j < csize() ; j ++)
                {
                    sfract = - rshear * i / (double) rsize();
                    jadj = j + (int)(sfract);
                    if(jadj >= 0 && jadj < csize())
                    {
                        _data[i][j] = temp[i][jadj];
                    }
                    else
                    {
                        _data[i][j] = 0;
                    }
                }
            }

            temp = *this;

            //heightwise
            int iadj=0;
            for(i=0; i != rsize() ; i ++)
            {
                for(j=0; j < csize() ; j ++)
                {
                    sfract = - cshear * j / (double) csize();
                    iadj = i + (int)(sfract);
                    if(iadj >= 0 && iadj < rsize())
                    {
                        _data[i][j] = temp[iadj][j];
                    }
                    else
                    {
                        _data[i][j] = 0;
                    }
                }
            }

            return true;
        }

        /**
         * @brief elementwise threshhold function.
         * @param threshold [in]; value to threshold with
         * @param low [in]; value for elements lower than threshold
         * @param high [in]; value for elements higher than threshold
         * @return true if successful, false otherwise
         */
        bool threshold(double threshold, double low = 0, double high = 255)
        {
            for(unsigned i = 0; i <_rows; i++) //iterate down the rows
            {
                for(unsigned j = 0; j < _cols; j++) //iterate down the cols
                {
                    _data[i][j] = (_data[i][j] > threshold)? high : low;
                }
            }
            return true;
        }

        /**
         * @brief squares each element.
         * @return true if successful, false otherwise
         */
        bool square()
        {
            for (unsigned i = 0; i < _rows; i++) //iterate down the rows
            {
                for (unsigned j = 0; j < _cols; j++) //iterate down the cols
                {
                    _data[i][j] *= _data[i][j];
                }
            }
            return true;
        }

        /**
         * @brief squares a submatrix.
         * @param r [in]; row coordinate to start squaring
         * @param c [in]; col coordinate to start squaring
         * @param h [in]; height of submatrix
         * @param w [in]; width of submatrix
         * @return true if successful, false otherwise
         */
        bool square(unsigned r, unsigned c, unsigned h, unsigned w)
        {
            if(r+h > _rows || c+w > _cols)
            {
                return false;
            }
            for (unsigned i = r; i < r+h; i++) //iterate down the rows
            {
                for (unsigned j = c; j < c+w; j++) //iterate down the cols
                {
                    _data[i][j] *= _data[i][j];
                }
            }
            return true;
        }

        /**
         * @brief square roots each element.
         * @return true if successful, false otherwise
         */
        bool square_root()
        {
            for (unsigned i = 0; i < _rows; i++) //iterate down the rows
            {
                for (unsigned j = 0; j < _cols; j++) //iterate down the cols
                {
                    _data[i][j] = sqrt(_data[i][j]);
                }
            }
            return true;
        }

        /**
         * @brief square roots a submatrix.
         * @param r [in]; row coordinate to start square rooting
         * @param c [in]; col coordinate to start square rooting
         * @param h [in]; height of submatrix
         * @param w [in]; width of submatrix
         * @return true if successful, false otherwise
         */
        bool square_root(unsigned r, unsigned c, unsigned h, unsigned w)
        {
            if(r+h > _rows || c+w > _cols)
            {
                return false;
            }
            for (unsigned i = r; i < r+h; i++) //iterate down the rows
            {
                for (unsigned j = c; j < c+w; j++) //iterate down the cols
                {
                    _data[i][j] = sqrt(_data[i][j]);
                }
            }
            return true;
        }

        /**
         * @brief Rescales the matrix elements linearly onto [lower,upper]
         *
         * Rescales the matrix elements linearly onto [lower,upper] from
         * [this->min(), this->max()], doesn't check for zero interval length.
         * @param lower [in]; new lower bound of matrix
         * @param upper [in]; new upper bound of matrix
         * @return true if successful, false otherwise
         */
        bool rescale(Type lower, Type upper)
        {
            Type max_f = max_value();
            Type min_f = min_value();
            Type old_width = max_f-min_f;
            Type new_width = upper-lower;
            for(unsigned i = 0; i < rsize(); i++)
            {
                for(unsigned j = 0; j < csize(); j++)
                {
                    _data[i][j] = (_data[i][j]-min_f)/old_width*new_width+lower;
                }
            }
            return true;
        }

        /**
         * @brief Rescales the matrix elements linearly onto [lower,upper]
         *
         * Rescales the matrix elements linearly onto [lower,upper] from [0,
         * this->max()] Doesn't check for zero interval length.  Uses zero for
         * the current lower bound: better for Image manipulation or small
         * ranges
         * @param lower [in]; new lower bound of matrix
         * @param upper [in]; new upper bound of matrix
         * @return true if successful, false otherwise
         */
        bool rescale_zero(double lower, double upper)
        {
            double max_f = max_value();
            double min_f = 0; //min_value above, only diff.
            double old_width = max_f-min_f;
            double new_width = upper-lower;
            for(unsigned i = 0; i < rsize(); i++)
            {
                for(unsigned j = 0; j < csize(); j++)
                {
                    _data[i][j] = (_data[i][j]-min_f)/old_width*new_width+lower;
                }
            }
            return true;
        }

        /**
         * @brief Apply the specified separable filter to this matrix,
         *        destructively.
         * @param rfilter    [in]; array holding filter
         * @param rfilterlen [in]; length of array holding filter
         * @param cfilter    [in]; array holding filter
         * @param cfilterlen [in]; length of array holding filter
         * @return true if successful, false otherwise
         */
        bool separable_filter(const Type * rfilter, unsigned rfilterlen,
                              const Type * cfilter, unsigned cfilterlen)
        {
            // make sure filters are oddly sized.
            PRECOND(cfilterlen%2 && rfilterlen%2 );  //odd size precondition
            if(!cfilter || !rfilter)//if one filter is bad...
            {
                return false; //quietly go to a corner, curl up, and die
            }

            // out of bounds reads are zeros,
            // and thus don't contribute to filtering.
            // want to center the filtered image in the appropriate place,
            // padding outside
            // with dirty little zeros
            Matrix<Type> temp;
            temp.zeros(_rows, _cols); //get right sized temp matrix

            unsigned j=0, i=0;
            //i filter
            for(i=0;i<_rows;i++)
            {
                for(j=0;j<_cols;j++)
                {
                    temp[i][j] = col_lin_filter(i,j,cfilter,cfilterlen);
                }
            }

            //j filter
            for(i=0;i<_rows;i++)
            {
                for(j=0;j<_cols;j++)
                {
                    _data[i][j] =  temp.row_lin_filter(i,j,rfilter,rfilterlen);
                }
            }
            return true;
        }

    protected: //Convolution assistance.

        /**
         * @brief Used to get a single half-separable_filter value in column
         *        (not a good idea to use with even length filters)
         * This is used as a helper to separable_filter, using it alone is probably silly.
         * @param r [in]; row coordinate
         * @param c [in]; col coordinate
         * @param cfilter [in]; column filter data
         * @param cfilterlen [in]; column filter length
         */
        Type col_lin_filter(unsigned r, unsigned c, const Type *cfilter,
                            unsigned cfilterlen) const
        {
            unsigned down = c - (cfilterlen-1)/2;
            unsigned up   = c + (cfilterlen-1)/2;
            unsigned i=0, fil=0;
            Type retval=(Type)0;
            // do not assume up and down won't both be oob.
            if(up > (_cols-1)) up = _cols-1;
            if(c < (cfilterlen-1)/2) //feel the unsignedness.
            {
                fil += (cfilterlen-1)/2 - c;
                down = 0;
            }
            //fil is inited in declare and up/down bounds
            for(i=down;i<=up;i++, fil++)
            {
                retval += _data[r][i] * cfilter[fil];
            }
            return retval;
        }

        /**
         * @brief Used to get a single half-separable_filter value in row
         *        (not a good idea to use with even length filters)
         * This is used as a helper to separable_filter, using it alone is probably silly.
         * @param r [in]; row coordinate
         * @param c [in]; col coordinate
         * @param rfilter [in]; row filter data
         * @param rfilterlen [in]; row filter length
         */
        Type row_lin_filter(unsigned r, unsigned c, const Type *rfilter,
                            unsigned rfilterlen) const
        {
            //don't bounds check r and c.
            unsigned down = r - (rfilterlen-1)/2;
            unsigned up =   r + (rfilterlen-1)/2;
            unsigned i=0, fil=0;
            Type retval=(Type)0;
            //bounds up and down
            // do not assume up and down won't both be oob.
            if(up > (_rows-1)) up = _rows-1;
            if(r < (rfilterlen-1)/2)
            {
                fil += (rfilterlen-1)/2 - r;
                down = 0;
            }
            ///fil is inited in declare and up/down bounds
            for(i=down;i<=up;i++, fil++)
            {
                retval += _data[i][c] * rfilter[fil];
            }
            return retval;
        }

    public: // Arithmetic operators
        /**
         * @brief Add one matrix to another matrix
         * @param mat [in]; matrix to sum with
         * @return reference to *this
         */
        Matrix& operator+= (const Matrix<Type> & mat)
        {
            PRECOND ((_rows == mat.nrows ()) && (_cols == mat.ncols ()));
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    _data[i][j] += mat[i][j];
                }
            }
            return *this;
        }

        /**
         * @brief Add one scalar to a matrix, piecewise.
         * @param scalar [in]; value to add.
         * @return reference to *this
         */
        Matrix& operator+= (const Type scalar)
        {
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] += scalar;
                    }
                }
            }
            return *this;
        }

        /**
         * @brief Substract one matrix from another matrix
         * @param mat [in]; matrix to subtract with
         * @return reference to *this
         */
        Matrix& operator-= (const Matrix<Type>& mat)
        {
            PRECOND ((_rows == mat.nrows ()) && (_cols == mat.ncols ()));
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    _data[i][j] -= mat[i][j];
                }
            }
            return *this;
        }

        /**
         * @brief Scale a matrix
         * @param scale [in]; multiply factor
         * @return reference to *this
         */
        Matrix& operator*= (const Type scale)
        {
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] *= scale;
                    }
                }
            }
            return *this;
        }

        /**
         * @brief Scale a matrix
         * @param scale [in]; division factor
         * @return reference to *this
         */
        Matrix& operator/= (const Type scale)
        {
            return operator*=( ( Type )1 / scale );
        }

        /**
         * @brief Return the sum of two matrices
         * @param mat [in]; matrix to sum with
         * @return Matrix that is this + mat
         */
        Matrix operator+ (const Matrix<Type> & mat) const
        {
            Matrix<Type> res = (*this);
            return (res += mat);
        }

        /**
         * @brief Return the difference of two matrices
         * @param mat [in]; matrix to subtract with
         * @return Matrix that is this - mat
         */
        Matrix operator- (const Matrix<Type> & mat) const
        {
            Matrix<Type> res = (*this);
            return (res -= mat);
        }

        /**
         * @brief Return the scaling of a matrix
         * @param scale [in]; scalar to multiply with
         * @return Matrix that is this * scale
         */
        Matrix operator* (const Type scale) const
        {
            Matrix<Type> res = (*this);
            return (res *= scale);
        }

        /**
         * @brief Return the scaling of a matrix
         * @param scale [in]; scalar to divide by
         * @return Matrix that is this / scale
         */
        Matrix operator/ (const Type scale) const
        {
            Matrix<Type> res = (*this);
            return (res /= scale);
        }


        /**
         * @brief Compute the product of a matrix by a vector
         * @param vec [in]; vector to multiply
         * @return Vector that is this * vec
         */
        Vector<Type> operator* (const Vector<Type> & vec) const
        {
            PRECOND (_cols == vec.size ());
            Vector<Type> res((Type) 0, _rows) ;
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    res[i] += _data[i][j] * vec[j];
                }
            }
            return res;
        }

        /**
         * @brief Compute the product of a matrix by a vector
         * @param vec [in]; vector to multiply
         * @return Vector that is this * vec
         * @pre this->rsize() is 3
         */
        Vector<Type> operator* (const Vector3 & vec) const
        {
            PRECOND (_cols == 3);
            Vector<Type> res((Type) 0, _rows) ;
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    res[i] += _data[i][j] * vec[j];
                }
            }
            return res;
        }

        /**
         * @brief Compute the product of the transpose of a matrix by a vector
         * @param vec [in]; vector to multiply
         * @return Vector that is this->transpose() * vec
         */
        Vector<Type> transp_mul (const Vector<Type> & vec) const
        {
            PRECOND (_rows == vec.size ());
            Vector<Type> res((Type) 0, _cols) ;
            for (unsigned j = 0; j < _cols; j++)
            {
                for (unsigned i = 0; i < _rows; i++)
                {
                    res[j] += _data[i][j] * vec[i];
                }
            }
            return res;
        }

        /**
         * @brief Compute the product of a matrix by a matrix
         * @param mat [in]; matrix to multiply with
         * @return Matrix that is this * mat
         */
        Matrix operator* (const Matrix<Type> & mat) const
        {
            PRECOND (_cols == mat.nrows ());
            Matrix<Type> res((Type) 0, _rows, mat.ncols ());
            Type *c = res.get_linear_data ();
            const Type *aa = _data[0];
            const Type *bb = mat.get_linear_data ();
            unsigned matcols = mat.csize();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < _rows; ++i)
            {
                for (unsigned j = 0; j < mat.ncols (); ++j)
                {
                    sum = (Type) 0;
//                     const Type *a = _data[i];
//                     const Type *b = &mat[0][j];
                    const Type *a = aa + i * _cols;
                    const Type *b = bb + j;
                    for (unsigned k = 0; k < _cols; ++k)
                    {
                        sum += (*a) * (*b);
                        ++a;
                        b += matcols;
                    }
                    *c++ = sum;
                }
            }
            return res;
        }

        /**
         * @brief Compute the product with a matrix
         *        (only valid if mat is a square matrix)
         * @param mat [in]; matrix to multiply with
         * @return reference to *this
         */
        Matrix& operator*= (const Matrix<Type> & mat)
        {
            PRECOND ((mat.nrows() == mat.ncols()) &&
                     (_cols == mat.nrows ()) );
            if (this == &mat)
            {
                // The extra copy of data is needed in the case A *= A;
                *this = *this * mat;
            }
            else
            {
                Vector<Type> vec(ncols());
                for (unsigned i = 0; i < _rows; i++)
                {
                    // Save the row contents since we would overwrite them
                    get_row (i, &vec);
                    for (unsigned j = 0; j < _cols; j++)
                    {
                        _data[i][j] = (Type) 0;
                        for (unsigned k = 0; k < mat.nrows(); k++)
                        {
                            _data[i][j] += vec[k] * mat[k][j];
                        }
                    }
                }
            }
            return *this;
        }

        /**
         * @brief Add a vector of length rsize to all columns.
         * @param vec [in]; vector to add to columns
         * @return reference to *this
         */
        Matrix& column_add (const Vector<double> & vec)
        {
            PRECOND (_rows == vec.size ());
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    _data[i][j] += vec[i];
                }
            }
            return *this;
        }

        /**
         * @brief Add a vector of length rsize to all columns.
         * @param vec [in]; vector to add to columns
         * @return reference to *this
         * @pre this->rsize() is 3
         */
        Matrix& column_add (const Vector3 & vec)
        {
            PRECOND (_rows == 3);
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    _data[i][j] += vec[i];
                }
            }
            return *this;
        }

        /**
         * @brief Adds val to each element in column index.
         * @param index [in]; column to add val to
         * @param val [in]; scalar to add to column
         * @return reference to *this
         */
        Matrix& column_add (unsigned index, const Type val)
        {
            if(index > csize()) return *this;
            for(unsigned ii=0; ii<rsize(); ii++) //walk the column
            {
                _data[ii][index] += val;
            }
            return *this;
        }

        /**
         * @brief Add a vector of length rsize to all rows.
         * @param vec [in]; vector to add to rows
         * @return reference to *this
         */
        Matrix& row_add (const Vector<double> & vec)
        {
            PRECOND (_cols == vec.size ());
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    _data[i][j] += vec[j];
                }
            }
            return *this;
        }

        /**
         * @brief Add a vector of length rsize to all rows.
         * @param vec [in]; vector to add to rows
         * @return reference to *this
         * @pre this->csize() is 3
         */
        Matrix& row_add (const Vector3 & vec)
        {
            PRECOND (_cols == 3);
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
                    _data[i][j] += vec[j];
                }
            }
            return *this;
        }

        /**
         * @brief Adds val to each element in row index.
         * @param index [in]; row to add val to
         * @param val [in]; scalar to add to row
         * @return reference to *this
         */
        Matrix& row_add (unsigned index, const Type val)
        {
            if(index > rsize()) return *this;
            for(unsigned ii=0; ii<csize(); ii++) //walk the row
            {
                _data[index][ii] += val;
            }
            return *this;
        }

    public: // Matrix operators
        /**
         * @brief Compute the transpose of the matrix in-place
         * @return reference to this object
         */
        Matrix& transpose ()
        {
            PRECOND (_data != NULL);
            Matrix m( _cols,_rows);
            // if m can not be allocated, m._cols=0 and m._rows == 0;
            // meaning this loop will not be executed;
            for(unsigned r = 0; r < m._cols;++r)
            {
                for(unsigned c  = 0; c < m._rows;++c)
                {
                    m[c][r] = (*this)[r][c];
                }
            }
            clear();
            _data = m._data;
            _rows = m._rows;
            _cols = m._cols;
            
            m._data=0;
            return *this;
        }

        /**
         * @brief Compute the determinant of the matrix
         * @return determinant of the matrix
         */
        Type determinant() const
        {
            PRECOND ((_data != NULL) && (_data[0] != NULL));
            PRECOND (_rows == _cols);
            if (_rows == 1)  // Specialized for a 1x1 (pretty dumb, ha!)
            {
                return _data[0][0];
            }
            else if (_rows == 2)  // Specialized for a 2x2
            {
                return (_data[0][0]*_data[1][1] - _data[0][1]*_data[1][0]);
            }
            else if (_rows == 3) // Specialized for a 3x3
            {
                return (+_data[0][0]*(_data[1][1]*_data[2][2]-_data[2][1]*_data[1][2])
                        -_data[1][0]*(_data[0][1]*_data[2][2]-_data[2][1]*_data[0][2])
                        +_data[2][0]*(_data[0][1]*_data[1][2]-_data[1][1]*_data[0][2]));
            }
            else // General case: using LU decomposition
            {
                Matrix<Type> LU(nrows (), ncols ());
                Vector<int> index;
                int permutation;
                lu ((*this), &LU, &index, &permutation);
                Type det = (Type) permutation;
                for (unsigned i = 0; i < ncols (); i++)
                {
                    det *= LU[i][i];
                }
                return det;
            }
        }

        /**
         * @brief Compute the determinant of the matrix
         * @return determinant of the matrix
         */
        Type det() const {return determinant();}

        /**
         * @brief Compute the inverse of the matrix
         * @param inv_ptr [out]; inverse of matrix
         */
        void inverse (Matrix<Type>* inv_ptr) const
        {
            PRECOND ((inv_ptr != NULL) && (_data != NULL) && (_data[0] != NULL));
            PRECOND (_rows == _cols);
            Matrix<Type>& inv = *inv_ptr;
            if (_rows == 1)  // Specialized for a 1x1 (pretty dumb, ha!)
            {
                inv.fast_resize (_rows, _rows);
                PRECOND(_data[0][0] != 0);
                inv[0][0] = ((Type) 1)/_data[0][0];
            }
            else if (_rows == 2)  // Specialized inverse for a 2x2
            {
                inv.fast_resize (_rows, _rows);
                Type d = determinant ();
                PRECOND(d != 0);
                inv[0][0] =  _data[1][1] / d;
                inv[0][1] = -_data[0][1] / d;
                inv[1][0] = -_data[1][0] / d;
                inv[1][1] =  _data[0][0] / d;
            }
            else if (_rows == 3)  // Specialized inverse for a 3x3
            {
                inv.fast_resize (_rows, _rows);
                Type d = determinant ();
                PRECOND(d != 0);
                inv[0][0] =  (_data[1][1]*_data[2][2]-_data[2][1]*_data[1][2]) / d;
                inv[0][1] = -(_data[0][1]*_data[2][2]-_data[2][1]*_data[0][2]) / d;
                inv[0][2] =  (_data[0][1]*_data[1][2]-_data[1][1]*_data[0][2]) / d;
                inv[1][0] = -(_data[1][0]*_data[2][2]-_data[2][0]*_data[1][2]) / d;
                inv[1][1] =  (_data[0][0]*_data[2][2]-_data[2][0]*_data[0][2]) / d;
                inv[1][2] = -(_data[0][0]*_data[1][2]-_data[1][0]*_data[0][2]) / d;
                inv[2][0] =  (_data[1][0]*_data[2][1]-_data[2][0]*_data[1][1]) / d;
                inv[2][1] = -(_data[0][0]*_data[2][1]-_data[2][0]*_data[0][1]) / d;
                inv[2][2] =  (_data[0][0]*_data[1][1]-_data[1][0]*_data[0][1]) / d;
            }
            else // General case
            {
                inv.fast_resize (_rows, _rows);
                invert ((*this), &inv);
            }
        }

        /**
         * @brief Compute the pseudo-inverse of the matrix
         * @param inv [out]; pseudo-inverse of matrix
         */
        void pinverse (Matrix<Type>* inv) const
        {
            PRECOND ((inv != NULL) && (_data != NULL) && (_data[0] != NULL));
            inv->fast_resize (_cols, _rows);
            pinvert ((*this), inv);
        }

    public: // Binary operators
        /**
         * @brief Test if two matrices are equal
         * @param mat [in]; matrix to compare to
         * @return true if equal, false otherwise
         */
        bool operator== (const Matrix<Type> & mat) const
        {
            if ((_rows != mat.nrows ()) || (_cols != mat.ncols ()))
            {
                return false;
            }
            for (unsigned i = 0; i < _rows; i++)
            {
                for (unsigned j = 0; j < _cols; j++)
                {
// FIXME: Need a template equal function!
                    if (double_not_equal((double) _data[i][j],
                                         (double) mat[i][j]))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        /**
         * @brief Test if two matrices are not equal
         * @param mat [in]; matrix to compare to
         * @return true if not equal, false otherwise
         */
        bool operator!= (const Matrix<Type>& mat) const
        {
            return !(*this==mat);
        }

    public: // Statistical operators
        /**
         * @brief Compute the max value of the matrix
         * @return maximum value in the matrix
         */
        Type max_value () const
        {
            PRECOND (_data != NULL);
            Type max_value= _data[0][0];
            for (unsigned i = 1; i < (_rows*_cols); i++)
            {
                if (_data[0][i] > max_value)
                {
                    max_value = _data[0][i];
                }
            }
            return max_value;
        }

        /**
         * @brief Compute the max value of the matrix in a block
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return maximum value in the matrix block
         */
        Type max_value (unsigned row, unsigned col, unsigned height,
                        unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            Type max_value = _data[row][col];
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    if (_data[oj][oi] > max_value)
                    {
                        max_value = _data[oj][oi];
                    }
                }
            }
            return max_value;
        }

        /**
         * @brief Compute the max value of one of the columns in the matrix.
         * @param col [in]; col to search for max
         * @return maximum value in the matrix block
         */
        Type max_value_col (unsigned col) const
        {
            PRECOND (col >= 0);
            PRECOND (col < _cols);
            PRECOND (_data != NULL);
            Type max_value = _data[0][col];
            unsigned ii=0;
            for( ii=0; ii<_rows; ii++)
            {
                max_value = MAX(max_value, _data[ii][col]);
            }
            return max_value;
        }

        /**
         * @brief Compute the max value of one of the columns in the matrix.
         * @param col [in]; col to search for max
         * @param minrow [in]; lower row index, to search from
         * @param maxrow [in]; upper row index, to search to
         * @return maximum value in the matrix block
         */
        Type max_value_col (unsigned col, unsigned minrow, unsigned maxrow) const
        {
            PRECOND (col >= 0);
            PRECOND (col < _cols);
            PRECOND (_data != NULL);
            if(minrow > _rows) minrow = _rows;
            if(maxrow > _rows) maxrow = _rows;
            Type max_value = _data[minrow][col];
            unsigned ii=0;
            for( ii=minrow; ii<maxrow; ii++)
            {
                max_value = MAX(max_value, _data[ii][col]);
            }
            return max_value;
        }

        /**
         * @brief Compute the max value of one of the rows in the matrix.
         * @param row [in]; row to search for max
         * @return maximum value in the matrix block
         */
        Type max_value_row (unsigned row) const
        {
            PRECOND (row >= 0);
            PRECOND (row < _rows);
            PRECOND (_data != NULL);
            Type max_value = _data[row][0];
            unsigned ii=0;
            for( ii=0; ii<_cols; ii++)
            {
                max_value = MAX(max_value, _data[row][ii]);
            }
            return max_value;
        }

        /**
         * @brief Compute the max value of one of the rows in the matrix.
         * @param row [in]; row to search for max
         * @param mincol [in]; lower col index, to search from
         * @param maxcol [in]; upper col index, to search to
         * @return maximum value in the matrix block
         */
        Type max_value_row (unsigned row, unsigned mincol, unsigned maxcol) const
        {
            PRECOND (row >= 0);
            PRECOND (row < _rows);
            PRECOND (_data != NULL);
            if(mincol > _cols) mincol = _cols;
            if(maxcol > _cols) maxcol = _cols;
            Type max_value = _data[row][mincol];
            unsigned ii=0;
            for( ii=mincol; ii<maxcol; ii++)
            {
                max_value = MAX(max_value, _data[row][ii]);
            }
            return max_value;
        }

        /**
         * @brief Compute the min value of the matrix
         * @return minimum value in the matrix block
         */
        Type min_value () const
        {
            PRECOND (_data != NULL);
            Type min_value = _data[0][0];
            for (unsigned i = 1; i < (_rows*_cols); i++)
            {
                if (_data[0][i] < min_value)
                {
                    min_value = _data[0][i];
                }
            }
            return min_value;
        }

        /**
         * @brief Compute the min value of the matrix
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return minimum value in the matrix block
         */
        Type min_value (unsigned row, unsigned col, unsigned height,
                        unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            Type min_value = _data[row][col];
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    if (_data[oj][oi] < min_value)
                    {
                        min_value = _data[oj][oi];
                    }
                }
            }
            return min_value;
        }

        /**
         * @brief Compute the min value of one of the columns in the matrix.
         * @param col [in]; col to search for max
         * @return minimum value in the matrix block
         */
        Type min_value_col (unsigned col) const
        {
            PRECOND (col >= 0);
            PRECOND (col < _cols);
            PRECOND (_data != NULL);
            Type min_value = _data[0][col];
            unsigned ii=0;
            for( ii=0; ii<_rows; ii++)
            {
                min_value = MIN(min_value, _data[ii][col]);
            }
            return min_value;
        }

        /**
         * @brief Compute the min value of one of the columns in the matrix.
         * @param col [in]; col to search for max
         * @param minrow [in]; lower row index, to search from
         * @param maxrow [in]; upper row index, to search to
         * @return minimum value in the matrix block
         */
        Type min_value_col (unsigned col, unsigned minrow, unsigned maxrow) const
        {
            PRECOND (col >= 0);
            PRECOND (col < _cols);
            PRECOND (_data != NULL);
            if(minrow > _rows) minrow = _rows;
            if(maxrow > _rows) maxrow = _rows;
            Type min_value = _data[minrow][col];
            unsigned ii=0;
            for( ii=minrow; ii<maxrow; ii++)
            {
                min_value = MIN(min_value, _data[ii][col]);
            }
            return min_value;
        }

        /**
         * @brief Compute the min value of one of the rows in the matrix.
         * @param row [in]; row to search for max
         * @return minimum value in the matrix block
         */
        Type min_value_row (unsigned row) const
        {
            PRECOND (row >= 0);
            PRECOND (row < _rows);
            PRECOND (_data != NULL);
            Type min_value = _data[row][0];
            unsigned ii=0;
            for( ii=0; ii<_cols; ii++)
            {
                min_value = MIN(min_value, _data[row][ii]);
            }
            return min_value;
        }

        /**
         * @brief Compute the min value of one of the rows in the matrix.
         * @param row [in]; row to search for max
         * @param mincol [in]; lower col index, to search from
         * @param maxcol [in]; upper col index, to search to
         * @return minimum value in the matrix block
         */
        Type min_value_row (unsigned row, unsigned mincol, unsigned maxcol) const
        {
            PRECOND (row >= 0);
            PRECOND (row < _rows);
            PRECOND (_data != NULL);
            if(mincol > _cols) mincol = _cols;
            if(maxcol > _cols) maxcol = _cols;
            Type min_value = _data[row][mincol];
            unsigned ii=0;
            for( ii=mincol; ii<maxcol; ii++)
            {
                min_value = MIN(min_value, _data[row][ii]);
            }
            return min_value;
        }

        /**
         * @brief Compute the sum of the contents of the matrix
         * @return sum of all elements of the matrix
         */
        Type sum () const
        {
            PRECOND (_data != NULL);
            Type sum = (Type) 0;
            for (int i = 0;(unsigned) i < (_rows*_cols); i++)
            {
                sum += _data[0][i];
            }
            return sum;
        }

        /**
         * @brief Compute the sum of the contents of the matrix in a sub-block
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return sum of the block
         */
        Type sum (int row, int col, int height, int width) const
        {
            PRECOND ((unsigned)col+width  <= ncols());
            PRECOND ((unsigned)row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            Type sum = 0;
            int oi=0,oj=0;
            int lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += _data[oj][oi];
                }
            }
            return sum;
        }

        /**
         * @brief Compute the sum of the contents of the matrix
         *
         * Compute the sum of the contents of the matrix in a sub-block.  This
         * function also pads the outside of the block.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         */
        Type sum_padded (int row, int col, int height, int width) const
        {
            PRECOND (height > 0);
            PRECOND (width  > 0);
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND ((unsigned)col <= _cols);
            PRECOND ((unsigned)row <= _rows);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);

            int rstart = row - height / 2;
            int rstop  = row + height / 2+1;
            int cstart = col - width  / 2;
            int cstop  = col + width  / 2+1;

            if(height == 1)
            {
                rstart = row;
                rstop = row+1;
            }
            if(width == 1)
            {
                cstart = col;
                cstop = col+1;
            }

            //given row/col/height/width that meet precond,
            //won't get both start/stop < 0 or > row/col
            if(rstart < 0) rstart = 0;
            if(cstart < 0) cstart = 0;
            if((unsigned)rstop > _rows) rstop = _rows;
            if((unsigned)cstop > _cols) cstop = _cols;

            Type sum = 0;
            int oi=0,oj=0;

            for(oj=rstart;oj<rstop;oj++)
            {
                for(oi=cstart;oi<cstop;oi++)
                {
                    sum += _data[oj][oi];
                }
            }
            return sum;
        }

        /**
         * @brief Compute the sum of the contents of the matrix,
         *        pointwise squared
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return sum square of the specified block
         */
        double sum_square (unsigned row, unsigned col, unsigned height,
                           unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            Type sum = 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += _data[oj][oi] * _data[oj][oi];
                }
            }
            return sum;
        }

        /**
         * @brief Compute the sum of the contents of the matrix,
         *        pre-applying op pointwise
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param op  [in]; function to apply before sum
         * @return sum of matrix, pre-applying op.
         */
        double sum_pointwise_function (unsigned row, unsigned col,
                                       unsigned height, unsigned width,
                                       DfuncType op) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            Type sum = 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += op(_data[oj][oi]);
                }
            }
            return sum;
        }

        /**
         * @brief Compute the mean value of the matrix, treating the matrix as a singe list.
         * @return mean value of the matrix
         */
        double mean () const
        {
            return sum()/((double)(_rows * _cols));
/*
  PRECOND (_data != NULL);
  PRECOND (_cols > 0);
  PRECOND (_rows > 0);
  Type sum = (Type) 0;
  for (unsigned i = 0; i < (_rows*_cols); i++)
  {
  sum += _data[0][i];
  }
  return (sum/((double) (_rows*_cols)));
*/
        }

        /**
         * @brief Compute the mean value of a matrix sub-block
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return mean value of the matrix sub-block
         */
        double mean (unsigned row, unsigned col, unsigned height,
                     unsigned width) const
        {

            return sum(row, col, height, width)/((double)(height * width));

/*
  PRECOND (col+width  <= ncols());
  PRECOND (row+height <= nrows());
  PRECOND (col >= 0);
  PRECOND (row >= 0);
  PRECOND (_data != NULL);
  Type sum = 0;
  unsigned oi=0,oj=0;
  unsigned lr=row+height, lc=col+width;
  for(oj=row;oj<lr;oj++)
  {
  for(oi=col;oi<lc;oi++)
  {
  sum += _data[oj][oi];
  }
  }
  return sum/((double)(height * width));
*/
        }

        /**
         * @brief Compute the mean value of a padded sub-block of the matrix
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return mean value of a padded block of the matrix.
         */
        double mean_padded (unsigned row, unsigned col, unsigned height,
                            unsigned width) const
        {
            double sum = sum_padded(row, col, height, width);
            return sum/((double)(height * width));
        }

        /**
         * @brief Compute the mean value of the matrix
         *
         * Compute the mean value of the matrix in a sub block, padded, with
         * the area of the block passed in to avoid a frequent multiply.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param area  [in]; width * height (for speed)
         */
        double mean_padded (unsigned row, unsigned col, unsigned height,
                            unsigned width, double area) const
        {
            double sum = sum_padded(row, col, height, width);
            return sum/area;
        }

        /**
         * @brief Compute the median value of the matrix, treating the matrix as a single list.
         * @return median value of the matrix
         */
        Type  median() const
        {
            PRECOND (_data != NULL);
            PRECOND (_cols > 0);
            PRECOND (_rows > 0);
            unsigned size = _rows*_cols;
            if (size&1) // _size is odd
            {
                // Need to copy the array because kth_smallest modify it
                Type *copy = new Type[size];
                memcpy (copy, _data[0], size*sizeof(Type));
                unsigned median_pos = size/2;
                double median = kth_smallest (copy, size, median_pos);
                delete [] copy;
                // Return the middle element
                return median;
            }
            else         // _size is even
            {
                // Need to copy the array because kth_smallest modify it
                Type *copy = new Type[size];
                memcpy (copy, _data[0], size*sizeof(Type));
                unsigned median_pos = size/2 - 1;
                double median = kth_smallest (copy, size, median_pos);

                memcpy (copy, _data[0], size*sizeof(Type));
                median_pos += 1;
                median += kth_smallest (copy, size, median_pos);
                delete [] copy;
                // Return the mean of the two middle elements
                return (median/2);
            }

        }

        /**
         * @brief Compute the pct percentile value of the matrix.
         *
         * Default behavior computes the 50th percentile of the matrix
         * (median).  Any other percentile can be specified, as a value [0,1]
         *
         * @return percentile'th value of the matrix.
         */
        double percentile ( double pct = .50)
        {
            PRECOND (_data != NULL);
            PRECOND (_cols > 0);
            PRECOND (_rows > 0);
            unsigned size = _rows*_cols;
            // Need to copy the array because kth_smallest modify it
            Type *copy = new Type[size];
            memcpy (copy, _data[0], size*sizeof(Type));
            unsigned pct_pos = unsigned(size * pct);
            double percentile = kth_smallest (copy, size, pct_pos);
            delete [] copy;
            // Return the percentile-th element
            return percentile;
        }

        /**
         * @brief Computes the standard deviation of the entire matrix.  It
         * simply treats the matrix as a list of elements, and computes the
         * standard deviation.
         * @return standard deviation of the matrix
         */
        double std () const
        {
            PRECOND (_data != NULL);
            PRECOND (_cols > 0);
            PRECOND (_rows > 0);
            double data_mean = mean ();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < (_rows*_cols); i++)
            {
                sum += (_data[0][i] - (Type) data_mean) *
                    (_data[0][i] - (Type) data_mean);
            }
            return (sqrt(sum/((double) (_rows*_cols))));
        }

        /**
         * @brief Compute the standard deviation of the matrix across a block.
         * This treats the elements of the block as a list, and compute the
         * standard deviation thereof.
         *
         * This computes the standard deviation from row to row + height, and col to col+height
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return standard deviation of the block
         */
        double std (unsigned row, unsigned col, unsigned height,
                    unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;

            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sqrt(sum/((double)(height * width))));
        }

        /**
         * @brief Compute the standard deviation of the matrix across a block,
         * virtually padding with zeros.
         *
         * Height/width centered on row/col, so the std of the block from row -
         * height/2 -> row + height/2 and col - width/2 -> col + width/2
         *
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return standard deviation, calculated with a padded block
         */
        double std_padded (unsigned row, unsigned col, unsigned height,
                           unsigned width) const
        {
            PRECOND (height > 0);
            PRECOND (width  > 0);
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (col <= _cols);
            PRECOND (row <= _rows);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);


            int rstart = row - height / 2;
            int rstop  = row + height / 2 +1;
            int cstart = col - width  / 2;
            int cstop  = col + width  / 2 +1;
            if(height == 1)
            {
                rstart = row;
                rstop = row+1;
            }
            if(width == 1)
            {
                cstart = col;
                cstop = col+1;
            }

            //given row/col/height/width that meet precond,
            //won't get both start/stop < 0 or > row/col
            if(rstart < 0) rstart = 0;
            if(cstart < 0) cstart = 0;
            if(rstop > _rows) rstop = _rows;
            if(cstop > _cols) cstop = _cols;

            double data_mean = mean_padded (row, col, height, width);
            Type sum = (Type) 0;
            int oi=0,oj=0;

            for(oj=rstart;oj<rstop;oj++)
            {
                for(oi=cstart;oi<cstop;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sqrt(sum/((double)(height * width))));
        }

        /**
         * @brief Compute the standard deviation of the matrix across
         *        a block using a cached value for the mean of the data in the block.
         *
         * This routine uses data_mean as a passed in value to avoid
         * recalculating it, if one happens to have it lying around.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @return standard deviation of the block
         */
        double std (unsigned row, unsigned col, unsigned height, unsigned width,
                    double data_mean) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);

            //double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sqrt(sum/((double)(height * width))));
        }

        /**
         * @brief Compute the standard deviation of the matrix across a
         *        block using cached values of the data's mean value and
         *        the area of the data block.  Save cycles over other methods.
         *
         * Often called like:
         *  std(i, j, h, w, mean(i,j,h,w), h*w);
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @param area  [in]; height * width
         * @return standard deviation of the block
         */
        double std (unsigned row, unsigned col, unsigned height, unsigned width,
                    double data_mean, double area) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            //double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sqrt(sum/area));
        }

        /**
         * @brief Compute the standard deviation of the matrix across a block,
         * virtually padding with zeros around the outside.
         *
         * Height/width centered on row/col, so the std of the block from row -
         * height/2 -> row + height/2 and col - width/2 -> col + width/2
         *
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @param area  [in]; height * width
         * @return standard deviation of the block
         */
        double std_padded (unsigned row, unsigned col, unsigned height,
                           unsigned width, double data_mean, double area) const
        {
            PRECOND (height > 0);
            PRECOND (width  > 0);
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (col <= _cols);
            PRECOND (row <= _rows);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);

            unsigned rstart = row - height / 2;
            unsigned rstop  = row + height / 2 +1;
            unsigned cstart = col - width  / 2;
            unsigned cstop  = col + width  / 2 +1;

            if(height == 1)
            {
                rstart = row;
                rstop = row+1;
            }
            if(width == 1)
            {
                cstart = col;
                cstop = col+1;
            }

            //given row/col/height/width that meet precond,
            //won't get both start/stop < 0 or > row/col
            if(rstart < 0) rstart = 0;
            if(cstart < 0) cstart = 0;
            if(rstop > _rows) rstop = _rows;
            if(cstop > _cols) cstop = _cols;

            //double data_mean = mean_padded (row, col, height, width); //optimized away, see args.
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;

            for(oj=rstart;oj<rstop;oj++)
            {
                for(oi=cstart;oi<cstop;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sqrt(sum/area));
        }

        /**
         * @brief Compute the standard deviation of the matrix, treating the
         * matrix as a single list.
         * @return covariance of the matrix
         */
        double cov () const
        {
            PRECOND (_data != NULL);
            PRECOND (_cols > 0);
            PRECOND (_rows > 0);
            double data_mean = mean ();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < (_rows*_cols); i++)
            {
                sum += (_data[0][i] - (Type) data_mean) *
                    (_data[0][i] - (Type) data_mean);
            }
            return (sum/((double) (_rows*_cols)));
        }

        /**
         * @brief Compute the covariance of the matrix across a block
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return covariance of the matrix
         */
        double cov (unsigned row, unsigned col, unsigned height, unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sum/((double)(height * width)));
        }

        /**
         * @brief Compute the covariance of the matrix across a block
         *        using cached values.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @return covariance of the matrix
         */
        double cov (unsigned row, unsigned col, unsigned height, unsigned width,
                    double data_mean) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);

            Type sum = (Type) 0;
            Type tval = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    tval = (_data[oj][oi] - (Type) data_mean) ;
                    sum += tval * tval;
                }
            }
            return (sum/((double)(height * width)));
        }

        /**
         * @brief Compute the covariance of the matrix across a block
         *        using cached values.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @param area  [in]; height * width
         * @return covariance of the matrix
         */
        double cov (unsigned row, unsigned col, unsigned height,
                    unsigned width, double data_mean, double area) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            Type sum = (Type) 0;
            Type tval = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    tval = (_data[oj][oi] - (Type) data_mean) ;
                    sum += tval * tval;

                }
            }
            return (sum/area);
        }

        /**
         * @brief Compute the standard deviation of the matrix, treating the
         * matrix as a single list. Normalizes the cov with (N-1).
         * @return unbiased covariance of the matrix
         */
        double cov_unbiased () const
        {
            PRECOND (_data != NULL);
            PRECOND (_cols > 0);
            PRECOND (_rows > 0);
            double data_mean = mean ();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < (_rows*_cols); i++)
            {
                sum += (_data[0][i] - (Type) data_mean) *
                    (_data[0][i] - (Type) data_mean);
            }
            return (sum/((double) (_rows*_cols-1)));
        }

        /**
         * @brief Compute the covariance of the matrix across a block
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return unbiased covariance of the matrix
         */
        double cov_unbiased (unsigned row, unsigned col, unsigned height,
                             unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sum/((double)(height * width-1)));
        }

        /**
         * @brief Compute the covariance of the matrix across a block
         *        using cached values.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @return unbiased covariance of the matrix
         */
        double cov_unbiased (unsigned row, unsigned col, unsigned height,
                             unsigned width, double data_mean) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            //double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sum/((double)(height * width-1)));
        }

        /**
         * @brief Compute the standard deviation of the matrix.
         *        (normalized by N-1).  Treats the matrix as a list.
         * @return unbiased standard deviation of the matrix
         */
        double std_unbiased () const
        {
            PRECOND (_data != NULL);
            PRECOND (_cols > 0);
            PRECOND (_rows > 0);
            if (_rows*_cols == 1)
            {
                return ((Type) 0);
            }

            double data_mean = mean ();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < (_rows*_cols); i++)
            {
                sum += (_data[0][i] - (Type) data_mean) *
                    (_data[0][i] - (Type) data_mean);
            }
            return (sqrt(sum/((double) (_rows*_cols)-1.0)));
        }

        /**
         * @brief Compute the standard deviation of the matrix.
         *        (normalized by N-1). Treats the matrix as a list.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @return unbiased standard deviation of the matrix
         */
        double std_unbiased (unsigned row, unsigned col, unsigned height,
                             unsigned width) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);
            double data_mean = mean (row, col, height, width);
            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sqrt(sum/((double)(height * width)-1.0)));
        }

        /**
         * @brief Compute the standard deviation of the matrix
         *        (normalized by N-1). Treats the matrix as a list.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @return unbiased standard deviation of the matrix
         */
        double std_unbiased (unsigned row, unsigned col, unsigned height,
                             unsigned width, double data_mean) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);

            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sum/((double)(height * width)-1.0));
        }

        /**
         * @brief Compute the standard deviation of the matrix
         *        (normalized by N-1). Treats the matrix as a list.
         * @param row [in]; row coordinate to anchor block
         * @param col [in]; col coordinate to anchor block
         * @param height [in]; height of block
         * @param width  [in]; width of block
         * @param data_mean  [in]; mean of the data in the block
         * @param area  [in]; height * width
         * @return unbiased standard deviation of the matrix
         */
        double std_unbiased (unsigned row, unsigned col, unsigned height,
                             unsigned width, double data_mean, double area) const
        {
            PRECOND (col+width  <= ncols());
            PRECOND (row+height <= nrows());
            PRECOND (col >= 0);
            PRECOND (row >= 0);
            PRECOND (_data != NULL);
            PRECOND (_data[0] != NULL);

            Type sum = (Type) 0;
            unsigned oi=0,oj=0;
            unsigned lr=row+height, lc=col+width;
            for(oj=row;oj<lr;oj++)
            {
                for(oi=col;oi<lc;oi++)
                {
                    sum += (_data[oj][oi] - (Type) data_mean) *
                        (_data[oj][oi] - (Type) data_mean);
                }
            }
            return (sum/(area - 1.0));
        }

    public: // Output

        /**
         * @brief prints the matrix on std::cout
         */
        void print() const
        {
            print(std::cerr, 0, 0, _rows, _cols);
        }

        /**
         * @brief Print a the contents of a Matrix to the specified stream
         * @param out [in]; stream to write to
         * @param mat [in]; matrix to write
         * @return reference to the ostream passed in
         */
        friend std::ostream&
        operator<< (std::ostream& out, const Evolution::Matrix<Type> & mat)
        {
            mat.print(out, 0, 0, mat.rsize(), mat.csize());
            return out;
        }

        /**
         * @brief prints the matrix on std::cout
         */
        void print_matrix() const
        {
            print(std::cout, 0, 0, _rows, _cols);
        }

        /**
         * @brief print out a matrix sub-block
         * @param out [in]; the stream to write to
         * @param r [in]; row coordinate to anchor block
         * @param c [in]; col coordinate to anchor block
         * @param h [in]; height of block
         * @param w [in]; width of block
         */
        void print(std::ostream& out, unsigned r, unsigned c, unsigned h,
                   unsigned w) const
        {
            PRECOND(r+h <= _rows);
            PRECOND(c+w <= _cols);

            char oss_rep[80];
            out << "{ ";
            out << "size: " << rsize() << "(rows)x" << csize() << "(cols)\n";

            sprintf(oss_rep, "%p", (void*)_data);
            out << "_data \t" << oss_rep << std::endl;
            if ( rsize() > 0 && csize() > 0 )
            {
                sprintf(oss_rep, "%p", (void*)_data[0]);
                out << "_data[0] \t"<< oss_rep << std::endl;
            }

            //print matrix data block
            out << "matrix: (\n";
            if ( rsize() > 0 && csize() > 0 )
            {
                unsigned lr=r+h, lc=c+w;
                for (unsigned i = r; i < lr; i++)
                {
                    out << "\t";
                    for (unsigned j = c; j < lc-1; j++)
                    {
                        out << (*this)[i][j] << ", ";
                    }
                    out << (*this)[i][lc-1] << "\n";
                }
            }
            else
            {
                out << " Empty matrix, zero sizes.\n";
            }

            out << "\t)\n}";
        }

    protected: // Helpers

        /**
         * @brief Initializes the matrix to have size: rows x cols
         * @param rows [in]; number of rows in the new matrix
         * @param cols [in]; number of cols in the new matrix
         */
        Matrix & init (unsigned rows, unsigned cols)
        {
            // check is already have proper size
            if((rows == _rows) && (cols == _cols) && (_data != NULL))
            {
                return *this;
            }
            PRECOND (rows > 0);
            PRECOND (cols > 0);

            _data = new Type*[rows];
            if (_data == NULL)
            {
                _rows = _cols = 0; //failed alloc
            }
            else
            {
                for (unsigned i = 1; i < rows; i++)
                {
                    _data[i] = NULL;
                }
                _data[0] = new Type[rows*cols];
                if (_data[0] == NULL)
                {
                    delete [] _data;
                    _rows = _cols = 0;
                    _data = NULL;
                }
                else
                {
                    for (unsigned i = 1; i < rows; i++)
                    {
                        _data[i] = _data[i-1] + cols;
                    }
                }
            }
            _rows = rows;
            _cols = cols;

            return *this;
        }

    protected: // Implementation

        unsigned     int _rows;    ///< Number of rows in the matrix
        unsigned     int _cols;    ///< Number of columns in the matrix
        Type         **_data;  ///< Data
    }; // end class Matrix

    // External operators
    /**
     * @brief Compute the SVD of a matrix
     * @param A [in]; matrix to decompose
     * @param U [out]; U of SVD
     * @param S [out]; S of SVD
     * @param V [out]; V of SVD
     * @return Result indicating success or failure.
     */
    EVOLUTION_EXPORT_MATH Result svd (const Matrix<double>& A,
                                      Matrix<double>* U,
                                      Vector<double>* S,
                                      Matrix<double>* V);

    /**
     * @brief Compute the LU decomposition of a matrix
     * @param A [in]; matrix to decompose
     * @param LU [out]; LU decomposition of A
     * @param index [out]; LU decomposition indicies (map LU to pre-rearrangement state)
     * @param permutation [out]; permutation
     * @return Result indicating success or failure.
     */
    EVOLUTION_EXPORT_MATH Result lu (const Matrix<double>& A,
                                     Matrix<double>* LU,
                                     Vector<int>* index,
                                     int* permutation);

    /**
     * @brief Computes the inverse of a matrix.
     * @param A [in]; matrix to invert
     * @param inv [out]; output inverse
     * @return Result indicating success or failure.
     */
    EVOLUTION_EXPORT_MATH Result invert (const Matrix<double>& A,
                                         Matrix<double>* inv);

    /**
     * @brief Computes the pseudo-inverse of a matrix.
     * @param A [in]; matrix to pseudo-invert
     * @param pinv [out]; output pseudo-inverse
     * @return Result indicating success or failure.
     */
    EVOLUTION_EXPORT_MATH Result pinvert (const Matrix<double>& A,
                                          Matrix<double>* pinv);

    /**
     * @brief Computes the pseudo-inverse of a matrix (gives out the results
     * of the SVD used for computing the pseudo-inverse).
     * @param A [in]; matrix to decompose
     * @param U [out]; U of SVD
     * @param S [out]; S of SVD
     * @param V [out]; V of SVD
     * @param pinv [out]; output pseudo-inverse
     * @return Result indicating success or failure.
     */
    EVOLUTION_EXPORT_MATH Result pinvert (const Matrix<double>& A,
                                          const Matrix<double>& U,
                                          const Vector<double>& S,
                                          const Matrix<double>& V,
                                          Matrix<double>* pinv);

    /**
     * @brief Computes the pseudo-inverse of a matrix (gives out the results
     * of the SVD used for computing the pseudo-inverse).
     * @param A [in]; left hand multiplicand
     * @param B [in]; right hand multiplier
     * @param C [out]; output matrix multiply result
     * @return Result indicating success or failure.
     */
    EVOLUTION_EXPORT_MATH Result matrix_multiply (const Matrix<double>& A,
                                                  const Matrix<double>& B,
                                                  Matrix<double>* C);

} // end namespace Evolution

#ifndef NDEBUG

/**
 * @brief Prints the matrix when running the gdb debugger.
 *
 * @param m [in]; Matrix to print.
 */
EVOLUTION_EXPORT_MATH void gdb_print_matrix(const Evolution::Matrix<double>& m);

#endif

#endif  // INCLUDE_EVOLUTION_MATH_MATRIX_HPP
