#include    "FTFace.h"
#include    "FTFont.h"
#include    "FTGlyphContainer.h"
#include    "FTBBox.h"


FTFont::FTFont()
:   numFaces(0),
    glyphList(0),
    numGlyphs(0),
    preCache(true),
    err(0)
{
    pen.x = 0;
    pen.y = 0;
}


FTFont::~FTFont()
{
    Close();
}


bool FTFont::Open( const char* fontname, bool p)
{
    preCache = p;
    
    if( face.Open( fontname))
    {
        FT_Face* ftFace = face.Face();      
        numGlyphs = (*ftFace)->num_glyphs;
        
        return true;
    }
    else
    {
        err = face.Error();
        return false;
    }
}


bool FTFont::Open( const unsigned char *pBufferBytes, size_t bufferSizeInBytes, bool p )
{
    preCache = p;
    
    if( face.Open( pBufferBytes, bufferSizeInBytes ))
    {
        FT_Face* ftFace = face.Face();      
        numGlyphs = (*ftFace)->num_glyphs;
        
        return true;
    }
    else
    {
        err = face.Error();
        return false;
    }
}


bool FTFont::Attach( const char* filename)
{
    return face.Attach( filename);
}


void FTFont::Close()
{
    delete glyphList;
}


bool FTFont::FaceSize( const unsigned int size, const unsigned int res )
{
    charSize = face.Size( size, res);

    if( glyphList)
        delete glyphList;
    
    glyphList = new FTGlyphContainer( &face, numGlyphs, preCache);
    
    if( MakeGlyphList())
    {
        return true;
    }
    else
    {
        return false;
    }
}


unsigned int FTFont::FaceSize() const
{
    return charSize.CharSize();
}


bool FTFont::MakeGlyphList()
{
    for( unsigned int c = 0; c < numGlyphs; ++c)
    {
        if( preCache)
        {
            glyphList->Add( MakeGlyph( c), c);
        }
        else
        {
            glyphList->Add( NULL, c);
        }
    }
    
    return !err; // FIXME what err?
}


bool FTFont::CharMap( FT_Encoding encoding)
{
    err = face.CharMap( encoding);
    return !err;
}


int FTFont::Ascender() const
{
    return charSize.Ascender();
}


int FTFont::Descender() const
{
    return charSize.Descender();
}


void FTFont::BBox( const char* string,
                   float& llx, float& lly, float& llz, float& urx, float& ury, float& urz)
{
    const unsigned char* c = (unsigned char*)string;
    llx = lly = llz = urx = ury = urz = 0.0f;
    FTBBox bbox;
 
    while( *c)
    {
        if( !glyphList->Glyph( static_cast<unsigned int>(*c)))
        {
            unsigned int g = face.CharIndex( static_cast<unsigned int>(*c));
            glyphList->Add( MakeGlyph( g), g);
        }
        
        bbox = glyphList->BBox( *c);
        
        // Lower extent
        lly = lly < bbox.lowerY ? lly: bbox.lowerY;
        // Upper extent
        ury = ury > bbox.upperY ? ury: bbox.upperY;
        // Depth
        urz = urz < bbox.upperZ ? urz: bbox.upperZ;

        // Width
        urx += glyphList->Advance( *c, *(c + 1));
        ++c;
    }
    
    //Final adjustments
    llx = glyphList->BBox( *string).lowerX;
    urx -= glyphList->Advance( *(c - 1), 0);
    urx += bbox.upperX;

}

void FTFont::BBox( const wchar_t* string,
                   float& llx, float& lly, float& llz, float& urx, float& ury, float& urz)
{
    const wchar_t* c = string;
    llx = lly = llz = urx = ury = urz = 0.0f;
    FTBBox bbox;
 
    while( *c)
    {
        if( !glyphList->Glyph( static_cast<unsigned int>(*c)))
        {
            unsigned int g = face.CharIndex( static_cast<unsigned int>(*c));
            glyphList->Add( MakeGlyph( g), g);
        }
        
        bbox = glyphList->BBox( *c);
        
        // Lower extent
        lly = lly < bbox.lowerY ? lly: bbox.lowerY;
        // Upper extent
        ury = ury > bbox.upperY ? ury: bbox.upperY;
        // Depth
        urz = urz < bbox.upperZ ? urz: bbox.upperZ;

        // Width
        urx += glyphList->Advance( *c, *(c + 1));
        ++c;
    }
    
    //Final adjustments
    llx = glyphList->BBox( *string).lowerX;
    urx -= glyphList->Advance( *(c - 1), 0);
    urx += bbox.upperX;

}


float FTFont::Advance( const wchar_t* string)
{
    const wchar_t* c = string;
    float width = 0.0f;

    while( *c)
    {
        width += doAdvance( *c, *(c + 1));
        ++c;
    }

    return width;
}


float FTFont::Advance( const char* string)
{
    const unsigned char* c = (unsigned char*)string;
    float width = 0.0f;

    while( *c)
    {
        width += doAdvance( *c, *(c + 1));
        ++c;
    }

    return width;
}


float FTFont::doAdvance( const unsigned int chr, const unsigned int nextChr)
{
    if( !glyphList->Glyph( chr))
    {
        unsigned int g = face.CharIndex( chr);
        glyphList->Add( MakeGlyph( g), g);
    }

    return glyphList->Advance( chr, nextChr);
}


void FTFont::render( const char* string )
{
    const unsigned char* c = (unsigned char*)string;
    pen.x = 0; pen.y = 0;

    while( *c)
    {
        doRender( *c, *(c + 1));
        ++c;
    }
}


void FTFont::render( const wchar_t* string )
{
    const wchar_t* c = string;
    pen.x = 0; pen.y = 0;

    while( *c)
    {
        doRender( *c, *(c + 1));
        ++c;
    }
}


void FTFont::doRender( const unsigned int chr, const unsigned int nextChr)
{
    if( !glyphList->Glyph( chr))
    {
        unsigned int g = face.CharIndex( chr);
        glyphList->Add( MakeGlyph( g), g);
    }

    FTPoint kernAdvance = glyphList->render( chr, nextChr, pen);
    
    pen.x += kernAdvance.x;
    pen.y += kernAdvance.y;
}

