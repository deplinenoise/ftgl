#include    "FTFace.h"
#include    "FTLibrary.h"
#include    "FTCharmap.h"


#include FT_TRUETYPE_TABLES_H

FTFace::FTFace( const char* filename)
:   charMap(0),
    numGlyphs(0),
    err(0)
{
    const FT_Long DEFAULT_FACE_INDEX = 0;
    ftFace = new FT_Face;

    err = FT_New_Face( *FTLibrary::Instance().GetLibrary(), filename, DEFAULT_FACE_INDEX, ftFace);

    if( err)
    {
        delete ftFace;
        ftFace = 0;
    }
    else
    {
        charMap = new FTCharmap( *ftFace);
        numGlyphs = (*ftFace)->num_glyphs;
    }
}


FTFace::FTFace( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
:   charMap(0),
    numGlyphs(0),
    err(0)
{
    const FT_Long DEFAULT_FACE_INDEX = 0;
    ftFace = new FT_Face;

    err = FT_New_Memory_Face( *FTLibrary::Instance().GetLibrary(), (FT_Byte *)pBufferBytes, bufferSizeInBytes, DEFAULT_FACE_INDEX, ftFace);

    if( err)
    {
        delete ftFace;
        ftFace = 0;
    }
    else
    {
        charMap = new FTCharmap( *ftFace);
        numGlyphs = (*ftFace)->num_glyphs;
    }
}


FTFace::~FTFace()
{
    delete charMap;
    Close();
}


bool FTFace::Attach( const char* filename)
{
    err = FT_Attach_File( *ftFace, filename);
    return !err;
}


bool FTFace::Attach( const unsigned char *pBufferBytes, size_t bufferSizeInBytes)
{
    FT_Open_Args open;

    open.flags = (FT_Open_Flags)1; // FT_OPEN_MEMORY;
    open.memory_base = (FT_Byte *)pBufferBytes;
    open.memory_size = bufferSizeInBytes;

    err = FT_Attach_Stream( *ftFace, &open);
    return !err;
}


void FTFace::Close()
{
    if( ftFace)
    {
        FT_Done_Face( *ftFace);
        delete ftFace;
        ftFace = 0;
    }
}


void* FTFace::FontTable( unsigned int tableName) const
{
//    return FT_Get_Sfnt_Table( *ftFace, tableName);
    return 0;
}


const FTSize& FTFace::Size( const unsigned int size, const unsigned int res)
{
    charSize.CharSize( ftFace, size, res, res);
    err = charSize.Error();

    return charSize;
}


bool FTFace::CharMap( FT_Encoding encoding)
{
    bool result = charMap->CharMap( encoding);
    err = charMap->Error();
    return result;
}


unsigned int FTFace::UnitsPerEM() const
{
    return (*ftFace)->units_per_EM;
}


unsigned int FTFace::CharIndex( unsigned int index) const
{
    return charMap->CharIndex( index);
}


FTPoint FTFace::KernAdvance( unsigned int index1, unsigned int index2)
{
    float x, y;
    x = y = 0.0f;

    if( FT_HAS_KERNING((*ftFace)) && index1 && index2)
    {
        FT_Vector kernAdvance;
        kernAdvance.x = kernAdvance.y = 0;

        err = FT_Get_Kerning( *ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);
        if( !err)
        {   
            x = static_cast<float>( kernAdvance.x) / 64;
            y = static_cast<float>( kernAdvance.y) / 64;
        }
    }
    
    return FTPoint( x, y, 0.0);
}


FT_Glyph* FTFace::Glyph( unsigned int index, FT_Int load_flags)
{
    err = FT_Load_Glyph( *ftFace, index, load_flags);   
    err = FT_Get_Glyph( (*ftFace)->glyph, &ftGlyph);
        
    if( !err)
    {
        return &ftGlyph;
    }
    else
    {
        return NULL;
    }
}



