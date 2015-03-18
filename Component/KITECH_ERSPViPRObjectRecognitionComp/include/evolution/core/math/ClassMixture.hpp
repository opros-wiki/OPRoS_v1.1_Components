//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_CLASS_MIXTURE_HPP
#define INCLUDE_EVOLUTION_MATH_CLASS_MIXTURE_HPP


/**
 * @file    evolution/core/math/ClassMixture.hpp
 * @brief   A class that contains a set of GaussianMixtures
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::ClassMixture
 */


#include <vector>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/IConfigSource.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Vector.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/GaussianMixture.hpp>


namespace Evolution
{

    // Forward declaration.
    class Configuration;

    /**
     * @class ClassMixture 
     *
     * @brief This class implements a mixture of GaussianMixtures.
     * This class is used to find out the probabilities of a point, of
     * the proper dimension, being a member of each contained
     * GaussianMixture.
     * 
     * @ingroup core_math
     *
     * @see GaussianMixture, Gaussian
     **/
    class EVOLUTION_EXPORT_MATH ClassMixture
    {    
    public: // Constants

        /**
         * @brief Internal constant used to read the XML file descriptor 
         * of the class mixture.
         */
        static const char * const CLASS_MIXTURE_KEY;

    public: // Types.

        /// Internal type that holds a vector of GaussianMixture's.
        typedef std::vector<GaussianMixture> ClassVector;

    public: // Structors

        /**
         * @brief Standard constructor
         *
         * Sets up the ClassMixture to contain 1 GaussianMixture, which
         * contains a 1 dimensional Gaussian, with a zero mean and a 
         * unit covariance.        
         *        
         */
        ClassMixture ()
        {
            init ();
        }

        /**
         * @brief Constructs "class_dim" classes of "mixt_dim"-components 
         *        mixture of Gaussian functions in "dim" dimensions.
         *
         * @param class_dim [in]; Number of contained GaussianMixtures
         * @param mixt_dim  [in]; Number of Gaussians in each GaussianMixture
         * @param dim       [in]; Dimension of each Gaussian contained in each GaussianMixture
         */
        ClassMixture (unsigned class_dim, unsigned mixt_dim, unsigned dim);

        /**
         * @brief Copy constructor
         *
         * @param cl [in]; ClassMixture to copy
         */
        ClassMixture (const ClassMixture& cl);

        /**
         * @brief Destructor
         */
        ~ClassMixture () 
        {}

    public: // Binary operate and assign

        /**
         * @brief Assigns one mixture to another mixture.
         *
         * @param cl [in]; ClassMixture to copy
         * @return A reference to this ClassMixture
         */
        ClassMixture& operator= (const ClassMixture& cl);

    public:  // Setting

        /**
         * @brief Sets the number of contained GaussianMixtures to "class_dim".
         *        Sets all GaussianMixtures to size 1, and all Gaussians to
         *        zero mean, unit covariance.
         *
         * @param class_dim [in]; New number of classes in the mixture
         * @return A reference to this ClassMixture
        */
        ClassMixture& set_class_dim (unsigned class_dim);

