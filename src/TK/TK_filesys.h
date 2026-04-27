#ifndef __TK_FILESYS_H__
#define __TK_FILESYS_H__
#include <TK/TK_typetraits.h>
#include <TK/TK_string.h>
#include <TK/TK_stdint.h>
#include <TK/TK_stream.h>
#include <TK/TK_macro.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @file TK_filesys.h
 *  @brief File I/O definitions.
 *  
 *  @since This file is avaible since TK 1.0
 */



/** @struct TK_IFile TK_filesys.h
 *  @implements TK_IStream
 *  @brief A file that can only be read from.
 *  
 *  @since This struct is avaible since TK 1.0
 */
typedef
    struct TK_IFile TK_IFile;
    struct TK_IFile
{
    TK_IStream istream;
    void *nativeHandle;
    
    struct
    {
        TK_bool hasError;
        TK_i32 error;
    } _detail;
};



/** @struct TK_IFile TK_filesys.h
 *  @implements TK_OStream
 *  @brief A file that can only be written to.
 *  
 *  @since This struct is avaible since TK 1.0
 */
typedef
    struct TK_OFile TK_OFile;
    struct TK_OFile
{
    TK_OStream ostream;
    void *nativeHandle;
    
    struct
    {
        TK_bool hasError;
        TK_i32 error;
    } _detail;
};



/** @struct TK_OFile TK_filesys.h
 *  @implements TK_IOStream
 *  @brief A file that can be written to and read from.
 *  
 *  @since This struct is avaible since TK 1.0
 */
typedef
    struct TK_IOFile TK_IOFile;
    struct TK_IOFile
{
    TK_IOStream iostream;
    void *nativeHandle;
    
    struct
    {
        TK_bool hasError;
        TK_i32 error;
    } _detail;
};

typedef
    struct TK_OpenFileOpt TK_OpenFileOpt;
    struct TK_OpenFileOpt
{
    TK_StringSlice path;
    TK_bool append;
};

/** @brief Close an already open file of any kind. 
 *  
 *  @param[in] __VA_ARGS__ The file to close.
 *  
 *  @since This macro is avaible since TK 1.0
 */
#define TK_CloseFile(...)                               \
    ({                                                  \
        TK_Type_Decay(__VA_ARGS__) __x = (__VA_ARGS__); \
                                                        \
        _Generic(__x,                                   \
            TK_IFile *: TK_CloseIFile,                  \
            TK_OFile *: TK_CloseOFile,                  \
            TK_IOFile *: TK_CloseIOFile                 \
        )(__x);                                         \
    })



/** @brief Open a file for reading. 
 *  @attention This may fail, check for errors in `.istream.geterror`
 *  
 *  @return An opened file for reading.
 *  @param[in] __VA_ARGS__ The options on how to open the file.
 *  
 *  @since This macro is avaible since TK 1.0
 */
#define TK_OpenIFile(...) TK_OpenIFile_o((TK_OpenFileOpt){ __VA_ARGS__ })



/** @brief Open a file for writing. 
 *  @attention This may fail, check for errors in `.ostream.geterror`
 *  
 *  @return An opened file for reading.
 *  @param[in] __VA_ARGS__ The options on how to open the file.
 *  
 *  @since This macro is avaible since TK 1.0
 */
#define TK_OpenOFile(...) TK_OpenOFile_o((TK_OpenFileOpt){ __VA_ARGS__ })



/** @brief Open a file for reading. 
 *  @attention This may fail, check for errors in `.iostream.geterror`
 *  
 *  @return An opened file for reading.
 *  @param[in] __VA_ARGS__ The options on how to open the file.
 *  
 *  @since This macro is avaible since TK 1.0
 */
#define TK_OpenIOFile(...) TK_OpenIOFile_o((TK_OpenFileOpt){ __VA_ARGS__ })



/** @brief The function variant of @ref TK_OpenIFile
 *  
 *  @return An opened file for reading.
 *  @param[in] opt The options on how to open the file.
 *  
 *  @since This function is avaible since TK 1.0
 */
TK_IFile TK_OpenIFile_o( TK_OpenFileOpt opt );



/** @brief The function variant of @ref TK_OpenOFile
 *  
 *  @return An opened file for writing.
 *  @param[in] opt The options on how to open the file.
 *  
 *  @since This function is avaible since TK 1.0
 */
TK_OFile TK_OpenOFile_o( TK_OpenFileOpt opt );



/** @brief The function variant of @ref TK_OpenIOFile
 *  
 *  @return An opened file for reading nad writing.
 *  @param[in] opt The options on how to open the file.
 *  
 *  @since This function is avaible since TK 1.0
 */
TK_IOFile TK_OpenIOFile_o( TK_OpenFileOpt opt );



/** @brief The function variant of @ref TK_OpenClose when a @ref TK_IFile is passed.
 *  @attention The file will be set to an invalid state after it's closed.
 *  
 *  @return If the file closed successfully.
 *  @param[in] file The file to close.
 *  
 *  @since This function is avaible since TK 1.0
 */
TK_bool TK_CloseIFile( TK_IFile *file );



/** @brief The function variant of @ref TK_OpenClose when a @ref TK_OFile is passed.
 *  @attention The file will be set to an invalid state after it's closed.
 *  
 *  @return If the file closed successfully.
 *  @param[in] file The file to close.
 *  
 *  @since This function is avaible since TK 1.0
 */
TK_bool TK_CloseOFile( TK_OFile *file );



/** @brief The function variant of @ref TK_OpenClose when a @ref TK_IOFile is passed.
 *  @attention The file will be set to an invalid state after it's closed.
 *  
 *  @return If the file closed successfully.
 *  @param[in] file The file to close.
 *  
 *  @since This function is avaible since TK 1.0
 */
TK_bool TK_CloseIOFile( TK_IOFile *file );



#ifdef __cplusplus
}
#endif

#endif