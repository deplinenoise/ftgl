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

#ifndef    __FTSimpleLayout__
#define    __FTSimpleLayout__

#ifdef __cplusplus

#include "FTLayout.h"
#include "FTBBox.h"

class FTFont;

class FTGL_EXPORT FTSimpleLayout : public FTLayout
{
    public:
        /**
         * Initializes line spacing to 1.0, alignment to
         * ALIGN_LEFT and wrap to 100.0
         */
        FTSimpleLayout();

        /**
         * Destructor
         */
        ~FTSimpleLayout() {}

        /**
         * Set he font to use for rendering the text.
         *
         * @param fontInit A pointer to the new font.  The font is
         *                 referenced by this but will not be
         *                 disposed of when this is deleted.
         */
        void SetFont(FTFont *fontInit);

        /**
         * @return The current font.
         */
        FTFont *GetFont();

        /**
         * The maximum line length for formatting text.
         *
         * @param LineLength The new line length.
         */
        void SetLineLength(const float LineLength);

        /**
         * @return The current line length.
         */
        float GetLineLength() const;

        /**
         * The text alignment mode used to distribute
         * space within a line or rendered text.
         *
         * @param Alignment The new alignment mode.
         */
        void SetAlignment(const FTGL::TextAlignment Alignment);

        /**
         * @return The text alignment mode.
         */
        FTGL::TextAlignment GetAlignment() const;

        /**
         * Sets the line height.
         *
         * @param LineSpacing The height of each line of text expressed as
         *                    a percentage of the current fonts line height.
         */
        void SetLineSpacing(const float LineSpacing);

        /**
         * @return The line spacing.
         */
        float GetLineSpacing() const;

        /**
         * Render a string of characters and distribute extra space amongst
         * the whitespace regions of the string.
         *
         * @param string      'C' style string to output.
         * @param ExtraSpace  The amount of extra space to add to each run of
         *                    whitespace.
         */
        void RenderSpace(const char *string, const float ExtraSpace = 0.0);

        /**
         * Render a string of characters and distribute extra space amongst
         * the whitespace regions of the string.
         *
         * @param string      wchar_t string to output.
         * @param ExtraSpace  The amount of extra space to add to each run of
         *                    whitespace.
         */
        void RenderSpace(const wchar_t *string, const float ExtraSpace = 0.0);
};

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
namespace C {
#endif

FTGL_EXPORT void  ftglLayoutBBox           (FTGLlayout *, const char*, float []);

FTGL_EXPORT void  ftglLayoutRender         (FTGLlayout *, const char*);
FTGL_EXPORT void  ftglLayoutRenderMode     (FTGLlayout *, const char*, int);
FTGL_EXPORT void  ftglLayoutRenderSpace    (FTGLlayout *, const float);

FTGL_EXPORT void      ftglLayoutSetFont    (FTGLlayout *, FTGLfont*);
FTGL_EXPORT FTGLfont* ftglLayoutGetFont    (FTGLlayout *);

FTGL_EXPORT void  ftglLayoutSetLineLength  (FTGLlayout *, const float);
FTGL_EXPORT float ftglLayoutGetLineLength  (FTGLlayout *);

#ifdef __cplusplus
FTGL_EXPORT void                ftglLayoutSetAlignment   (FTGLlayout *, const FTGL::TextAlignment);
FTGL_EXPORT FTGL::TextAlignment ftglLayoutGetAlignement  (FTGLlayout *);
#else
FTGL_EXPORT void                ftglLayoutSetAlignment   (FTGLlayout *, const TextAlignment);
FTGL_EXPORT int                 ftglLayoutGetAlignement  (FTGLlayout *);
#endif

FTGL_EXPORT void  ftglLayoutSetLineSpacing (FTGLlayout *, const float);
FTGL_EXPORT float ftglLayoutGetLineSpacing (FTGLlayout *);

#ifdef __cplusplus
}
}
#endif

#endif  /* __FTSimpleLayout__ */

