//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_GAUSSIAN_MIXTURE_HPP
#define INCLUDE_EVOLUTION_MATH_GAUSSIAN_MIXTURE_HPP


/**
 * @file    evolution/core/math/GaussianMixture.hpp
 * @brief   This class contains a mixture of arbitrary dimension 
 *          Gaussian functions. 
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::GaussianMixture
 */


#include <vector>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Gaussian.hpp>


namespace Evolution
{

    // Forward declaration
    class Configuration;

    /**
     * @class GaussianMixture
     *
     * @brief A mixture of Gaussian functions class
     *          
     * This class provides functionality for creating, modifying,
     * saving, reading, and evaluating mixtures of Gaussians. 
     * This is most useful for doing probabilistic modeling.
     *
     * @ingroup core_math
     *
     * @see Gaussian, ClassMixture
     **/
    class EVOLUTION_EXPORT_MATH GaussianMixture
    {    
    public: // Constants
        /**
         * @brief Internal constant used to read the XML file descriptor 
         * of the Gaussian Mixture.
         */
        static const char * const GAUSSIAN_MIXTURE_KEY;

    public: // Types
        /// Internal type that holds a vector of Gaussian's.
        typedef std::vector<Gaussian> GaussianVector;

    public: // Structors
        /**
         * @brief Standard constructor
         *
         * Constructs a 1-component mixture of Gaussian functions in 1D.
         */
        GaussianMixture ()
        {
            init ();
        }

        /**
         * @brief Constructs a "mixt_dim"-component mixture of Gaussian 
         *        functions in "dim" dimensions.
         * 
         * @param mixt_dim [in]; Number of Gaussuan components in the mixture
         * @param dim      [in]; Dimension of each Gaussian contained in the mixture
         */
        GaussianMixture (unsigned mixt_dim, unsigned dim);

        /**
         * @brief Copy constructor
         * 
         * @param m [in]; GaussianMixture to copy.
         */
        GaussianMixture (const GaussianMixture& m);

        /**
         * @brief Destructor
         */
        ~GaussianMixture () {;}

    public: // Binary operate and assign.

        /**
         * @brief Assigns one mixture to another mixture.
         * 
         * @param m [in]; GaussianMixture to copy.
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& operator= (const GaussianMixture& m);

    public:  // Setting

        /**
         * @brief Sets the number of components of the mixture.
         * Unconditionally sets all contained Gaussian functions to be
         * zero mean, unit covariance.
         *        
         * @param mixt_dim [in]; New number of Gaussian's in the mixture
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_mixt_dim (unsigned mixt_dim);

        /**
         * @brief Sets the dimensionality of the Gaussian components
         * of the mixture. Sets all Gaussians to be zero mean. unit
         * covariance.
         *        
         * @param dim [in]; New dimensionality of internal Gaussian's
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_dim (unsigned dim);

        /**
         * @brief Sets the mean vector of a Gaussian component. 
         * 
         * Setting the dim of a Gaussian to something different than its 
         * current value will result in the sigma matrix being resized and 
         * set to the identity matrix of size dim.
         *
         * @param m [in];    Index of Gaussian to which to set the mean vector.
         * @param mu [in];   Double array containing new mean vector
         * @param dim [in];  Length of the new mean vector
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_mean (unsigned m, const double* mu, unsigned dim)
        {
            _gaussians[m].set_mean (mu, dim);
            return *this;
        }

        /**
         * @brief Sets the mean vector of a Gaussian component. 
         * 
         * Setting the dim of a Gaussian to something different than its 
         * current value will result in the sigma matrix being resized and 
         * set to the identity matrix of dim.
         * 
         * @param m [in];    Index of Gaussian to which to set mean vector.
         * @param mu [in];   Vector to use as new mean vector.  The value of dim will be mu.size ().
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_mean (unsigned m, const Vector<double>& mu)
        {
            _gaussians[m].set_mean (mu);
            return *this;
        }

        /**
         * @brief Sets the covariance matrix of the Gaussian component 
         *        m of the mixture.
         * 
         *  If the covariance matrix is diagonal, the length of sigma
         *  is assumed to be only dim, otherwise the length is assumed
         *  to be dim*dim.  If the new dim is different than the current
         *  dimension of the Gaussian component, then the mean vector
         *  for that Gaussian will be resized and set to zero.
         *  
         * @param m [in];     Index of the Gaussian component to modify.
         * @param sigma [in]; Pointer to an array of doubles where the matrix is stored row by row.
         * @param dim   [in]; Dimensionality of the square covariance matrix
         * @param is_diagonal [in]; Flag that is true if the covariance matrix
         *                    is diagonal.
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_sigma (unsigned m, const double* sigma, 
                                    unsigned dim, bool is_diagonal = false)
        {
            _gaussians[m].set_sigma (sigma, dim, is_diagonal);
            return *this;
        }

        /**
         * @brief Sets the covariance matrix of the Gaussian component 
         *        m of the mixture
         * 
         * If the new dim is different than the current dimension of
         * the Gaussian component, then the mean vector for that
         * Gaussian will be resized and set to zero.
         *  
         * @param m [in]; Index to the Gaussian to change.
         * @param sigma [in]; Matrix to be used as the sigma matrix.
         * @param is_diagonal [in]; Flag that is true if the covariance matrix
         *                    is diagonal.
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_sigma (unsigned m, const Matrix<double>& sigma,  
                                    bool is_diagonal = false)
        {
            _gaussians[m].set_sigma (sigma, is_diagonal);
            return *this;
        }

        /**
         * @brief Sets the Gaussian component m of the mixture.
         * 
         * @param m [in]; Index of Gaussian to modify.
         * @param g [in]; Gaussian to copy.
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_gaussian (unsigned m, const Gaussian &g)
        {
            _gaussians[m] = g;
            return *this;
        }

        /**
         * @brief Sets the mixture weight for component m.
         * 
         * @param m [in]; Index to the Gaussian to modify.
         * @param weight [in]; New weight for the mth Gaussian
         * @return A reference to this GaussianMixture
         */
        GaussianMixture& set_mixt_weight (unsigned m, double weight)
        {
            _mixt_weights[m] = weight;
            _log_mixt_weights[m] = evo_log(weight);
            return *this;
        }

