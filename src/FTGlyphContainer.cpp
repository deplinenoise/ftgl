#include	"FTGlyphContainer.h"
#include	"FTGlyph.h"
#include	"FTFace.h"

#include "mmgr.h"

FTGlyphContainer::FTGlyphContainer( FTFace* f, unsigned int g, bool p)
:	preCache( p),
	numGlyphs( g),
	face( f),
	err( 0)
{
	// Fill the glyphlist with null glyphs
	glyphs.resize( g, NULL);
}


FTGlyphContainer::~FTGlyphContainer()
{
	GlyphVector::iterator iter;
	for( iter = glyphs.begin(); iter != glyphs.end(); ++iter)
	{
		if( *iter)
		{
			delete *iter;
		}
	}
	
	glyphs.clear();
}


bool FTGlyphContainer::Add( FTGlyph* tempGlyph, unsigned int g)
{
	glyphs[g] = tempGlyph;
	return true;
}


FTGlyph* FTGlyphContainer::Glyph( const unsigned int c) const
{
	return glyphs[face->CharIndex( c)];
}


FTBBox FTGlyphContainer::BBox( const unsigned int index) const
{
	return glyphs[face->CharIndex( index)]->BBox();
}


float FTGlyphContainer::Advance( unsigned int index, unsigned int next)
{
	unsigned int left = face->CharIndex( index);
	unsigned int right = face->CharIndex( next);
	
	float width = face->KernAdvance( left, right).x;
	width += glyphs[left]->Advance();
	
	return width;
}


FT_Vector& FTGlyphContainer::render( unsigned int index, unsigned int next, FT_Vector pen)
{
	kernAdvance.x = 0; kernAdvance.y = 0;
	
	unsigned int left = face->CharIndex( index);
	unsigned int right = face->CharIndex( next);
	
	kernAdvance = face->KernAdvance( left, right);
		
	if( !face->Error())
	{
		advance = glyphs[left]->Render( pen);
	}
	
	kernAdvance.x = advance + kernAdvance.x;
//	kernAdvance.y = advance.y + kernAdvance.y;
	return kernAdvance;
}
