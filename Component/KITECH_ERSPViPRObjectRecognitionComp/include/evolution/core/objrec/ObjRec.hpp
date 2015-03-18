//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.

#ifndef INCLUDE_EVOLUTION_CORE_OBJREC_OBJ_REC_HPP
#define INCLUDE_EVOLUTION_CORE_OBJREC_OBJ_REC_HPP

/**
 * @file    evolution/core/objrec/ObjRec.hpp
 *
 * @brief The file ObjRec.hpp contains the declarations of the classes
 * that define the public interface of the Object Recognition
 * library. Three classes are declared in this file: ObjRecKeypointList,
 * ObjRecDatabase, and ObjRecQuery. These classes provide the
 * infrastructure for a complete recognition system. The
 * ObjRecKeypointList class provides the signal processing front-end to
 * the system. The ObjRecDatabase handles training of the system. The
 * ObjRecQuery class is the recognition back-end of the system.
 *
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @date    2003/03/19
 */

#include <vector>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/objrec/PositCoplanar.hpp>

namespace Evolution
{
    // Forward declarations
    class Image;
    class Vector2;
    class CameraParameters;

    // Additional Types
    /**
     * @brief
     * A position on the screen in pixels measured from the image origin
     * (upper-left corner)..
     *
     * @ingroup core_objrec
     * @ingroup vision_objrec
     */
    struct FeaturePoint
    {
        float x;  ///< x coordinate measured from the origin: upper-left corner
        float y;  ///< y coordinate measured from the origin: upper-left corner
    };

    /// List of feature points from a particular object.
    typedef std::vector<FeaturePoint> FeaturePointList;

    /// List of user data from a particular object.
    typedef std::vector<int> FeatureUserDataList;

    /// List of points that define a polygon that will enclose features
    typedef std::vector<Vector2> Polygon;

    // Opaque types.
    class ObjRecKeypointListImpl;
    class ObjRecDatabaseImpl;
    class ObjRecQueryImpl;

    /**
     * @brief This class implements the image processing front-end to
     * the object recognition system. It encompasses image processing
     * methods that allow for the extraction of salient structures or
     * feature points from images. (This class is derived from the
     * @see ObjectImpl class, a generic class that handles reference
     * counting and thread safety issues.)
     *
     * @ingroup core_objrec
     * @ingroup vision_objrec
     */
    class EVOLUTION_EXPORT_OBJREC ObjRecKeypointList : public ObjectImpl
    {
    public: // Constants
        /// Interface ID for the class.
        /// @see obtain_interface ()
        static const char *INTERFACE_ID;

    public: // Structors
        /// Constructor.
        ObjRecKeypointList ();

    public: // IObject Interfaces
        /**
         * @brief
         * IObject, ObjectImpl interface.
         * This function obtains the specified interface from the
         * instance of the class if the instance supports the given
         * interface ID and the caller's security ticket has access to
         * it.
         * @see IObject::obtain_interface (), ObjectImpl::obtain_interface ()
         *
         * @param  ticket        [in];  security ticket (a placeholder)
         * @param  interface_id  [in]; the interface ID of the requested
         *   interface.  The only interface this class supports is
         *   defined by ObjRecKeypointList::INTERFACE_ID.
         * @param  interface_ptr [out]; the pointer to the interface,
         *   non-NULL if the call succeeds.
         *
         * @return  RESULT_SUCCESS,
         *          RESULT_NOT_FOUND,
         *          RESULT_INSUFFICIENT_ACCESS,
         *          or another context-dependent error.
         */
        Result obtain_interface (TicketId ticket, const char* interface_id,
                                 void** interface_ptr);

    public: // Image processing
        /**
         * @brief
         * Creates a new list of features for the given image.
         * An instance of this class should not be reused for different
         * images. You should create a new instance for every incoming
         * image.
         * (You can only call 'extract' once per lifetime of the object.)
         *
         * @param image_data  [in]; the image.
         * @param width       [in]; image width.
         * @param height      [in]; image height.
         * @param rowstride   [in]; number of bytes per row of the image.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result extract(const unsigned char *image_data,
                       unsigned             width,
                       unsigned             height,
                       unsigned             rowstride);

        /**
         * @brief *** Deprecated *** use extract () instead
         * @deprecated
         */
        Result hash(const unsigned char *image_data,
                    unsigned             width,
                    unsigned             height,
                    unsigned             rowstride)
        {
            return extract (image_data, width, height, rowstride);
        }

        /**
         * @brief
         * Creates a new list of features for the given image.
         * An instance of this class should not be reused for different
         * images. You should create a new instance for every incoming
         * image.
         * (You can only call 'extract' once per lifetime of the object.)
         *
         * @param image_data  [in]; the image.
         * @param width       [in]; image width.
         * @param height      [in]; image height.
         * @param rowstride   [in]; number of bytes per row of the image.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result extract(const short         *image_data,
                       unsigned             width,
                       unsigned             height,
                       unsigned             rowstride);

        /**
         * @brief
         * Creates a new list of features for the given image.
         * An instance of this class should not be reused for different
         * images. You should create a new instance for every incoming
         * image.
         * (You can only call 'extract' once per lifetime of the object.)
         *
         * @param image            [in]; the image.
         * @param pass_image_copy  [in]; whether to retain a copy of the
         *                         image internally (defaults to false).
         *                         If a copy is made, it may be accessed
         *                         with get_image().
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result extract(const Image         *image,
                       bool                 pass_image_copy = false);

        /**
         * @brief *** Deprecated *** use extract () instead
         * @deprecated
         */
        Result hash(const Image         *image,
                    bool                 pass_image_copy = false)
        {
            return extract (image, pass_image_copy);
        }

