//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_GAUSSIAN_HPP
#define INCLUDE_EVOLUTION_MATH_GAUSSIAN_HPP


/**
 * @file    evolution/core/math/Gaussian.hpp
 * @brief   An arbitrary dimension Gaussian function class
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Gaussian
 */


#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Matrix.hpp>


namespace Evolution
{

    // Forward declaration
    class Configuration;

    /**
     * @class Gaussian
     *
     * @brief A Gaussian function class
     *
     * This class stores one Gaussian function, defined by mu and
     * sigma. The class supports arbitrary dimension Gaussian
     * functions. Mixtures of Gaussians are handled by the
     * GaussianMixture class. GaussianMixutres allow a collection of
     * Gaussians to be evaluated as a group. Collections of
     * GaussianMixtures are dealt with by ClassMixture. ClassMixture
     * allows collections of GaussianMixtures to be evaluated against
     * each other.
     *
     * All of the likelihoods returned from the Gaussian are log_likelihoods,
     * as often times the likelihood of a given datapoint for a given Gaussian
     * results in double underflow.
     *
     * This class also contains a static utility function gauss() that
     * generates a point that is a member of a zero mean, unit covariance
     * Gaussian distribution, which is not a high-quality random number.
     * Simply double gpoint = Gaussian::gauss().
     *
     * @see GaussianMixture, ClassMixture
     *
     * @ingroup core_math
     **/
    class EVOLUTION_EXPORT_MATH Gaussian
    {    
    public: // Constants
        /**
         * @brief Internal constant used to read the XML file descriptor 
         * of the Gaussian.
         */
        static const char * const GAUSSIAN_KEY;

    public: // Static Gaussian point distribution function
        /**
         * @brief Returns a point that is a member of a zero mean,
         * unit covariance Gaussian distribution. 
         * This function uses the Box-Mueller method to generate the
         * normally distributed random variable based a uniformly
         * distributed random variable. The function internally calls 
         * rand() function to obtain the uniformly distributed random
         * variable points.
         *
         * @return Value of the normalized Gaussian function
         */
        static double gauss();
        

    public: // Structors
        /**
         * @brief Constructs a canonical Gaussian function in 1D.
         */
        Gaussian ()
        {
            init ();
        }

        /**
         * @brief Constructs a Gaussian function of dim dimensions.
         *
         * @param dim [in]; Dimension of Gaussian function to create.
         */
        Gaussian (unsigned dim);

        /**
         * @brief Copy constructor
         *
         * @param g [in]; Gaussian to copy.
         */
        Gaussian (const Gaussian& g);

        /**
         * @brief Destructor
         */
        ~Gaussian () {;}

    public: // Binary operate and assign

        /**
         * @brief Assigns one Gaussian function to another Gaussian function.
         *
         * @param g [in]; Gaussian to copy.
         * @return A reference to this object after copying
         */
        Gaussian& operator= (const Gaussian& g);

    public:  // Setting

        /**
         * @brief Sets the dimensionality of the Gaussian function.
         * Resizes mu and sigma, and sets them to zero mean, unit
         * covariance.
         *
         * @param dim [in]; Dimension to which to resize.
         * @return A reference to this object after modification
         */
        Gaussian& set_dim (unsigned dim);

        /**
         * @brief Sets the mean vector of the Gaussian function
         *        If dim != current_dim, then sigma will be resized,
         *        and set to the identity matrix for dim.
         *
         * @param mu  [in]; Pointer to a double array from which to extract mean.
         * @param dim [in]; Dimension to which to resize.
         * @return A reference to this object after modification
         */
        Gaussian& set_mean (const double* mu, unsigned dim);

        /**
         * @brief Sets the mean vector of the Gaussian function
         *        If mu.size() != current_dim, then sigma will be resized,
         *        and set to the identity matrix for mu.size().
         *
         * @param mu  [in]; Vector<double> to copy for the mean vector.
         * @return A reference to this object after modification
         */
        Gaussian& set_mean (const Vector<double>& mu);

