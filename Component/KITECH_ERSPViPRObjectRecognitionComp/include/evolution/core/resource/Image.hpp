//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file   evolution/core/resource/Image.hpp
 * @brief  An object that encapsulates an image.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::Image
 */


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IMAGE_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IMAGE_HPP


#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/math/Matrix.hpp>
#include <evolution/core/math/Matrix3.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/resource/Colorspace.hpp>
#include <evolution/core/resource/CameraParameters.hpp>


namespace Evolution 
{

    class ICamera;
    class IImageReader;
    class IImageWriter;

    typedef Matrix<ColorTriplet> ColorMatrix; ///< Matrix of color triplet values.

    /**
     * @class Image 
     *
     * @brief This class defines a representation of a raw image,
     * generally captured from a camera, with additional meta-data
     * describing the capture event.
     *
     * YUV planar format is preferred for capture because it is
     * trivial to obtain grayscale values for a single pixel and for
     * the full image. Also, it seems that most cameras support this
     * format for capture.
     *
     * However, most of the drawing functions (line/circle/etc) are optimized
     * best for RGB images, and several vision based behaviors prefer RGB
     * formatted Image inputs.
     *
     * Image, like Matrix, has a pretty absurd number of member functions.
     * Check their doxygen for more details.
     *
     * @ingroup core_vision
     * @ingroup core_vision_aibo
     */
    class EVOLUTION_EXPORT_RESOURCE Image : public ObjectImpl
    {
    public: // Structors

        /**
         *@brief Default constructor
         */
        Image ();

        /**
         * @brief This Image constructor is used to initialize an entire image to one specified rgb 
         * pixel value. If a non-COLOR_FORMAT_RGB24 is specified, the 
         * pixel will still be copied in a packed 3 byte way.  This will
         * result in odd things happening.
         * @param   width      [in]:  Width of new Image object
         * @param   height     [in]:  Height of new Image object
         * @param   red        [in]:  Optional value of red filler byte
         * @param   green      [in]:  Optional value of green filler byte
         * @param   blue       [in]:  Optional value of blue filler byte
         * @param   dst_format [in, opt]:  Optional format specifier for created Image
         */
        Image (unsigned width, unsigned height, 
               uchar red = 0, uchar green = 0, uchar blue = 0,
               ColorFormat dst_format = COLOR_FORMAT_RGB24);

        /**
         * @brief This is the Image constructor most often used. This creates an image, possibly
         * from a raw image data buffer, format, timestamp and camera parameter.
         * 
         * The format of the destination image should not be COLORSPACE_UNSPECIFIED or 
         * COLOR_FORMAT_UNSPECIFIED.
         *
         * When passing in a src_buffer and no src_format, COLOR_FORMAT_UNSPECIFIED 
         * means that src buffer will simply be copied into the newly created
         * data, with no color transformations taking place.  
         *
         * @param   width      [in]; Width of new Image object
         * @param   height     [in]; Height of new Image object
         * @param   dst_format [in]; ColorFormat of created Image
         * @param   src_buffer [in, opt]; Optional source data to copy into Image
         * @param   src_format [in, opt]; Optional ColorFormat specifier for new Image
         * @param   timestamp  [in, opt]; Optional: Timestamp of image creation
         * @param   params     [in, opt]; Optional: Parameters of camera used to create Image
         */
        Image (unsigned width, unsigned height,
               ColorFormat dst_format,
               const uchar* src_buffer = NULL,
               ColorFormat src_format = COLOR_FORMAT_UNSPECIFIED,
               Timestamp timestamp = 0, const CameraParameters* params = NULL);

        /**
         * @brief This Image constructor constructs an Image of width m.csize(), height m.rsize(),
         * with color format COLOR_FORMAT_GRAYSCALE, containing the data of m.
         *
         * @param m [in];  Matrix<T> to be turned into a grayscale Image.  The
         *                 type T needs to be able to be converted into a double
         *                 or int or unsigned byte
         */
        template <class T>
        Image (const Matrix<T>& m) : ObjectImpl(ObjectImpl::THREAD_SAFETY_FAST)
        {
            _data_size = 0;
            _data = NULL;
            _timestamp = 0;
            _camera_params = NULL;
            copy_matrix (m);
        } // end matrix constructor

        /**
         * @brief This Image constructor constructs a matrix from a ColorMatrix. 
         *
         * The created Image is of width colors.csize(), height colors.rsize().
         * The colorspace is assumed to be RGB24.
         * 
         * @param colors [in]; The ColorMatrix to be transformed into an Image
         */
        Image (const ColorMatrix& colors) : ObjectImpl(ObjectImpl::THREAD_SAFETY_FAST)
        {
            _data_size = 0;
            _data = NULL;
            _timestamp = 0;
            _camera_params = NULL;
            copy_matrix (colors);
        } // end color matrix constructor

        /**
         * @brief Copy constructor
         *
         * This is an efficient constructor to use if you want to get a copy of
         * this Image with a different colorspace efficiently.
         *
         * @param other  [in]; Image reference to copy
         * @param format [in, opt]; Format for copied image. If different from
         *                          other, the image data is converted to the
         *                          new format
         */
        Image (const Image& other, ColorFormat format = COLOR_FORMAT_UNSPECIFIED)
         : ObjectImpl(ObjectImpl::THREAD_SAFETY_FAST)
        {
            _data_size = 0;
            _data = NULL;
            _timestamp = 0;
            _camera_params = NULL;
            copy (other, format);
        }

        /// Destructor.
        ~Image ();

    public: // Assignment

        /**
         * @brief The copy function copies another Image, with a possible 
         * colorspace conversion.
         *
         * @param other  [in]; Image reference to copy
         * @param format [in, opt]; Format for copied image. If different from
         *                          other, the image data is converted to the
         *                          new format
         */
        void copy (const Image& other,
                   ColorFormat format = COLOR_FORMAT_UNSPECIFIED);

        /**
         * @brief The copy function copies another Image, starting
         * from a point different from (0,0).
         *
         * @param other  [in]; Image to be copied.
         * @param xoffset [in]; x-coordinate of the point from the copy will start.
         * @param yoffset [in]; y-coordinate of the point from the copy will start.
         */
        Result copy (int xoffset, int yoffset, const Image& other); 

        /**
         * @brief This function creates a grayscale image from the given Matrix m.
         *
         * Used to create an Image from a Matrix<T>.  The Image will be of
         * width m.csize() and height m.rsize(), with an 8bpp grayscale
         * colorspace.  The T objects in the Matrix m will be directly cast to
         * an unsigned char.  If that isn't a reasonable cast, don't use this
         * function.
         *
         * @param m [in]; Matrix to acquire data from
         */
        template <class T>
        void copy_matrix (const Matrix<T>& m)
        {
            if (alloc_buffer (m.ncols (), m.nrows (), COLOR_FORMAT_GRAYSCALE) != RESULT_SUCCESS)
            {
                ERSP_LOG_CLASS_WARN (Image)("Failed to allocate buffer.");
                return;
            }
            const T* m_data = m.get_linear_data ();
            for (register size_t i = 0; i < _data_size; ++i)
            {
                _data[i] = (uchar)m_data[i];
            }
        } // end copy_matrix()

