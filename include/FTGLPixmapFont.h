#ifndef     __FTGLPixmapFont__
#define     __FTGLPixmapFont__


#include "FTGL.h"

#include "FTFont.h"

class FTGlyph;


/**
 * FTGLPixmapFont is a specialisation of the FTFont class for handling
 * Pixmap (Grey Scale) fonts
 *
 * @see     FTFont
 */
class FTGL_EXPORT FTGLPixmapFont : public FTFont
{
    public:
        /**
         * Open and read a font file. Sets Error flag.
         *
         * @param fontname  font file name.
         */
        FTGLPixmapFont( const char* fontname);
        
        /**
         * Open and read a font from a buffer in memory. Sets Error flag.
         *
         * @param pBufferBytes  the in-memory buffer
         * @param bufferSizeInBytes  the length of the buffer in bytes
         */
        FTGLPixmapFont( const unsigned char *pBufferBytes, size_t bufferSizeInBytes);
        
        /**
         * Destructor
         */
        ~FTGLPixmapFont();
        
        /**
         * Renders a string of characters
         * 
         * @param string    'C' style string to be output.   
         */
        void render( const char* string);
        
        /**
         * Renders a string of characters
         * 
         * @param string    wchar_t string to be output.     
         */
        void render( const wchar_t* string);


    private:
        /**
         * Construct a FTPixmapGlyph.
         *
         * @param g The glyph index NOT the char code.
         * @return  An FTPixmapGlyph or <code>null</code> on failure.
         */
        inline virtual FTGlyph* MakeGlyph( unsigned int g);
        
};


#endif  //  __FTGLPixmapFont__