        /**
         * @brief Sets the mixture weights for all Gaussians.
         * 
         * If mixt_dim is not the same as the current size of the
         * altered GaussianMixture, then the GaussianMixture is
         * changed to have mixt_dim Gaussians, all of zero mean and
         * unit covariance.
         *
         * @param weight [in]; Double pointer to an array of weights.
         * @param mixt_dim [in]; Number of contained Gaussians.
         * @return A reference to this GaussianMixture
         * @return A reference to this ClassMixture
         *
         * @pre The number of components in the mixture must be equal to mixt_dim.
         */
        GaussianMixture& set_mixt_weight (const double* weight, unsigned mixt_dim);

        /**
         * @brief Sets the mixture weights for all Gaussians.
         * 
         * If mixt_dim is not the same as the current size of the
         * altered GaussianMixture, then the GaussianMixture is
         * changed to have mixt_dim Gaussians, all of zero mean and
         * unit covariance.
         *
         * @param weight [in]; Vector containing new weights
         * @return A reference to this GaussianMixture
         *
         * @pre The number of components in the mixture must be equal to the size of weight.
         */
        GaussianMixture& set_mixt_weight (const Vector<double>& weight);

    public: // Queries
        
        /**
         * @brief Gets the dimensionality of the mixture (number of Gaussians).
         *
         * @return The number of components of the mixture
         */
        unsigned get_mixture_dim () const {return _mixt_dim;}

        /**
         * @brief Get the dimensionality of the Gaussian components 
         *        of the mixture.
         *
         * @return The dimensionality of the Gaussians in the mixture
         */
        unsigned get_dim () const {return _dim;}

        /**
         * @brief Gets the element i of the mean vector of the Gaussian
         *        component m of the mixture.
         * @param m [in]; Index of Gaussian to read.
         * @param i [in]; Index of the element of the mean to return.
         * @return Component i of the mean of the mth Gaussian
         */
        double get_mean_i (unsigned m, unsigned i) const 
        {
            return _gaussians[m].get_mean_i (i);
        }

        /**
         * @brief Gets the element ij of the covariance matrix 
         *        of the Gaussian component m of the mixture.
         * @param m [in]; Index of the Gaussian to read.
         * @param i [in]; Row index into the cov matrix
         * @param j [in]; Col index into the cov matrix
         * @return Element ij of the corresponding covariance matrix
         */
        double get_sigma_ij (unsigned m, unsigned i, unsigned j) const 
        {
            return _gaussians[m].get_sigma_ij (i, j);
        }

        /**
         * @brief Gets the mixture weight of the mth component of the mixture.
         * @param m [in]; Index of the Gaussian to read.
         * @return Value of mixture weight for m Gaussian
         */
        double get_mixt_weight_m (unsigned m) const { return _mixt_weights[m]; }

