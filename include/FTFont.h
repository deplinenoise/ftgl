#ifndef    __FTFont__
#define    __FTFont__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "FTFace.h"
#include "FTGL.h"

class FTGlyphContainer;
class FTGlyph;
class FTLayout;

/**
 * FTFont is the public interface for the FTGL library.
 *
 * Specific font classes are derived from this class. It uses the helper
 * classes FTFace and FTSize to access the Freetype library. This class
 * is abstract and deriving classes must implement the protected
 * <code>MakeGlyph</code> function to create glyphs of the
 * appropriate type.
 *
 * It is good practice after using these functions to test the error
 * code returned. <code>FT_Error Error()</code>
 *
 * @see     FTFace
 * @see     FTSize
 * @see     FTGlyphContainer
 * @see     FTGlyph
 */
class FTGL_EXPORT FTFont
{
    public:
        /**
         * Open and read a font file. Sets Error flag.
         *
         * @param fontname  font file name.
         */
        FTFont( const char* fontname);
        
        /**
         * Open and read a font from a buffer in memory. Sets Error flag.
         *
         * @param pBufferBytes  the in-memory buffer
         * @param bufferSizeInBytes  the length of the buffer in bytes
         */
        FTFont( const unsigned char *pBufferBytes, size_t bufferSizeInBytes);
        
        /**
         * Destructor
         */
        virtual ~FTFont();
        
        /**
         * Attach auxilliary file to font e.g font metrics.
         *
         * Note: not all font formats implement this function.
         *
         * @param filename  auxilliary font file name.
         * @return          <code>true</code> if file has been attached
         *                  successfully.
         */
        bool Attach( const char* filename);

        /**
         * Attach auxilliary data to font e.g font metrics, from memory
         *
         * Note: not all font formats implement this function.
         *
         * @param pBufferBytes  the in-memory buffer
         * @param bufferSizeInBytes  the length of the buffer in bytes
         * @return          <code>true</code> if file has been attached
         *                  successfully.
         */
        bool Attach( const unsigned char *pBufferBytes, size_t bufferSizeInBytes);

        /**
         * Set the character map for the face.
         *
         * @param encoding      Freetype enumerate for char map code.
         * @return              <code>true</code> if charmap was valid and
         *                      set correctly
         */
        bool CharMap( FT_Encoding encoding );
        
        /**
         * Set the char size for the current face.
         *
         * @param size      the face size in points (1/72 inch)
         * @param res       the resolution of the target device.
         * @return          <code>true</code> if size was set correctly
         */
        bool FaceSize( const unsigned int size, const unsigned int res = 72);
        
        /**
         * Get the current face size in points.
         *
         * @return face size
         */
        unsigned int FaceSize() const;
        
        /**
         * Set the extrusion distance for the font. Only implemented by
         * FTGLExtrdFont
         *
         * @param d  The extrusion distance.
         */
        virtual void Depth( float d){}

        /**
         * Get the global ascender height for the face.
         *
         * @return  Ascender height
         */
        float Ascender() const;
        
        /**
         * Gets the global descender height for the face.
         *
         * @return  Descender height
         */
        float Descender() const;

        /**
         * Get the bounding box for a string.
         *
         * @param String    a char buffer
         * @param StartIdx  The index of the first character of String
         *                  to check.
         * @param EndIdx    The index of the last character of String to
         *                  check.  If < 0 then characters will be parsed
         *                  until a '\0' is encountered.
         * @param llx       lower left near x coord
         * @param lly       lower left near y coord
         * @param llz       lower left near z coord
         * @param urx       upper right far x coord
         * @param ury       upper right far y coord
         * @param urz       upper right far z coord
         */
        void BBox(const char *String,const int StartIdx,const int EndIdx,
                  float& llx, float& lly, float& llz, float& urx, float& ury, float& urz);
                  