        /**
         * @brief The copy_matrix function creates a grayscale image from the
         * given Matrix m.
         *
         * It is used to create an Image from a Matrix<uchar>. The Image will
         * be of width m.csize() and height m.rsize().
         *
         * @param m [in]; Matrix to acquire data from
         */
        void copy_matrix (const Matrix<uchar>& m)
        {
            if (alloc_buffer (m.ncols (), m.nrows (), COLOR_FORMAT_GRAYSCALE) != RESULT_SUCCESS)
            {
                ERSP_LOG_CLASS_WARN (Image)("Failed to allocated buffer.");
                return;
            }
            memcpy (_data, m.get_linear_data (), _data_size);
        } // end copy_matrix()

        /**
         * @brief This function creates a color image from the given ColorMatrix colors, 
         * assumed to be in RGB24 format.
         *
         * It is used to create an Image from a ColorMatrix.  The Image will
         * be of width m.csize() and height m.rsize().
         *
         * @param colors [in]; The color matrix from which the image
         *                     will be created
         */
        void copy_matrix (const ColorMatrix& colors)
        {
            if (alloc_buffer (colors.ncols (), colors.nrows (), COLOR_FORMAT_RGB24) != RESULT_SUCCESS)
            {
                ERSP_LOG_CLASS_WARN (Image)("Failed to allocate image buffer.");
                return;
            }
            const ColorTriplet* matrix_data = colors.get_linear_data ();
            if (matrix_data == NULL)
            {
                ERSP_LOG_CLASS_WARN (Image)("Cannot create image from null matrix data.");
                return;
            }
            unsigned long matrix_size = colors.ncols () * colors.nrows ();
            uchar* p = _data;
            const ColorTriplet* current_pixel;
            for (unsigned long i = 0; i < matrix_size; ++i)
            {
                current_pixel = &matrix_data[i];
                *(p++) = (*current_pixel)[0];
                *(p++) = (*current_pixel)[1];
                *(p++) = (*current_pixel)[2];
            }
        } // end copy_matrix(ColorMatrix)

        /**
         * @brief Assignment operator.
         *
         * @param other [in]; Image to copy
         *
         * @return Reference to this object.
         */ 
        Image& operator= (const Image& other)
        {
            copy (other);
            return (*this);
        } // end operator=()

        /**
         * @brief The acquire_buffer function takes ownership of a buffer of pixels.
         *
         * The function takes ownership of buffer's memory, interpreting it as a
         * width by height block of src_format formatted pixels.
         *
         * @param   width      [in];  Width of acquired buffer
         * @param   height     [in];  Height of acquired buffer
         * @param   src_format [in];  ColorFormat buffer is in
         * @param   buffer     [in];  Buffer of pixels to acquire
         */
        void acquire_buffer (unsigned width, unsigned height,
                             ColorFormat src_format, uchar* buffer);

        /**
         * @brief The copy_buffer function copies a block of pixel data.
         *
         * This function copies buffer (in format src_format) to this object, converting
         * to dst_format if dst_format is specified.
         *
         * @param   width      [in];  Width of copied buffer
         * @param   height     [in];  Height of copied buffer
         * @param   src_format [in];  ColorFormat buffer is in
         * @param   buffer     [in];  Buffer of pixels to copy
         * @param   dst_format [in, opt]; ColorFormat of copy destination
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result copy_buffer (unsigned width, unsigned height, 
                            ColorFormat src_format, const uchar* buffer,
                            ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED);

    public: // Reference counting

        /**
         * @brief Use this function to add a reference.
         *
         * @return  The updated reference count.
         */
        unsigned add_ref () const;

        /**
         * @brief Use this function to remove a reference.
         *
         * @return The updated reference count.
         */
        unsigned remove_ref () const;

        /**
         * @brief How many references are there?
         *
         * @return The current reference count.
         */
        unsigned ref_count () const;

    public: // Characteristics

        /** 
         * @brief Use this function to get image width.
         *
         * @return Width of the image.
         */
        unsigned get_width () const { return _width; }

        /**
         * @brief Use this function to get image height.
         *
         * @return Height of the image.
         */
        unsigned get_height () const { return _height; }

        /**
         * @brief Use this function to get the pixel data format.
         *
         * @return Format of the pixel data.
         */
        ColorFormat get_format () const { return _format; }

    public: // Access

        /**
         * @brief The get_pixel function obtains the values at the specified coordinate.
         *
         * Variables v2 and v3 are only optional if type == GRAYSCALE,
         * in which case they are ignored.  If type does not match
         * the Colorspace of the image, the pixel will be converted.    
         * Doing a conversion more than a few times (if doing image processing
         * especially) is very slow.  If a large amount of pixel colorspace
         * conversion is needed, try converting the entire buffer at once, 
         * or making a second converted buffer, operating on it, and 
         * transforming the data back after the operation.
         *
         * @see Evolution::ColorspaceConverter 
         *
         * @note  For grayscale, only the first value will be set.
         * @param   x_coord [in];  The x location of desired pixel
         * @param   y_coord [in];  The y location of desired pixel
         * @param   v1      [out]; Byte 1 value for pixel
         * @param   v2      [out, opt]; Optional byte 2 value for pixel
         * @param   v3      [out, opt]; Optional byte 3 value for pixel
         * @param   type    [in, opt];  Desired return pixel colorspace.
         */
        void get_pixel (unsigned x_coord, unsigned y_coord,
                        uchar* v1, uchar* v2 = NULL, uchar* v3 = NULL,
                        Colorspace type = COLORSPACE_UNSPECIFIED) const;

        /**
         * @brief This is a convienience wrapper for get_pixel, colorspace YCBCR.
         *
         * Convienience wrapper for get_pixel, colorspace YCBCR
         *
         * @param   x_coord [in];  x of desired pixel
         * @param   y_coord [in];  y of desired pixel
         * @param   y       [out];  y value for pixel
         * @param   cb      [out];  cb value for pixel
         * @param   cr      [out];  cr value for pixel
         */ 
        void get_pixel_ycbcr (unsigned x_coord, unsigned y_coord,
                              uchar* y, uchar* cb, uchar* cr) const
        {
            PRECOND (y && cb && cr);
            get_pixel (x_coord, y_coord, y, cb, cr, COLORSPACE_YCBCR);
        }

        /**
         * @brief This is a convienience wrapper for get_pixel, colorspace RGB.
         * 
         * Convienience wrapper for get_pixel, colorspace RGB
         * @param   x_coord [in];  x of desired pixel
         * @param   y_coord [in];  y of desired pixel
         * @param   r       [out];  r value for pixel
         * @param   g       [out];  g value for pixel
         * @param   b       [out];  b value for pixel
         */ 
        void get_pixel_rgb (unsigned x_coord, unsigned y_coord,
                            uchar* r, uchar* g, uchar* b) const
        {
            PRECOND (r && g && b);
            get_pixel (x_coord, y_coord, r, g, b, COLORSPACE_RGB);
        }

        /**
         * @brief This is a convienience wrapper for get_pixel, colorspace grayscale.
         *
         * Convienience wrapper for get_pixel, colorspace grayscale
         * @param   x_coord [in];  x of desired pixel
         * @param   y_coord [in];  y of desired pixel
         * @param   lum     [out];  grayscale value (luminance) of pixel.
         */ 
        void get_pixel_grayscale (unsigned x_coord, unsigned y_coord, uchar* lum) const
        {
            PRECOND (lum);
            get_pixel (x_coord, y_coord, lum, NULL, NULL, COLORSPACE_GRAYSCALE);
        }

