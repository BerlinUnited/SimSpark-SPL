BEGIN {
    if( change <= 0 )
        exit -1;
}

/AM_INIT_AUTOMAKE/ {
    if( $2 ~ /\)$/ )
        {
            ver = substr( $2, 0, length( $2 ) - 1 );
            tail=1;
        }
    else
        ver = $2;

    n = split( ver, ver_array, "." );
    
    while( change > n )
        ver_array[ ++n ] = 0;
    
    ver_array[ change ]++;

    while( ++change <= n )
        ver_array[ change ] = 0;

    $2 = "";
    for( i = 1; i < n; ++i )
        $2 = $2 ver_array[ i ] ".";
    $2 = $2 ver_array[ n ];
    if( tail )
        $2 = $2 ")";
}

{ print $0; }