        /**
         * @brief Sets the covariance matrix of the Gaussian function
         * (if the covariance matrix is diagonal, the length of sigma
         * is assumed to be only dim, otherwise the length is assumed
         * to be dim*dim).
         *
         * @param sigma [in]; Pointer to an array of doubles
         * @param dim   [in]; Dimensionality of the square covariance matrix
         * @param is_diagonal [in]; Optional flag that is true if the
         *                          covariance matrix is diagonal.  
         *                          Default is false.
         * @return A reference to this object after modification
         */
        Gaussian& set_sigma (const double* sigma, unsigned dim, 
                             bool is_diagonal = false);
        /**
         * @brief Sets the covariance matrix of the Gaussian function.  
         *        If sigma.rsize() != current_dim, the Gaussian will be
         *        resized and the mean set to a zero vector.
         *
         * @param sigma [in];       Matrix to copy to the sigma matrix.
         * @param is_diagonal [in]; Optional flag that is true if the 
         *                          covariance matrix is diagonal.
         *                          Default is false.
         * @return A reference to this object after modification.
         */
        Gaussian& set_sigma (const Matrix<double>& sigma, 
                             bool is_diagonal = false);

    public: // Queries
        
        /**
         * @brief Gets the dimensionality of the Gaussian function.
         * @return Dimensionality of this Gaussian.
         */
        unsigned get_dim () const {return _dim;}

        /**
         * @brief Gets the element i of the mean vector 
         *        of the Gaussian function.
         *
         * @param i [in]; Index of the element to return.
         *
         * @return Value of the i-th element of the  mean of the Gaussian
         */
        double get_mean_i (unsigned i) const {return _mu[i];}

        /**
         * @brief Gets the element ij (row, column) of the covariance matrix 
         *        of the Gaussian function.
         *
         * @param i [in]; Row index of the element to return.
         * @param j [in]; Column index of the element to return.
         *
         * @return Value of the ijth element of the covariance matrix
         * of the Gaussian
         */
        double get_sigma_ij (unsigned i, unsigned j) const {return _sigma[i][j];}

        /**
         * @brief Gets the element ij (row, column) of the inverse
         * covariance matrix of the Gaussian function.
         *
         * @param i [in]; Row index of the element to return.
         * @param j [in]; Column index of the element to return.
         *
         * @return Value of the ijth element of the inverse covariance
         * matrix of the Gaussian
         */
        double get_inv_sigma_ij (unsigned i, unsigned j) const {return _inv_sigma[i][j];}

        /**
         * @brief Gets the determinant of the covariance matrix 
         *        of the Gaussian function.
         *
         * @return Determinant of the covariance matrix
         */
        double get_det_sigma () const {return _det_sigma;}

        /**
         * @brief Gets the log of the determinant of the covariance matrix 
         *        of the Gaussian function.
         *
         * @return Logarithm of the determinant of the covariance matrix
         */
        double get_log_det_sigma () const {return _log_det_sigma;}

        /**
         * @brief Returns the _is_diagonal flag value.
         *
         * @return Value of _is_diagonal
         */
        bool is_diagonal () const {return _is_diagonal;}

    public: // Likelihood computation

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point.
         * 
         * @param data [in]; A double array of length data_dim encoding 
         *                   a point in the input space of dimension data_dim
         * @param data_dim [in]; Dimensionality of input data vector
         * @return Log-likelihood value of the given data point
         */
        double log_likelihood (const double * data, unsigned data_dim) const;

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point.
         *
         * @param data [in]; Data Vector for which to compute loglikelihood.
         * @return Log-likelihood value of the given data point
         */
        double log_likelihood (const Vector<double>& data) const;

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        set of data points (one data point per row of the matrix).
         *
         * @param data          [in];  Input data comprising one data point 
         * per row of the matrix.
         * @param gaussian_lkhd [out]; Vector of log likelihoods in which each 
         * element of the vector corresponds to each row of the input data.
         * @return Result indicating success or failure
         */
        Result log_likelihood (const Matrix<double>& data, 
                               Vector<double> * gaussian_lkhd) const;


    public: // File loading
        /** 
         * @brief Loads a Gaussian model from a file. This function
         * opens and closes the file.
         *
         * @param filename [in];  Filename from which to read the Gaussian. 
         * @return Result indicating success or failure
         */
        Result read_gaussian (const char * filename);

        /** 
         * @brief Loads one of a series of Gaussians using 
         *        a Configuration object.  This uses the XML
         *        config library, and is usually only used when loading entire
         *        ClassMixtures or GaussianMixtures from file storage.
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
        Result read_gaussian (Configuration config, ConfigKey parent, 
                              unsigned index);

        /** 
         * @brief Saves a Gaussian model to a file. This function
         * opens and closes the file.
         *
         * @param filename [in];  Filename to which to write the Gaussian.  
         * @return Result indicating success or failure
         */
        Result write_gaussian (const char * filename);

