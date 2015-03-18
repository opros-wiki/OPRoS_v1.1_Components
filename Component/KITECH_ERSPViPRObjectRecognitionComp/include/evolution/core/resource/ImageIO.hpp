//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file  evolution/core/resource/ImageIO.hpp
 * @brief This file declares ImageIO class; IImageReaderProvider,
 * IImageWriterProvider, IImageReader, IImageWriter,
 * IImageInputStream, and IImageOutputStream interfaces;
 * ImageMemoryInputStream, ImageMemoryOutputStream,
 * ImageFileInputStream and ImageFileOutputStream
 * implementations.
 * @author Copyright (C) 2001-2007, Evolution
 * @see    Evolution::ImageIO
 * @see    Evolution::ReaderRegistry
 * @see    Evolution::WriterRegistry
 * @see    Evolution::ImageMemoryInputStream
 * @see    Evolution::ImageMemoryOutputStream
 * @see    Evolution::ImageFileInputStream
 * @see    Evolution::ImageFileOutputStream
 */

#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IMAGEIO_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IMAGEIO_HPP


#include <evolution/core/util/ObjectRegistry.hpp>
#include <evolution/core/resource/Colorspace.hpp>

#include <list>


namespace Evolution
{

    // Forward Declarations
    class Image;
    class TCPSocket;

    class IImageReaderProvider;
    class IImageWriterProvider;
    class IImageReader;
    class IImageWriter;
    class IImageInputStream;
    class IImageOutputStream;

    /**
     * @interface IImageReaderProvider
     *
     * @brief This defines the IImageReaderProvider interface, for classes
     * that are image reader providers for various image formats.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE IImageReaderProvider
    {
    public:

        /**
         * @brief Constructor.
         */
        virtual ~IImageReaderProvider () {};

        /**
         * @brief The get_format_name function returns the name of the
         * format handled by the provider.
         *
         * @return The name of the format handled by the provider.
         */
        virtual const char* get_format_name () const = 0;

        /**
         * @brief The get_extensions function returns a list of file
         * extensions handled by the provider.
         *
         * @return The list of file extensions handled by the provider.
         */
        virtual std::list<String> get_extensions () const = 0;

        /**
         * @brief The get_mime_types function returns a list of MIME types
         * handled by the provider.
         *
         * @return The list of MIME types handled by the provider.
         */
        virtual std::list<String> get_mime_types () const = 0;

        /**
         * @brief The can_read function should return true if the provider
         * thinks it recognizes and can handle the image data, false
         * otherwise.
         *
         * @param data     [in]; A pointer to the image data
         * @param data_len [in]; The size of the image data
         *
         * @return true if the provider recognizes the image data, false otherwise.
         */
        virtual bool can_read (const uchar* data, size_t data_len) const = 0;

