// String related efuns file
//
//

//--------------------------------------------------
string auto_wrap( string inString, int wrapLength )
{
    string *strArr;
    string *outArr;
    int outIndex = 0;

    outArr = ({ "" });

    strArr = explode( inString, " " );

    for( int i = 0; i < sizeof( strArr ); i++ )
    {
        if( strlen( strArr[i] ) == 0 )
        {
            continue;
        }

        if( strlen( outArr[outIndex] + strArr[i] ) > ( wrapLength - 1 ) )
        {
            outIndex++;
            outArr += ({ strArr[i] });
            outArr[outIndex] += " ";
        }
        else
        {
            outArr[outIndex] += strArr[i] + " ";
        }
    }

    return implode( outArr, "\n" );
}
