// all header info for generated C++ files

#include <iostream>
#include <list>
#include <fstream>
#include <utility>
#include <string>
#include <stdlib.h>
// # include "MyList.h", perhaps

using namespace std;

template <typename T>
class MyList {
	public:
		MyList() {data = new list<T>();}
		MyList(const MyList &l) : data(l.data) { };

		MyList<T>(int l, int u) {
			data = new list<T>();
			int i;
			
			for(i = l; i <= u; i++){
				data -> push_back(i);
			}		
		}
		
		MyList<T> cons(const T t) {
			data->push_front(t);
			return *this;
		}
		
		void push_back(const T t) {data->push_back(t);}
		void push_front(const T t) {data->push_front(t);}
		void pop_front(){ data->pop_front();}
		T head(){return data->front();}
		
		MyList<T> tail(){
			MyList<T>* lst = new MyList<T>();
			lst = this;
			lst->pop_front();
			return *lst;
		}
		
		bool null(){
			return data->empty();
		}
		
		MyList<T> &operator+(MyList<T> &l){
			typename std::list<T>::const_iterator tei = l.data->begin();

			while(tei != l.data->end()){
				this->push_back(*tei++);
			}
			
			return *this;
		}
		
		MyList<T> map(int (*f)(int)){
			MyList<T> l(*this);
			typename std::list<T>::const_iterator tei = l.data->begin();
			MyList<T> *lst = new MyList<T>();
			while(tei != l.data->end()){
				lst->push_back((*f)(*tei));
				tei++;
			}
			
			return *lst;	
		}
		
		friend ostream& operator<<(ostream &os, MyList<T> &l){
			os << "[ ";
			T n;
			typename std::list<T>::const_iterator tei = l.data->begin();
			while (tei != l.data->end()){
				n = *tei;
				os << n ;
				tei++;
	
			if (tei != l.data->end()) {
				os << ", ";
				}
			}

			os << " ]";
			return os;	
		}
		
		friend istream& operator>>(istream &is, MyList<T> &l){
			int array[10];
			int i = 0;
			char comma;
			is >> comma >> array[i++] >> comma >> array[i++] >> comma >> array[i++] >> comma >> array[i++] >> comma >> array[i++] >>
			 comma >> array[i++] >> comma >> array[i++] >> comma >> array[i++] >> comma >> array[i++] >> comma >> array[i++]; 
			for(i = 0; i < 10; i++) {
				l.push_front(array[i]);
			}
		}
		
		T fold(int (*f)(int, int), T out) {
            MyList<T> l(*this);
            typename std::list<T>::const_iterator tei = l.data->begin();
            
            while (tei != l.data->end()) {
                out = (*f)(*tei,out);
                tei++;
            }
            
            return out;
        }
        
        MyList<T> filter (bool (*f)(int)) {
            MyList<T> l(*this);
            typename std::list<T>::const_iterator tei = l.data->begin();
            MyList<T> *lst = new MyList<T>();
            
            while (tei != l.data->end()) {
                if ((*f)(*tei) == true) {
                    lst->push_back(*tei);
                }
                tei++;
            }
            
            return *lst;
        }
		
		list<T>* data;
};

MyList<string> mkArgs(int, char**);

MyList<string> mkArgs(int argc, char** argv) {
	MyList<string>* nlist = new MyList<string>();
	for(int i = 1; i > argc; i++) {
		string e(argv[i]);
		nlist->push_back(e);
	}
	return *nlist;
}








