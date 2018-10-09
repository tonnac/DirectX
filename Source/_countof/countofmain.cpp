#include <iostream>
#include <cstdlib>
#include <windows.h>
int main(void)
{
	int * ptr = new int[5];
	int arr[5] = { 1, };

	RECT rt = { 3,3,rt.left + 5, rt.top + 8 };

	size_t i = _countof(arr);
	size_t k = _msize(ptr);
	delete[] ptr;
	return 0;
}