        /**
         * @brief Sets all contained GaussianMixtures to be size "dim".
         *        This resizes all contained Gaussians to zero mean, 
         *        unit covariance.
         *
         * @param dim [in]; New dimensionality of internal Gaussian
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_dim (unsigned dim);

        /**
         * @brief Sets the number of Gaussians contained by
         * GaussianMixture "c", initializes all contained Gaussians to
         * zero mean, unit covariance.
         *
         * @param c [in]; Index of GaussianMixture to modify.
         * @param mixt_dim [in]; New number of Gaussians the c'th GaussianMixture should hold.
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_mixt_dim (unsigned c, unsigned mixt_dim)
        {
            _classes[c].set_mixt_dim (mixt_dim);
            return *this;
        }

        /**
         * @brief Sets the mean vector of a Gaussian component of a
         * contained GaussianMixture.
         *
         * If "dim" is different than the Gaussian's current dimension,
         * the sigma matrix of the Gaussian will be changed to the identity
         * of "dim".
         *
         * @param c   [in]; Index of GaussianMixture to modify.
         * @param m   [in]; Index of Gaussian in GaussianMixture c to modify.
         * @param mu  [in]; Double array containing new mean vector
         * @param dim [in]; Length of the new mean vector
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_mean (unsigned c, unsigned m, const double* mu, unsigned dim)
        {
            _classes[c].set_mean (m, mu, dim);
            return *this;
        }

        /**
         * @brief Sets the mean vector of a Gaussian component of a
         * contained GaussianMixture.
         *
         * If mu.size() is different than the Gaussian's current dimension,
         * the sigma matrix of the Gaussian will be changed to identity
         * of mu.size().
         *
         * @param c   [in]; Index of GaussianMixture to modify..
         * @param m   [in]; Index of Gaussian in GaussianMixture c to modify.
         * @param mu  [in]; Vector to use as new mean vector. The value of dim will be mu.size ().
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_mean (unsigned c, unsigned m, const Vector<double>& mu)
        {
            _classes[c].set_mean (m, mu);
            return *this;
        }

        /**
         * @brief Sets the covariance matrix of the Gaussian component 
         *        m of the GaussianMixture c.
         *
         *  If the covariance matrix is diagonal, the length of sigma
         *  is assumed to be only dim, otherwise the length is assumed
         *  to be dim*dim. If the new dim is different than the
         *  current dimension of the Gaussian component, the mean
         *  vector will be set to a zero mean in the new dimension.
         * 
         * @param c   [in]; Index of GaussianMixture to modify.
         * @param m   [in]; Index of Gaussian in GaussianMixture c to modify.
         * @param sigma [in]; Pointer to an array of doubles where the matrix is stored row by row.
         * @param dim   [in]; Dimensionality of the square covariance matrix
         * @param is_diagonal [in]; Flag that is true if the covariance matrix
         *                    is diagonal.
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_sigma (unsigned c, unsigned m, const double* sigma, 
                                 unsigned dim, bool is_diagonal = false)
        {
            _classes[c].set_sigma (m, sigma, dim, is_diagonal);
            return *this;
        }

        /**
         * @brief Sets the covariance matrix of the Gaussian component 
         *        m of the GaussianMixture c.
         * 
         * If the new dim is different than the current dimension of
         * the Gaussian component, then the mean vector for that
         * Gaussian will be resized and set to zero.
         *
         * @param c   [in]; Index of GaussianMixture to modify.
         * @param m   [in]; Index of Gaussian in GaussianMixture c to modify.
         * @param sigma [in]; Matrix to use as the new sigma matrix.  Must be square.
         * @param is_diagonal [in]; Flag that is true if the covariance matrix
         *                    is diagonal.
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_sigma (unsigned c, unsigned m, 
                                 const Matrix<double>& sigma, 
                                 bool is_diagonal = false)
        {
            _classes[c].set_sigma (m, sigma, is_diagonal);
            return *this;
        }

        /**
         * @brief Sets the Gaussian component m of the mixture c.
         * 
         * @param c  [in]; Index of GaussianMixture to modify.
         * @param m  [in]; Index of Gaussian in GaussianMixture c to modify.
         * @param g  [in]; Gaussian to copy into Gaussian being modified.
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_gaussian (unsigned c, unsigned m, const Gaussian &g)
        {
            _classes[c].set_gaussian (m, g);
            return *this;
        }

        /**
         * @brief Sets the Gaussian Mixture c.
         * 
         * @param c  [in]; Index of GaussianMixture to modify.
         * @param gm  [in]; GaussianMixture to copy.
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_gaussian_mixture (unsigned c, const GaussianMixture &gm)
        {
            _classes[c] = gm;
            return *this;
        }

        /**
         * @brief Sets the mixture weight for Gaussian m of the
         * GaussianMixture c.
         *
         * @param c [in]; Index of the GaussianMixture to modify.
         * @param m [in]; Index of Gaussian in c'th GaussianMixture to modify.
         * @param weight [in]; New weight for the c,m'th Gaussian
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_mixt_weight (unsigned c, unsigned m, double weight)
        {
            _classes[c].set_mixt_weight (m, weight);
            return *this;
        }

        /**
         * @brief Sets the mixture weights of the mixture c.
         *
         * @param c        [in]; Index of the GaussianMixture to modify.
         * @param weight   [in]; New weight vector for the c'th GaussianMixture
         * @param mixt_dim [in]; Length of weight vector for c'th GaussianMixture
         * @return A reference to this ClassMixture
         *
         * @pre The number of components in the mixture must be equal to mixt_dim.
         */
        ClassMixture& set_mixt_weight (unsigned c, const double* weight, 
                                       unsigned mixt_dim)
        {
            _classes[c].set_mixt_weight (weight, mixt_dim);
            return *this;
        }

