#ifndef __TK_STREAM_H__
#define __TK_STREAM_H__
#include <TK/TK_stdint.h>

typedef struct TK_IStream  TK_IStream;
typedef struct TK_OStream  TK_OStream;
typedef union  TK_IOStream TK_IOStream;
typedef TK_usize TK_IStreamReadFunc   (TK_IStream  *stream, void *buf, TK_usize len);
typedef TK_usize TK_OStreamWriteFunc  (TK_OStream  *stream, void *buf, TK_usize len);
typedef TK_usize TK_IOStreamReadFunc  (TK_IOStream *stream, void *buf, TK_usize len);
typedef TK_usize TK_IOStreamWriteFunc (TK_IOStream *stream, void *buf, TK_usize len);

struct TK_IStream
{
    TK_IOStreamReadFunc *read;
    TK_IOStreamWriteFunc *_;
};

struct TK_OStream
{
    TK_IOStreamReadFunc *_;
    TK_IOStreamWriteFunc *write;
};

union TK_IOStream
{
    struct
    {
        TK_IOStreamReadFunc *read;
        TK_IOStreamWriteFunc *write;
    };
    
    TK_IStream istream;
    TK_OStream ostream;
};

#endif