/* math.c
 *
 * Emraef @ GBmud
 * 08-APR-05
 * Simul-efuns
 *
 */

int i_min( int x, int y );
float f_min( float x, float y );

mixed min( mixed x, mixed y )
{
    if( floatp( x ) || floatp( y ) )
    {
       return f_min( x, y );
    }
    
    if( intp( x ) || intp( y ) )
    {
       return i_min( x, y );
    }

    return x;
}

int i_min( int x, int y )
{
    return (x > y) ? y : x;
}

int i_max( int x, int y )
{
    return (x < y) ? y : x;
}

float f_min( float x, float y )
{
    return (x > y) ? y : x;
}

float f_max( float x, float y )
{
    return (x < y) ? y : x;
}
