/AM_INIT_AUTOMAKE/ {
    if( $2 ~ /\)$/ )
        ver = substr( $2, 0, length( $2 ) - 1 );
    else
        ver = $2;
    n = split( ver, ver_array, "." );
    if( n >= 3 && ver_array[ n ] ~ /[0-9]+/ )
        ver_array[ n ]++;
    else
        {
            n++;
            ver_array[ n ] = 1;
        }
    out = $1 " ";
    for( i = 1; i < n; ++i )
        out = out ver_array[ i ] ".";
    out = out ver_array[ n ] ")";
    $0 = out;
}

{ print $0; }