    public: // Likelihood computation

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point.
         *
         * @param data     [in]; Input data vector
         * @param data_dim [in]; Dimensionality of the input data. Must match dim of underlying Gaussians.
         * @return Log-likelihood for the input data
         */
        double log_likelihood (const double * data, unsigned data_dim);

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point
         *
         * @param data [in]; Input data vector

         * @return Log-likelihood for the input data
         */
        double log_likelihood (const Vector<double>& data);

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        set of data points (one data point per row of the matrix).
         * @param data [in]; A set of data vectors provided in a
         * matrix.  The data vectors are stored in the rows.
         * @param mixture_lkhd [out]; Vector of likelihoods for each row of the matrix
         * @return Result indicating success or failure of computation
         */
        Result log_likelihood (const Matrix<double>& data, 
                               Vector<double> * mixture_lkhd) ;

    public: // File loading
        /** 
         * @brief Loads a GaussianMixture model from a file.
         * @param filename [in]; Filename from which to read the GaussianMixture.
         * @return Result indicating success or failure of computation
         */
        Result read_gaussian_mixture (const char * filename);

        /** 
         * @brief Loads one of a series of GaussianMixtures 
         *        using a Configuration object. This uses the XML
         *        config library, and is usually only used when loading entire
         *        ClassMixtures from file storage.
         *
         * @param config [in]; Configuration element 
         * @param parent [in]; ConfigKey element
         * @param index [in]; Index in the series of Gaussians
         *
         * @return Result indicating success or failure.
         * 
         * @see Configuration, ConfigKey
         * @see GaussianMixture, ClassMixture
         */
        Result read_gaussian_mixture (Configuration config, ConfigKey parent, 
                                      unsigned index);

        /** 
         * @brief Saves a GaussianMixture model to a file.
         * @param filename [in];  Filename to which to write the GaussianMixture.
         * @return Result indicating success or failure of computation
         */
        Result write_gaussian_mixture (const char * filename);

        /** 
         * @brief Saves one of a series of GaussianMixtures 
         *        using a Configuration object. Usually used by ClassMixture
         *        when saving out a set of Gaussians.
         *
         * @param config [in]; Configuration element 
         * @param parent [in]; ConfigKey element
         * @param index [in]; Index in the series of Gaussians
         *
         * @return Result indicating success or failure.
         * 
         * @see Configuration, ConfigKey
         * @see GaussianMixture, ClassMixture
         *
         */
        Result write_gaussian_mixture (Configuration config, ConfigKey parent, 
                                       unsigned index);

        /** 
         * @brief Loads a GaussianMixture model from a file in binary format.
         * @param filename [in]; Filename from which to read the GaussianMixture.
         * @return Result indicating success or failure of computation
         */
        Result bin_read_gaussian_mixture (const char* filename);

        /** 
         * @brief Saves a GaussianMixture model to a file in binary format.
         * @param filename [in]; Filename to which to write the GaussianMixture.
         * @return Result indicating success or failure of computation
         */
        Result bin_write_gaussian_mixture (const char* filename);
        
    public: // Training
        /** 
         * @brief Sets training parameters.
         * @param pert_factor [in]; perturbation factor used to split the 
         *                          means of the clusters when doing 
         *                          vector quantization with bianry splitting.
         * @param thd         [in]; training threshold.
         * @param tP          [in]; debug parameter that enables 
         *                          an optional display of internals
         *                          of the computation.
         * @return Result indicating success or failure of computation
         */
        void set_training_parameters (double pert_factor, double thd, int tP) 
        {
            _pert_factor = pert_factor;
            _d_threshold = thd;
            toPrint    = tP;
        }

        /** 
         * @brief Performs vector quantization on the data.
         *        The observation matrix should have one observation
         *        per row (number of columns of the matrix should be 
         *        equal to _dim)
         * @param obs                 [in]; matrix of data to be quantized
         *                                  one observation per row.
         * @param min_vector_per_code [in]; required minimum number of 
         *                                  observations in each cluster.
         * @return Result indicating success or failure of computation
         */
        Result compute_vector_quantization (Matrix<double>& obs, 
                                            int min_vector_per_code);

        /** 
         * @brief Trains the Gaussian mixture using the EM algorithm
         *        The observation matrix should have one observation
         *        per row (number of columns of the matrix should be 
         *        equal to _dim)
         * @param obs                 [in]; matrix of training data.
         * @return Result indicating success or failure of computation
         */
        Result compute_em (Matrix<double>& obs);