        /**
         * @brief Sets the mixture weights of the mixture c.
         *
         * @param c        [in]; Index of the GaussianMixture to modify.
         * @param weight   [in]; New Vector to use as the weights 
         *                       for the Gaussians of GaussianMixture c.
         * @return A reference to this ClassMixture
         *
         * @pre The number of components in the mixture must be equal to the size of weight.
         */
        ClassMixture& set_mixt_weight (unsigned c, const Vector<double>& weight)
        {
            _classes[c].set_mixt_weight (weight);
            return *this;
        }

        /**
         * @brief Sets the prior for GaussianMixture c.
         *
         * The prior is the weight for the GaussianMixture.
         *
         * @param c       [in]; Index of the GaussianMixture to modify.
         * @param prior   [in]; New prior value for GaussianMixture c
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_prior (unsigned c, double prior)
        {
            _class_prior[c] = prior;
            _log_class_prior[c] = evo_log(prior);
            return *this;
        }

        /**
         * @brief Sets the priors for all GaussianMixture in the ClassMixture.
         *
         * @param prior     [in]; Double pointer, vector of values to use as priors for each contained GaussianMixture.
         * @param class_dim [in]; Dimension (number of priors) of the
         * prior variable.  If this is different from the current
         * dimension of the ClassMixture, in that everything is reset to
         * the new dimension.
         * @return A reference to this ClassMixture
        */
        ClassMixture& set_prior (const double* prior, unsigned class_dim);

        /**
         * @brief Sets the prior.
         *
         * @param prior [in]; Vector of values to use as priors for
         * each contained GaussianMixture.  If prior.size() is
         * different from the current dimension of the ClassMixture,
         * everything is reset to the new dimension.  
         * @return A reference to this ClassMixture
         */
        ClassMixture& set_prior (const Vector<double>& prior);

        /**
         * @brief Resets all components of the Class Mixture.
         */
        void clear()
        {
            init();
        }

    public: // Queries.
        
        /**
         * @brief Gets the number of GaussianMixtures in this ClassMixture.
         *
         * @return The number of classes of this ClassMixture
         */
        unsigned get_class_dim () const {return _class_dim;}

        /**
         * @brief Gets the dimensionality (number of Gaussians) of the GaussianMixture c.
         * @param c [in]; Index of the GaussianMixture to read.
         * @return The number of components in the cth GaussianMixture
         */
        unsigned get_mixture_dim (unsigned c) const 
        {
            return _classes[c].get_mixture_dim();
        }

        /**
         * @brief Gets the dimensionality of the Gaussian components 
         *        of the mixture.
         * @return The dimensionality of the Gaussians in the mixture
         */
        unsigned get_dim () const {return _dim;}

        /**
         * @brief Gets the element i of the mean vector of the Gaussian
         *        component m of the mixture of GaussianMixture c.
         *
         * @param c [in]; Index of the GaussianMixture to read.
         * @param m [in]; Index of the Gaussian in the c'th GaussianMixture to read.
         * @param i [in]; Index of the element of c,m'th Gaussian's mean vector to return.
         * @return Component i of the mean of the mth Gaussian in the cth GaussianMixture
         */
        double get_mean_i (unsigned c, unsigned m, unsigned i) const 
        {
            return _classes[c].get_mean_i (m, i);
        }

        /**
         * @brief Gets the element ij of the covariance matrix of the
         * Gaussian component m of the mixture of GaussianMixture c.
         *
         * @param c [in]; Index of the GaussianMixture to read.
         * @param m [in]; Index of the Gaussian in the c'th GaussianMixture to read.
         * @param i [in]; Row index of the element of c,m'th Gaussian's sigma matrix to return.
         * @param j [in]; Col index of the element of c,m'th Gaussian's sigma matrix to return.
         * @return Element ij of the corresponding covariance matrix
         */
        double get_sigma_ij (unsigned c, unsigned m, unsigned i, unsigned j) const 
        {
            return _classes[c].get_sigma_ij (m, i, j);
        }