        /**
         * @brief Use this function to get an rgb24 pixel. It assumes source and destination are in proper format.
         * 
         * Gets an rgb24 pixel, assumes source and destination are in proper
         * format.  No checking is done on the access, which makes the access
         * faster.  Use this function to access images when speed matters, and
         * do pre-formatting and checking for entire blocks of processing.
         * This function is faster than get_pixel_rgb.
         * @param   x_coord [in];  x of desired pixel
         * @param   y_coord [in];  y of desired pixel
         * @param   r       [out];  r value for pixel
         * @param   g       [out];  g value for pixel
         * @param   b       [out];  b value for pixel
         */ 
        void get_pixel_rgb24_fast (unsigned x_coord, unsigned y_coord,
                                   uchar* r, uchar* g, uchar* b) const
        {
            int o1 = 3*(x_coord + y_coord * get_width());
            *r = _data[o1];
            *g = _data[++o1];
            *b = _data[++o1];
        }

        /**
         * @brief Use this function get a grayscale pixel. It assumes source and destination are in proper format.
         * 
         * Gets a grayscale pixel, but assumes source and destination are in
         * proper format.  Faster than get_pixel_grayscale.
         * @param   x_coord [in];  x of desired pixel
         * @param   y_coord [in];  y of desired pixel
         * @param   lum     [out];  grayscale value (luminance) of pixel.
         */ 
        void get_pixel_grayscale_fast (unsigned x_coord, unsigned y_coord,
                                       uchar* lum) const
        {
            *lum = _data[x_coord + y_coord * get_width()];
        }

        /**
         * @brief Use this function to get an iyuv pixel. It assumes source and destination are in proper format.
         * 
         *  Gets a iyuv pixel, but assumes source and destination are in proper format. 
         * @param   x_coord [in];  x of desired pixel
         * @param   y_coord [in];  y of desired pixel
         * @param   y       [out];  y value for pixel
         * @param   u       [out];  u value for pixel
         * @param   v       [out];  v value for pixel
         */ 
        void get_pixel_iyuv_fast (unsigned x_coord, unsigned y_coord,
                                  uchar* y, uchar* u, uchar* v) const
        {
            int len = get_width() * get_height();
            int o1 = len + x_coord / 2 + y_coord / 2 * (get_width () / 2); 
            *y = _data[x_coord + y_coord * get_width()];
            *u = _data[o1];
            *v = _data[o1 + len / 4];
        }

        /**
         * @brief This function returns a const pointer to the internal pixel data block.
         * 
         * Returns a const pointer to the internal pixel data block.
         * @return pointer to raw pixel buffer.
         */ 
        const uchar* get_raw_data () const { return (_data); }

        /**
         * @brief This function returns a modifiable pointer to the internal pixel data block.
         * 
         * Returns a modifiable pointer to the internal pixel data block.  It
         * is faster to operate directly on the data block pointer sometimes,
         * which is why this function is useful.
         * @return pointer to raw pixel buffer.
         */ 
        uchar* get_raw_data () { return (_data); }

        /**
         * @brief This function returns the size of the internal pixel data block.
         * 
         * Gets the size of the internal pixel data block.
         * @return the size of the internal data block.
         */ 
        size_t get_data_size () const { return (_data_size); }

    public: // Metadata

        /**
         * @brief Use this function to get the timestamp for image creation.
         * 
         * Obtains the timestamp for image creation.
         * 
         * @return Timestamp of the image creation.
         */ 
        Timestamp get_timestamp () const { return _timestamp; }

        /**
         * @brief Use this function to get the constant camera parameters at the time of image creation.
         * 
         *  Obtains the constant camera parameters at the time of image creation.
         *  
         * @return CameraParameters at the time of image creation.
         */ 
        const CameraParameters* get_camera_parameters () const { return _camera_params; }

        /**
         * @brief Use this function to get the constant camera parameters at the time of image creation.
         *
         *  Obtains a modifiable pointer to the CameraParameters at the time of image creation.
         * @return CameraParameters at the time of image creation.
         */ 
        CameraParameters* get_camera_parameters () { return _camera_params; }

    public: // Setting

        /**
         * @brief Use this function to convert the image to the specified format.
         *
         * @param format [in]; Format to change the image pixel buffer to.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */ 
        Result convert (ColorFormat format);

        /**
         * @brief Use this function to set the timestamp.
         *
         * Sets the timestamp.
         * @param timestamp [in]; value to change the timestamp to.
         */ 
        void set_timestamp (Timestamp timestamp) { _timestamp = timestamp; }

        /**
         * @brief Use this function to set the camera parameters. Parameters may be NULL (camera unknown).
         *
         * Sets the camera parameters, which may be NULL (camera unknown).
         * Note that the CameraParameters are copied and stored internally, no
         * pointer to the passed in const object is kept.
         * @param camera_params [in]; value to change the timestamp to.
         */ 
        void set_camera_parameters (const CameraParameters* camera_params);

#if !EVOLUTION_PLATFORM_AIBO
        /**
         * @brief Use this function to Noisify the image.
         * 
         * This function adds gaussian noise with m mean and v variance to
         * every byte of imagedata.
         *
         * @param m [in]; value to change the timestamp to.
         * @param v [in]; value to change the timestamp to.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */ 
        void noisify ( double m=1.0, double v=1.0);
#endif

    public: // Drawing

