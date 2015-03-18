//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_MATH_COMPONENTS_HPP
#define INCLUDE_EVOLUTION_CORE_MATH_COMPONENTS_HPP


/**
 * @file    evolution/core/math/Clustering.hpp
 * @brief   Clustering infrastructure.
 * 
 * This file contains an implementation of a depth-first connect components algorithm for segmenting matricies.
 * 
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::RowSlice
 * @see Evolution::RowSliceArray
 * @see Evolution::Clustering
 */

#include <float.h>
#include <vector>
#include <math.h>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Matrix2.hpp>
#include <evolution/core/math/Matrix.hpp>

namespace Evolution
{

    class Cluster;

    /**
     * @class RowSlice
     * @brief RowSlice class
     *
     * Used to turn a row of a matrix into a smaller unit of data.
     * RowSlice slice;
     * slice.get_start() is the first over-threshold index of a continuous over-threshold set of data in the row.
     * slice.get_stop () is the index after the last over-threshold index.  
     * slice.get_y    () is the y index of the 'slice' in the matrix
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH RowSlice
    {
    public: // Structors

        /**
         * @brief basic constructor
         **/
        RowSlice() 
        {
            _y = -1;
            _start = -1;
            _stop = -1;
            _flag = 0;
        }

        /**
         * @brief Parameterized constructor
         * @param y [in]; y value 
         * @param start [in]; start of RowSlice
         * @param stop [in]; stop of RowSlice
         **/
        RowSlice(int y, int start, int stop) 
        {
            _y = y;
            _start = start;
            _stop = stop;
        }

        /**
         * @brief equal-p NEED INFO
         * @param other [in]; RowSlice to compare to
         * @return true if equal, false otherwise
         **/
        bool operator== (const RowSlice& other) const 
        {
            if(_y == other._y && 
               _start == other._start && 
               _stop == other._stop)
            {       
                return true;
            }
            return false;
        }

        /**
         * @brief not-equal-p NEED INFO
         * @param other [in]; RowSlice to compare to
         * @return true if not equal, false otherwise
         **/
        bool operator!= (const RowSlice& other) const 
        {
            return (!(*this == other));
        }

        /**
         * @brief Greater-than-p. Defined as having start > o.stop.  
         *        otherwise '=='
         * @param o [in]; RowSlice to compare to
         * @return true if this > o, false otherwise
         **/
        bool operator >  (const RowSlice& o)  const
        {
            if(_start > o._stop)
            {       
                return true;
            }
            return false;
        }

        /**
         * @brief Less-than-p. 
         *
         * Less-than-p. 
         * Greater than and less than can leave a large possible hole
         * (min(start,o.start)->max(stop,o.stop) that is 'equal'.  Of course,
         * operator== doesn't test for that.  
         * @param o [in]; RowSlice to compare to
         * @return true if this < o, false otherwise
         **/
        bool operator < (const RowSlice& o) const 
        {
            if(_stop < o._start)
            {       
                return true;
            }
            return false;
        }

        /**
         * @brief Test for adjacency.
         * Tests for adjacency of two RowSlices. 
         *  y1-y0 == 0 means same row, no adjacency.  
         *  |y1-y0| > 1 means not on adjacent lines.  
         *  (are these two slices strictly adjacent in
         *  matrixspace)
         * @param o [in]; RowSlice to compare to
         * @return true if this is adjacent to o, false otherwise
         **/
        bool adjacent(const RowSlice& o) const
        {
            if(abs(_y-o._y) != 1) //Not on adjacent rows.
            {
                return false; 
            }
            if(_stop <= o._stop && _stop >  o._start) 
            { //_stop is contained in the other.
                return true;
            }
            if(_start < o._stop && _start >= o._start)
            { //_start is contained in the other.
                return true; 
            }
            if(_start <= o._start && _stop >= o._stop) 
            { //_this envelops the other.
                return true;  
            }
            return false; //Covers all cases.
        }

        /**
         * @brief Sorting comparator. Makes greater than ordinal compare.
         * @param o [in]; RowSlice to compare to
         * @return true if this sorts greater than o
         **/
        bool sort_greater(const RowSlice& o) 
        {
            if(_y > o._y || 
               (_y == o._y && _start > o._start) || 
               (_y == o._y && _start == o._start && _stop > o._stop))
            {       
                return true;       
            }
            return false;
        }

