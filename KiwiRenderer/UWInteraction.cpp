
#include "stdafx.h"

#include "UWInteraction.h"

extern InteractEvent MT_OnUkagakaTouch;
extern InteractEvent MT_OnUkagakaInteract;
extern InteractEvent MT_OnElementClick;

UWInteraction::UWInteraction()
{
	tops = vector<int>();
	bottoms = vector<int>();

	Order2IDTable = map<UINT, int>();

	RECT r1 = { 100, 80, 120, 50 };
	RECT r2 = { 0, 100, 80, 50 };
	//left top right bottom
	AddRectangle(r1);
	AddRectangle(r2);
}


UWInteraction::~UWInteraction()
{
}

//I dont want to explain this code, its too complicated, wich these things never got a bug,
//otherwise I dont know how to fix it.
HRESULT UWInteraction::AddRectangle(RECT rectangle) {

	int top = rectangle.top,
		bottom = rectangle.bottom,
		left = rectangle.left,
		right = rectangle.right;

	/*
	 a_________ b
	  |					  |
	  |					  |
	d|_________|c
	*/

	POINT a = { left, top },
		b = { right, top },
		c = {right, bottom},
		d = {left, bottom};

	POINT verticles[4] = { a, b ,c, d };
	for (POINT p : verticles) {
		for (RECT r : rectangles) {
			if (IsPointInRectangle(p, r))
				return E_FAIL;
		}
	}

	unsigned int n = 0;
	auto i = tops.begin(),
		j = bottoms.begin();
	auto k = rectangles.begin();

	for (int t : tops) {
		if (t > top) {
			n++; i++; j++; k++; 
			continue;
		}
		tops.insert(i, top);
		bottoms.insert(j, bottom);
		break;
	}
	//i = bottoms.begin();
	//for (int b : bottoms) {
	//	if (b < bottom)
	//		i++; continue;
	//	tops.insert(i, bottom);
	//	break;
	//}
	 
	if (tops.size() == 0) {
		tops.push_back(top);
		bottoms.push_back(bottom);
	}

	rectangles.insert(k, rectangle);

	Order2IDTable[n] = Order2IDTable.size();

	return S_OK;
}

int UWInteraction::GetRectByPoint(POINT point) {
	int x = point.x,
		y = point.y;

	vector<int> possibles = vector<int>();

	int i = 0;
	for (int t : tops) {
		i++;
		if (y > t) {
			continue;
		}
		else {
			int b = bottoms[i-1];
			if (y >= b) {
				possibles.push_back(i-1);
			}
		}
	}

	for (int p : possibles) {
		RECT r = rectangles[p];
		if (x < r.left) {
			continue;
		}
		else {
			if (x < r.right) {
				return p;
			}
		}
	}

	return -1;
}

bool UWInteraction::IsPointInRectangle(POINT point, RECT rect) {
	if (rect.top + point.y < rect.bottom) {
		if (rect.left + point.x < rect.right) {
			return true;
		}
	}
	return false;
}
