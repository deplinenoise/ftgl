#include	"gl.h"

#include	"FTPixmapGlyph.h"
#include	"FTGL.h"


FTPixmapGlyph::FTPixmapGlyph( FT_Glyph glyph, const int gi)
:	FTGlyph(gi),
	destWidth(0),
	destHeight(0),
	numGreys(0),
	data(0)
{
	if( !glyph->format == ft_glyph_format_bitmap)
	{ return;}
	
	// This function will always fail if the glyph's format isn't scalable????
	FT_Error err = FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1);
	if( err)
	{return;}

	FT_BitmapGlyph  bitmap = (FT_BitmapGlyph)glyph;
	FT_Bitmap*      source = &bitmap->bitmap;

	//check the pixel mode
	//ft_pixel_mode_grays
	    
	int srcWidth = source->width;
	int srcHeight = source->rows;
	int srcPitch = source->pitch;
    
	numGreys = source->num_grays;
	advance = glyph->advance.x >> 16; // this is 6 in the freetype docs!!!!!!

 	pos.x = bitmap->left;
	pos.y = srcHeight - bitmap->top;
	
   // FIXME The buffer is upside down. What about dest alignment?
    destWidth = srcWidth;
    destHeight = srcHeight;
    
    data = new unsigned char[destWidth * destHeight * 4];
    
    for(int y = 0; y < srcHeight; ++y)
    {
    	--destHeight;
    	for(int x = 0; x < srcWidth; ++x)
    	{
			*( data + ( destHeight * destWidth  + x) * 4 + 0) = 0xFF;
			*( data + ( destHeight * destWidth  + x) * 4 + 1) = 0xFF;
			*( data + ( destHeight * destWidth  + x) * 4 + 2) = 0xFF;
			*( data + ( destHeight * destWidth  + x) * 4 + 3) = *( source->buffer + ( y * srcPitch) + x);
    	}    	
    }

    destHeight = srcHeight;

	// discard glyph image (bitmap or not)
	// Is this the right place to do this?
	FT_Done_Glyph( glyph );
}


FTPixmapGlyph::~FTPixmapGlyph()
{
	delete[] data;
}


float FTPixmapGlyph::Render( const FT_Vector& pen)
{
	if( data != 0 )
	{
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);
		
		// Move the glyph origin
		glBitmap( 0, 0, 0.0, 0.0, pen.x + pos.x, pen.y - pos.y, (const GLubyte *)0 );

		glPixelStorei( GL_UNPACK_ROW_LENGTH, destWidth);

		glDrawPixels( destWidth, destHeight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)data );

		// Restore the glyph origin
		glBitmap( 0, 0, 0.0, 0.0, -pen.x - pos.x, -pen.y + pos.y, (const GLubyte *)0 );

		glPopClientAttrib();
	}

	return advance;
}