        /**
         * @brief Sorting comparator. Makes less than ordinal compare.
         * @param o [in]; RowSlice to compare to
         * @return true if this sorts less than o
         **/
        bool sort_less(const RowSlice& o) 
        {
            if(_y < o._y || 
               (_y == o._y && _start < o._start) || 
               (_y == o._y && _start == o._start && _stop < o._stop))
            {       
                return true;       
            }
            return false;
        }

        /**
         * @brief Determines whether a point is contained in the row slice.
         * @param xpoint [in]; x-coordinate of the point.
         * @return boolean value indicating whether the point is contained 
         * in the row slice.
         */
        bool xpoint_contained (const int xpoint)
        {
            if( xpoint < _stop && xpoint >= _start)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /**
         * @brief Determines whether a point is contained in the row slice.
         * @param xpoint [in]; x-coordinate of the point.
         * @param ypoint [in]; x-coordinate of the point.
         * @return boolean value indicating whether the point is contained 
         * in the row slice.
         */
        bool points_contained (const int xpoint, const int ypoint)
        {
            if (ypoint != _y) return false;
            if( xpoint < _stop && xpoint >= _start)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    public: // Setters
        /**
         * @brief Sets the y offset.
         * @param y [in]; value to set as y offset.
         */
        void set_y (int y)
        {
            _y = y;
        }

        /**
         * @brief Sets the start offset.
         * @param start [in]; value to set as start offset.
         */
        void set_start (int start)
        {
            _start = start;
        }

        /**
         * @brief Sets the stop offset.
         * @param stop [in]; value to set as stop offset.
         */
        void set_stop (int stop)
        {
            _stop = stop;
        }

    public: // Accessors

        /**
         * @brief Gets the offset in the matrix of the region.
         * @return y value of this RowSlice
         */
        int& get_y ()
        {
            return _y;
        }

        /**
         * @brief Gets the offset in the matrix of the region.
         * @return y value of this RowSlice
         */
        const int& get_y () const 
        {
            return _y;
        }

        /**
         * @brief Gets the start of row slice.
         * @return start value of this RowSlice
         */
        int& get_start ()
        {
            return _start;
        }

        /**
         * @brief Gets the start of row slice.
         * @return start value of this RowSlice
         */
        const int& get_start () const 
        {
            return _start;
        }

        /**
         * @brief Gets the end of row slice.
         * @return stop value of this RowSlice
         */
        int& get_stop ()
        {
            return _stop;
        }

        /**
         * @brief Gets the end of row slice.
         * @return stop value of this RowSlice
         */
        const int& get_stop () const 
        {
            return _stop;
        }

    public: //flags

        /**
         * @brief Gets the value of the flag on this RowSlice.
         * @return flag value of this RowSlice
         */
        unsigned long get_flag() { return _flag; }

        /**
         * @brief Sets the clustering flag.
         * @param flag [in]; Value to set as clustering flag.
         */
        void set_flag(unsigned long flag ) { _flag = flag; }

        // Friend declarations
        friend class Cluster; ///<Friend declaration.
        friend class Clustering; ///<Friend declaration.

    protected:  // Implementation

        unsigned long _flag; ///< clustering flag.
        int _y;     ///< Offset in the matrix of the region.
        int _start; ///< Start of row slice.
        int _stop;  ///< End of row slice.
    }; // end class RowSlice

    typedef std::vector<RowSlice> RawCluster;  ///< RawCluster is a set of RowSlices
    typedef std::vector<RawCluster> RawClusterSet; ///< RawClusterSet is a std::vector of RawClusters
    typedef std::vector<int>      XVector; ///< Convience typedef for a vector of x values.
    typedef std::vector<int>      YVector; ///< Convience typedef for a vector of y values.
    typedef std::vector<Cluster>  ClusterSet; ///< A std::vector of Clusters



    /**
     * @class RowSliceArray
     * @brief Array of RowSlices
     *
     * Just an array of RowSlices, similar to every other Array type class written, ever.  
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH RowSliceArray
    {
        public:
        ///Constructor
        RowSliceArray() : _slice_count(0), _size(0), _slices(NULL) {}
        
        ///Destructor
        ~RowSliceArray() { delete[] _slices; }

        public:
        unsigned _slice_count; ///< Number of slices in this array
        unsigned _size; ///< number of slots allocated in this RowSliceArray
        RowSlice * _slices; ///< the array containing the slices.

        public:
        /**
         * @brief Get the element at index.
         * @param index [in]; index to find the RowSlice.
         * @return the RowSlice at index
         */
        RowSlice& at(unsigned int index)
        {
            return _slices[index];
        }

        /**
         * @brief Resizes the Array.
         *
         * Resizes the Array.  Old data is copied into the new array to the
         * limits of the new size.  New data in an array larger than the old
         * one is initialized to 0.
         * @param size [in]; new size for the array.
         * @return Result encoding success or failure.
         */
        Result resize ( unsigned size )
        {
            if( size < _size ) return RESULT_PRECONDITION_ERROR;
            if( size == _size ) return RESULT_SUCCESS;
            RowSlice * tmp=NULL;
            unsigned ii=0;
            if( _slices == NULL)
            {
                _slices = new RowSlice[size];
                if(_slices == NULL) return RESULT_OUT_OF_MEMORY;
            }
            else
            {
                tmp = _slices;
                _slices = new RowSlice[size];
                if(_slices == NULL) return RESULT_OUT_OF_MEMORY;
                for( ii=0; ii<size; ii++)
                {
                    _slices[ii] = tmp[ii];
                } 
                delete[] tmp;
            }
            return RESULT_SUCCESS;
        }

        /**
         * @brief Clear out the array.
         * @return Result encoding success or failure.
         */
        Result clear ( )
        {
            delete [] _slices;
            _slice_count = 0;
            _slices = NULL;
            _size = 0;
            return RESULT_SUCCESS;
        }
    };

