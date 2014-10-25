#include "../samples/common_header.h" 
using namespace std; 
int add (int n, int m) {
 return(n + m) ;
}

int mul (int n, int m) {
 return(n * m) ;
}

int main ( int argc, char **argv ) {
 MyList<string> args = mkArgs ( argc, argv) ;
 return ({int x = 3 ;
int y = 4 ;
int z = 5 ;
( (( mul (x, y ) ) > ( add (y, z ) )) ? ( mul (x, 9 ) ) : ( add (y, 20 ) ) ); }) ;
}
