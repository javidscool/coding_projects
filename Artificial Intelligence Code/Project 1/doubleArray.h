//todo:: doubleArrayCleanup

#ifndef DOUBLEARRAY
#define DOUBLEARRAY


template <class T>
T** newArrayDouble(int x, int y){
	T** doubleArray = new T*[x];
	//(T**)calloc(x,sizeof(T*));

	for(int i = 0; i < x; i++){
		doubleArray[i] = new T[y];//
		//(T*)calloc(y, sizeof(T));
	}

	return doubleArray;
}


template<class T>
void zero2dArray(T** doubleArray, int sx, int sy){
	
	for(int i = 0; i < sx; i++){
		for(int j = 0; j< sy; j++){
			doubleArray[i][j] = NULL;
		}
	}
}


//in case i forget what i called it, for convenience
template<class T>
inline T** newDoubleArray(int x, int y){
	return newArrayDouble<T>(x,y);
}

template<class T>
inline T** doubleArray(int x, int y){
	return newArrayDouble<T>(x,y);
}

template<class T>
inline T** DoubleArray(int x, int y){
	return newArrayDouble<T>(x,y);
}

#endif