    public: // Set methods
        /**
         * @brief Specifies the distance from the camera to the object
         * at the moment in which the image was taken.
         *
         * @param distance [in]; distance from camera to the object in
         * the image.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_distance(float distance);

        /**
         * @brief
         * Specifies the units of the distance from the camera to the
         * object at the moment in which the image was
         * taken. @see set_distance.
         *
         * @param units       [in]; units of distance.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_distance_units(const String &units);

        /**
         * @brief
         * Specifies the distance from the camera to the object
         * at the moment in which the image was taken.
         *
         * @param distance   [in]; distance from camera to the object in
         *                   the image.
         * @param units      [in]; units of the distance.
         *
         * @return           Result indicating success or the reason
         *                   for failure.
         */
        Result set_distance (const float distance, const String &units);

        /**
         * @brief Sets the optional raw image. After a call to this
         * function with a non-NULL image, an internal copy of the
         * image is stored in the ObjRecKeypointList.
         *
         * @param image   [in]; the new raw image (may be NULL).
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_image(const Image *image);

        /**
         * @brief
         * Sets the optional timestamp of the image.
         *
         * @param timestamp   [in]; the timestamp when the image was taken.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_timestamp(Timestamp timestamp);

        /**
         * @brief Sets the parameters of the camera at the time the image was taken.
         *
         * Only the pointer is copied, so if the parameters will be
         * changed externally (for example via a pan-tilt head) they
         * should be copied by the caller.
         *
         * @param params [in]; A pointer to the camera parameters used
         * to take the picture that these keypoints came from.
         */
        void set_camera_parameters( CameraParameters *params );

    public: // Get methods
        /**
         * @brief
         * Gets the processing state, true if ready
         */
        bool get_results_ready() const;

        /**
         * @brief
         * Pause until the keypoints are fully ready to be used.
         * This returns false if an error occurs.
         */
        bool wait_until_ready() const;

        /**
         * @brief
         * For debugging, get the state of the keypoint-list.
         */
        const char *get_state_name() const;


        /**
         * @brief
         * Gets the distance from the camera to the object.
         *
         * @return distance from the camera to the object.
         */
        float get_distance() const;

        /**
         * @brief
         * Gets the units of the distance from the camera to the object.
         *
         * @return units of distance.
         */
        const char *get_distance_units() const;

        /**
         * @brief
         * Gets the distance from the camera to the object, also obtains the
         * associated units.
         *
         * @param distance   [out]; stored distance from camera to the
         * object in the image.
         * @param units      [out]; units of the distance.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_distance (float *distance, String *units) const;

        /**
         * @brief
         * Provides the width of the image.
         *
         * @return width of the image.
         */
        unsigned get_image_width() const;

        /**
         * @brief
         * Provides the height of the image.
         *
         * @return the height of the image.
         */
        unsigned get_image_height() const;

        /**
         * @brief
         * Provides number of features extracted from the given image.
         *
         * @return the size of the hash table.
         */
        unsigned get_num_features() const;

        /**
         * @brief *** Deprecated *** use get_num_features() instead
         * @deprecated
         */
        unsigned get_hash_size() const {return get_num_features ();}

        /**
         * @brief
         * Obtain the optional raw image (which can be used for debugging
         * and display purposes).
         *
         * @return pointer to the internally stored image.
         */
        const Image *get_image() const;

        /**
         * @brief *** Deprecated *** use get_image() instead
         * @deprecated
         */
        const Image *peek_image() const {return get_image ();}

        /**
         * @brief
         * Obtain the timestamp of the given image.
         * (if 0, it is not necessarily valid).
         *
         * @return the stored timestamp.
         */
        double get_timestamp() const;

        /**
         * @brief
         * Obtain the position of all the feature points.
         * These are x,y locations in pixels.
         *
         * @param points [out]; pointer to an array of FeaturePoints of
         *                      size equal to the number of features
         *                      in the image.
         *
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         *
         * @see FeaturePoint, FeaturePointList
         *
         */
        Result get_feature_points (FeaturePointList *points) const;

        /**
         * @brief Gets the parameters of the camera at the time the image was taken.
         *
         * @return A pointer to the camera parameters used
         * to take the picture that these keypoints came from.
         */
        const CameraParameters *get_camera_parameters()
        {
            return _camera_params;
        }

    public: // Feature points edition
        /**
         * @brief 
         * Remove features from the current list.
         *
         * @param polygon   [in]; Enclosing hull of features to be removed.
         *
         * @return            Result indicating success or the reason 
         *                    for failure.
         *
         *
         */
        Result remove_features (Polygon &polygon);

        /**
         * @brief 
         * Select features enclosed by the polygon 
         *
         * @param polygon   [in]; Enclosing hull of features to be selected.
         * @param features [out]; New ObjRecKeypointList composed of the 
         *                        selected features.
         *
         * @return            Result indicating success or the reason 
         *                    for failure.
         *
         * @see Polygon
         *
         */
        Result select_features (Polygon &polygon, 
                                ObjRecKeypointList *features) const;