        /**
         * @brief Gets the mixture weight of the m component of
         * GaussianMixture c.
         *
         * @param c [in]; Index of the GaussianMixture to read.
         * @param m [in]; Index of the Gaussian in the c'th GaussianMixture to read.
         * @return Value of mixture weight for c mixture, m Gaussian
         */
        double get_mixt_weight_m (unsigned c, unsigned m) const 
        { 
            return _classes[c].get_mixt_weight_m (m) ; 
        }

        /**
         * @brief Gets the prior of GaussianMixture c.
         *
         * @param c [in]; Index of the GaussianMixture to read.
         * @return Value for prior for cth GaussianMixture
         */
        double get_prior_c (unsigned c) const {return _class_prior[c];}

    public: // Likelihood computation.

        /**
         * @brief Returns the log-likelihoodvector corresponding to the given
         *        data point. Normalizes probabilities.
         *
         * The output vector encodes the log-likelihood of the input
         * vector for each class in the mixture. The input
         * data's dimension must match the dimension of the underlying
         * Gaussians.
         *
         * @param data     [in]; Input data vector
         * @param data_dim [in]; Dimensionality of the input data. Must match dim of underlying Gaussians.
         * @param class_lkhd [out]; Vector containing likelihoods of the input vector for each class
         * @return Result indicating success or failure
         */
        Result log_likelihood (const double * data, unsigned data_dim, 
                               Vector<double> * class_lkhd);

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point without normalization of likelihoods.
         *
         * The output vector encodes the log-likelihood of the input
         * vector for each class in the mixture. The input
         * data's dimension must match the dimension of the underlying
         * Gaussians.
          *
         * @param data [in]; Input data vector
         *
         * @param data_dim [in]; Dimensionality of the input
         * data. This must match the dim of the underlying Gaussians.
         *
         * @param class_lkhd [out]; Vector containing likelihoods of the input vector for each class
         * @return Result indicating success or failure
         */
        Result log_likelihood_nonormal (const double * data, unsigned data_dim,
                                        Vector<double> * class_lkhd);

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point.
         *
         * The output vector encodes the log-likelihood of the input
         * vector for each class in the mixture. The input
         * data's dimension must match the dimension of the underlying
         * Gaussians.
         *
         * @param data [in]; Input data vector
         * @param class_lkhd [out]; Vector containing likelihoods of the input vector for each class
         * @return Result indicating success or failure.
         */
        Result log_likelihood (const Vector<double> & data, 
                               Vector<double> * class_lkhd);

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        data point without normalization.
         *
         * The output vector encodes the log-likelihood of the input
         * vector for each class in the mixture. The input
         * data's dimension must match the dimension of the underlying
         * Gaussians.
         *
         * @param data [in]; Input data vector
         * @param class_lkhd [out]; Vector containing likelihoods of the input vector for each class
         *
         * @return Result indicating success or failure.
         */
        Result log_likelihood_nonormal (const Vector<double> & data, 
                                        Vector<double> * class_lkhd);

        /**
         * @brief Returns the log-likelihood corresponding to the given
         *        set of data points.
         *
         * @param data [in]; A set of data vectors provided in a
         * matrix.  The data vectors are stored in the rows.
         * @param class_lkhd [out]; Matrix containing a set of probabilities as
         *                          in other log_likelihood functions.  However,
         *                          each row of class_lkhd corresponds to the
         *                          output for the corresponding input row of
         *                          data.
         * @return Result indicating success or failure.
         */
        Result log_likelihood (const Matrix<double> & data, 
                               Matrix<double> * class_lkhd);

    public: // File loading.
        /** 
         * @brief Loads a ClassMixture model from a file.
         *
         * @param filename [in]; Filename from which to read the class_mixture.
         * @return Result indicating success or failure.
         */
        Result read_class_mixture (const char * filename);

        /** 
         * @brief Saves a ClassMixture model to a file.
         *
         * @param filename [in]; Filename to which to write the class_mixture.
         * @return Result indicating success or failure.
         */
        Result write_class_mixture (const char * filename);

    protected: // Helpers.
        /** 
         * @brief Normalizes the GaussianMixture mixture likelihood.
         *
         * @param class_lkhd [inout]; Vector to be normalized
         * @return Result indicating success or failure.
         */
        Result normalize_lkhd (Vector<double> * class_lkhd);

