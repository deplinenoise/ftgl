#include    "FTVectoriser.h"
#include    "FTGL.h"


#ifndef CALLBACK
#define CALLBACK
#endif

#ifdef __APPLE_CC__    
    typedef GLvoid (*GLUTesselatorFunction)(...);
#endif

#if defined( __mips ) || defined( __linux__ )
    typedef GLvoid (*GLUTesselatorFunction)();
#endif
    

void CALLBACK ftglError( GLenum errCode, FTMesh* mesh)
{
    mesh->Error( errCode);
}

void CALLBACK ftglVertex( void* data, FTMesh* mesh)
{
    FTGL_DOUBLE* vertex = (FTGL_DOUBLE*)data;
    mesh->AddPoint( vertex[0], vertex[1], vertex[2]);
}


void CALLBACK ftglBegin( GLenum type, FTMesh* mesh)
{
    mesh->Begin( type);
}


void CALLBACK ftglEnd( FTMesh* mesh)
{
    mesh->End();
}


void CALLBACK ftglCombine( FTGL_DOUBLE coords[3], void* vertex_data[4], GLfloat weight[4], void** outData, FTMesh* mesh)
{
    FTGL_DOUBLE* vertex = (FTGL_DOUBLE*)coords;
    mesh->tempPointList.push_back( FTPoint( vertex[0], vertex[1], vertex[2]));
    
    *outData = &mesh->tempPointList[ mesh->tempPointList.size() - 1].x;
}
        

FTMesh::FTMesh()
:   err(0)
{
    tesselationList.reserve( 16);
    tempPointList.reserve( 128);
}


FTMesh::~FTMesh()
{
    for( size_t t = 0; t < tesselationList.size(); ++t)
    {
        delete tesselationList[t];
    }
    tesselationList.clear();

    tempPointList.clear();
}


void FTMesh::AddPoint( const FTGL_DOUBLE x, const FTGL_DOUBLE y, const FTGL_DOUBLE z)
{
    currentTesselation->AddPoint( x, y, z);
}

void FTMesh::Begin( GLenum m)
{
    currentTesselation = new FTTesselation;
    currentTesselation->meshType = m;
}


void FTMesh::End()
{
    tesselationList.push_back( currentTesselation);
}


int FTMesh::size() const
{
    int s = 0;
    for( size_t t = 0; t < tesselationList.size(); ++t)
    {
        s += tesselationList[t]->size();
// FIXME What the hell is this for? Data in FTPolyglyph
        ++s;
    }
    return s;
}


//=============================================================================


FTVectoriser::FTVectoriser( const FT_Glyph glyph)
:   contour(0),
    mesh(0),
    contourFlag(0),
    kBSTEPSIZE( 0.2f)
{
    if( glyph)
    {
        FT_OutlineGlyph outline = (FT_OutlineGlyph)glyph;
        ftOutline = outline->outline;
    
        contourList.reserve( ftOutline.n_contours);
    }
}


FTVectoriser::~FTVectoriser()
{
    for( size_t c = 0; c < contours(); ++c)
    {
        delete contourList[c];
    }

    contourList.clear();
    
    if( mesh)
        delete mesh;
}


int FTVectoriser::points()
{
    int s = 0;
    for( size_t c = 0; c < contours(); ++c)
    {
        s += contourList[c]->size();
    }
    
    return s;
}


void FTVectoriser::Process()
{
    short first = 0;
    short last;
    const short contourCount = ftOutline.n_contours;
    
    for( short c = 0; c < contourCount; ++c)
    {
        contour = new FTContour;
        contourFlag = ftOutline.flags;
        last = ftOutline.contours[c];

        for( int p = first; p <= last; ++p)
        {
            switch( ftOutline.tags[p])
            {
                case FT_Curve_Tag_Conic:
                    p += Conic( p, first, last);
                    break;
                case FT_Curve_Tag_Cubic:
                    p += Cubic( p, first, last);
                    break;
                case FT_Curve_Tag_On:
                default:
                    contour->AddPoint( ftOutline.points[p].x, ftOutline.points[p].y);
            }
        }
        
        contourList.push_back( contour);
        first = last + 1;
    }
}


int FTVectoriser::Conic( const int index, const int first, const int last)
{
    int next = index + 1;
    int prev = index - 1;
    
    if( index == last)
    {
        next = first; 
    }
    
    if( index == first)
    {
        prev = last; 
    }
    
    if( ftOutline.tags[next] != FT_Curve_Tag_Conic)
    {
        ctrlPtArray[0][0] = ftOutline.points[prev].x;   ctrlPtArray[0][1] = ftOutline.points[prev].y;
        ctrlPtArray[1][0] = ftOutline.points[index].x;  ctrlPtArray[1][1] = ftOutline.points[index].y;
        ctrlPtArray[2][0] = ftOutline.points[next].x;   ctrlPtArray[2][1] = ftOutline.points[next].y;
        
        evaluateCurve( 2);
        return 1;
    }
    else
    {
        int next2 = next + 1;
        if( next == last)
            next2 = first;
        
        // create a phantom point
        float x = ( ftOutline.points[index].x + ftOutline.points[next].x) * 0.5f;
        float y = ( ftOutline.points[index].y + ftOutline.points[next].y) * 0.5f;
        
        // process first curve
        ctrlPtArray[0][0] = ftOutline.points[prev].x;   ctrlPtArray[0][1] = ftOutline.points[prev].y;
        ctrlPtArray[1][0] = ftOutline.points[index].x;  ctrlPtArray[1][1] = ftOutline.points[index].y;
        ctrlPtArray[2][0] = x;                          ctrlPtArray[2][1] = y;
        
        evaluateCurve( 2);
        
        // process second curve
        ctrlPtArray[0][0] = x;                          ctrlPtArray[0][1] = y;
        ctrlPtArray[1][0] = ftOutline.points[next].x;   ctrlPtArray[1][1] = ftOutline.points[next].y;
        ctrlPtArray[2][0] = ftOutline.points[next2].x;  ctrlPtArray[2][1] = ftOutline.points[next2].y;
        evaluateCurve( 2);
        
        return 2;
    }
}


