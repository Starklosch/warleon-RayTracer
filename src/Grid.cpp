#include <Grid.hpp>
#include <limits>

namespace war{

template<class T>
Grid<T>::Grid(const point_t& min,const point_t& max,const index_t& dimensions):
	min(min),max(max),dimensions(dimensions),size(max-min){}

template<class T>
typename Grid<T>::bucket_t& Grid<T>::operator[](const index_t& i){
	const size_t i1d = i[0]*dimensions[0]*dimensions[1] + i[1]*dimensions[1] + i[2];
	if(data.find(i1d)==data.end())data[i1d];//default init cell
	return data[i1d];
}

template<class T>
const typename Grid<T>::bucket_t& Grid<T>::operator[](const index_t& i)const {
	const size_t i1d = i[0]*dimensions[0]*dimensions[1] + i[1]*dimensions[1] + i[2];
	if(data.find(i1d)==data.end())data[i1d];//default init cell
	return data[i1d];
}

template<class T>
typename Grid<T>::Iterator Grid<T>::begin(const Ray& ray){
	return Iterator(this,{0,0,0}); //TODO IMPLEMENT
}

template<class T>
typename Grid<T>::Iterator Grid<T>::end(){
	size_t maxi = std::numeric_limits<size_t>::max();
	return Iterator(nullptr,{maxi,maxi,maxi});
}

template<class T>
typename Grid<T>::index_t Grid<T>::worldToGrid(const point_t& p){
	const auto wi = (p - min)/size;
	return {size_t(wi.x),size_t(wi.y),size_t(wi.z)};
}

template<class T>
Grid<T>::Iterator::Iterator(Grid<T>* g,const index_t& curr):grid(g),current(curr){
}

template<class T>
typename Grid<T>::Iterator& Grid<T>::Iterator::operator++(int){
	//TODO IMPLEMENT
}

template<class T>
typename Grid<T>::bucket_t& Grid<T>::Iterator::operator*(){
	return (*grid)[current];
}

template<class T>
typename Grid<T>::bucket_t* Grid<T>::Iterator::operator->(){
	return &(*grid)[current];
}

template<class T>
bool Grid<T>::Iterator::operator!=(const Iterator& other)const {
	return this->grid != other.grid || this->current != other.current;
}
};

