#include "../samples/common_header.h" 
using namespace std; 
int add (int n, int m) {
 return(n + m) ;
}

int main ( int argc, char **argv ) {
 MyList<string> args = mkArgs ( argc, argv) ;
 return ({int x = 4 ;
int y = 5 ;
( add (x, y ) ); }) ;
}