    /**
     * @class Cluster
     * @brief Representation of a set of adjacent points in a 2dimensional lattice.
     *
     * A cluster represents a set of contiguous non-zero values in a 2-D
     * matrix.  Clusters have a center of mass, a covariance matrix, a
     * rectangular bounding box, a size (number of contained points), and a
     * list of which x and y coordinates are contained in the cluster. 
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH Cluster
    {
    public: // Structors.
        /**
         * @brief Clean cluster
         **/
        Cluster()
        {
            _xcm = 0; _ycm = 0; _size =0 ;
            _xvec.clear(); _yvec.clear();
            _cov.zeros();
        }
        
        /**
         * @brief Copy constructor
         * @param o [in]; Cluster to copy.
         */
        Cluster(const Cluster& o)
        {
            _size = o._size;
            _xcm = o._xcm; 
            _ycm = o._ycm; 
            _xvec = o._xvec;
            _yvec = o._yvec;
            _cov = o._cov; 
            _xl = o._xl;
            _xh = o._xh;
            _yl = o._yl;
            _yh = o._yh;
        }

        /**
         * @brief Construct a cluster from a raw_cluster.
         * @param raw_cluster [in]; RawCluster to get raw data from.
         */
        Cluster(RawCluster& raw_cluster)
        {
            //set (raw_cluster);
        }