    public: // Parameter Setting
        /**
         * @brief
         * Method that allows to set the value of various parameters.
         *
         * @param flag_name   [in]; Name of the flag to be set, available 
         *                          flags are:
         *  @li @c feature-quality  (0.0 ... 1.0) default: 0.85 @n
         *         Defines a threshold in the local prominence of a feature.
         *  @li @c feature-strength (0.0 ... 1.0) default: 1.0 @n
         *         Defines a threshold in the amplitude of a feature.
         *  @li @c use-upsampling-method (true - false) default: true @n
         *         Selects whether to double the resolution of the image 
         *         before extracting features.
         *  @li @c max-num-features (0 ... ) default: 0 @n
         *         Limits the number of features to be extracted from the 
         *         given image. Setting this parameter to 0 (default value) 
         *         means that there is no limit in the number of features to be
         *         extracted. 
         *  @li @c feature-type ("default" - "small") default: "default" @n
         *         Selects the type of descriptor to be used for the features.
         *         The "default" descriptor type corresponds to the original
         *         implementation of the system. The "small" descriptor is a
         *         compact and optimized version of the original one.
         *
         * Example:
         * @code
         * ...
         * // This snippet of code shows how to set a value for the
         * // 'feature-quality' parameter.
         * ObjRecKeypointList * kpt = new ObjRecKeypointList();
         * // Setting flags for the ObjRecKeypointList
         * if (kpt->set_flag ("feature-quality", 0.8) != RESULT_SUCCESS)
         *     std::cerr << "Failed to set 'feature-quality' flag\n";
         * ...
         * @endcode
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_flag(const char *flag_name, ...);

        /**
         * @brief
         * Method that allows to get the value of various parameters. See the
         * description of the parameters in the @see set_flag() method.
         *
         * @param flag_name   [in]; Name of flag whose value will be provided,
         *                    available flags are:
         *  @li @c feature-quality  (0.0 ... 1.0)
         *  @li @c feature-strength (0.0 ... 1.0)
         *  @li @c use-upsampling-method (true - false)
         *  @li @c max-num-features (0 ... )
         *  @li @c feature-type ("default" - "small") 
         * @param rv          [out]; output value
         *
         * Example:
         * @code
         * ...
         * // This snippet of code shows how to get the current value of the
         * // 'feature-quality' parameter.
         * ObjRecKeypointList * kpt = new ObjRecKeypointList();
         * // Getting default values for parameters of the ObjRecKeypointList
         * double fq;
         * if (kpt->get_flag ("feature-quality", &fq) != RESULT_SUCCESS)
         *     std::cerr << "Failed to get 'feature-quality' flag\n";
         *
         * std::cout << "Default value of 'feature-quality' is " << fq << "\n";
         * ...
         * @endcode
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_flag (const char *flag_name, void *rv) const;

    public: // Serialization
        /**
        * @brief 
        * Method that dumps to memory all the information stored in
        * the ObjRecKeypointList.  The memory buffer will be allocated by the
        * system, thus there is no need for the user to pre-allocate the buffer.
        * It is the user's responsibility to deallocate the memory with stdlib's
        * free().
        *
        * @param data [out]; Pointer to the buffer where the information was
        *                    stored.
        * @param size [out]; Size of memory block.
        *
        * @return        Result indicating success or the reason 
        *                for failure.
        */
        Result save_to (void **data, unsigned int *size);

        /**
        * @brief
        * Method that loads from memory all the information for
        * the ObjRecKeypointList.
        * 
        * @param data    [in]; Pointer to memory to read
        * @param size    [in]; Size of memory block.
        *
        * @return        Result indicating success or the reason 
        *                for failure.
        */
        Result load_from(void *data , unsigned int size);

    public:
#ifndef DOXYGEN_INTERNAL
        /* ready-notifications, for nonblocking use */
        typedef void (*HandleAvailable) (Result              result,
                                         ObjRecKeypointList *list,
                                         void               *callback_data);
#endif

    protected: // Friend classes
        /// Friend class.
        /// @see Evolution::ObjRecQuery
        friend class ObjRecQuery;
        /// Friend class.
        /// @see Evolution::ObjRecQuery
        friend class ObjRecDatabase;
        /// Internal implementation.
        friend class ObjRecDatabaseImpl;
        /// Internal implementation.
        friend class ObjRecInternals;

    private: // Implementation
        ObjRecKeypointListImpl *_implementation; ///< Internal implementation.
        const Image *_image;           ///< Optional image storage.
        double _timestamp;             ///< Optional timestamp storage.
        CameraParameters *_camera_params; ///< Parameters from the time the source image was taken.

    private: // Internal functions
#ifndef DOXYGEN_INTERNAL
        static void handle_kp_list_creation_completed (Result, void *);
        static void handle_set_image_completed (Result, void *);
#endif

    protected:
        /// Destructor.
        ~ObjRecKeypointList();
    }; // end class ObjRecKeypointList

    /// Backward compatibility
    typedef ObjRecKeypointList ObjRecImageHash;



    /**
     * @brief Class in charge of the creation, edition, loading, and
     * saving of databases of objects.  The ObjRecDatabase class takes
     * the set of features stored by the ObjRecKeypointList and places
     * the object in a new database or adds it to an existing one. The
     * ObjRecDatabase class is able to provide information on the
     * various objects stored in a given database and is also able to
     * erase objects from the database. This class is derived from the
     * ObjectImpl class, a generic class that handles reference
     * counting and thread safety issues. Each model in the database
     * is identified by two "unique" elements: a label string provided
     * by the user of the class at model creation time and an ID
     * number that is created internally once the model is added to
     * the database. The user should refer to the model using either
     * identifier (the model ID number can be queried to the database
     * given the label).
     *
     * @ingroup core_objrec
     * @ingroup vision_objrec
     */
    class EVOLUTION_EXPORT_OBJREC ObjRecDatabase : public ObjectImpl
    {
    public: // Types
        /**
         * @brief Model information structure: Each object in the
         * database is represented by a unique "model" that collects
         * all the information for the object. This structure provides
         * an interface to obtain all the pieces of information stored
         * for the object.
         */
        struct ModelInfo
        {
            unsigned num;       /**< Model ID number: this ID is
                                 *  assigned by the database at the
                                 *  time in which the model is
                                 *  added. The number is unique and is
                                 *  always positive. Caveat: if
                                 *  numbers might not be consecutive
                                 *  due to addition/deletion of
                                 *  models, use the model label as the
                                 *  main id for models. */

