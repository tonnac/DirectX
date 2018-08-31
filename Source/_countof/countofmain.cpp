#include <iostream>
#include <cstdlib>

int main(void)
{
	int * ptr = new int[5];
	int arr[5] = { 1, };

	size_t i = _countof(arr);
	size_t k = _msize(ptr);
	delete[] ptr;
	return 0;
}