        /**
         * Get the bounding box for a string.
         *
         * @param String    a wchar_t buffer
         * @param StartIdx  The index of the first character of String
         *                  to check.
         * @param EndIdx    The index of the last character of String
         *                  to check.    If < 0 then characters will
         *                  be parsed until a '\0' is encountered.
         * @param llx       lower left near x coord
         * @param lly       lower left near y coord
         * @param llz       lower left near z coord
         * @param urx       upper right far x coord
         * @param ury       upper right far y coord
         * @param urz       upper right far z coord
         */
        void BBox(const wchar_t *String,const int StartIdx,const int EndIdx,
                  float& llx, float& lly, float& llz, float& urx, float& ury, float& urz);
        
        /**
         * Get the bounding box for a string.
         *
         * @param string    a char string
         * @param llx       lower left near x coord
         * @param lly       lower left near y coord
         * @param llz       lower left near z coord
         * @param urx       upper right far x coord
         * @param ury       upper right far y coord
         * @param urz       upper right far z coord
         */
        void BBox( const char* string, float& llx, float& lly, float& llz, float& urx, float& ury, float& urz)
            { BBox(string,0,-1,llx,lly,llz,urx,ury,urz); }

        /**
         * Get the bounding box for a string.
         *
         * @param string    a wchar_t string
         * @param llx       lower left near x coord
         * @param lly       lower left near y coord
         * @param llz       lower left near z coord
         * @param urx       upper right far x coord
         * @param ury       upper right far y coord
         * @param urz       upper right far z coord
         */
        void BBox( const wchar_t* string, float& llx, float& lly, float& llz, float& urx, float& ury, float& urz)
            { BBox(string,0,-1,llx,lly,llz,urx,ury,urz); }
            
        /**
         * Get the advance width for a string.
         *
         * @param string    a wchar_t string
         * @return      advance width
         */
        float Advance( const wchar_t* string);

        /**
         * Get the advance width for a string.
         *
         * @param string    a char string
         * @return      advance width
         */
        float Advance( const char* string);

        /**
         * Render a string of characters
         * 
         * @param string    'C' style string to be output.   
         */
        virtual void Render( const char* string );

        /**
         * Render a string of characters
         * 
         * @param string    wchar_t string to be output.     
         */
        virtual void Render( const wchar_t* string );

        /**
         * Queries the Font for errors.
         *
         * @return  The current error code.
         */
        FT_Error Error() const { return err;}
    protected:
        /**
         * Construct a glyph of the correct type.
         *
         * Clients must overide the function and return their specialised
         * FTGlyph.
         *
         * @param g The glyph index NOT the char code.
         * @return  An FT****Glyph or <code>null</code> on failure.
         */
        inline virtual FTGlyph* MakeGlyph( unsigned int g) = 0;
        
        /**
         * Current face object
         */
        FTFace face;
        
        /**
         * Current size object
         */
        FTSize charSize;

        /**
         * Current error code. Zero means no error.
         */
        FT_Error err;
        
    private:        
        /**
         * Render a character.
         * This function does an implicit conversion on it's arguments.
         * 
         * @param chr       current character
         * @param nextChr   next character
         * @param origin	 The position of the origin of the character.
         *                  After rendering the point referenced by origin 
         *                  will be incremented by the kerning advance of 
         *                  char and nextChr.
         */
        inline void DoRender( const unsigned int chr, const unsigned int nextChr, FTPoint &origin);
        
        /**
         * Check that the glyph at <code>chr</code> exist. If not load it.
         *
         * @param chr  character index
         */
        inline void CheckGlyph( const unsigned int chr);

        /**
         * An object that holds a list of glyphs
         */
        FTGlyphContainer* glyphList;
        
        /**
         * Current pen or cursor position;
         */
        FTPoint pen;
        
        /* Allow FTLayout classes to access DoRender and CheckGlyph */
        friend class FTLayout;
};


#endif  //  __FTFont__

