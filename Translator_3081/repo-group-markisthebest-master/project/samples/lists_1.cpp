#include "../samples/common_header.h" 
using namespace std; 
int main ( int argc, char **argv ) {
 MyList<string> args = mkArgs ( argc, argv) ;
 return ({MyList< int >someNumbers = ({ MyList<int> temp; temp.push_back(1);
temp.push_back(2);
temp.push_back(3);
temp; }) ;
({ cout << someNumbers << endl ; cout.good() ? 0 : 1 ; }); }) ;
}