            String label;       /**< Model label assigned by the user
                                 * at the time of training. */

            long unsigned group; /**< Group ID assigned by the user at
                                  * the time of training. */

            float distance;     /**< Distance between the camera and
                                 * the object at the time capture of
                                 * the training image. */

            String units;       /**< Units of distance. */
            unsigned num_features; /**< Number of features comprising the
                                    * model. */

            unsigned width;     /**< Width of the original image used to
                                   create the model */
            unsigned height;    /**< Height of the original image used to
                                   create the model */

            /**
             * @brief Prints the contents of the ModelInfo to the specified
             *        stream.
             *
             * @param out [in]; Stream to which to print the contents of ModelInfo.
             * @param info [in]; ModelInfo whose contents will be printed to the stream.
             *
             * @return Stream reference given in the out parameter.
             */
            EVOLUTION_EXPORT_OBJREC friend std::ostream&
            operator<< (std::ostream& out,
                        const Evolution::ObjRecDatabase::ModelInfo & info);
        };

        /// List of model information structs (used for obtaining
        /// information on all the models present in the database).
        typedef std::vector<ModelInfo> ModelInfoList;

    public: // Constants
        /// Interface ID for the class.
        /// @see obtain_interface ()
        static const char *INTERFACE_ID;

    public: // Structors
        /**
         * @brief
         * Default constructor; creates an anonymous database.
         */
        ObjRecDatabase();

        /**
         * @brief This function provides a reference to a global
         * database. If the database is not loaded in memory yet, this
         * method loads/creates a new one. An internal list of all the
         * global databases is kept in memory and the databases are
         * indexed by the file name. Using get_by_filename() is
         * encouraged in the case in which a set of databases are
         * going to be shared by various modules. However, using
         * get_by_filename() is discouraged in the case in which a
         * database pointing to the same file would be created and
         * destroyed a few times in the process of operation of the
         * program. In this later case, it is advisable to create the
         * database with the default constructor and perform loading
         * using load_from().
         *
         * @param filename      [in]; name of the file containing the database.
         * @param maybe_create  [in]; if true and the database does not exist,
         *                      a new database is created.
         * @param result_out    [out]; returns a Result indicating
         * success or the reason for failure.
         *
         * @return pointer to the ObjRecDatabase object or NULL in
         * case of failure.
         */
        static ObjRecDatabase *get_by_filename(const char *filename,
                                               bool maybe_create = false,
                                               Result* result_out = NULL);
#ifndef DOXYGEN_INTERNAL
        static ObjRecDatabase *try_by_filename(const char *filename);
        static void set_by_filename(const char *filename, ObjRecDatabase *database);
#endif

    public: // IObject Interfaces
        /**
         * @brief
         * IObject, ObjectImpl interface.
         * This function obtains the specified interface from the
         * instance of the class if the instance supports the given
         * interface ID and the caller's security ticket has access to
         * it.
         * @see IObject::obtain_interface (), ObjectImpl::obtain_interface ()
         *
         * @param   ticket        [in];  security ticket (a placeholder)
         * @param interface_id    [in]; the interface ID of the requested
         *   interface.  The only interface this class supports is defined
         *   by ObjRecDatabase::INTERFACE_ID.
         * @param interface_ptr   [out]; the pointer to the interface,
         *   non-NULL if the call succeeds.

         * @return  RESULT_SUCCESS,
         *          RESULT_NOT_FOUND,
         *          RESULT_INSUFFICIENT_ACCESS,
         *          or another context-dependent error.
         */
        Result obtain_interface (TicketId ticket, const char* interface_id,
                                 void** interface_ptr);

    public: // Serialization
        /**
         * @brief Saves the current database to file. This function
         * should be used only if the database was created using the
         * get_by_filename() method. In case that the database was
         * created using the default constructor, the function
         * save_to() must be used.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result save() const;

        /**
         * @brief Saves the current database to a stream. The stream
         * could be a file stream, a memory stream, etc.
         *
         * @param os          [in]; output stream where the database will be
         *                    saved.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result save_to(std::ostream &os) const;

        /**
         * @brief
         * Loads the current database from a stream. The stream
         * could be a file stream, a memory stream, etc.
         *
         * @param is          [in]; input stream from which to load the
         *                    database.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result load_from(std::istream &is);

        /**
         * @brief Loads the current database from a memory pointer.
         *
         * The stream could be a file stream, a memory stream, etc.  
         *
         * @param data    [in]; Pointer to memory to read
         * @param size    [in]; Size of memory block.
         *
         * @return        Result indicating success or the reason 
         *                for failure.
         */
        Result load_from(void * data , unsigned int size);