        /**
         * @brief Use this function to set a pixel from an arbitrary colorspace to
         * the Image ColorSpace.
         * 
         * If the incoming colorspace is different from the Image object's 
         * colorspace, the pixel is transformed to the appropriate ColorSpace. 
         * If the pixels are in the same colorspace, it's much faster to use 
         * one of the ColorSpace specific set operations, if one of 
         * the appropriate type is available. 
         * 
         * @param   x_coord  [in];  The x position of desired set location
         * @param   y_coord  [in];  The y position of desired set location
         * @param   v1       [in];  Byte 1 value for pixel
         * @param   v2       [in];  Optional byte 2 value for pixel
         * @param   v3       [in];  Optional byte 3 value for pixel
         * @param   source_space [in];  Colorspace of pixel passed in
         */
        void set_pixel (unsigned x_coord, unsigned y_coord,
                        uchar v1, uchar v2 = 0, uchar v3 = 0,
                        Colorspace source_space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief The set_pixel_rgb24_fast function is a completely and utterly unsafe pixel setting method.
         * 
         * It doesn't check bounds.  It doesn't check image depth.
         * It doesn't bother with Colorspace.  It'll gladly write values
         * into a null Image object.  Use with care.
         * The function assumes the target Image is COLOR_FORMAT_RGB24.
         *
         * @param   x   [in];  x position of desired set location.
         * @param   y   [in];  y position of desired set location.
         * @param   v1  [in];  red value for pixel.
         * @param   v2  [in];  green value for pixel.
         * @param   v3  [in];  blue value for pixel.
         */
        void set_pixel_rgb24_fast (unsigned x, unsigned y,
                                   uchar v1 = 0, uchar v2 = 0, uchar v3 = 0)
        {
            int o1=(y*_width + x) * 3;
            _data [ o1     ] = v1;
            _data [ o1 + 1 ] = v2;
            _data [ o1 + 2 ] = v3;
        }

        /**
         * @brief The set_pixel_iyuv_fast function is a completely and utterly unsafe pixel setting method.
         * It doesn't check bounds.  It doesn't check image depth.
         * It doesn't bother with Colorspace.  It'll gladly write values
         * into a null Image object.  Use with care.
         * The function assumes the target Image is COLOR_FORMAT_IYUV
         *
         * @param   x  [in];  x position of desired set location.
         * @param   y  [in];  y position of desired set location.
         * @param   v1 [in];  y value for pixel.
         * @param   v2 [in];  u value for pixel.
         * @param   v3 [in];  v value for pixel.
         */
        void set_pixel_iyuv_fast (unsigned x, unsigned y,
                                  uchar v1 = 0, uchar v2 = 0, uchar v3 = 0)
        {
            int len = get_width() * get_height();
            int o1 = len + x / 2 + y / 2 * (get_width () / 2); 
            _data[x + y * get_width()]  = v1;
            _data[o1]                   = v2;
            _data[o1 + len / 4]         = v3;
        }

        /**
         * @brief The set_pixel_grayscale_fast function is a completely and utterly unsafe pixel setting method.
         * It doesn't check bounds.  It doesn't check image depth.
         * It doesn't bother with Colorspace.  It'll gladly write values
         * into a null Image object.  Use with care.
         * Assumes the target Image is COLOR_FORMAT_GRAYSCALE
         *
         * @param   x   [in];  x position of desired set location.
         * @param   y   [in];  y position of desired set location.
         * @param   lum [in];  luminance value for pixel.
         */
        void set_pixel_grayscale_fast (unsigned x, unsigned y,
                                       uchar lum)
        {
            _data[ x + y * get_width() ] = lum;
        }

        /**
         * @brief This function 
         * bounds checked 3 dimensional pixel setting function.
         * It does _NOT_ check to see if image is 8 or 24 bpp.  It only checks
         * to see if the x,y value given would cause an invalid write.
         *
         * Currently the function ignores Colorspace argument and assumes 
         * you want COLOR_FORMAT_RGB24.
         *
         * @param   x  [in];  x position of desired set location.
         * @param   y  [in];  y position of desired set location.
         * @param   v1 [in];  r value for pixel.
         * @param   v2 [in];  g value for pixel.
         * @param   v3 [in];  b value for pixel.
         * @param   space [in];  Meant to be the source Colorspace of the
         *                              pixel.  Currently ignored.
         */
        void set_pixel_bounded_rgb (unsigned x, unsigned y,
                                    uchar v1, uchar v2 = 0, uchar v3 = 0,
                                    Colorspace space = COLORSPACE_UNSPECIFIED)
        {
            if( x< _width && y < _height )
            {
                _data [ (y*_width + x) * 3 + 0 ] = v1;
                _data [ (y*_width + x) * 3 + 1 ] = v2;
                _data [ (y*_width + x) * 3 + 2 ] = v3;
            }
        }

        /**
         * @brief This function sets the YCbCr values at the specified coordinate.
         * 
         * Convenience wrapper for set_pixel for the ycbcr colorspace.
         * 
         * @param   x_coord  [in];  x position of desired set location.
         * @param   y_coord  [in];  y position of desired set location.
         * @param   y        [in];  luminance value for pixel.
         * @param   cb       [in];  cb (blue chrominance) value for pixel
         * @param   cr       [in];  rb (red  chrominance) value for pixel
         */
        void set_ycbcr (unsigned x_coord, unsigned y_coord, 
                        uchar y, uchar cb, uchar cr)
        {
            set_pixel (x_coord, y_coord, y, cb, cr, COLORSPACE_YCBCR);
        }
        
        /**
         * @brief This function sets the RGB values at the specified coordinate.
         *
         * Convenience wrapper for set_pixel for the rgb colorspace.
         * 
         * @param   x_coord  [in];  x position of desired set location.
         * @param   y_coord  [in];  y position of desired set location.
         * @param   r        [in];  red value for pixel.
         * @param   g        [in];  green value for pixel.
         * @param   b        [in];  blue value for pixel.
         */
        void set_rgb (unsigned x_coord, unsigned y_coord, 
                      uchar r, uchar g, uchar b)
        {
            //set_pixel (x_coord, y_coord, r, g, b, COLORSPACE_RGB);
            _data [ (y_coord*_width + x_coord) * 3 + 0 ] = r;
            _data [ (y_coord*_width + x_coord) * 3 + 1 ] = g;
            _data [ (y_coord*_width + x_coord) * 3 + 2 ] = b;
        }

        /**
         * @brief This function sets the YUV values at the specified coordinate.
         * 
         * Convenience wrapper for set_pixel for the iyuv colorspace.
         * 
         * @param   x_coord  [in];  x position of desired set location.
         * @param   y_coord  [in];  y position of desired set location.
         * @param   y        [in];  luminance value for pixel.
         * @param   u        [in];  u value for pixel.
         * @param   v        [in];  v value for pixel.
         */
        void set_yuv (unsigned x_coord, unsigned y_coord,
                      uchar y, uchar u, uchar v)
        {
            size_t len = get_width () * get_height ();
            _data[x_coord + y_coord * get_width()            ] = y; 
            _data[x_coord + y_coord * get_width() + len      ] = u;
            _data[x_coord + y_coord * get_width() + len + len] = v;
        }

        /**
         * @brief Sets a numbered component of a colorspace.
         * 
         * set_pixel_component(0,0, 255, 1) called on an rgb image sets the 0,0
         * position to have a 255 green value.  012 corresponds to YUV in IYUV
         * space.  This just sets 1 component of a given pixel value. 
         * 
         * @param   x_coord      [in];  x position of desired set location.
         * @param   y_coord      [in];  y position of desired set location.
         * @param   v            [in];  value for pixel component.
         * @param   component    [in];  v value for pixel.
         * @param   source_space [in];  space to treat the component value as from.
         */
        void set_pixel_component( unsigned x_coord, unsigned y_coord,
                                  uchar v, unsigned component = 0,
                                  Colorspace source_space = COLORSPACE_UNSPECIFIED)
        {
            uchar values[3];
            if(component >= 3 || (component >= 1 && COLORSPACE_GRAYSCALE))
            {
                return;
            }
            get_pixel(x_coord, y_coord, &values[0], &values[1], &values[2], source_space);
            values[component] = v;
            set_pixel( x_coord, y_coord, values[0], values[1], values[2], source_space);
        }

        /**
         * @brief The draw_line function draws a line from (x0,y0) to (x1,y1).
         *
         * Draws a line from (x0,y0) to (x1,y1).  If the colorspace is left
         * unspecified, the pixels will be drawn with no conversion at all.
         * If the x,y coordinates are out of bounds, it will clip off the
         * excess (and, in the case of negative values, invalid) values and not
         * do those draws.
         *
         * @param x0 [in]; The x coordinate of line origin
         * @param y0 [in]; The y coordinate of line origin
         * @param x1 [in]; The x coordinate of line terminus
         * @param y1 [in]; The y coordinate of line terminus
         * @param v1 [in]; The first color byte's value
         * @param v2 [in]; The second color byte's value
         * @param v3 [in]; The third color byte's value
         * @param space [in]; Colorspace of the input color value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_line (int x0, int y0, int x1, int y1,
                          uchar v1, uchar v2 = 0, uchar v3 = 0,
                          Colorspace space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief The draw_line function draws a line from (x0,y0) to (x1,y1).
         * 
         * Draws a line from (x0,y0) to (x1,y1), only drawing pixels that appear
         * in the image.
         *
         * @param x0 [in]; The x coordinate of line origin
         * @param y0 [in]; The y coordinate of line origin
         * @param x1 [in]; The x coordinate of line terminus
         * @param y1 [in]; The y coordinate of line terminus
         * @param v1 [in]; The first color byte's value
         * @param v2 [in]; The second color byte's value
         * @param v3 [in]; The third color byte's value
         * @param space [in]; The colorspace of the input color value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_line (unsigned x0, unsigned y0, unsigned x1, unsigned y1,
                          uchar v1, uchar v2 = 0, uchar v3 = 0,
                          Colorspace space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief This function draws a line from (x0,y0) to (x1,y1).
         *
         * Draws a line from an origin (sx,sy) along a Vector2.
         * draw_line(-1,-1,2,2) draws a line from -1,-1 to 1,1.
         *
         * @param sx    [in]; The x coordinate of line origin
         * @param sy    [in]; The y coordinate of line origin
         * @param vec   [in]; The Vector describing line to be drawn from origin
         *                    sx,sy
         * @param v1    [in]; The first color byte's value
         * @param v2    [in, opt]; Optional. The second color byte's value
         * @param v3    [in, opt]; Optional. The third color byte's value
         * @param space [in, opt]; Optional. The colorspace of the input color
         *                         value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_line(int sx, int sy, Vector2 vec, 
                         uchar v1=0, uchar v2=0, uchar v3=0, 
                         Colorspace space= COLORSPACE_UNSPECIFIED);
        
        /**
         * @brief This function draws a box from (x0,y0) to (x0+width, y0+height).
         *
         * Draws a box from (x0,y0) to (x0+width, y0+height).  The line width of
         * the sides is 1, and the center is left untouched.
         *
         * @param x0     [in]; Upper left corner of box, x coord (in image coordinates)
         * @param y0     [in]; Upper left corner of box, y coord (in image coordinates)
         * @param width  [in]; Width of the box
         * @param height [in]; Height of the box
         * @param v1     [in]; First color byte's value
         * @param v2     [in, opt]; Second color byte's value
         * @param v3     [in, opt]; Third color byte's value
         * @param space  [in, opt]; Colorspace of the input color value
         *                          triplet (v1,v2,v3).
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_box (unsigned x0, unsigned y0, unsigned width, unsigned height,
                         uchar v1, uchar v2 = 0, uchar v3 = 0,
                         Colorspace space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief This function draws a filled box from (x0,y0) to (x1,y1).
         *
         * Draws a filled box from (x0,y0) to (x1+width,y1+height).
         * The box is filled with the same color as the outside.
         *
         * @param x0    [in]; Lower left corner of box.
         * @param y0    [in]; Lower left corner of box.
         * @param wx    [in]; Width of box
         * @param wy    [in]; Height of box
         * @param v1    [in]; First color byte's value.
         * @param v2    [in, opt]; Optional. Second color byte's value.
         * @param v3    [in, opt]; Optional. Third color byte's value.
         * @param space [in, opt]; Optional. Colorspace of the input color value
         *                         triplet (v1,v2,v3).
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_filled_box (unsigned x0, unsigned y0, unsigned wx, unsigned wy,
                                uchar v1, uchar v2 = 0, uchar v3 = 0,
                                Colorspace space = COLORSPACE_UNSPECIFIED);

#ifndef DOXYGEN_INTERNAL
        /**
         * @brief This function draws an arc centered at (cx,cy) with the
         * given radius on angles [start,stop].
         *
         * @param cx     [in]; The x center of the arc
         * @param cy     [in]; The y center of the arc
         * @param radius [in]; The radius of the arc
         * @param start  [in]; The start angle
         * @param stop   [in]; The stop angle
         * @param step   [in]; 
         * @param v1     [in]; First color byte's value
         * @param v2     [in, opt]; Optional second color byte's value
         * @param v3     [in, opt]; Optional third color bytes's value
         * @param space  [in, opt]; Optional colorspace of the input color value
         *                          triplet (v1,v2,v3)
         *
         *
         * @return RESULT_NOT_IMPLEMENTED.
         */
        Result draw_arc (unsigned int cx, unsigned int cy, 
                         double radius, double start, double stop, double step, 
                         uchar v1, uchar v2 = 0, uchar v3 = 0,
                         Colorspace space = COLORSPACE_UNSPECIFIED);
#endif // end DOXYGEN_INTERNAL

        /**
         * @brief This function draws a circle centered at (cx,cy) with the given radius.
         *
         * Draws a circle centered at (cx,cy) with the given radius.
         *
         * @param cx     [in]; Center of the circle, x coord
         * @param cy     [in]; Center of the circle, y coord
         * @param radius [in]; Radius of circle.
         * @param v1     [in]; First color byte's value.
         * @param v2     [in, opt]; Optional. Second color byte's value.
         * @param v3     [in, opt]; Optional. Third color byte's value.
         * @param space  [in, opt]; Optional. Colorspace of the input color value
         *                          triplet (v1,v2,v3).
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_circle (unsigned int cx, unsigned int cy, unsigned int radius,  
                            uchar v1, uchar v2 = 0, uchar v3 = 0,
                            Colorspace space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief This function draws a circle centered at (cx,cy) with the given radius.
         *
         * Draws a circle centered at (cx,cy) with the given radius.  However,
         * bounds-check each pixel draw to avoid invalid writes.
         *
         * @param cx     [in]; Center of the circle, x coord
         * @param cy     [in]; Center of the circle, y coord
         * @param radius [in]; Radius of circle
         * @param v1     [in]; First color byte's value
         * @param v2     [in, opt]; Optional. Second color byte's value
         * @param v3     [in, opt]; Optional. Third color byte's value
         * @param space  [in, opt]; Optional. Colorspace of the input color
         *                          value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_circle_safe (unsigned int cx, unsigned int cy, unsigned int radius,  
                                 uchar v1, uchar v2 = 0, uchar v3 = 0,
                                 Colorspace space = COLORSPACE_UNSPECIFIED);

#ifndef DOXYGEN_INTERNAL
        /**
         * @brief This function draws an ellipse centered at (cx,cy).
         *
         * @return RESULT_NOT_IMPLEMENTED.
         */
        Result draw_ellipse (unsigned int cx, unsigned int cy, 
                             double Sxx, double Sxy, double Syy, double mag,
                             uchar v1, uchar v2 = 0, uchar v3 = 0,
                             Colorspace space = COLORSPACE_UNSPECIFIED);
#endif // end DOXYGEN_INTERNAL

        /**
         * @brief This function draws an ellipse centered at (cx,cy).
         *
         * @param cx [in]; Center of the ellipse, x coord
         * @param cy [in]; Center of the ellipse, y coord
         * @param xp [in]; x-value of ellipse p point
         * @param yp [in]; y-value of ellipse p point
         * @param xq [in]; x-value of ellipse q point
         * @param yq [in]; y-value of ellipse q point
         * @param v1 [in]; First color byte's value
         * @param v2 [in]; Second color byte's value
         * @param v3 [in]; Third color byte's value
         * @param space  [in, opt]; Optional. Colorspace of the input color
         *                          value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_ellipse (unsigned int cx, unsigned int cy, 
                             int xp, int yp, int xq, int yq,
                             uchar v1, uchar v2, uchar v3,
                             Colorspace space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief Draws a parallelogram.
         *
         * Draws a parallelogram.
         * bounds-check each pixel draw to avoid invalid writes.
         * @param cx [in]; Center of the parallelogram, x coord
         * @param cy [in]; Center of the parallelogram, y coord
         * @param xp [in]; x-value of parallelogram p point
         * @param yp [in]; y-value of parallelogram p point
         * @param xq [in]; x-value of parallelogram q point
         * @param yq [in]; y-value of parallelogram q point
         * @param v1 [in]; First color byte's value
         * @param v2 [in]; Second color byte's value
         * @param v3 [in]; Third color byte's value
         * @param space  [in, opt]; Optional. Colorspace of the input color
         *                          value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_parallelogram (unsigned int cx, unsigned int cy, 
                                   int xp, int yp, int xq, int yq,
                                   uchar v1, uchar v2, uchar v3,
                                   Colorspace space = COLORSPACE_UNSPECIFIED);

        /**
         * @brief Draws an arbitrary conic section.
         *
         * Draws an arbitrary section of 0 = Ax^2 + By^2 + Cxy + Dx + Ey + F
         * bounds-check each pixel draw to avoid invalid writes.
         *
         * @param cx [in]; Center of the conic section, x coord
         * @param cy [in]; Center of the conic section, y coord
         * @param xs [in]; Start point x
         * @param ys [in]; Start point y
         * @param xe [in]; Stop point x
         * @param ye [in]; Stop point y
         * @param A  [in]; Coefficient of x^2
         * @param B  [in]; Coefficient of y^2
         * @param C  [in]; Coefficient of xy
         * @param D  [in]; Coefficient of x
         * @param E  [in]; Coefficient of y
         * @param F  [in]; Coefficient of constant
         * @param v1 [in]; First color byte's value
         * @param v2 [in]; Second color byte's value
         * @param v3 [in]; Third color byte's value
         * @param space [in, opt]; Optional. Colorspace of the input color
         *                         value triplet (v1,v2,v3)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result draw_conic (int cx, int cy, //center point.
                           int xs, int ys, //start point.
                           int xe, int ye, //end point
                           int A, int B, int C, int D, int E, int F, //coefficients.
                           uchar v1, uchar v2, uchar v3, Colorspace space = COLORSPACE_UNSPECIFIED); //coefficients.

    public: // Helper functions for draw. not useful publicly.
#ifndef DOXYGEN_INTERNAL

        ///internal, used for draw_circle. draws a set of 8 symmetric grid-aligned circle points.
        void eight_circle_points(int cx, int cy, int x, int y, 
                                 uchar v1, uchar v2, uchar v3,
                                 Colorspace space);

        static int get_octant( int d, int e); //probably wrong.
#endif // end DOXYGEN_INTERNAL

    public: // Alteration

        /**
         * @brief This function sets all internal data to 0.
         */
        void clear()
        {
            if(_data)
            {
                for(unsigned i = 0; i < _data_size ; i ++ )
                {
                    _data[i] = 0;
                }
            }
        }

        /**
         * @brief This function generates random red/green/blue 
         * channel noise, then, if(smooth), smooths
         * the image with a gaussian filter of x/y size smooth_size.
         * 
         *  Generates random red/green/blue channel noise, then, 
         *  if(smooth), smoothes *  the image with a gaussian filter 
         *  of x/y size smooth_size.  The resultant image
         *  will have a COLOR_FORMAT_RGB24 color format. 
         * 
         * @param width       [in]; Width of image to make
         * @param height      [in]; Height of image to make
         * @param smooth      [in, opt]; Optional. Run a gaussian convolution
         *                               over the resultant random data
         * @param smooth_size [in, opt]; Optional. Size of gaussian convolution
         *                               to smooth data with, must be odd length
         */
        void solid_noise_rgb(int width, int height,
                             bool smooth=true, int smooth_size=15);

        /**
         * @brief This function  Up samples the image by the given factor.
         *  
         *  This changes the width/height of this image by the specified
         *  factors, and scales the data to fit the new dimensions.  
         *  No smoothing or convolution is done on the resampled data.
         *  
         *  @param xfactor [in]; Factor to upscale the x dimension of the
         *                       image by
         *  @param yfactor [in]; Factor to upscale the y dimension of the
         *                       image by
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result up_sample (unsigned xfactor, unsigned yfactor);

        /**
         * @brief This function creates a copy of the image, up sampled by the given factor.
         *  
         * Creates a new image of size (width * xfactor, height * yfactor) and
         * resamples a copy of this image's data to place into the new image.
         * No smoothing or convolution is done on the resampled data.
         *  
         * @param xfactor [in]; Factor to upsample the x dimension of the image by
         * @param yfactor [in]; Factor to upsample the y dimension of the image by
         * @param output  [out]; Pointer to image object to place copy in
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result up_sample_copy (unsigned xfactor, unsigned yfactor, Image* output) const;

        /**
         * @brief This function down samples the image by the given
         * factor. The sampling may be smoothed by a filter or fast
         * and rough.
         * 
         * Down samples the image by the given factor in the specified
         * dimensions. The sampling may be smoothed by a gaussian
         * filter or fast and rough.
         *
         * @param xfactor [in]; Factor to downsample the x dimension of the image by
         * @param yfactor [in]; Factor to downsample the y dimension of the image by
         * @param smooth  [in, opt]; Optional. Whether or not to smooth the
         *                         resulting image
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result down_sample (unsigned xfactor, unsigned yfactor, bool smooth = true);

        /**
         * @brief This function creates a copy of the image, down sampled by the given
         * factor. The sampling may be smoothed by a filter or fast
         * and rough.
         *
         * @param xfactor [in]; Factor to downsample the x dimension of the image by
         * @param yfactor [in]; Factor to downsample the y dimension of the image by
         * @param output  [out]; pointer to image object to place copy in
         * @param smooth  [in, opt]; Optional. Whether or not to smooth the resulting image
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result down_sample_copy (unsigned xfactor, unsigned yfactor,
                                 Image* output, bool smooth = true) const;

        /**
         * @brief This function crops the image to the specified rectangle (x0,y0) - (x1,y1).
         *
         * Crops the image to the specified rectangle (x0,y0) -> (x1,y1).
         * Changes width to x1-x0, height to y1-y0. Keeps all data in the crop rectangle, discards the remainder.
         * 
         * @param x0 [in]; Upper left corner of crop rectangle, x coordinate (in image coordinates)
         * @param y0 [in]; Upper left corner of crop rectangle, y coordinate (in image coordinates)
         * @param x1 [in]; Lower right corner of crop rectangle, x coordinate (in image coordinates)
         * @param y1 [in]; Lower right corner of crop rectangle, y coordinate (in image coordinates)
         *
         * @return RESULT_SUCCESS on success or an error result on error.
         */
        Result crop (unsigned x0, unsigned y0, unsigned x1, unsigned y1);

        /**
         * @brief This function creates a copy of the subimage in the specified rectangle
         * (x0,y0) - (x1,y1).
         *
         * Crops the image to the specified rectangle (x0,y0) -> (x1,y1).
         * Changes width to x1-x0, height to y1-y0. Copies all data contained
         * within the crop rectangle to the target image.
         * 
         * @param x0     [in]; Upper left corner of crop rectangle, x coordinate (in image coordinates)
         * @param y0     [in]; Upper left corner of crop rectangle, y coordinate (in image coordinates)
         * @param x1     [in]; Lower right corner of crop rectangle, x coordinate (in image coordinates)
         * @param y1     [in]; Lower right corner of crop rectangle, y coordinate (in image coordinates)
         * @param output [out]; Pointer to image object to place copy in
         *
         * @return RESULT_SUCCESS on success or an error result on error.
         */
        Result crop_copy (unsigned x0, unsigned y0,
                          unsigned x1, unsigned y1,
                          Image* output) const;

        /**
         * @brief This function resizes the buffer without preserving existing data.
         * 
         * Resizes the buffer without preserving existing data.  This is used to resize an image. 
         *
         * @param width      [in];  New width of buffer
         * @param height     [in]; New height of buffer
         * @param dst_format [in, opt]; Optional. ColorFormat of the new buffer
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result resize_buffer (unsigned width, unsigned height,
                              ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED)
        {
            if (dst_format == COLOR_FORMAT_UNSPECIFIED
                && _format == COLOR_FORMAT_UNSPECIFIED) 
            {
                return RESULT_PRECONDITION_ERROR;
            }
            if (dst_format == COLOR_FORMAT_UNSPECIFIED)
            {
                dst_format = _format;
            }
            return (alloc_buffer (width, height, dst_format));
        } // end resize_buffer()

        /**
         * @brief This function reformats the buffer without preserving existing
         * data.
         * 
         * Resizes the buffer without preserving existing data. After a call,
         * this Image object will have blank or garbage data in the new
         * ColorFormat.
         *
         * @param dst_format [in]; ColorFormat of the new buffer
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result reformat_buffer (ColorFormat dst_format)
        {
            PRECOND (dst_format != COLOR_FORMAT_UNSPECIFIED);
            return (alloc_buffer (_width, _height, dst_format));
        } // end reformat_buffer()

    public: // Conversion.

        /**
         * @brief This function outputs the image to a grayscale matrix,
         * converting the image if needed.
         * 
         * Outputs the image to a grayscale matrix, converting the image
         * data if needed.
         *
         * @param m [out]; Matrix to write image information to
         */
        template<class T>
        void output_matrix (Matrix<T>* m) const 
        {
            if(m == NULL) return;
            m->fast_resize (_height, _width);
            if (((unsigned)m->ncols () != _width) || ((unsigned)m->nrows () != _height))
            {
                ERSP_LOG_CLASS_WARN (Image)("Failed to size output matrix from image.");
                return;
            }

            // Handle grayscale friendly formats.
            size_t len = m->nrows () * m->ncols ();
            T* m_data = m->get_linear_data ();
            switch (_format)
            {
            case COLOR_FORMAT_IYUV:
            case COLOR_FORMAT_YVU9:
            case COLOR_FORMAT_YV12:
            case COLOR_FORMAT_YUV:
            case COLOR_FORMAT_GRAYSCALE:
            {
                for (register size_t i = 0; i < len; ++i)
                {
                    m_data[i] = _data[i];
                }
                return;
            }
            default:
                break;
            } // end friendly formats

            Image image_copy (*this, COLOR_FORMAT_GRAYSCALE);
            uchar* data = image_copy.get_raw_data ();
            for (register size_t i = 0; i < len; ++i)
            {
                m_data[i] = data[i];
            }
        } // end output_matrix()

        /**
         * @brief This function outputs the image to a grayscale matrix,
         * converting the image if needed.
         *
         * @param m [out]; Matrix to write image information to
         */
        void output_matrix (Matrix<uchar>* m) const;

        /**
         * @brief This function outputs the image to an RGB24 matrix,
         * converting the image if needed.
         *
         * @param colors [out]; ColorMatrix to write information to
         */
        void output_matrix (ColorMatrix* colors) const;

        /**
         * @brief This function copies a block to mx0,my0 in m of w,h from ix0,
         * iy0 in this Image object.
         * 
         * The function outputs the image to a matrix, of h by w, rooted at
         * ix0,iy0 in image, and copied into mx0, my0 in matrix m. If anything
         * goes out of bounds a RESULT_PRECONDITION_ERROR is returned.
         *
         * @param  m   [in];  Matrix<double> to read the data from
         * @param  ix0 [in];  image x root to copy block from
         * @param  iy0 [in];  image y root to copy block from
         * @param  mx0 [in];  matrix x root to copy block to
         * @param  my0 [in];  matrix y root to copy block to
         * @param  w   [in];  Width of the block to copy
         * @param  h   [in];  Height of the block to copy
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        template<class T>
        Result output_matrix (Matrix<T>* m, unsigned ix0, unsigned iy0, 
                              unsigned mx0, unsigned my0, 
                              unsigned w, unsigned h) const
        {
            unsigned ix1 = ix0 + w; 
            unsigned iy1 = iy0 + h;
            unsigned mx1 = mx0 + w; 
            unsigned my1 = my0 + h; 

            if((ix1 > get_height()) ||
               (iy1 > get_height()) ||
               (mx1 > (unsigned)m->csize())    ||
               (my1 > (unsigned)m->rsize())    ||
               (ix0 > ix1)    ||
               (iy0 > iy1)    ||
               (mx0 > mx1)    ||
               (my0 > my1)    )
            {
                return RESULT_PRECONDITION_ERROR;
            }

            switch (_format)
            {
            case COLOR_FORMAT_IYUV:
            case COLOR_FORMAT_YVU9:
            case COLOR_FORMAT_YV12:
            case COLOR_FORMAT_YUV:
            case COLOR_FORMAT_GRAYSCALE:
            {
                //transfer Y to matrix.
                unsigned int i=0, j=0;
                for( i=0; i<h; i++)
                {
                    for( j=0; j<w; j++)
                    {
                        (*m)[i+my0][j+mx0] = _data[(i+ix0) * _width + (j+iy0)]; //y component first in _data, as a block
                    }
                }
                return RESULT_SUCCESS;               
            }
            default:
                break;
            } // end friendly formats

            if(_format == COLOR_FORMAT_RGB24)
            {
                //transfer rgb to matrix, ntsc style.
                unsigned int i=0, j=0;
                unsigned offset=0;
                for(i=0;i<h;i++)
                {
                    for(j=0;j<h;j++)
                    {
                        offset = ( (i + iy0) * _width + j + ix0 ) * 3;
                        (*m)[i+my0][j+mx0] = .26*_data[offset + 0] +.62* _data[offset + 1] + .12*_data[offset + 2] ;
                    }
                }
                return RESULT_SUCCESS;
            }
            return RESULT_NOT_IMPLEMENTED; //not imped for friendly and rgb formats.
        } // end output_matrix(Matrix<uchar>)

        /**
         * @brief This function takes the values in channel, writes
         * their numeric value to matrix m, and resizes m.
         *
         * @param  m       [in]; Matrix to read the data from
         * @param  channel [in]; Channel of the Image to take data from.
         *                       Channel -1 results in averaging channel data
         *                       before write.
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        template<class T>
        Result output_matrix (Matrix<T>* m, int channel) const
        {
            if(m == NULL) return RESULT_PRECONDITION_ERROR;
            if(channel < 0 || channel > 2) return RESULT_PRECONDITION_ERROR;

            m->fast_resize (_height, _width);
            if (((unsigned)m->ncols () != _width) || ((unsigned)m->nrows () != _height))
            {
                ERSP_LOG_CLASS_WARN (Image)("Failed to size output matrix from image.");
                return RESULT_FAILURE;
            }
            
            // Handle planar formats
            size_t len = m->nrows () * m->ncols ();
            T* m_data = m->get_linear_data ();
            size_t offset = channel * m->ncols();
            switch (_format)
            {
            case COLOR_FORMAT_GRAYSCALE:
                if(channel > 0) return RESULT_PRECONDITION_ERROR;
                //fallthrough
            case COLOR_FORMAT_IYUV:
            {
                for (register size_t i = 0; i < len; ++i)
                {
                    m_data[i] = _data[i + offset];
                }
                return RESULT_SUCCESS;
            }
            default:
                break;
            } // end good planar
             
            if(_format == COLOR_FORMAT_RGB24)
            {
                unsigned rows      = m->rsize();
                unsigned cols      = m->csize();
                uchar rgb[3];
                for (unsigned i = 0; i < rows; ++i)
                {
                    for (unsigned j = 0; j < cols; ++j)
                    {
                        get_pixel_rgb24_fast(j, i, &rgb[0], &rgb[1], &rgb[2]);
                        (*m)[i][j] = rgb[channel];
                    }
                }
            }  

            return RESULT_SUCCESS;
        }

        /**
         * @brief This function resizes image to rsize,csize of m, then copies
         * the data into the image from m.
         *
         * The function reads a matrix into the specified channel, and if the
         * channel is negative, the values will be written into all channels
         * (resulting in 3 channel grayscale). If the channel value is larger
         * than the number of channels, the matrix will be written into no
         * channels.  The function resizes the image.
         * 
         * @param  m       [in]; Matrix<double> to read the data from
         * @param  channel [in, opt]; Optional. Copy the block into channel
         *                            0,1,2 (r,g,b;y,u,v). Channel -1 means
         *                            all channels. Channel > num_channels does
         *                            nothing
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_matrix_channel(Matrix<double>& m, int channel = -1);

        /**
         * @brief This function copies a block from mx0,my0 in m of w,h to ix0, iy0 in this Image.
         * 
         * The function copies a w,h block from m rooted at mx,my to this rooted at ix,iy in the channel
         * specified.  If channel < 1, it overwrites all channels.  If channel > num_channels,
         * nothing is done.
         * as a cautionary notice, this only really works for rgb right now.
         *
         * @param   m       [in];  Matrix<double> to read the data from
         * @param   ix0     [in];  Image x root to copy block to
         * @param   iy0     [in];  Image y root to copy block to
         * @param   mx0     [in];  Matrix x root to copy block from
         * @param   my0     [in];  Matrix y root to copy block from
         * @param   w       [in];  Width of the block to copy
         * @param   h       [in];  Height of the block to copy
         * @param   channel [in, opt]; Optional. Copy the block into channel
         *                             0,1,2 (r,g,b;y,u,v). -1 means all channels.
         *                             Channel > num_channels does nothing
         *                       
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_matrix_channel(Matrix<double>& m, 
                                   unsigned ix0, unsigned iy0, 
                                   unsigned mx0, unsigned my0, 
                                   unsigned w, unsigned h, 
                                   int channel = -1);
        
        /**
         * @brief This function creates a false color image from the matrix
         * passed in.
         *
         * The false color starts at black (value <= low), scales linearly
         * through the spectrum, and ends at white at high. (value >= high).
         *
         * The color_format ends up RGB24.  If low or high are NAN, low or high
         * are taken from the min/max values of the matrix.
         *
         * @param  m    [in]; Matrix<double> to read the data from
         * @param  low  [in]; If m[i][j] <= low, image[j][i] = low
         * @param  high [in]; If m[i][j] >= high, image[j][i] = high
         * 
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_matrix_false_color(Matrix<double>& m, double low, double high);

    public: // Input/Output

        /**
         * @brief  This function prints the image to the stream.
         *
         * @param out [out]; The output stream to write to
         */
        void print (std::ostream& out) const;

        /**
         * @brief This function reads image data from a file and stores it in this Image.
         * 
         * @note If the filename is path has a recognizable extension, file_format becomes
         * redundant.  At least jpg, png, bmp, and raw are currently supported.
         *
         * @param path        [in]; The path + filename of the file to write.
         *                          Can include format extension
         * @param file_format [in, opt]; Optional quality to pass to format
         *                               processor. Higher is better
         * @param dst_format  [in, opt]; Optional file format. Needed if no
         *                               extension is given in path
         * @param reader      [in, opt]; Optional. Almost never used. Allows
         *                               IImageReader interface to be passed
         *                               to read image data from
         * 
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_file (const char* path, const char* file_format = NULL,
                          ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED,
                          IImageReader* reader = NULL);

        /**
         * @brief This function reads raw image data from file and stores 
         * it in this Image.
         * 
         * A raw file is just a buffer write of some image data in uncompressed
         * format, following some the convention of it's ColorSpace. This
         * function, given the information it needs, reads that information
         * in and stores it in this Image. 
         * 
         * @param   path        [in];  The path + filename of the raw file to read
         * @param   src_format  [in];  Format of the incoming raw file
         * @param   src_width   [in];  Width of raw data buffer
         * @param   src_height  [in];  Height of raw data buffer
         * @param   dst_format  [in, opt]; Optional. Format of Image object
         *                                 (if different from src_format)
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_raw_file (const char* path, ColorFormat src_format,
                              unsigned src_width, unsigned src_height,
                              ColorFormat dst_format = COLOR_FORMAT_UNSPECIFIED);

        /**
         * @brief This function writes the current image data to a file, often
         * autodetecting the file format from the path name.
         * 
         * @note If the filename is path has a recognizable extension,
         * file_format becomes redundant. At least jpg, png, bmp, and raw are
         * currently supported.
         *
         * @param path        [in]; The path + filename of the file to write.
         *                          Can include format extension
         * @param quality     [in, opt]; Optional quality to pass to format
         *                               processor. Higher is better
         * @param file_format [in, opt]; Optional file format. Needed if no
         *                               extension is given in path
         * @param writer      [in, opt]; Optional. Almost never used. Allows
         *                               IImageWriter interface to be passed to
         *                               feed image data to
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result write_file (const char* path, 
                           double quality = 0.5,
                           const char* file_format = NULL,
                           IImageWriter* writer = NULL) const;

#ifndef DOXYGEN_INTERNAL
        //this appears to be a helper function for some of the above. I'm not
        //sure exactly why to put it in doxygen, so I'm refraining.
        Result write_file_with_extension (const char* path,
                                          double quality = 0.5,
                                          const char* file_format = NULL,
                                          IImageWriter* writer = NULL) const;
#endif // end DOXYGEN_INTERNAL

    public: // Equality

        /**
         * @brief Equality operator. A bit silly perhaps, but useful
         * for testing.
         *
         * @param other [in]; The image to compare with
         *
         * @return true if the images are equal.
         */
        bool operator== (const Image& other) const;

        /**
         * @brief Inequality operator.
         *
         * @param other [in]; The image to compare with
         *
         * @return true if the images are not equal.
         */
        bool operator!= (const Image& other) const
        {
            return !(*this == other);
        }

    protected: // Helpers

#ifndef DOXYGEN_INTERNAL
        /**
         * @brief This function allocates the buffer for the specified dimensions.
         */
        Result alloc_buffer (unsigned width, unsigned height, ColorFormat format);        

        // Check to see if two lines intersect each other.  p0 q0 only written to
        // if bool is true.
        bool intersect (int px0, int py0, int qx0, int qy0,
                        int px1, int py1, int qx1, int qy1,
                        double * x0, double * y0);
#endif // end DOXYGEN_INTERNAL

    protected: // Implementation

        unsigned           _width;          ///< Width.
        unsigned           _height;         ///< Height.
        size_t             _data_size;      ///< Size of the data.
        uchar*             _data;           ///< Data.
        ColorFormat        _format;         ///< Format.
        Timestamp          _timestamp;      ///< Time at image creation.
        CameraParameters*  _camera_params;  ///< Camera parameters during image creation.

        /// Logging declaration
        ERSP_LOG_CLASS_CATEGORY_DECLARE(Image, "Evolution.Core.Resource.Image");
    }; // end class Image

} // end namespace Evolution

/**
 * @brief Stream operator for image class.
 *
 * @param out   [out]; The output stream to write to
 * @param image [in]; Image object to write on stream
 *
 * @return Reference to the ostream passed in.
 */ 
inline std::ostream& 
operator<< (std::ostream& out, const Evolution::Image& image)
{
    image.print (out);
    return (out);
}

#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IMAGE_HPP
