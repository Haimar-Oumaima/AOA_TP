//
void baseline(unsigned n , float x [ n ] , const float y [ n ] , const float z [ n ][ n ]) {

unsigned i , j ;

    for ( j =0; j < n ; j ++)
        for ( i =0; i < n ; i ++)
             x [ i ] += y [ j ] * z [ i ][ j ];
}
