/*
 * GUTS - Generic Utilities 
 * Copyright (c) 2006-2010 Gino van den Bergen, DTECTA 
 * http://www.dtecta.com
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Gino van den Bergen makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef GUTS_ENDIAN_HPP
#define GUTS_ENDIAN_HPP

#include <consolid/consolid.h>

namespace guts
{
    template <size_t size>
    void swizzle(char* RESTRICT dest, const char* RESTRICT src);

    template <>
    FORCEINLINE 
    void swizzle<1>(char* RESTRICT dest, const char* RESTRICT src)
    {
        dest[0] = src[0];
    } 
    
    template <>
    FORCEINLINE 
    void swizzle<2>(char* RESTRICT dest, const char* RESTRICT src)
    {
        dest[0] = src[1];
        dest[1] = src[0];
    } 
    
    template <>
    FORCEINLINE 
    void swizzle<4>(char* RESTRICT dest, const char* RESTRICT src)
    {
        dest[0] = src[3];
        dest[1] = src[2];
        dest[2] = src[1];
        dest[3] = src[0];
    } 
    
    template <>
    FORCEINLINE 
    void swizzle<8>(char* RESTRICT dest, const char* RESTRICT src)
    {
        dest[0] = src[7];
        dest[1] = src[6];
        dest[2] = src[5];
        dest[3] = src[4]; 
        dest[4] = src[3];
        dest[5] = src[2];
        dest[6] = src[1];
        dest[7] = src[0];
    }   
    

#ifdef BIG_ENDIAN

    template <typename Stream, typename T>
    void readLittleEndian(Stream& is, T& value)
    {
        char src[sizeof(T)];
        is.read(src, sizeof(T));
        swizzle<sizeof(T)>(reinterpret_cast<char*>(&value), src);
    }

    template <typename Stream, typename T>
    void writeLittleEndian(Stream& os, const T& value)
    {
        char dest[sizeof(T)]; 
        swizzle<sizeof(T)>(dest, reinterpret_cast<const char*>(&value));
        os.write(dest, sizeof(T));
    }

    template <typename Stream, typename T>
    void readBigEndian(Stream& is, T& value)
    {
        is.read(reinterpret_cast<char*>(&value), sizeof(T));  
    }  
    
    template <typename Stream, typename T>
    void writeBigEndian(Stream& os, const T& value)
    {
        os.write(reinterpret_cast<const char*>(&value), sizeof(T));  
    }

#else

    template <typename Stream, typename T>
    void readLittleEndian(Stream& is, T& value)
    {
        is.read(reinterpret_cast<char*>(&value), sizeof(T));  
    }  
    
    template <typename Stream, typename T>
    void writeLittleEndian(Stream& os, const T& value)
    {
        os.write(reinterpret_cast<const char*>(&value), sizeof(T));  
    }

    template <typename Stream, typename T>
    void readBigEndian(Stream& is, T& value)
    {
        char src[sizeof(T)];
        is.read(src, sizeof(T));
        swizzle<sizeof(T)>(reinterpret_cast<char*>(&value), src);
    }

    template <typename Stream, typename T>
    void writeBigEndian(Stream& os, const T& value)
    {
        char dest[sizeof(T)]; 
        swizzle<sizeof(T)>(dest, reinterpret_cast<const char*>(&value));
        os.write(dest, sizeof(T));
    }

#endif

}

#endif

