#include	"FTTextureGlyph.h"
#ifdef FTGL_DEBUG
	#include "mmgr.h"
#endif


FTTextureGlyph::FTTextureGlyph( FT_Glyph glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height)
:	FTGlyph(),
	destWidth(0),
	destHeight(0),
	glTextureID(id),
	activeTextureID(0)
{
	// FIXME This function will always fail if the glyph's format isn't scalable????
	err = FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1);
	if( err || glyph->format != ft_glyph_format_bitmap)
	{
		return;
	}

	FT_BitmapGlyph  bitmap = ( FT_BitmapGlyph)glyph;
	FT_Bitmap*      source = &bitmap->bitmap;

	// FIXME check the pixel mode
	//ft_pixel_mode_grays
	    
	int srcPitch = source->pitch;   
    destWidth = source->width;
    destHeight = source->rows;
    
    // Not sure what the standard behavior should be here?
    if( destWidth && destHeight)
    {
	    unsigned char* data = new unsigned char[destWidth * destHeight];

	    for(int y = 0; y < destHeight; ++y)
	    {
	    	for(int x = 0; x < destWidth; ++x)
	    	{
				*( data + ( y * destWidth  + x)) = *( source->buffer + ( y * srcPitch) + x);
	    	}    	
	    }

		glBindTexture( GL_TEXTURE_2D, glTextureID);
		glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, destWidth, destHeight, GL_ALPHA, GL_UNSIGNED_BYTE, data);
		delete [] data;
	}


//		0    
//		+----+
//		|    |
//		|    |
//		|    |
//		+----+
//		     1
	
	// Texture co-ords
	uv[0].x = static_cast<float>(xOffset) / static_cast<float>(width);
	uv[0].y = static_cast<float>(yOffset) / static_cast<float>(height);
	uv[1].x = static_cast<float>( xOffset + destWidth) / static_cast<float>(width);
	uv[1].y = static_cast<float>( yOffset + destHeight) / static_cast<float>(height);
	
	advance = glyph->advance.x >> 16;
	bBox = FTBBox( glyph);

 	pos.x = bitmap->left;
	pos.y = bitmap->top;
}


FTTextureGlyph::~FTTextureGlyph()
{}


float FTTextureGlyph::Render( const FT_Vector& pen)
{
	glGetIntegerv( GL_TEXTURE_2D_BINDING_EXT, &activeTextureID);
	if( activeTextureID != glTextureID)
	{
		glBindTexture( GL_TEXTURE_2D, (GLuint)glTextureID);
	}
	
	float x = pen.x + pos.x;
	float y = pen.y + pos.y;
	 
	glBegin( GL_QUADS);
		glTexCoord2f( uv[0].x, uv[0].y);    glVertex2f( x, y);
		glTexCoord2f( uv[0].x, uv[1].y);    glVertex2f( x, y - destHeight);
		glTexCoord2f( uv[1].x, uv[1].y);    glVertex2f( x + destWidth,	y - destHeight);
		glTexCoord2f( uv[1].x, uv[0].y);    glVertex2f( x + destWidth,	y);
	glEnd();

	return advance;

}