        /**
         * @brief Saves the current database to file. If the database
         * was created using the get_by_filename () method, this
         * function might be used to save the database to a different
         * file.
         *
         * @param filename    [in]; name of the file where the database
         *                    will be saved.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result save_to(const char *filename) const;

        /**
         * @brief Quickly load a list of database files to the current
         * database.
         * Note that this is NOT EQUIVALENT to calling load_from() iteratively
         * on the list of files.  The difference is that the merge key
         * lists are read directly from the files and are not connected
         * together.
         *
         * @param file_list    [in]; A list of filenames to load databases
         *                     from.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result load_file_list_fast(std::vector<std::string> &file_list);

        /**
         * @brief Loads the current database from file. This function
         * should be used to load a database from file in the case
         * that the database has been created using the default
         * constructor. This function might be also used to append a
         * database to a currently loaded one.
         *
         * @param filename    [in]; name of the file where the database
         *                    is stored.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result load_from(const char *filename);

        /**
         * @brief Saves the specified group of models from the
         * database to file. More than one group could be saved at
         * once by specifying the different ids of the groups ORed
         * together. However, no group information would be saved on
         * the database file (all group information would be lost).
         *
         * @param filename    [in]; name of the file where the database
         *                    will be saved.
         * @param group       [in]; id of the desired group of models
         *                    to be saved.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result save_group(const char *filename, long unsigned group) const;

        /**
         * @brief Loads a database file into the current ObjRecDatabase
         * and assigns the given group id to all loaded
         * models. This function should be used to load a database
         * from file in the case that the database has been created
         * using the default constructor.
         *
         * @param filename    [in]; name of the file where the database
         *                    is stored.
         * @param group       [in]; id of the desired group of models
         *                    to be loaded.
         * @param no_merge    [in] If true, don't merge merge key lists
         *                    across loaded databases
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result load_group(const char *filename, long unsigned group,
                          bool no_merge=false);

    public: // Database information retrieval
        /**
         * @brief
         * Provides the number of models in the database.
         *
         * @return  number of models in the database.
         */
        unsigned get_num_models() const;

        /**
         * @brief Provides the number of groups in the database. The
         * default number of groups is 1 unless there is no model
         * loaded on the database. All models belong to the group
         * 0x00000000 by default.
         *
         * @return  number of groups in the database.
         */
        unsigned get_num_groups () const;

        /**
         * @brief Provides the number of models in a group. A return
         * value of 'zero' implies that this group does not exists in
         * the database.
         *
         * @param group       [in]; id of the group whose number
         *                          of models has been requested
         *
         * @return  number of models in a group
         */
        unsigned get_num_group_models (long unsigned group) const;

        /**
         * @brief Provides the total number of features stored in the
         * database (in other words, it gives the number of all
         * features in all models stored in the database).
         *
         * @return number of features comprising all the models.
         */
        unsigned get_total_num_features () const;

        /**
         * @brief *** Deprecated *** use get_total_num_features() instead
         * @deprecated
         */
        unsigned get_total_hash_size() const
        {
            return get_total_num_features ();
        }

        /**
         * @brief
         * Queries the database for all the models.
         *
         * @param models [out]; pointer to a ModelInfoList
         * buffer. This buffer will be internally resized within the
         * function if more storage space is needed. However, if there
         * is enough space allocated, the buffer will NOT be shrunk
         * (if the buffer will be reused to obtain information from
         * various databases, it would be a good idea to clear()
         * the buffer before calling this function).
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_all_models(ModelInfoList *models) const;

        /**
         * @brief
         * Queries the database for information about a model by name.
         *
         * @param label       [in]; label of the model.
         * @param out         [out]; pointer to a ModelInfo structure.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_model_info_by_label(const String &label,
                                       ModelInfo    *out) const;

        /**
         * @brief Queries the database for information about a model
         * by using its unique ID number.
         *
         * @param number      [in]; Number ID of the model (always positive
         *                    and is never reused).
         * @param out         [out]; Pointer to a ModelInfo structure.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_model_info_by_number(unsigned      number,
                                        ModelInfo    *out) const;

        /**
         * @brief
         * Obtain the position of all the feature points in a given model.
         * These are x,y locations in pixels.
         *
         * @param number [in];   Number ID of the model (always positive
         *                       and is never reused)
         * @param points [out];  Pointer to an array of ModelInfo::hash_size
         *                       FeaturePoints.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         *
         * @see FeaturePoint, FeaturePointList
         *
         */
        Result get_model_feature_points(unsigned      number,
                                        FeaturePointList *points) const;

        /**
         * @brief
         * Changes the label of a model in the database.
         *
         * @param number      [in]; Number ID of the model (always positive
         * and is never reused).

         * @param label       [in]; New label for the the model.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_model_label (unsigned number, const String &label);

        /**
         * @brief Changes the distance between the camera and the
         * object in the corresponding model in the database.
         *
         * @param number      [in]; Number ID of the model (always positive
         * and is never reused).
         * @param dist        [in]; Distance between the camera and the
         *                    corresponding object.
         * @param units       [in]; Units of distance.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_model_distance (unsigned number, float dist,
                                   const char *units);

        /**
         * @brief
         * Changes the group label of a model in the database.
         *
         * @param model_label  [in]; model label.
         *
         * @param group [in]; new group id for the the model, this
         * group id would replace the previously existing one.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_model_group (const String &model_label,
                                long unsigned group);

    public: // add/query/remove hashes from the database
        /**
         * @brief Queries the database for existence of a model with
         * the given label.
         *
         * @param label    [in]; label of the model that we are looking for.
         *
         * @return              boolean value indicating whether the
         *                      model exists in the database.
         */
        bool has_model_by_label (const String & label) const;

        /**
         * @brief *** Deprecated *** use has_model_by_label () instead
         * @deprecated
         */
        bool has_hash_by_name(const String & label) const
        {
            return has_model_by_label (label);
        }