        /**
         * @brief Set a cluster from a raw_cluster.
         * @param raw_cluster [in]; RawCluster to get raw data from.
         */
        void set(RawCluster& raw_cluster)
        {
            // Initialization
            _xvec.clear(); 
            _yvec.clear();
            _cov.zeros();
            //_xcm = 0;     ///< x center of mass
            //_ycm = 0;     ///< y center of mass
            //_xl = INT_MAX; ///< lower x of bbox
            //_yl = INT_MAX; ///< lower y of bbox
            //_xh = 0;      ///< upper x of bbox
            //_yh = 0;      ///< upper y of bbox
            //_size = 0; ///< holds the size of the cluster.


            int j=0;
            unsigned int ui=0;
            int xl=0, yl=0, xh=0, yh=0;
            double xcm=0, ycm=0;
            xl = raw_cluster.at(ui)._start;
            xh = raw_cluster.at(ui)._stop;
            yl = yh = raw_cluster.at(ui)._y;
            for(ui = 0; ui < raw_cluster.size(); ui++)
            {
                // Initialization of the bounding box corner coordinates.
                //if (ui == 0)
                //{
                    //xl = raw_cluster[ui]._start;
                    //xh = raw_cluster[ui]._stop;
                    //yl = yh = raw_cluster[ui]._y;
                //}
                //else 
                //{
                    //Check x sets of the bounding box.
                    if(raw_cluster[ui]._start < xl) 
                    {
                        xl = raw_cluster[ui]._start;
                    }
                    if(raw_cluster[ui]._stop  > xh) 
                    {
                        xh = raw_cluster[ui]._stop;
                    }

                    //Check y sets of the bounding box.
                    if(raw_cluster[ui]._y < yl) 
                    {
                        yl = raw_cluster[ui]._y;
                    }
                    if(raw_cluster[ui]._y > yh) 
                    {
                        yh = raw_cluster[ui]._y;
                    }
                //}

                for(j = raw_cluster[ui]._start; j < raw_cluster[ui]._stop; j++)
                { 
                    _xvec.push_back(j);
                    _yvec.push_back(raw_cluster[ui]._y);
                    xcm += j; //get the sum of x
                    ycm += raw_cluster[ui]._y; //get the sum of y
                    
                } 
            }
            //I'm getting more nervous about the parallel data structures. NEED INFO
            _xcm = (int)xcm / _xvec.size(); 
            _ycm = (int)ycm / _yvec.size();
            double sx2  = 0;
            double sy2  = 0;
            double sxy2 = 0;
            for(ui = 0; ui < _xvec.size(); ui++)
            {
                sx2  += (_xvec[ui] - _xcm) * (_xvec[ui] - _xcm) ;
                sy2  += (_yvec[ui] - _ycm) * (_yvec[ui] - _ycm) ;
                sxy2 += (_xvec[ui] - _xcm) * (_yvec[ui] - _ycm) ;
            }
            //xvec.size == yvec.size() is invariant.
            sx2  =   sx2  / _xvec.size () ;
            sy2  =   sy2  / _xvec.size () ;
            sxy2 =   sxy2 / _xvec.size () ;
            _cov[0][0] = sx2;
            _cov[1][0] = sxy2;
            _cov[0][1] = sxy2;
            _cov[1][1] = sy2;
            _xl = xl;
            _yl = yl;
            _xh = xh;
            _yh = yh;
            _size = _xvec.size();
        }
        
        /**
         * @brief Clear the cluster contents.
         */
        void clear()
        {
            _xcm = 0; 
            _ycm = 0;  
            _size = 0;
            _xvec.clear(); 
            _yvec.clear();
            _cov.zeros();
        }

        /**
         * @brief Assign one cluster to another cluster.
         * @param o [in]; Cluster to copy.
         * @return reference to *this
         */
        Cluster& operator=(const Cluster& o)
        {
            _size = o._size;
            _xcm = o._xcm; 
            _ycm = o._ycm; 
            _xvec = o._xvec;
            _yvec = o._yvec;
            _cov = o._cov; 
            _xl = o._xl;
            _xh = o._xh;
            _yl = o._yl;
            _yh = o._yh;
            return *this;
        }

        ///Logging facilities
        ERSP_LOG_CLASS_CATEGORY_DECLARE(Cluster, "Evolution.Core.Math.Cluster");

    public: // Accessors
        /**
         * @brief Gets the x center of mass.
         * @return x center of mass
         */
        int& get_xcm ()
        {
            return _xcm;
        }

        /**
         * @brief Gets the x center of mass.
         * @return x center of mass
         */
        const int& get_xcm () const 
        {
            return _xcm;
        }

        /**
         * @brief Gets the y center of mass.
         * @return y center of mass
         */
        int& get_ycm ()
        {
            return _ycm;
        }

        /**
         * @brief Gets the y center of mass.
         * @return y center of mass
         */
        const int& get_ycm () const 
        {
            return _ycm;
        }

        /**
         * @brief Gets the lower x of bbox.
         * @return lower x value of the bounding box
         */
        unsigned int& get_xl ()
        {
            return _xl;
        }

        /**
         * @brief Gets the lower x of bbox.
         * @return lower x value of the bounding box
         */
        const unsigned int& get_xl () const 
        {
            return _xl;
        }

        /**
         * @brief Gets the lower y of bbox.
         * @return lower y value of the bounding box
         */
        unsigned int& get_yl ()
        {
            return _yl;
        }

        /**
         * @brief Gets the lower y of bbox.
         * @return lower y value of the bounding box
         */
        const unsigned int& get_yl () const 
        {
            return _yl;
        }

        /**
         * @brief Gets the upper x of bbox.
         * @return upper x value of the bounding box
         */
        unsigned int& get_xh ()
        {
            return _xh;
        }

