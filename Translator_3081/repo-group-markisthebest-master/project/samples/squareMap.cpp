#include "../samples/common_header.h" 
using namespace std; 
int square (int n) {
 return(n * n) ;
}

int main ( int argc, char **argv ) {
 MyList<string> args = mkArgs ( argc, argv) ;
 return ({MyList< int >numbers = ({ MyList<int> temp(1, 10); temp; }) ;
MyList< int >squaredNumbers = (numbers.map(square)) ;
({ cout << squaredNumbers << endl ; cout.good() ? 0 : 1 ; }); }) ;
}