int FTVectoriser::Cubic( const int index, const int first, const int last)
{
    int next = index + 1;
    int prev = index - 1;
    
    if( index == last)
    {
        next = first; 
    }
    
    int next2 = next + 1;
    
    if( next == last)
    {
        next2 = first;
    }
    
    if( index == first)
    {
        prev = last; 
    }
    
    ctrlPtArray[0][0] = ftOutline.points[prev].x;       ctrlPtArray[0][1] = ftOutline.points[prev].y;
    ctrlPtArray[1][0] = ftOutline.points[index].x;      ctrlPtArray[1][1] = ftOutline.points[index].y;
    ctrlPtArray[2][0] = ftOutline.points[next].x;       ctrlPtArray[2][1] = ftOutline.points[next].y;
    ctrlPtArray[3][0] = ftOutline.points[next2].x;      ctrlPtArray[3][1] = ftOutline.points[next2].y;

    evaluateCurve( 3);
    return 2;
}


// De Casteljau algorithm contributed by Jed Soane
void FTVectoriser::deCasteljau( const float t, const int n)
{
    //Calculating successive b(i)'s using de Casteljau algorithm.
    for( int i = 1; i <= n; i++)
    {
        for( int k = 0; k <= (n - i); k++)
        {
            bValues[i][k][0] = (1 - t) * bValues[i - 1][k][0] + t * bValues[i - 1][k + 1][0];
            bValues[i][k][1] = (1 - t) * bValues[i - 1][k][1] + t * bValues[i - 1][k + 1][1];
        }
    }
       
    //Specify next vertex to be included on curve
    contour->AddPoint( bValues[n][0][0], bValues[n][0][1]);
}


// De Casteljau algorithm contributed by Jed Soane
void FTVectoriser::evaluateCurve( const int n)
{
    // setting the b(0) equal to the control points
    for( int i = 0; i <= n; i++)
    {
        bValues[0][i][0] = ctrlPtArray[i][0];
        bValues[0][i][1] = ctrlPtArray[i][1];
    }

    float t; //parameter for curve point calc. [0.0, 1.0]

    for( int m = 0; m <= ( 1 / kBSTEPSIZE); m++)
    {
        t = m * kBSTEPSIZE;
        deCasteljau( t, n);  //calls to evaluate point on curve at t.
    }
}


void FTVectoriser::GetOutline( FTGL_DOUBLE* data)
{
    int i = 0;
    
    for( size_t c= 0; c < contours(); ++c)
    {
        const FTContour* contour = contourList[c];
        
        for( size_t p = 0; p < contour->size(); ++p)
        {
            data[i] = static_cast<FTGL_DOUBLE>(contour->pointList[p].x / 64.0f); // is 64 correct?
            data[i + 1] = static_cast<FTGL_DOUBLE>(contour->pointList[p].y / 64.0f);
            data[i + 2] = 0.0f; // static_cast<FTGL_DOUBLE>(contour->pointList[p].z / 64.0f);
            i += 3;
        }
    }
}


void FTVectoriser::MakeMesh( FTGL_DOUBLE zNormal)
{
    if( mesh)
    {
        delete mesh;
    }
        
    mesh = new FTMesh;
    
    GLUtesselator* tobj = gluNewTess();

    gluTessCallback( tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction)ftglBegin);
    gluTessCallback( tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction)ftglVertex);
    gluTessCallback( tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction)ftglCombine);
    gluTessCallback( tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction)ftglEnd);
    gluTessCallback( tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction)ftglError);
    
    if( contourFlag & ft_outline_even_odd_fill) // ft_outline_reverse_fill
    {
        gluTessProperty( tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    }
    else
    {
        gluTessProperty( tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    }
    
    
    gluTessProperty( tobj, GLU_TESS_TOLERANCE, 0);
    gluTessNormal( tobj, 0.0f, 0.0f, zNormal);
    gluTessBeginPolygon( tobj, mesh);
    
        for( size_t c = 0; c < contours(); ++c)
        {
            const FTContour* contour = contourList[c];

            gluTessBeginContour( tobj);
            
                for( size_t p = 0; p < contour->size(); ++p)
                {
                    FTGL_DOUBLE* d = const_cast<FTGL_DOUBLE*>(&contour->pointList[p].x);
                    gluTessVertex( tobj, d, d);
                }

            gluTessEndContour( tobj);
        }
        
    gluTessEndPolygon( tobj);

    gluDeleteTess( tobj);
}


void FTVectoriser::GetMesh( FTGL_DOUBLE* data)
{
    int i = 0;
    
    // fill out the header
    size_t msize = mesh->tesselationList.size();
    data[0] = msize;
    
    for( int p = 0; p < data[0]; ++p)
    {
        FTTesselation* tesselation = mesh->tesselationList[p];
        size_t tesselationSize =  tesselation->pointList.size();
        int tesselationType =  tesselation->meshType;
        
        data[i+1] = tesselationType;
        data[i+2] = tesselationSize;
        i += 3;
        for( size_t q = 0; q < ( tesselation->pointList.size()); ++q)
        {
            data[i] = tesselation->pointList[q].x / 64.0f; // is 64 correct?
            data[i + 1] = tesselation->pointList[q].y / 64.0f;
            data[i + 2] = 0.0f; // static_cast<FTGL_DOUBLE>(mesh->pointList[p].z / 64.0f);
            i += 3;
        }
    }
}
