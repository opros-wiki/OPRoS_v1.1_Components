//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_CLASS_MIXTURE_CACHE_HPP
#define INCLUDE_EVOLUTION_MATH_CLASS_MIXTURE_CACHE_HPP


/**
 * @file    evolution/core/math/ClassMixtureCache.hpp
 * @brief   A compact representation of a very computationally intensive, 
 *          multi-dimensional Gaussian model
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ClassMixtureCache
 */


#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/util/CompactCacheTable.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/ClassMixture.hpp>


namespace Evolution
{

    /**
     * @class ClassMixtureCache
     * @brief A compact representation of a very computationally intensive, 
     * multi-dimensional Gaussian model
     *
     * Each element of the table is a 32-bit integer. The compactness
     * property of the table is achieved by storing only a single bit
     * of information for each class in the mixture. The corresponding
     * bit of the element of the table will be turned on/off depending
     * on whether the likelihood for the corresponding class is
     * larger/smaller than the threshold. Given that each element of the
     * table is a 32-bit integer, then the ClassMixtureCache table
     * could be only used for a mixture of up-to 32 classes.
     *
     * @see CompactCacheTable
     *
     * @ingroup core_math
     *
     **/
    class EVOLUTION_EXPORT_MATH ClassMixtureCache : public CompactCacheTable
    {    
    public: // Structors

        /**
         * @brief Constructs a blank table.
         *        
         */
        ClassMixtureCache ()
            : CompactCacheTable()
        {
            _threshold = 0;
            _class_dim = 0;
            _valid_contents = false; 
        }

        /**
         * @brief Constructs the cache table for the corresponding
         * ClassMixture.
         *        
         *
         * @param gmm [in]; ClassMixture for which likelihoods will be stored in the table.
         * @param minx [in]; Minimum x value of cache
         * @param miny [in]; Minimum y value of cache
         * @param maxx [in]; Maximum x value of cache
         * @param maxy [in]; Maximum y value of cache
         * @param threshold [in]; Likelihood threshold
         */
        ClassMixtureCache (ClassMixture& gmm, int minx, int miny, 
                           int maxx, int maxy, double threshold);


    public:  // Setting

        /**
         * @brief Rebuilds the cache table reflecting the new parameters.
         *        
         * @param gmm [in]; ClassMixture for which likelihoods will be stored in the table.
         * @param minx [in]; Minimum x value of cache
         * @param miny [in]; Minimum y value of cache
         * @param maxx [in]; Maximum x value of cache
         * @param maxy [in]; Maximum y value of cache
         * @param threshold [in]; Likelihood threshold
         * @return Boolean output: true on success, false on failure
         */
        bool set_cache (ClassMixture& gmm, int minx, int miny, int maxx, 
                        int maxy, double threshold);

    public: // Queries

        /**
         * @brief Get the value of the table at location (x,y) for class z.
         *
         * @param x [in]; X coordinate of the element
         * @param y [in]; Y coordinate of the element
         * @param z [in]; Class component index
         * @return Value of the z-bit of the (x,y) element of the table
         *
         * @see CompactCacheTable::get_val().
         */
        inline bool log_likelihood(int x, int y, int z)
        {
            return get_val(x, y, z);
        }

        /**
         * @brief Get the threshold value for the cache.
         *
         * @return The value of the threshold
         */
        inline double get_threshold()
        {
            return _threshold;
        };

        /**
         * @brief Get the dimension of the cache.
         *
         * @return The dimension of the cached ClassMixture
         */
        inline int get_class_dim()
        {
            return _class_dim;
        };

    public: // File loading

        /** 
         * @brief Loads a ClassMixtureCache model from a file.
         * @param filename [in]; File name from which to read.
         * @result RESULT_SUCCESS if success, result code describing failure otherwise.
         */
        Result read_class_mixture_cache (const char * filename);

        /** 
         * @brief Writes a ClassMixtureCache model to a file.
         * @param filename [in]; File name to which to save.
         * @result RESULT_SUCCESS if success, result code describing failure otherwise.
         */
        Result write_class_mixture_cache (const char * filename); 

    protected: // Implementation

        double _threshold;      ///< Cache threshold value
        bool   _valid_contents; ///< Whether the cache contents are valid
        int    _class_dim;      ///< Number of classes in the ClassMixture object to be cached
    }; // end class ClassMixtureCache

} // end namespace Evolution

/**
 * @brief Prints the contents of the cache table to the specified stream.
 *
 * @param out [in]; Stream to which to print the contents table.
 * @param c [in]; ClassMixtureCache whose contents will be printed to the stream.
 *
 * @return Stream reference given in the out parameter.
 */
inline std::ostream& operator<< (std::ostream& out, const Evolution::ClassMixtureCache & c) { return out; } ;

#endif  // INCLUDE_EVOLUTION_MATH_CLASS_MIXTURE_CACHE_HPP
