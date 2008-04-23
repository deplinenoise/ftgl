/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, you can redistribute and/or modify this software under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 */

#include "config.h"

#include "FTFace.h"
#include "FTLibrary.h"

#include FT_TRUETYPE_TABLES_H

FTFace::FTFace( const char* fontFilePath)
:   numGlyphs(0),
    fontEncodingList(0),
    err(0)
{
    const FT_Long DEFAULT_FACE_INDEX = 0;
    ftFace = new FT_Face;

    err = FT_New_Face( *FTLibrary::Instance().GetLibrary(), fontFilePath, DEFAULT_FACE_INDEX, ftFace);

    if( err)
    {
        delete ftFace;
        ftFace = 0;
    }
    else
    {
        numGlyphs = (*ftFace)->num_glyphs;
        hasKerningTable = FT_HAS_KERNING((*ftFace));
    }
}


FTFace::FTFace( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
:   numGlyphs(0),
    err(0)
{
    const FT_Long DEFAULT_FACE_INDEX = 0;
    ftFace = new FT_Face;

    err = FT_New_Memory_Face(*FTLibrary::Instance().GetLibrary(),
                             (FT_Byte const *)pBufferBytes, bufferSizeInBytes,
                             DEFAULT_FACE_INDEX, ftFace);
    if( err)
    {
        delete ftFace;
        ftFace = 0;
    }
    else
    {
        numGlyphs = (*ftFace)->num_glyphs;
    }
}


FTFace::~FTFace()
{
    if( ftFace)
    {
        FT_Done_Face( *ftFace);
        delete ftFace;
        ftFace = 0;
    }
}


bool FTFace::Attach( const char* fontFilePath)
{
    err = FT_Attach_File( *ftFace, fontFilePath);
    return !err;
}


bool FTFace::Attach(const unsigned char *pBufferBytes,
                    size_t bufferSizeInBytes)
{
    FT_Open_Args open;

    open.flags = FT_OPEN_MEMORY;
    open.memory_base = (FT_Byte const *)pBufferBytes;
    open.memory_size = bufferSizeInBytes;

    err = FT_Attach_Stream( *ftFace, &open);
    return !err;
}


const FTSize& FTFace::Size( const unsigned int size, const unsigned int res)
{
    charSize.CharSize( ftFace, size, res, res);
    err = charSize.Error();

    return charSize;
}


unsigned int FTFace::CharMapCount()
{
    return (*ftFace)->num_charmaps;
}


FT_Encoding* FTFace::CharMapList()
{
    if( 0 == fontEncodingList)
    {
        fontEncodingList = new FT_Encoding[CharMapCount()];
        for( size_t encodingIndex = 0; encodingIndex < CharMapCount(); ++encodingIndex)
        {
            fontEncodingList[encodingIndex] = (*ftFace)->charmaps[encodingIndex]->encoding;
        }
    }
    
    return fontEncodingList;
}


FTPoint FTFace::KernAdvance( unsigned int index1, unsigned int index2)
{
    float x, y;
    x = y = 0.0f;

    if( hasKerningTable && index1 && index2)
    {
        FT_Vector kernAdvance;
        kernAdvance.x = kernAdvance.y = 0;

        err = FT_Get_Kerning( *ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
        if( !err)
        {   
            x = static_cast<float>( kernAdvance.x) / 64.0f;
            y = static_cast<float>( kernAdvance.y) / 64.0f;
        }
    }
    
    return FTPoint( x, y, 0.0);
}


FT_GlyphSlot FTFace::Glyph( unsigned int index, FT_Int load_flags)
{
    err = FT_Load_Glyph( *ftFace, index, load_flags);
    if( err)
    {
        return NULL;
    }

    return (*ftFace)->glyph;
}