        /** 
         * @brief Saves one of a series of Gaussians using 
         *        a Configuration object. Usually only used by ClassMixture or
         *        GaussianMixture when saving out a set of Gaussians.
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
        Result write_gaussian (Configuration config, ConfigKey parent, 
                               unsigned index);

    protected: // Helpers
        /** 
         * @brief Loads a Gaussian using a Configuration object.
         * @internal
         * @param config [in]; Configuration element 
         * @param gaussian_key [in]; ConfigKey element
         * @return Result indicating success or failure.
         */
        Result read_gaussian (Configuration config, ConfigKey gaussian_key);

        /** 
         * @brief Saves a Gaussian using a Configuration object.
         * @internal
         * @param config [in]; Configuration element 
         * @param gaussian_key [in]; ConfigKey element
         * @return Result indicating success or failure.
         */
        Result write_gaussian (Configuration config, ConfigKey gaussian_key);

        /**
         * @brief Initializes the internal variables to the ones
         * corresponding to a canonical one-dimensional Gaussian
         * function.
         * @internal
         * @param dim [in]; Dimensionality of the Gaussian
         */
        void init (unsigned dim = 1)
        {
            PRECOND (dim > 0);
            _dim = dim;
            _mu.resize (0.0, _dim);
            _sigma.eye (_dim);
            _inv_sigma.eye (_dim);
            _g_const = - 0.5 * _dim * log (2 * M_PI);
            _det_sigma = 1.0;
            _log_det_sigma = 0.0;
            _is_diagonal = false;
        }
        
        /**
         * @brief Compute the internal variables: inverse covariance
         *        and determinant.
         */
        void compute_internals ()
        {
            compute_determinant ();
            compute_inverse_covariance ();
        }

        /**
         * @brief Compute the determinant of the covariance matrix.
         */
        void compute_determinant ();

        /**
         * @brief Compute the inverse of the covariance matrix.
         */
        void compute_inverse_covariance ();

        /**
         * @brief Check that the covariance matrix is proper
         *        i.e., it has non-zero determinant
         * @return Boolean value: true if the covariance has non-zero determinant.
         */
        bool check_covariance ();

    protected: // Implementation

        /**
         * The internal representation.
         */
        unsigned            _dim;     ///< Dimensionality of the Gaussian function
        Vector<double> _mu;           ///< Mean vector
        Matrix<double> _sigma;        ///< Covariance matrix
        Matrix<double> _inv_sigma;    ///< Inverse covariance matrix

        double         _g_const;      ///< Gaussian constant: - _dim/2 * log (2 * PI)
        double         _det_sigma;    ///< Determinant of sigma
        double         _log_det_sigma;///< Log of the determinant of sigma

        bool           _is_diagonal;  ///< Flag that is true if the covariance matrix is diagonal.
    }; // end class Gaussian

} // end namespace Evolution

// External operators
/**
 * @brief Print the contents of a Gaussian function to the specified stream.
 * @param out [in]; Stream to which to print the contents of the Gaussian.
 * @param g [in]; Gaussian whose contents will be printed to the stream.
 *
 * @return Stream reference given in the out parameter.
 */
inline std::ostream& operator<< (std::ostream& out, const Evolution::Gaussian & g)
{
    unsigned D = g.get_dim ();
    out << "{ ";
    out << "dimension: " << D << "\n"; 
    out << "is diagonal: " << (g.is_diagonal () ? "true\n" : "false\n");
    out << "mean vector: ( ";
    for (unsigned i=0; i < D-1; i++)
    {
        out << g.get_mean_i (i) << ", ";
    }
    out << g.get_mean_i (D-1) << " )\n";

    out << "covariance matrix: ( ";
    for (unsigned i=0; i < D; i++)
    {
        out << "( ";
        for (unsigned j=0; j < D-1; j++)
        {
            out << g.get_sigma_ij (i, j) << ", ";
        }
        out << g.get_sigma_ij (i, D-1) << " )";
    }
    out << " )\n";

    out << "inverse covariance matrix: ( ";
    for (unsigned i=0; i < D; i++)
    {
        out << "( ";
        for (unsigned j=0; j < D-1; j++)
        {
            out << g.get_inv_sigma_ij (i, j) << ", ";
        }
        out << g.get_inv_sigma_ij (i, D-1) << " )";
    }
    out << " )\n";

    out << "determinant: " << g.get_det_sigma () << "\n";
    out << "log determinant: " << g.get_log_det_sigma () << "\n";

    return out << "}";
}

#endif  // INCLUDE_EVOLUTION_MATH_GAUSSIAN_HPP
