#ifndef     __FTBBox__
#define     __FTBBox__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTGL.h"
#include "FTPoint.h"


/**
 * FTBBox is a convenience class for handling bounding boxes.
 */
class FTGL_EXPORT FTBBox
{
    public:
        /**
         * Default constructor. Bounding box is set to zero.
         */
        FTBBox()
        :   lowerX(0),
            lowerY(0),
            lowerZ(0),
            upperX(0),
            upperY(0),
            upperZ(0)
        {}
        
        /**
         * Constructor. Extracts a bounding box from a freetype glyph. Uses
         * the control box for the glyph. <code>FT_Glyph_Get_CBox()</code>
         *
         * @param glyph A freetype glyph
         */
        FTBBox( FT_Glyph glyph)
        {
            FT_BBox bbox;
            FT_Glyph_Get_CBox( glyph, ft_glyph_bbox_subpixels, &bbox );
            
            lowerX = bbox.xMin >> 6;
            lowerY = bbox.yMin >> 6;
            lowerZ = 0;
            upperX = bbox.xMax >> 6;
            upperY = bbox.yMax >> 6;
            upperZ = 0; 
        }       

        /**
         * Destructor
         */
        ~FTBBox()
        {}
        

        /**
         * Move the Bounding Box by a vector.
         *
         * @param distance The distance to move the bbox in 3D space.
         */
        FTBBox Move( FTPoint distance)
        {
            lowerX += distance.x;
            lowerY += distance.y;
            lowerZ += distance.z;
            upperX += distance.x;
            upperY += distance.y;
            upperZ += distance.z;
            return *this;
        }

        /**
         * Operator + Adds two FTBBox's together
         *
         * @param a
         * @param b
         * @return
         */
        friend FTBBox operator + ( const FTBBox &a, const FTBBox &b);

        /**
         * The bounds of the box
         */
        // Make these ftPoints & private
        float lowerX, lowerY, lowerZ, upperX, upperY, upperZ;
    protected:
    
    
    private:

};


#endif  //  __FTBBox__

