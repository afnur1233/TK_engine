#ifndef __TK_STREAM_H__
#define __TK_STREAM_H__
#include <TK/TK_string.h>
#include <TK/TK_stdint.h>
#include <TK/TK_assert.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @file TK_stream.h
 *  @brief Header for the stream interfaces.
 *  @attention This file only provides the interfaces.
 *  
 *  A stream is an abstraction over data input/output. Data can be streamed in, or out of a stream.
 *  
 *  @since This file is avaible since TK 1.0
 */



/** @enum TK_StreamOrigin TK_stream.h
 *  @brief Defines how and where the offset is applied.
 *  
 *  @since This enum is avaible since TK 1.0
 */
typedef
    enum TK_StreamOrigin TK_StreamOrigin;
    enum TK_StreamOrigin
{
    /** @brief Go `offset` bytes forward from the begining of the stream.
     *  
     *  @since This enum is avaible since TK 1.0
     */
    TK_STREAM_GO_FROM_BEGIN,
    
    /** @brief Go `offset` bytes back from the begining of the stream.
     *  
     *  @since This interface is avaible since TK 1.0
     */
    TK_STREAM_GO_FROM_END,
    
    /** @brief Go `offset` bytes forward from the current position of the stream.
     *  
     *  @since This enum is avaible since TK 1.0
     */
    TK_STREAM_GO_FORWARD,
    
    /** @brief Go `offset` bytes back from the current position of the stream.
     *  
     *  @since This interface is avaible since TK 1.0
     */
    TK_STREAM_GO_BACK,
};



/** @interface TK_IStream TK_stream.h
 *  @brief Interface for stream input.
 *  
 *  @since This interface is avaible since TK 1.0
 */