        /**
         * @brief Gets the upper x of bbox.
         * @return upper x value of the bounding box
         */
        const unsigned int& get_xh () const 
        {
            return _xh;
        }

        /**
         * @brief Gets the upper y of bbox.
         * @return upper y value of the bounding box
         */
        unsigned int& get_yh ()
        {
            return _yh;
        }

        /**
         * @brief Gets the upper y of bbox.
         * @return upper y value of the bounding box
         */
        const unsigned int& get_yh () const 
        {
            return _yh;
        }

        /**
         * @brief Gets the covariance matrix.
         * @return 2x2 covariance matrix of the cluster member points
         */
        Matrix2& get_cov ()
        {
            return _cov;
        }

        /**
         * @brief Gets the covariance matrix.
         * @return 2x2 covariance matrix of the cluster member points
         */
        const Matrix2& get_cov () const 
        {
            return _cov;
        }

        /**
         * @brief Gets the x values of the cluster.
         * @return reference to the vector of x values of the cluster
         */
        XVector& get_xvec ()
        {
            return _xvec;
        }

        /**
         * @brief Gets the x values of the cluster.
         * @return reference to the vector of x values of the cluster
         */
        const XVector& get_xvec () const
        {
            return _xvec;
        }

        /**
         * @brief Gets the y values of the cluster.
         * @return reference to the vector of y values of the cluster
         */
        YVector& get_yvec ()
        {
            return _yvec;
        }

        /**
         * @brief Gets the y values of the cluster.
         * @return reference to the vector of y values of the cluster
         */
        const YVector& get_yvec () const
        {
            return _yvec;
        }

        /**
         * @brief Gets the size of the cluster.
         * @return the size of the cluster
         */
        int& size () 
        {
            return _size;
        }

        /**
         * @brief Gets the size of the cluster.
         * @return the size of the cluster
         */
        const int& size () const 
        {
            return _size;
        }

