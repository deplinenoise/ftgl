#include    "FTGLExtrdFont.h"
#include    "FTExtrdGlyph.h"


FTGLExtrdFont::FTGLExtrdFont( const char* fontname)
:   FTFont( fontname),
    depth( 0.0f)
{}


FTGLExtrdFont::FTGLExtrdFont( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
:   FTFont( pBufferBytes, bufferSizeInBytes),
    depth( 0.0f)
{}


FTGLExtrdFont::~FTGLExtrdFont()
{}


FTGlyph* FTGLExtrdFont::MakeGlyph( unsigned int g)
{
    FT_Glyph* ftGlyph = face.Glyph( g, FT_LOAD_NO_HINTING);

    if( ftGlyph)
    {
        FTExtrdGlyph* tempGlyph = new FTExtrdGlyph( *ftGlyph, depth);
        return tempGlyph;
    }

    err = face.Error();
    return NULL;
}