typedef
    struct TK_IStream TK_IStream;
    struct TK_IStream
{
    /** @brief Get if the stream is at the end.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return If the stream is at the end.
     *  @param[in] self The stream to check.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_bool (*atend)(TK_IStream *self);
    
    /** @brief Get if an error accoured and give it's description.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return If an error accoured.
     *  @param[in] self The stream to get the error of.
     *  @param[out] description The description of the error.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_bool (*geterror)(TK_IStream *self, TK_StringSlice *description);
    
    /** @brief Get the size of the data this stream is reading.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return The size of the data this stream is reading.
     *  @param[in] self The stream to get the size of.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_usize (*size)(TK_IStream *self);
    
    /** @brief Get the position of the stream.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return The streams position.
     *  @param[in] self The stream to get the position of.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_usize (*where)(TK_IStream *self);
    
    /** @brief Set the position of the stream.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return The streams new position.
     *  @param[in] self The stream to get the position of.
     *  @param[in] origin Where and how the offset is applied.
     *  @param[in] offset Where to go.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_bool (*go)(TK_IStream *self, TK_StreamOrigin origin, TK_isize offset);
    
    /** @brief Read data from the stream (blocking).
     *  
     *  @return The amount of bytes read. This value can not be bigger then the amount requested.
     *  @param[in] self The stream to read from.
     *  @param[out] data The buffer to read to.
     *  @param[in] size The amount of bytes to read.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_usize (*read)(TK_IStream *self, void *data, TK_usize size);
    
    /// @brief Padding so `sizeof(TK_IStream) == sizeof(TK_OStream)`
    void *_;
};



/** @interface TK_OStream TK_stream.h
 *  @brief Interface for stream output.
 *  
 *  @since This interface is avaible since TK 1.0
 */
typedef
    struct TK_OStream TK_OStream;
    struct TK_OStream
{
    /** @brief Get if the stream is at the end.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return If the stream is at the end.
     *  @param[in] self The stream to check.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_bool (*atend)(TK_OStream *self);
    
    /** @brief Get if an error accoured and give it's description.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return If an error accoured.
     *  @param[in] self The stream to get the error of.
     *  @param[out] description The description of the error.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_bool (*geterror)(TK_OStream *self, TK_StringSlice *description);
    
    /** @brief Get the size of the data this stream has.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return The size of the data this stream has.
     *  @param[in] self The stream to get the size of.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_usize (*size)(TK_OStream *self);
    
    /** @brief Get the position of the stream.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return The streams position.
     *  @param[in] self The stream to get the position of.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_usize (*where)(TK_OStream *self);
    
    /** @brief Set the position of the stream.
     *  @attention This function is optional, check if the stream implementation you're using has it.
     *  
     *  @return The streams new position.
     *  @param[in] self The stream to get the position of.
     *  @param[in] origin Where and how the offset is applied.
     *  @param[in] offset Where to go.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_bool (*go)(TK_OStream *self, TK_StreamOrigin origin, TK_isize offset);
    
    /// @brief Padding so `sizeof(TK_IStream) == sizeof(TK_OStream)`
    void *_;
    
    /** @brief Write data to the stream (blocking).
     *  
     *  @return The amount of bytes written. This value can not be bigger then the amount requested.
     *  @param[in] self The stream to write to.
     *  @param[in] data The buffer to write from.
     *  @param[in] size The amount of bytes to write.
     *  
     *  @since This function is avaible since TK 1.0
     */
    TK_usize (*write)(TK_OStream *self, void *data, TK_usize size);
};



/** @interface TK_IOStream TK_stream.h
 *  @brief Interface for stream input and output.
 *  
 *  @since This interface is avaible since TK 1.0
 */
typedef
    union TK_IOStream TK_IOStream;
    union TK_IOStream
{
    struct
    {
        /** @brief Get if the stream is at the end.
         *  @attention This function is optional, check if the stream implementation you're using has it.
         *  
         *  @return If the stream is at the end.
         *  @param[in] self The stream to check.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_bool (*atend)(TK_IOStream *self);
        
        /** @brief Get if an error accoured and give it's description.
         *  @attention This function is optional, check if the stream implementation you're using has it.
         *  
         *  @return If an error accoured.
         *  @param[in] self The stream to get the error of.
         *  @param[out] description The description of the error.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_bool (*geterror)(TK_IOStream *self, TK_StringSlice *description);
        
        /** @brief Get the size of the data this stream has.
         *  @attention This function is optional, check if the stream implementation you're using has it.
         *  
         *  @return The size of the data this stream has.
         *  @param[in] self The stream to get the size of.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_usize (*size)(TK_IOStream *self);
        
        /** @brief Get the position of the stream.
         *  @attention This function is optional, check if the stream implementation you're using has it.
         *  
         *  @return The streams position.
         *  @param[in] self The stream to get the position of.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_usize (*where)(TK_IOStream *self);
        
        /** @brief Set the position of the stream.
         *  @attention This function is optional, check if the stream implementation you're using has it.
         *  
         *  @return The streams new position.
         *  @param[in] self The stream to get the position of.
         *  @param[in] origin Where and how the offset is applied.
         *  @param[in] offset Where to go.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_bool (*go)(TK_IOStream *self, TK_StreamOrigin origin, TK_isize offset);
        
        /** @brief Read data from the stream (blocking).
         *  
         *  @return The amount of bytes read. This value can not be bigger then the amount requested.
         *  @param[in] self The stream to read from.
         *  @param[out] data The buffer to read to.
         *  @param[in] size The amount of bytes to read.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_usize (*read)(TK_IOStream *self, void *data, TK_usize size);
        
        /** @brief Write data to the stream (blocking).
         *  
         *  @return The amount of bytes written. This value can not be bigger then the amount requested.
         *  @param[in] self The stream to write to.
         *  @param[in] data The buffer to write from.
         *  @param[in] size The amount of bytes to write.
         *  
         *  @since This function is avaible since TK 1.0
         */
        TK_usize (*write)(TK_IOStream *self, void *data, TK_usize size);
    };
    
    TK_OStream ostream;
    TK_IStream istream;
};

TK_StaticAssert(sizeof(TK_IStream) == sizeof(TK_OStream));
TK_StaticAssert(sizeof(TK_IOStream) == sizeof(TK_OStream));



#ifdef __cplusplus
} // extern "C"
#endif



#endif // __TK_STREAM_H__