    protected: // Implementation
        int                  _xcm;   ///< x center of mass
        int                  _ycm;   ///< y center of mass
        unsigned int         _xl;    ///< lower x of bbox
        unsigned int         _yl;    ///< lower y of bbox
        unsigned int         _xh;    ///< upper x of bbox
        unsigned int         _yh;    ///< upper y of bbox
        Matrix2              _cov;   ///< covariance matrix
        XVector              _xvec;  ///< holds the x values of the cluster
        YVector              _yvec;  ///< holds the y values of the cluster
        int                  _size; ///< holds the size of the cluster.
    };  // end Cluster class

    /**
     * @class Clustering
     * @brief Object containing a set of clusters.
     *
     * This object is used to get a set of connected components (clusters) from a matrix.
     * Matrix<double> mat; //fill with some binary data.
     * Clustering clusters;
     * clusters.generate_components(mat);
     * 
     * To access the raw clusters, use get_raw_cluster_set() (a typedefed std::vector<RowSlice>) and access the vector.
     * To access the processed clustesr, use get_cluster_set() (a typedefed std::vector<Cluster>) and access the vector. 
     * 
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH Clustering : public ObjectImpl
    {
        public: //IObject interface.

            /**
             * @see Evolution::IObject::obtain_interface
             */
            virtual Result obtain_interface (TicketId ticket, 
                                             const char* interface_id,
                                             void** interface_ptr);

        public: 

        /**
         * @brief Gets the raw cluster set.
         * @return set of raw clusters
         */
        RawClusterSet& get_raw_cluster_set()
        {
            return _raw_clusters;
        }

        /**
         * @brief Gets the raw cluster set.
         * 
         * @return set of raw clusters
         */
        const RawClusterSet& get_raw_cluster_set() const
        {
            return _raw_clusters;
        }

        /**
         * @brief Gets the cluster set.
         * @return the set of clusters 
         */
        ClusterSet& get_cluster_set()
        {
            return _clusters;
        }

        /**
         * @brief Gets the cluster set.
         * @return the set of clusters 
         */
        const ClusterSet& get_cluster_set() const
        {
            return _clusters;
        }

        /**
         * @brief Gets the width. 
         * @return width of the original data 
         */
        int get_width() const
        { 
            return _width;
        } 

        /**
         * @brief Gets the height.
         * @return gets the height of the original data
         */
        int get_height() const
        { 
            return _height;
        } 

        /**
         * @brief Clears internal data.
         */
        void clear()
        {
            _raw_clusters.clear();
            _clusters.clear();
            _width=0;
            _height=0;
        }

        public: 
        static const char * const INTERFACE_ID ; ///< Interface Id, used for obtain_interface calls

        public: 
        /**
         * This function takes a matrix and a RowSliceArray pointer (just a
         * pointer to 1 of them), and processes the specified row of the matrix
         * into RowSlices.  The thr value is used to turn arbitrary Matrix<double> objects into binary matricies.
         * Usually the RowSliceArray object slices needs to be resized to matrix.csize()/2 before being passed in.
         * @param matrix [in]; matrix to slice up
         * @param row [in]; row to slice up
         * @param thr [in]; threshold for the slicing (we do so love the slicing!)
         * @param slices [out]; the slices found (pass in a single object, sized to fit the largest slice count)
         * @param slices_found [out]; number of slices found (after completion)
         * @return Result encoding success or failure.
         */
        static Result slice_matrix_row ( const Matrix<double>& matrix, unsigned row, double thr, RowSliceArray * slices, unsigned * slices_found);

        /**
         * This function repeatedly calls slice_matrix_row to fill an array of RowSliceArrays, from lowest y value to highest y value, with
         * RowSliceArrays representing that row.
         * @param matrix [in]; matrix to slice up
         * @param thr [in]; threshold for the slicing (we do so love the slicing!)
         * @param slices [out]; the slices found. (pass in NULL object, allocation done internally)
         * @param num_slices [out]; number of rows slices (just matrix.rsize())
         * @return Result encoding success or failure.
         */
        static Result slice_up_entire_matrix ( const Matrix<double>& matrix, double thr, RowSliceArray ** slices, unsigned *num_slices);

        /**
         * This function takes an array of RowSliceArrays, and an index into
         * them.  It finds the RowSlice at that index, and does a depth first
         * search to find the cluster associated with that RowSlice.  The
         * result is placed in the output cluster, with all flags in RowSlices
         * in the cluster set to flag.
         * 
         * Note that the RowSliceArray array is assumed to be in a least to most ordering based on the get_y() of the contained RowSlices.
         * @param row [in]; row to find the starting RowSlice
         * @param index [in]; index into row (column value)
         * @param slices [in]; slices to look through to find the connected component
         * @param num_slices [in]; number of slices in slices (array size parameter)
         * @param flag [in]; flag to mark all RowSlices in the connected component with
         * @param cluster [out]; The found RawCluster.
         * @return Result encoding success or failure.
         */
        static Result find_component ( int row, int index, const RowSliceArray * slices, const unsigned num_slices, 
                                        const unsigned long flag, RawCluster * cluster);

        public:

        /**
         * @brief Fill this clustering object from mat.
         *
         * Calling this function from a Clustering object will apply
         * 'threshold' to matrix as a threshold, slice up the entire matrix,
         * and repeatedly depth-first search until all connected componenets
         * are found and stored.
         *
         * @param mat [in]; Matrix<double> object to use to find components.
         * @param threshold [in]; value to use to threshold matrix into a binary matrix.
         * @return Result indicating success or failure.
         */
        Result generate_components(const Matrix<double> & mat, double threshold=0.5);

        /**
         *  @brief Fill this clustering object from mat.
         *
         * Calling this function from a Clustering object will apply
         * 'threshold' to matrix as a threshold, slice up the entire matrix,
         * and repeatedly depth-first search until all connected componenets
         * are found and stored.  it does the same thing as
         * generate_components, it simply has a different name.
         *
         * @param mat [in]; Matrix<double> object to use to find components.
         * @param threshold [in]; value to use to threshold matrix into a binary matrix.
         * @return Result indicating success or failure.
         */
        Result get_clusters_from_matrix(const Matrix<double> & mat, double threshold=0.5)
        {
            return generate_components(mat);
        }

        ///Logging facilities.
        ERSP_LOG_CLASS_CATEGORY_DECLARE(Clustering, "Evolution.Core.Math.Clustering");

        protected:
        RawClusterSet    _raw_clusters;      ///< Raw clusters.  Only stored in algorithm that computes clusters.
        ClusterSet       _clusters;          ///< Set of clusters.
        int                 _width;             ///< The width of the original data. 
        int                 _height;            ///< The height of the original data. 

    };

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_MATH_COMPONENTS_HPP
