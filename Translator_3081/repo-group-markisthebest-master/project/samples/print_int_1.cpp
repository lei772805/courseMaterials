#include "../samples/common_header.h" 
using namespace std; 
int main ( int argc, char **argv ) {
 MyList<string> args = mkArgs ( argc, argv) ;
 return ({ cout << (1 + 2) << endl ; cout.good() ? 0 : 1 ; }) ;
}
