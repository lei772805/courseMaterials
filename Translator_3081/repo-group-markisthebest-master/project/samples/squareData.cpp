#include "../samples/common_header.h"
using namespace std; 
MyList< int >  mkSquares(MyList< int >  ns) {
  return ( (ns.null() ) ? ({ MyList<int> temp;  temp; }) : ( ( mkSquares(( ns.tail() )) ).cons((( ns.head()) * ( ns.head()))) )) ;
}

int main ( int argc, char **argv ) { 
  MyList<string> args = mkArgs ( argc, argv) ;
  return ({ MyList< int >  numbers = ({ MyList<int> temp(1, 10); temp; }) ; 
MyList< int >  squaredNumbers = ( mkSquares(numbers) ) ; 
 ({ cout << squaredNumbers << endl ; cout.good() ? 0 : 1 ; })  ; }) ;
}