    public: // Comparison
        /**
         * @brief Evaluates whether two GaussianMixtures are equal.
         * @param gm [in]; GaussianMixture to test equality against.
         * @return Boolean value indicating equality
         */
        bool operator== (const GaussianMixture & gm) const;

        /**
         * @brief Evaluates whether two GaussianMixtures are equal.
         *        Allows to pass a threshold value.
         * @param gm [in]; GaussianMixture to test equality against.
         * @param thr [in]; Value to use in thresholded equality test.
         * @return Boolean value indicating equality to within thr.
         */
        bool compare (const GaussianMixture & gm, double thr) const;

    protected: // Helpers
        /** 
         * @brief Loads a GaussianMixture using a Configuration object.
         * @internal
         * @param config [in]; Configuration element 
         * @param gaussian_mixture_key [in]; ConfigKey element
         * @return Result indicating success or failure of computation
         */
        Result read_gaussian_mixture (Configuration config, 
                                      ConfigKey gaussian_mixture_key);

        /** 
         * @brief Saves a GaussianMixture using a Configuration object.
         * @internal
         * @param config [in]; Configuration element 
         * @param gaussian_mixture_key [in]; ConfigKey element
         * @return Result indicating success or failure of computation
         */
        Result write_gaussian_mixture (Configuration config, 
                                       ConfigKey gaussian_mixture_key);

        /**
         * @brief Initialize the variables to a canonical
         *        one-dimensional GaussianMixture function.
         * @internal
         * @param dim [in]; Dimensionality of the Gaussian
         */
        void init (unsigned mixt_dim = 1, unsigned dim = 1)
        {
            PRECOND (mixt_dim > 0);
            PRECOND (dim > 0);

            _mixt_dim = mixt_dim;
            _dim = dim;
            _mixt_weights.resize (1.0/((double)_mixt_dim), _mixt_dim);
            _log_mixt_weights.resize (evo_log(1.0/((double)_mixt_dim)), 
                                      _mixt_dim);
            _gaussians.clear ();
            _gaussians.resize (_mixt_dim);
            for (unsigned i = 0; i < _mixt_dim; i++) 
            {
                _gaussians[i].set_dim (_dim);
            }
        }

        /**
         * @brief Check that the mixture weights add to 1.0, 
         *        otherwise normalize.
         * @internal
         */
        void check_mixt_weight ();
        
    protected: // Implementation

        /**
         * The internal representation.
         */
        unsigned       _mixt_dim;         ///< Number of components of the mixture
        unsigned       _dim;              ///< Dimensionality of the Gaussian functions
        Vector<double> _mixt_weights;     ///< Mixture weights
        Vector<double> _log_mixt_weights; ///< Log of mixture weights
        GaussianVector _gaussians;        ///< Vector of Gaussian functions

        // Training parameters
        double _pert_factor;              ///< Perturbation factor for binary splitting
        double _d_threshold;              ///< Threshold
        int toPrint;                      ///< Internal flag used for debugging

    }; // end class GaussianMixture

} // end namespace Evolution

// External operators
/**
 * @brief Print a the contents of a Mixture of Gaussian functions 
 *        to the specified stream.
 * @param out [in]; Stream to which to print the contents of the GaussianMixture.
 * @param mix [in]; GaussianMixture whose contents will be printed to the stream.
 *
 * @return Stream reference given in the out parameter.
 */
inline std::ostream& operator<< (std::ostream& out, const Evolution::GaussianMixture & mix)
{
    unsigned M = mix.get_mixture_dim ();
    unsigned D = mix.get_dim ();
    out << "{ ";
    out << "number of components: " << M << "\n"; 
    for (unsigned m = 0; m < M; m++)
    {
        out << "{ ";
        out << "mixture weight: " << mix.get_mixt_weight_m (m) << "\n";

        out << "\t{ ";
        out << "\tdimension: " << D << "\n"; 
        out << "\tmean vector: ( ";
        for (unsigned i=0; i < D-1; i++)
        {
            out << mix.get_mean_i (m, i) << ", ";
        }
        out << mix.get_mean_i (m, D-1) << " )\n";
        
        out << "\tcovariance matrix: ( ";
        for (unsigned i=0; i < D; i++)
        {
            out << "( ";
            for (unsigned j=0; j < D-1; j++)
            {
                out << mix.get_sigma_ij (m, i, j) << ", ";
            }
            out << mix.get_sigma_ij (m, i, D-1) << " )";
        }
        out << " )\n";
        out << "\t}\n";
        out << "}\n";
    }
    return out << "}";
}

#endif  // INCLUDE_EVOLUTION_MATH_GAUSSIAN_MIXTURE_HPP