        /**
         * @brief Adds a new model to the database with the
         * corresponding label. This step corresponds to training on
         * the set of features representing the new object to be added
         * to the database. The label has to be unique in the
         * database, if the given label is already in use in the
         * database, add_model() will fail.
         *
         * @param label          [in]; label for the model that will be
         *                             added to to the database.
         * @param image_features [in]; ObjRecKeypointList to be added to the
         *                             database.
         * @param group          [in]; group id of the model that will be
         *                             added to the database (it might include
         *                             several groups ids ORed together).
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result add_model (const String & label,
                          ObjRecKeypointList *image_features,
                          unsigned long group = 0);

        /**
         * @brief *** Deprecated *** use add_model () instead
         * @deprecated
         */
        Result add_hash(const String & label, ObjRecImageHash *image_hash,
                        unsigned long group = 0)
        {
            return add_model (label, image_hash, group);
        }

        /**
         * @brief *** Deprecated *** use add_model () instead
         * @deprecated
         */
        Result add_model_to_group (const String & label,
                                   ObjRecImageHash *image_hash,
                                   long unsigned group)
        {
            return add_model (label, image_hash, group);
        }

        /**
         * @brief
         * Removes a model from the database.
         *
         * @param label    [in]; label of the model that will be removed
         *                       from the database.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result remove_model (const String & label);

        /**
         * @brief *** Deprecated *** use remove_model () instead
         * @deprecated
         */
        Result remove_hash(const String & label)
        {
            return remove_model (label);
        }

        /**
         * @brief Removes a group or groups from the database.
         * All models which are solely in the groups deleted
         * are also deleted.
         * Models existing in other nonzero groups will not be deleted.
         * (This behavior is inconsistent:  that is why groups are deprecated)
         *
         * @param group   [in]; id of the group that will be removed
         *                      from the database.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result remove_group (long unsigned group);

    public:  // State information.
        /**
         * @brief
         * For debugging, get the state of the keypoint-list.
         */
        const char *get_state_name() const;

        /**
         * @brief
         * Gets the processing state, true if ready
         */
        bool get_results_ready() const;

        
        /**
         * wait until the database is ready.
         * not recommended: wait on the query instead.
         * mostly valuable for timing tests.
         */
        bool wait_until_ready() const;

    public:  // Parameter setting

        /**
         * @brief
         * Method that allows to set the value of various parameters.
         * @see ObjRecKeypointList::set_flag()
         *
         * @param flag_name   name of flag to be set, available flags are: @n
         *  @li @c feature-quality  (0.0 ... 1.0) default: 0.85 @n
         *         Defines a threshold in the local prominence of a feature. 
         *         This parameter allows to select a smaller, more restrictive
         *         value for 'feature-quality' that the one that was used for
         *         feature extraction by the ObjRecKeypointList.
         *  @li @c feature-type ("default" - "small") default: "default" @n
         *         Selects the type of descriptor to be used for the features
         *         in the database (features of dissimilar types cannot be
         *         mixed).
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_flag (const char *flag_name, ...);

        /**
         * @brief
         * Method that allows to get the current value of various parameters.
         * @see set_flag()
         *
         * @param flag_name   [in]; name of flag whose value will be provided,
         *                    available flags are:
         *  @li @c feature-quality  (0.0 ... 1.0)
         *  @li @c feature-type ("default" - "small") 
         *  @li @c keypoint-list-for-model-by-number @n
         *         Allows to obtain the underlying ObjRecKeypointList for a
         *         particular model of the database. The syntax of the flag
         *         must be "keypoint-list-for-model-by-number:x" where 'x'
         *         is the unique model number provided on the ModelInfo 
         *         structure (field num of the structure).
         * @param rv          [out]; output value
         *
         * Example:
         * @code
         * ...
         * // This snippet of code shows how to extract the ObjRecKeypointList
         * // corresponding to a particular model in an ObjRecDatabase
         *
         * // Load a database from file
         * ObjRecDatabase * db = new ObjRecDatabase();
         * if (db->load_from("my_database_file_name.mdl") != RESULT_SUCCESS)
         *     std::cerr << "Failed to load database.\n";
         *
         * // Get information on all the models in the database
         * ObjRecDatabase::ModelInfoList info_list;
         * if (db->get_all_models (info_list) != RESULT_SUCCESS)
         *     std::cerr << "Failed to get model information.\n";
         *
         * // Getting the ObjRecKeypointList corresponding to the first model
         * char flag_name[1024];
         * ObjRecKeypointList *kp_list_out = NULL;
         * sprintf(flag_name, "keypoint-list-for-model-by-number:%d", 
         *         info[0].num);
         * if (db->get_flag(flag_name, &kp_list_out) != RESULT_SUCCESS)
         * {
         *     std::cerr << "Failed to obtain the ObjRecKeypointList for model"
         *               << info[0].label << "\n";
         * }
         * ...
         * @endcode
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_flag (const char *flag_name, void *rv) const;

#ifndef DOXYGEN_INTERNAL
        // internal use only
        Result save_group_with_writer(void *writer_ptr, long unsigned group) const;
        Result load_group_from_reader(void *reader_ptr, long unsigned group,
                                      bool no_merge=false);
#endif

        /**
         * @brief Function that forces the ObjRecDatabase to build the
         * internal search structure used for recognition. If this
         * function is not called, the search structure will be built
         * automatically on the first recognition request.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result maybe_rebuild();

    protected: // Friend classes
        /// Friend class.
        /// @see Evolution::ObjRecQuery
        friend class ObjRecQuery;
        /// Internal implementation.
        friend class ObjRecInternals;

    protected: // Implementation
        ObjRecDatabaseImpl *_implementation; ///< Internal implementation.

#ifndef DOXYGEN_INTERNAL
        // 'group' must be a single bit in each of these cases
        Result remove_one_group (long unsigned group);
        Result add_one_group (long unsigned group);
#endif
    protected:
        /// Destructor.
        ~ObjRecDatabase();
    }; // end class ObjRecDatabase

    /**
     * @brief Class that recognizes incoming images and provides
     * information on the resulting matches.If the image is found
     * among the objects present in the database, the class provides
     * information on the match (location, affine distortion, and
     * bounding box of the match). This class is derived from the
     * ObjectImpl class, a generic class that handles reference
     * counting and thread safety issues.
     *
     * @ingroup core_objrec
     * @ingroup vision_objrec
     */
    class EVOLUTION_EXPORT_OBJREC ObjRecQuery : public ObjectImpl
    {
    public: // Types
        /**
         * @brief Match information structure: This structure provides
         * an interface to obtain all the pieces of information on the
         * corresponding matching object.
         */
        struct EVOLUTION_EXPORT_OBJREC MatchInfo
        {
            /// Constructor
            MatchInfo();
            /// Copy Constructor
            MatchInfo (const MatchInfo& mi);
            /// Destructor
            ~MatchInfo();
            /// Operator '=' for assignment of MatchInfo structures.
            MatchInfo& operator= (const MatchInfo& mi);

