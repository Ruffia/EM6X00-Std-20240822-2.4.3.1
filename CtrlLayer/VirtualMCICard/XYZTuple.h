#pragma once

template<class T> 
class XYZTuple
{
public:
	T idx;
	T x;
	T y;
	T z;

public:
	XYZTuple(T tidx,T tx,T ty,T tz)
	{
		idx = tidx;
		x = tx;
		y = ty;
		z = tz;
	}
};