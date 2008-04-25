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

#include <iostream>

#include "FTExtrdGlyph.h"
#include "FTVectoriser.h"


FTExtrdGlyph::FTExtrdGlyph(FT_GlyphSlot glyph, float _depth,
                           float _frontOutset, float _backOutset,
                           bool useDisplayList)
:   FTGlyph(glyph),
    glList(0)
{
    bBox.SetDepth(-_depth);

    if(ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }

    vectoriser = new FTVectoriser(glyph);

    if((vectoriser->ContourCount() < 1) || (vectoriser->PointCount() < 3))
    {
        delete vectoriser;
        vectoriser = NULL;
        return;
    }

    hscale = glyph->face->size->metrics.x_ppem * 64;
    vscale = glyph->face->size->metrics.y_ppem * 64;
    depth = _depth;
    frontOutset = _frontOutset;
    backOutset = _backOutset;

    if(useDisplayList)
    {
        glList = glGenLists(3);

        /* Front face */
        glNewList(glList + 0, GL_COMPILE);
        RenderFront();
        glEndList();

        /* Back face */
        glNewList(glList + 1, GL_COMPILE);
        RenderBack();
        glEndList();

        /* Side face */
        glNewList(glList + 2, GL_COMPILE);
        RenderSide();
        glEndList();

        delete vectoriser;
        vectoriser = NULL;
    }
}


FTExtrdGlyph::~FTExtrdGlyph()
{
    if(glList)
    {
        glDeleteLists(glList, 1);
    }
    else if(vectoriser)
    {
        delete vectoriser;
    }
}


const FTPoint& FTExtrdGlyph::Render(const FTPoint& pen, int renderMode)
{
    glTranslatef(pen.X(), pen.Y(), 0);
    if(glList)
    {
        if(renderMode & FTGL::RENDER_FRONT)
            glCallList(glList + 0);
        if(renderMode & FTGL::RENDER_BACK)
            glCallList(glList + 1);
        if(renderMode & FTGL::RENDER_SIDE)
            glCallList(glList + 2);
    }
    else if(vectoriser)
    {
        if(renderMode & FTGL::RENDER_FRONT)
            RenderFront();
        if(renderMode & FTGL::RENDER_BACK)
            RenderBack();
        if(renderMode & FTGL::RENDER_SIDE)
            RenderSide();
    }
    glTranslatef(-pen.X(), -pen.Y(), 0);

    return advance;
}


void FTExtrdGlyph::RenderFront()
{
    vectoriser->MakeMesh(1.0, 1, frontOutset);
    glNormal3d(0.0, 0.0, 1.0);

    const FTMesh *mesh = vectoriser->GetMesh();
    for(unsigned int j = 0; j < mesh->TesselationCount(); ++j)
    {
        const FTTesselation* subMesh = mesh->Tesselation(j);
        unsigned int polygonType = subMesh->PolygonType();

        glBegin(polygonType);
            for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
            {
                FTPoint pt = subMesh->Point(i);

                glTexCoord2f(pt.X() / hscale,
                             pt.Y() / vscale);

                glVertex3f(pt.X() / 64.0f,
                           pt.Y() / 64.0f,
                           0.0f);
            }
        glEnd();
    }
}


void FTExtrdGlyph::RenderBack()
{
    vectoriser->MakeMesh(-1.0, 2, backOutset);
    glNormal3d(0.0, 0.0, -1.0);

    const FTMesh *mesh = vectoriser->GetMesh();
    for(unsigned int j = 0; j < mesh->TesselationCount(); ++j)
    {
        const FTTesselation* subMesh = mesh->Tesselation(j);
        unsigned int polygonType = subMesh->PolygonType();

        glBegin(polygonType);
            for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
            {
                FTPoint pt = subMesh->Point(i);

                glTexCoord2f(subMesh->Point(i).X() / hscale,
                             subMesh->Point(i).Y() / vscale);

                glVertex3f(subMesh->Point(i).X() / 64.0f,
                           subMesh->Point(i).Y() / 64.0f,
                           -depth);
            }
        glEnd();
    }
}


void FTExtrdGlyph::RenderSide()
{
    int contourFlag = vectoriser->ContourFlag();

    for(size_t c = 0; c < vectoriser->ContourCount(); ++c)
    {
        const FTContour* contour = vectoriser->Contour(c);
        unsigned int n = contour->PointCount();

        if(n < 2)
        {
            continue;
        }

        glBegin(GL_QUAD_STRIP);
            for(unsigned int j = 0; j <= n; ++j)
            {
                unsigned int cur = (j == n) ? 0 : j;
                unsigned int next = (cur == n - 1) ? 0 : cur + 1;

                FTPoint frontPt = contour->FrontPoint(cur);
                FTPoint backPt = contour->BackPoint(cur);

                FTPoint normal = FTPoint::GetNormal(frontPt, contour->FrontPoint(next));
                if(normal != FTPoint(0.0f, 0.0f, 0.0f))
                {
                    glNormal3dv(static_cast<const FTGL_DOUBLE*>(normal));
                }

                glTexCoord2f(frontPt.X() / hscale,
                             frontPt.Y() / vscale);

                if(contourFlag & ft_outline_reverse_fill)
                {
                    glVertex3f(backPt.X() / 64.0f, backPt.Y() / 64.0f, 0.0f);
                    glVertex3f(frontPt.X() / 64.0f, frontPt.Y() / 64.0f, -depth);
                }
                else
                {
                    glVertex3f(backPt.X() / 64.0f, backPt.Y() / 64.0f, -depth);
                    glVertex3f(frontPt.X() / 64.0f, frontPt.Y() / 64.0f, 0.0f);
                }
            }
        glEnd();
    }
}