            unsigned num;       /**< Match model id number. This number is
                                 * always positive and unique in
                                 * the database.  */
            String label;       ///< Match model label.
            long unsigned group;         ///< Match group id.
            int match_group;  // group number within image
            float distance;     /**< Estimated distance between the camera
                                 * and the matched object. */
            String units;       ///< Units of distance.
            unsigned num_features; ///< Number of features that were matched.
            double probability; ///< Probability of the match.
            float residual;     ///< Match residual error.
            float affine[2][3]; /**< Estimated affine transformation needed
                                 * to be applied to the original object
                                 * in order to fit the match on the image. */
            float rectangle[5][2]; /**< Vertices of the bounding box of
                                    * the match (vertex0 = vertex4). */
            int center_x;  ///< Match bounding box center (x coordinate).
            int center_y;  ///< Match bounding box center (y coordinate).

            Pose* pose[2];      /**< Match pose computed using the iterative
                                 *   pose estimation algorithm. The pose of
                                 *   match is only computed if the flag
                                 *   use-posit-coplanar is set to true. 
                                 *   @see Pose, PositCoplanar. */
            /**
             * @brief Prints the contents of the MatchInfo to the specified
             *        stream.
             *
             * @param out [in]; Stream to which to print the contents of MatchInfo.
             * @param info [in]; MatchInfo whose contents will be printed to the stream.
             *
             * @return Stream reference given in the out parameter.
             */
            EVOLUTION_EXPORT_OBJREC friend std::ostream&
            operator<< (std::ostream& out,
                        const Evolution::ObjRecQuery::MatchInfo & info);

        protected:
            /// Internal copy function.
            void copy (const MatchInfo& mi);

        };

        /// List of match information structs (used for obtaining information
        /// on all the matches present in the database).
        typedef std::vector<MatchInfo> MatchInfoList;

    public: // Constants
        /// Interface ID for the class.
        /// @see obtain_interface ()
        static const char *INTERFACE_ID;

    public: // Structors
        /// Default Constructor.
        ObjRecQuery();


    public: // IObject Interfaces
        /**
         * @brief
         * IObject, ObjectImpl interface.
         * This function obtains the specified interface from the
         * instance of the class if the instance supports the given
         * interface ID and the caller's security ticket has access to
         * it.
         * @see IObject::obtain_interface (), ObjectImpl::obtain_interface ()
         *
         * @param   ticket        [in];  security ticket (a placeholder)
         * @param interface_id    [in]; the interface ID of the requested
         *   interface.  This ID should be declared with the
         *   INTERFACE_ID constant member on the interface class.
         * @param interface_ptr    [out]; the pointer to the interface,
         *   non-NULL if the call succeeds.

         * @return  RESULT_SUCCESS,
         *          RESULT_NOT_FOUND,
         *          RESULT_INSUFFICIENT_ACCESS,
         *          or another context-dependent error.
         */
        Result obtain_interface (TicketId ticket, const char* interface_id,
                                 void** interface_ptr);

    public: // Recognition query
        /**
         * @brief
         * Search for possible matches to the specified image features in
         * the given database of models.
         *
         * Use the get_num_matches() method to determine how
         * many matches were found to objects in the database.
         *
         * @param database        [in]; database of objects used to perform
         *                              recognition.
         * @param image_features  [in]; set of features extracted from an
         *                              image to be recognized
         * @param group           [in]; id for the group of models to be
         *                              used for recognition.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result recognize (ObjRecDatabase     *database,
                          ObjRecKeypointList *image_features,
                          long unsigned      group = 0);

        /**
         * @brief *** Deprecated *** use recognize () instead
         * @deprecated
         */
        Result query(ObjRecDatabase  *database,
                     ObjRecImageHash *image_hash,
                     long unsigned    group = 0)
        {
            return recognize (database, image_hash, group);
        }

    public: // Recognition information retrieval
        /**
         * @brief
         * Provides number of object matches found by recognize().
         *
         * @return  number of object matches.
         */
        unsigned get_num_matches() const;

        /**
         * @brief
         * Obtain information on all matches to an image
         *
         * @param matches     [out]; pointer to a MatchInfoList
         * buffer. This buffer will be internally resized within the
         * function if more storage space is need. However, if there
         * is enough space allocated, the buffer will NOT be shrunk
         * (if the buffer will be reused to obtain information from
         * various matches, it would be a good idea to clear()
         * the buffer before calling this function).
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_all_matches(MatchInfoList *matches) const;

        /**
         * @brief
         * Obtain information on a particular match to an image.
         * After recognize() is called and get_num_matches() indicates
         * that one or more objects were matched in the database, call
         * this function with a match_index to get all the match
         * information.  This function could be used to iterate
         * through the various matches. The same result could be
         * obtained by calling get_all_matches () and then iterating
         * through the components of the MatchInfoList.
         *
         * @param match_index      [in]; In range [0..get_num_matches()].
         *                         Specifies which match to return info for
         * @param info             [out]; pointer to a MatchInfo structure
         *
         * @return                 Result indicating success or the reason
         *                         for failure.
         */
        Result get_match_info(unsigned match_index, MatchInfo *info) const;

