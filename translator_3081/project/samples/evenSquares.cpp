#include "../samples/common_header.h" 
using namespace std; 
int square (int n) {
 return(n * n) ;
}

bool even (int n) {
 return((n % 2) == 0) ;
}

int main ( int argc, char **argv ) {
 MyList<string> args = mkArgs ( argc, argv) ;
 return ({MyList< int >numbers = ({ MyList<int> temp(1, 10); temp; }) ;
MyList< int >squaredNumbers = (numbers.map(square)) ;
MyList< int >evenSquaredNumbers = (squaredNumbers.filter(even)) ;
({int p1 = ({ cout << squaredNumbers << endl ; cout.good() ? 0 : 1 ; }) ;
int p2 = ({ cout << evenSquaredNumbers << endl ; cout.good() ? 0 : 1 ; }) ;
(p1 + p2); }); }) ;
}