        /**
         * @brief This function returns a freshly allocated image reader.
         *
         * @return A freshly allocated image reader.
         */
        virtual IImageReader* create_reader () const = 0;
    }; // end class IImageReaderProvider

    /**
     * @interface IImageWriterProvider
     *
     * @brief This class defines the IImageWriterProvider interface,
     * for classes that are image writer providers for various image
     * formats.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE IImageWriterProvider
    {
    public:

        /**
         * @brief Constructor.
         */
        virtual ~IImageWriterProvider () {};

        /**
         * @brief The get_format_name function returns the name of the
         * format handled by the provider.
         *
         * @return The name of the format handled by the provider.
         */
        virtual const char* get_format_name () const = 0;

        /**
         * @brief The get_extensions function returns a list of file
         * extensions handled by the provider.
         *
         * @return The list of file extensions handled by the provider.
         */
        virtual std::list<String> get_extensions () const = 0;

        /**
         * @brief The get_mime_types function returns a list of MIME types
         * handled by the provider.
         *
         * @return The list of MIME types handled by the provider.
         */
        virtual std::list<String> get_mime_types () const = 0;

        /**
         * @brief The create_writer function returns a freshly allocated
         * image writer.
         *
         * @return A freshly allocated image reader.
         */
        virtual IImageWriter* create_writer () const = 0;
    }; // end class IImageWriterProvider


    /**
     * @class ImageIO
     *
     * @brief The ImageIO class is a system for reading and writing
     * images in various formats.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE ImageIO
    {
    public: // Types

        /// Definition for the Reader Registry
        typedef ObjectRegistry<IImageReaderProvider*> ReaderRegistry;
        /// Definition for the Writer Registry
        typedef ObjectRegistry<IImageWriterProvider*> WriterRegistry;

    protected: // Structors

        /**
         * @brief Don't use a contructor; all the useful methods of
         * ImageIO are static.
         */
        ImageIO () {}

        /**
         * @brief No need for a real destructor; all the useful methods of
         * ImageIO are static.
         */
        ~ImageIO () {}

    public: // Convenience Writing Functions

        /**
         * @brief The write function Writes an Image using an IImageWriter
         * that supports the specified format to a file.
         *
         * @param image   [in]; The image to write
         * @param format  [in]; The format in which to write the image
         *                      ("JPEG", "BMP", "PNG", and "RAW")
         * @param options [in]; Options governing how the image is formatted
         *                      and written
         * @param path    [in]; The destination file
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @see ImageWriter::set_option for option params
         */
        static Result write (const Image& image,
                             const char* format,
                             const char* options,
                             const char* path);

        /**
         * @brief The write function Writes an Image using an IImageWriter
         * that supports the specified format to a socket.
         *
         * @param image   [in]; The image to write
         * @param format  [in]; The format in which to write the image
         *                      ("JPEG", "BMP", "PNG", and "RAW")
         * @param options [in]; Options governing how the image is formatted
         *                      and written
         * @param socket  [in]; The destination socket
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @see ImageWriter::set_option for option params
         */
        static Result write (const Image& image,
                             const char* format,
                             const char* options,
                             TCPSocket* socket);

        /**
         * @brief The write function writes an Image using an IImageWriter
         * that supports the specified format to memory.
         *
         * @param image        [in]; The image to write
         * @param format       [in]; The format in which to write the image
         *                           ("JPEG", "BMP", "PNG", and RAW)
         * @param options      [in]; Options governing how the image is formatted
         *                           and written
         * @param data         [out]; A pointer to the destination memory block
         * @param data_max_len [in]; The size of the destination memory block
         * @param num_bytes_written_ptr [out]; A pointer to a size_t where the
         *                                     actual size of the raw image data
         *                                     that was written will be stored
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @pre data != NULL
         * @pre num_bytes_written_ptr != NULL
         *
         * @see ImageWriter::set_option for option params
         */
        static Result write (const Image& image,
                             const char* format,
                             const char* options,
                             uchar* data,
                             size_t data_max_len,
                             size_t* num_bytes_written_ptr);

    public: // Convenience Reading Functions

        /**
         * @brief The read function reads an Image from a file using an
         * ImageReader automatically chosen from among those currently
         * registered.
         *
         * @param path           [in]; The source file
         * @param format         [in]; The format in which the image was 
         *                             written ("JPEG", "BMP", "PNG", and RAW)
         *                             If NULL, the system will try to select
         *                             the proper reader automatically.
         * @param desired_format [in]; The desired ColorFormat of the new image
         * @param image_ptr      [out]; A pointer to where a pointer to the freshly
         *                              allocated Image will be stored
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @pre image_ptr != NULL
         */
        static Result read (const char* path,
                            const char* format,
                            ColorFormat desired_format,
                            Image** image_ptr);

        /**
         * @brief The read function reads an Image from memory using an
         * ImageReader automatically chosen from among those currently
         * registered.
         *
         * @param data           [in]; A pointer to the source memory block
         * @param data_len       [in]; The size (in bytes) of the source memory
         *                             block
         * @param format         [in]; The format in which the image was 
         *                             written ("JPEG", "BMP", "PNG", and RAW)
         *                             If NULL, the system will try to select
         *                             the proper reader automatically.
         * @param desired_format [in]; The desired ColorFormat of the new image
         * @param data_len_ptr [out]; A pointer to a size_t where the
         *                            actual size of the raw image data
         *                            that was read will be stored
         * @param image_ptr [out]; A pointer to where a pointer to the freshly
         *                         allocated Image will be stored
         *
         * @return RESULT_SUCCESS on success or an error result.
         *
         * @pre data_len_ptr != NULL
         * @pre image_ptr != NULL
         */
        static Result read (const uchar* data,
                            size_t data_len,
                            const char* format,
                            ColorFormat desired_format,
                            size_t* data_len_ptr,
                            Image** image_ptr);

    public: // Access

        /**
         * @brief The get_reader_format_names function returns a list of
         * the formats supported by the currently registered image
         * readers.
         *
         * @return A list of the formats supported by the currently registered
         * image reader.
         */
        static std::list<String> get_reader_format_names();

        /**
         * @brief The get_writer_format_names function returns a list of
         * the formats supported by the currently registered image
         * writers.
         * @return A list of the formats supported by the currently registered
         * image writers.
         */
        static std::list<String> get_writer_format_names();

        /**
         * @brief The get_reader_by_format_name returns a new image reader
         * that can handle the specified format.
         *
         * @param format [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         *
         * @return A new image reader object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageReader* get_reader_by_format_name(const char* format);

        /**
         * @brief The get_reader_by_extension function returns a new image
         * reader that can handle the specified file extension.
         *
         * @param extension [in]; The file extension
         *
         * @return A new image reader object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageReader* get_reader_by_extension(const char* extension);

        /**
         * @brief The get_reader_by_mime_type function returns a new image
         * reader that can handle the specified MIME type.
         *
         * @param mime_type [in]; The MIME type
         *
         * @return A new image reader object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageReader* get_reader_by_mime_type(const char* mime_type);

        /**
         * @brief The get_writer_by_format_name function returns a new
         * image writer that can handle the specified format (e.g.,
         * "JPEG", "PNG", "BMP").
         *
         * @param format [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         *
         * @return A new image writer object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageWriter* get_writer_by_format_name(const char* format);

        /**
         * @brief The get_writer_by_extension function returns a new image
         * writer that can handle the specified file extension (e.g.,
         * "jpg", "jpeg", "PNG").
         *
         * @param extension [in]; The file extension
         *
         * @return A new image writer object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageWriter* get_writer_by_extension(const char* extension);

        /**
         * @brief The get_writer_by_mime_type function returns a new image
         * writer that can handle the specified MIME type (e.g.,
         * "image/png", "image/gif", "image/jpeg").
         *
         * @param mime_type [in]; The MIME type
         *
         * @return A new image writer object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageWriter* get_writer_by_mime_type(const char* mime_type);

        /**
         * @brief The choose_reader_provider_for_file function uses
         * several criteria to find a suitable IImageReaderProvider.
         *
         * The function checks the following criteria, in order, if they
         * have been supplied:
         *
         * 1. Image format.
         * 2. MIME type
         * 3. Filename (checks the extension)
         * 4. File header data.
         *
         * @param format    [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         * @param mime_type [in]; The MIME type
         * @param path      [in]; The source file path
         * @param file      [in]; The source file
         *
         * @return A new IImageReaderProvider object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageReaderProvider* choose_reader_provider_for_file (
                                                        const char* format,
                                                        const char* mime_type,
                                                        const char* path,
                                                        FILE* file);

        /**
         * @brief The choose_reader_for_file function uses several
         * criteria to find a suitable IImageReader.
         *
         * Checks the following criteria, in order, if they have been supplied:
         * 1. Image format.
         * 2. MIME type
         * 3. Filename (checks the extension)
         * 4. File header data.
         *
         * @param format    [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         * @param mime_type [in]; The MIME type
         * @param path      [in]; The source file path
         * @param file      [in]; The source file
         *
         * @return A new IImageReader object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageReader* choose_reader_for_file (const char* format,
                                                     const char* mime_type,
                                                     const char* path,
                                                     FILE* file);

        /**
         * @brief The choose_reader_provider_for_memory function uses
         * several criteria to find a suitable IImageReaderProvider.
         *
         * Checks the following criteria, in order, if they have been supplied:
         * 1. Image format.
         * 2. MIME type
         * 3. Filename (checks the extension)
         * 4. Header data in memory.
         *
         * @param format    [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         * @param mime_type [in]; The MIME type
         * @param path      [in]; The source file path
         * @param data      [in]; The source data buffer
         * @param data_len  [in]; The size (in bytes) of the source data buffer
         *
         * @return A new IImageReaderProvider object (caller has the
         * responsibility to delete it), or NULL on failure.
         */
        static IImageReaderProvider* choose_reader_provider_for_memory (
                                            const char* format,
                                            const char* mime_type,
                                            const char* path,
                                            const uchar* data,
                                            size_t data_len);

        /**
         * @brief The choose_reader_for_memory function uses several
         * criteria to find a suitable IImageReader.
         *
         * Checks the following criteria, in order, if they have been supplied:
         * 1. Image format.
         * 2. MIME type
         * 3. Filename (checks the extension)
         * 4. Header data in memory.
         *
         * @param format    [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         * @param mime_type [in]; The MIME type
         * @param path      [in]; The source file path
         * @param data      [in]; The source data buffer
         * @param data_len  [in]; The size (in bytes) of the source data buffer
         *
         * @return A new IImageReader object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageReader* choose_reader_for_memory (const char* format,
                                                       const char* mime_type,
                                                       const char* path,
                                                       const uchar* data,
                                                       size_t data_len);

        /**
         * @brief The choose_writer_provider function uses several
         * criteria to find a suitable IImageWriterProvider.
         *
         * Checks the following criteria, in order, if they have been supplied:
         * 1. Image format.
         * 2. MIME type
         * 3. Filename (checks the extension)
         *
         * @param format    [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         * @param mime_type [in]; The MIME type
         * @param path      [in]; The source file path
         *
         * @return A new IImageReader object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageWriterProvider* choose_writer_provider (
                                                        const char* format,
                                                        const char* mime_type,
                                                        const char* path);

        /**
         * @brief The choose_writer function uses several criteria to find
         * a suitable IImageWriterProvider.
         *
         * Checks the following criteria, in order, if they have been supplied.
         * 1. Image format.
         * 2. MIME type
         * 3. Filename (checks the extension)
         *
         * @param format    [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         * @param mime_type [in]; The MIME type
         * @param path      [in]; The source file path
         *
         * @return A new IImageWriter object (caller has the responsibility to
         * delete it), or NULL on failure.
         */
        static IImageWriter* choose_writer (const char* format,
                                            const char* mime_type,
                                            const char* path);

    public: // Provider registration

        /**
         * @brief The register_reader_provider function is used by image
         * reader providers to register themselves as part of the ImageIO
         * framework.
         *
         * @param provider [in]; The provider to register
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        static Result register_reader_provider (IImageReaderProvider* provider);

        /**
         * @brief The register_writer_provider function is used by image
         * writer providers to register themselves as part of the ImageIO
         * framework.
         *
         * @param provider [in]; The provider to register
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        static Result register_writer_provider (IImageWriterProvider* provider);

        /**
         * @brief The get_reader_provider_by_format_name function looks up
         * an image reader provider for a given format ("JPEG", "BMP",
         * etc.).
         *
         * @param format [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageReaderProvider* get_reader_provider_by_format_name (const char* format);

        /**
         * @brief The get_reader_provider_by_extension function looks up
         * an image reader provider for a given file extension ("jpg",
         * "png", etc.).
         *
         * @param extension [in]; The image file extension
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageReaderProvider* get_reader_provider_by_extension (const char* extension);

        /**
         * @brief The get_reader_provider_by_mime_type function looks up
         * an image reader provider for a given MIME type ("image/png",
         * "image/gif", etc.).
         *
         * @param mime_type [in]; The MIME type
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageReaderProvider* get_reader_provider_by_mime_type (const char* mime_type);

        /**
         * @brief The get_reader_provider_for_data function tries to look
         * for magic numbers in image data to figure out which image
         * reader provider to use.
         *
         * @param data     [in]; The image data buffer
         * @param data_len [in]; Size of the image data buffer
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageReaderProvider* get_reader_provider_for_data (
                                                            const uchar* data,
                                                            size_t data_len);

        /**
         * @brief The get_writer_provider_by_format_name function looks up
         * an image writer provider for a given format ("JPEG", "BMP", "PNG"
         * etc.).
         *
         * @param format [in]; The image format ("JPEG", "BMP", "PNG", and RAW)
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageWriterProvider* get_writer_provider_by_format_name (const char* format);

        /**
         * @brief The get_writer_provider_by_extension function looks up
         * an image writer provider for a given file extension ("jpg",
         * "png", etc.).
         *
         * @param extension [in]; The image file extension
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageWriterProvider* get_writer_provider_by_extension (const char* extension);

        /**
         * @brief The get_writer_provider_by_mime_type function looks up
         * an image writer provider for a given MIME type ("image/png",
         * "image/gif", etc.).
         *
         * @param mime_type [in]; The MIME type
         *
         * @return The IImageReaderProvider (call must NOT delete this), or
         * NULL on failure.
         */
        static IImageWriterProvider* get_writer_provider_by_mime_type (const char* mime_type);

    protected: // Helpers

        /**
         * @brief Checks whether there exists a reader register. If
         * there is none, initializes a new reader registry.
         */
        static Result ensure_reader_registry ();

        /**
         * @brief Creates and initializes a reader registry for the
         * currently-supported image types.
         */
        static Result initialize_reader_registry ();

        /**
         * @brief Checks whether there exists a writer register. If
         * there is none, initializes a new reader registry.
         */
        static Result ensure_writer_registry ();

        /**
         * @brief Creates and initializes a writer registry for the
         * currently-supported image types.
         */
        static Result initialize_writer_registry ();
    }; // end class ImageIO

   /**
    * @interface IImageReader
    *
    * @brief This class defines the IImageReader interface for classes that
    * implement the reading of a particular image format.
    *
    * @ingroup core_vision
    */
   class EVOLUTION_EXPORT_RESOURCE IImageReader
    {
    public:

        virtual ~IImageReader () {};

        /**
         * @brief The set_input function sets the image reader's source of
         * input.
         *
         * It is acceptable for the image reader to begin reading
         * image data when this method is called (which may be a
         * convenient time to read image header data, for example).
         *
         * @param stream [in]; A pointer to the IImageInputStream from
         * which the image will be read
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result set_input (IImageInputStream* stream) = 0;

        /**
         * @brief The get_width function returns the width of the image, in
         * pixels.
         *
         * @return The width of the image, in pixels.
         */
        virtual int get_width () = 0;

        /**
         * @brief The get_height function returns the height of the image, in
         * pixels.
         *
         * @return The height of the image, in pixels.
         */
        virtual int get_height () = 0;

        /**
         * @brief The get_color_format function returns the color format of the
         * image.
         *
         * @return The color format of the image.
         */
        virtual ColorFormat get_color_format () = 0;

        /**
         * @brief This method provides a way for users of an image reader
         * to query for arbitrary metadata contained in an image.
         *
         * @param label [in]; A string identifying the metadata.  The
         * meaning of this string is entirely arbitrary, and may be
         * interpreted by the image reader as it sees fit.  For example,
         * the JPEG reader interprets "comment" to mean the comments
         * embedded in a JPEG file.
         *
         * @return a pointer to the metadata.  It is up to the image
         * reader to specify the semantics of this pointer.
         */
        virtual void* get_meta_data (const char* label);


        /**
         * @brief The read function actually reads the image data.
         *
         * @param data         [out]; A pointer to a buffer in which to save
         *                            the image data
         * @param data_len     [in];  The size of the data buffer
         * @param num_read_ptr [out]; The number of bytes read from the input
         *                            source
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result read (uchar* data,
                             size_t data_len,
                             size_t* num_read_ptr) = 0;
    }; // end class IImageReader

   /**
    * @interface IImageWriter
    *
    * @brief The IImageWriter class defines the IImageWriter
    * interface, for classes that implement the writing of a
    * particular image format.
    *
    * @ingroup core_vision
    */
    class EVOLUTION_EXPORT_RESOURCE IImageWriter
    {
    public:
        
        /**
         * @brief Constructor.
         */
        IImageWriter () {}

        /**
         * @brief Destructor
         */
        virtual ~IImageWriter () {}

        /**
         * @brief The set_output function sets the image writer's output
         * destination.
         *
         * @param stream [in]; A pointer to the IImageOutputStream to
         * which the image will be written
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result set_output (IImageOutputStream* stream) = 0;

        /**
         * @brief The set_option function offers a general way to set
         * various options related to writing out an image that may be
         * specific to a given format.
         *
         * For example, a number of image formats support embedding text
         * "comments" as part of the image data. An image writer could
         * allow one to specify a comment by using an option named
         * "comment", where the value was a string containing the comment.
         *
         * @param option [in]; A string identifying the option to set
         * @param value  [in]; A string specifing an option value
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result set_option (const char* option, const char* value);

        /**
         * @brief The set_options function offers a convenient way to set
         * several options at once.
         *
         * The format of the options string must be
         * "option1=value1;option2=value2...".
         *
         * @param options [in]; A string identifying the options to set.
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result set_options (const char* options);


        /**
         * @brief The write function actually writes the image data.
         *
         * @param width          [in];  The width of the image, in pixels
         * @param height         [in];  The height of the image, in pixels
         * @param color_format   [in];  The image's color format
         * @param data           [in];  A pointer to the image data
         * @param data_len       [in];  The size of the image data
         * @param num_bytes_written [out] The number of bytes written to the output destination
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result write (int width, int height, ColorFormat color_format,
                              const uchar* data, size_t data_len, 
                              size_t* num_bytes_written) = 0;

        /// Delimiter for option strings (e.g., ';')
        static const char OPTION_DELIMITER;

#ifndef DOXYGEN_INTERNAL
        protected: // Types

            typedef std::list<std::pair<String,String> > OptionList;

        protected: // Helper methods

            virtual OptionList parse_options (const char* options);
#endif // end DOXYGEN_INTERNAL
    }; // end class IImageWriter

    /**
     * @interface IImageInputStream
     *
     * @brief The IImageInputStream class defines the Input stream interface
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE IImageInputStream
    {
    public:

        /**
         * @brief Destructor.
         */
        virtual ~IImageInputStream () {};

        /**
         * @brief On EOF, this method should return a failure result, and
         * not set the byte.
         *
         * @param byte_ptr [out]; A pointer to a buffer in which to store
         *                        a byte of image data
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result read_byte (uchar* byte_ptr) = 0;

        /**
         * @brief On EOF, this method should read as many bytes of data as
         * possible, and return a success result.  If no more data remains
         * to be read, it should return a success result and set the
         * num_read argument to 0.
         *
         * @param buf          [out]; A pointer to a buffer in which to store
         *                            the image data
         * @param bufsize      [in];  The number of bytes to read from the stream
         * @param num_read_ptr [out]; THe number of byte that were actually read
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result read (uchar* buf, size_t bufsize, size_t* num_read_ptr) = 0;

        /**
         * @brief Returns the number of bytes read from this stream.
         *
         * @return The number of bytes read.
         */
        virtual size_t get_total_num_bytes_read () const = 0;
    }; // end class IImageInputStream

    /**
     * @interface IImageOutputStream
     *
     * @brief The IImageOutputStream class defines the Output stream interface
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE IImageOutputStream
    {
    public:

        /**
         * @brief Destructor.
         */
        virtual ~IImageOutputStream () {};

        /**
         * @brief Writes a single byte to the output stream.
         *
         * @param byte [in]; The byte to write to the output stream
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result write_byte (uchar byte) = 0;

        /**
         * @brief Writes a block of bytes to the output stream.
         *
         * @param buf     [in]; The image data buffer
         * @param bufsize [in]; The size (in bytes) of the image buffer
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        virtual Result write (const void* buf, size_t bufsize) = 0;
        
        /**
         * @brief Returns the number of bytes written to the output stream.
         *
         * @return The number of bytes written to the output stream.
         */
        virtual size_t get_total_num_bytes_written () const = 0;
    }; // end class IImageOutputStream

    /**
     * @brief The ImageMemoryInputStream class defines the Memory input stream.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE ImageMemoryInputStream : public IImageInputStream
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param data [in]; The image data buffer
         * @param size [in]; The size (in bytes) of the image data buffer
         */
        ImageMemoryInputStream (const void* data, size_t size);

        /**
         * @brief Empty destructor.
         */
        virtual ~ImageMemoryInputStream ();

        /**
         * @brief On EOF, this method should return a failure result, and
         * not set the byte.
         *
         * @param byte_ptr [out]; A pointer to a buffer in which to store
         *                        a byte of image data
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_byte (uchar* byte_ptr);

        /**
         * @brief On EOF, this method should read as many bytes of data as
         * possible, and return a success result.  If no more data remains
         * to be read, it should return a success result and set the
         * num_read argument to 0.
         *
         * @param buf          [out]; A pointer to a buffer in which to store
         *                            the image data
         * @param bufsize      [in];  The number of bytes to read from the stream
         * @param num_read_ptr [out]; THe number of byte that were actually read
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read (uchar* buf, size_t bufsize, size_t* num_read_ptr);

        /**
         * @brief Returns the number of bytes read from this stream.
         *
         * @return The number of bytes read.
         */
        virtual size_t get_total_num_bytes_read () const;

    protected:

        const uchar* _data; ///< Internal pointer to the image data buffer.
        size_t _size;       ///< Size of the image data buffer.
        size_t _offset;     ///< Current position in the image data buffer.
    }; // end class ImageMemoryInputStream

    /**
     * @brief The ImageMemoryOutputStream class defines the Memory
     * output stream.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE ImageMemoryOutputStream : public IImageOutputStream
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param data [out]; The image data buffer
         * @param size [in]; The size (in bytes) of the image data buffer
         */
        ImageMemoryOutputStream (void* data, size_t size);

        /**
         * @brief Empty destructor.
         */
        virtual ~ImageMemoryOutputStream ();

        /**
         * @brief Writes a single byte to the output stream.
         *
         * @param byte [in]; The byte to write to the output stream
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result write_byte (uchar byte);

        /**
         * @brief Writes a block of bytes to the output stream.
         *
         * @param buf     [in]; The image data buffer
         * @param bufsize [in]; The size (in bytes) of the image buffer
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result write (const void* buf, size_t bufsize);

        /**
         * @brief Returns the number of bytes written to the output stream.
         *
         * @return The number of bytes written to the output stream.
         */
        size_t get_total_num_bytes_written () const;

    protected:

        uchar* _data;   ///< Internal pointer to the image data buffer.
        size_t _size;   ///< Size of the image data buffer.
        size_t _offset; ///< Current position in the image data buffer.
    }; // end class ImageMemoryOutputStream

    /**
     * @class ImageFileInputStream 
     *
     * @brief The ImageFileInputStream class defines the File input and
     * output streams.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE ImageFileInputStream : public IImageInputStream
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param file [out]; The image file.
         */
        ImageFileInputStream(FILE* file);

        /**
         * @brief Empty destructor.
         */
        virtual ~ImageFileInputStream ();

        /**
         * @brief On EOF, this method should return a failure result, and
         * not set the byte.
         *
         * @param byte_ptr [out]; A pointer to a buffer in which to store
         *                        a byte of image data
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read_byte (uchar* byte_ptr);

        /**
         * @brief On EOF, this method should read as many bytes of data as
         * possible, and return a success result.  If no more data remains
         * to be read, it should return a success result and set the
         * num_read argument to 0.
         *
         * @param buf          [out]; A pointer to a buffer in which to store
         *                            the image data
         * @param bufsize      [in];  The number of bytes to read from the stream
         * @param num_read_ptr [out]; THe number of byte that were actually read
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result read (uchar* buf, size_t bufsize, size_t* num_read_ptr);

        /**
         * @brief Returns the number of bytes read from this stream.
         *
         * @return The number of bytes read.
         */
        size_t get_total_num_bytes_read () const;

    protected:

        FILE*  _file;           ///< Internal pointer to the file descriptor corresponding to the image file to be read.
        size_t _num_bytes_read; ///< Number of bytes read from file.
    }; // end class ImageFileInputStream

    /**
     * @brief The ImageFileOutputStream class defines the File input &
     * output streams.
     *
     * @ingroup core_vision
     */
    class EVOLUTION_EXPORT_RESOURCE ImageFileOutputStream : public IImageOutputStream
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param file [out]; The image file.
         */
        ImageFileOutputStream (FILE* file);

        /**
         * @brief Empty destructor.
         */
        virtual ~ImageFileOutputStream (); 

        /**
         * @brief Writes a single byte to the output stream.
         *
         * @param byte [out]; The byte to write to the output stream
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result write_byte (uchar byte);

        /**
         * @brief Writes a block of bytes to the output stream.
         *
         * @param buf     [in]; The image data buffer
         * @param bufsize [in]; The size (in bytes) of the image buffer
         *
         * @return RESULT_SUCCESS on success or an error result.
         */
        Result write (const void* buf, size_t bufsize);

        /**
         * @brief Returns the number of bytes written to the output stream.
         *
         * @return The number of bytes written to the output stream.
         */
        size_t get_total_num_bytes_written () const;

    protected:

        FILE*  _file;              ///< Internal pointer to the file descriptor corresponding to the image file to be written.
        size_t _num_bytes_written; ///< Number of bytes written to file.
    }; // end class ImageFileOutputStream

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IMAGEIO_HPP