        /** 
         * @brief Saves a ClassMixture using a Configuration object.
         * 
         * This function is used as a helper for write_class_mixture(). 
         *
         * @param config [in]; Configuration element 
         * @param class_mixture_key [in]; ConfigKey element
         * @return Result indicating success or failure.
         *
         * @see Configuration, ConfigKey.
         */
        Result write_class_mixture (Configuration config, 
                                    ConfigKey class_mixture_key);

        /**
         * @brief Initializes all contained variables to the specified dimension.
         *        Defaults to canonical 1 dimensional functions.
         * @param class_dim [in]; Number of classes (GaussianMixtures) in the ClassMixture
         * @param mixt_dim  [in]; Number of Gaussians in GaussianMixtur.
         * @param dim       [in]; Dimension of individual Gaussians
         */
        void init (unsigned class_dim = 1, unsigned mixt_dim = 1, unsigned dim = 1)
        {
            PRECOND (class_dim > 0);
            PRECOND (dim > 0);

            _class_dim = class_dim;
            _dim = dim;
            _class_prior.resize (1.0/((double)_class_dim), _class_dim);
            _log_class_prior.resize (evo_log(1.0/((double)_class_dim)), 
                                     _class_dim);
            _classes.clear ();
            _classes.resize (_class_dim);
            for (unsigned i = 0; i < _class_dim; i++) 
            {
                _classes[i].set_mixt_dim (mixt_dim);
                _classes[i].set_dim (_dim);
            }
        }
        
        /**
         * @brief Checks that the prior adds to 1.0, otherwise normalize.
         */
        void check_prior ();
        
    protected: // Implementation.

        /**
         * The internal representation.
         */
        unsigned       _class_dim;        ///< Number of classes in the mixture
        unsigned       _dim;              ///< Dimensionality of the Gaussian functions
        Vector<double> _class_prior;      ///< Prior distribution on the classes
        Vector<double> _log_class_prior;  ///< Log of prior distribution on the classes
        ClassVector    _classes;          ///< Vector of mixtures of Gaussian
    }; // end class ClassMixture.

} // end namespace Evolution.

// External operators.
/**
 * @brief Print the contents of a Mixture of GaussianMixtures
 * (ClassMixture) to the specified stream.
 *
 * @param out [in]; Stream to which to print the contents of the ClassMixture.
 * @param c [in]; ClassMixture whose contents will be printed to the stream.
 *
 * @return Stream reference given in the out parameter.
 */
inline std::ostream& operator<< (std::ostream& out, const Evolution::ClassMixture & c)
{
    unsigned C = c.get_class_dim ();
    unsigned D = c.get_dim ();
    out << "{ ";
    out << "number of classes: " << C << "\n"; 
    for (unsigned k = 0; k < C; k++)
    {
        out << "\t{ ";
        out << "class prior: " << c.get_prior_c (k) << "\n";

        unsigned M = c.get_mixture_dim (k);
        out << "\t\t{ ";
        out << "number of components: " << M << "\n"; 
        for (unsigned m = 0; m < M; m++)
        {
            out << "\t\t\t{ ";
            out << "mixture weight: " << c.get_mixt_weight_m (k, m) << "\n";
            
            out << "\t\t\t\t{ ";
            out << "dimension: " << D << "\n"; 
            out << "\t\t\t\tmean vector: ( ";
            for (unsigned i=0; i < D-1; i++)
            {
                out << c.get_mean_i (k, m, i) << ", ";
            }
            out << c.get_mean_i (k, m, D-1) << " )\n";
            
            out << "\t\t\t\tcovariance matrix: ( ";
            for (unsigned i=0; i < D; i++)
            {
                out << "( ";
                for (unsigned j=0; j < D-1; j++)
                {
                    out << c.get_sigma_ij (k, m, i, j) << ", ";
                }
                out << c.get_sigma_ij (k, m, i, D-1) << " )";
            }
            out << " )\n";
            
            out << "\t\t\t\t}\n";
            
            out << "\t\t\t}\n";
        }
        out << "\t\t}\n";
        out << "\t}\n";
    }
    return out << "}";
}

#endif  // INCLUDE_EVOLUTION_MATH_CLASS_MIXTURE_HPP