        /**
         * @brief
         * Obtain the position of all the feature points that were matched.
         * These are x,y locations in pixels.
         *
         * @param match_index         [in]; in range [0..get_num_matches()]
         *                            Specifies which match to return info for
         * @param matched_points      [out]; (optional) pointer to an array of
         *                            get_match_hash_size() FeaturePoints.
         *
         * @param model_points        [out]; (optional) pointer to an array of
         *                            get_match_hash_size() FeaturePoints.
         *                            The matched_points and model_points
         *                            arrays are order such that the points
         *                            in both arrays are in correspondence.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         *
         * @see FeaturePoint, FeaturePointList
         *
         */
        Result get_match_feature_points(unsigned match_index,
                                        FeaturePointList *matched_points,
                                        FeaturePointList *model_points=NULL) const;

        /**
         * @brief
         * Obtain the user0 value of all the feature points that were matched.
         * This value is user defined.  Typically it is used to as a keypoint
         * index.
         *
         * @param match_index         [in]; in range [0..get_num_matches()]
         *                            Specifies which match to return info for
         * @param matched_points      [out]; (optional)
         *
         * @param model_points        [out]; (optional)
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         *
         * @see FeatureIndexList
         *
         */
        Result get_match_feature_user0s(unsigned match_index,
                                        FeatureUserDataList *matched_points,
                                        FeatureUserDataList *model_points=NULL) const;

        /**
         * @brief
         * Calculates azimuth and elevation of a match.
         *
         * The azimuth is the horizontal position of the object
         * in radians (the amount to turn to center the object horizontally).
         *
         * The elevation is the vertical position of the object
         * in radians (the amount to tilt up/down to center the object
         * vertically).
         *
         * @param match_index         in range [0..get_num_matches()],
         *                            specifies which match to return info for.
         * @param fov_x               horizontal field of view of the camera
         *                            in radians.
         * @param fov_y               vertical field of view of the camera
         *                            in radians.
         * @param azimuth             coordinates of the match converted
         *                            to a polar representation in radians.
         * @param elevation           coordinates of the match converted
         *                            to a polar representation in radians.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_match_navigational_data(int match_index, double fov_x,
                                           double fov_y, double* azimuth,
                                           double* elevation) const;

    public: // State information
#ifndef DOXYGEN_INTERNAL
        /**
         * @brief
         * Gets the processing state, true if ready
         */
        bool get_results_ready() const;

        /**
         * @brief
         * Pause until the query is fully ready to be used.
         * This returns false if an error occurs.
         */
        bool wait_until_ready() const;

        /**
         * @brief
         * For debugging, get the state of the database.
         */
        const char *get_state_name() const;
#endif

    public: // Parameter setting
        /**
         * @brief
         * Method that allows to set the value of various parameters.
         * @see ObjRecKeypointList::set_flag()
         *
         * @param flag_name  [in]; name of flag to be set, available flags are:
         *  @li @c feature-quality (0.0 ... 1.0) default: 0.85 @n
         *         Defines a threshold in the local prominence of a feature. 
         *         This parameter allows to select a smaller, more restrictive
         *         value for 'feature-quality' that the one that was used for
         *         feature extraction by the ObjRecKeypointList.
         *  @li @c detection-threshold (0.0 ... 1.0) default: 0.95 @n
         *         Defines the minimum acceptable probability required for 
         *         a match to be accepted.
         *  @li @c allow-multiple (true/false) default: false @n
         *         Allows features from the input image to be used multiple
         *         times to obtain matches to the database. On 'true' many 
         *         more matches to models with similar features are returned
         *         by the recognizer.
         *  @li @c min-keypoints (3 ... ) default: 4  @n
         *         Defines the minimum number of keypoints required to 
         *         consider a match.
         *  @li @c use-posit-coplanar (true/false) default: false @n
         *         Enables estimation of the relative pose of the match w.r.t.
         *         the model using the coplanar version of the Posit algorithm.
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result set_flag(const char *flag_name, ...);

        /**
         * @brief
         * Method that allows to get the current value of various parameters.
         * @see set_flag()
         *
         * @param flag_name   [in]; name of flag whose value will be provided
         *                    available flags are:
         *  @li @c feature-quality  (0.0 ... 1.0)
         *  @li @c detection-threshold (0.0 ... 1.0)
         *  @li @c allow-multiple (true/false)
         *  @li @c min-keypoints (3 ... )
         *  @li @c use-posit-coplanar (true/false)
         * @param rv          [out]; output value
         *
         * @return            Result indicating success or the reason
         *                    for failure.
         */
        Result get_flag (const char *flag_name, void *rv) const;

    protected: // Friend classes
        /// Friend class.
        /// @see Evolution::ObjRecDatabase
        friend class ObjRecDatabase;
        /// Internal implementation.
        friend class ObjRecInternals;

    protected: // Implementation
        ObjRecQueryImpl *_implementation;         ///< Internal implementation.
    protected:
        /// Destructor.
        ~ObjRecQuery();
    }; // end class ObjRecQuery
} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_OBJREC_OBJREC_HPP
