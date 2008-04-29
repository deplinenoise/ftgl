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
 */

#ifndef __FTExtrdGlyph__
#define __FTExtrdGlyph__

#include <ftgl.h>

#ifdef __cplusplus


/**
 * FTExtrdGlyph is a specialisation of FTGlyph for creating tessellated
 * extruded polygon glyphs.
 *
 * @see FTGlyphContainer
 * @see FTVectoriser
 *
 */
class FTGL_EXPORT FTExtrdGlyph : public FTGlyph
{
    public:
        /**
         * Constructor. Sets the Error to Invalid_Outline if the glyph isn't
         * an outline.
         *
         * @param glyph The Freetype glyph to be processed
         * @param depth The distance along the z axis to extrude the glyph
         * @param frontOutset outset contour size
         * @param backOutset outset contour size
         * @param useDisplayList Enable or disable the use of Display Lists
         *                       for this glyph
         *                       <code>true</code> turns ON display lists.
         *                       <code>false</code> turns OFF display lists.
         */
        FTExtrdGlyph(FT_GlyphSlot glyph, float depth, float frontOutset,
                     float backOutset, bool useDisplayList);

        /**
         * Destructor
         */
        virtual ~FTExtrdGlyph();
};

#endif //__cplusplus

#endif  //  __FTExtrdGlyph__

