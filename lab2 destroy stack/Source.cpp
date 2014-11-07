#include <stdio.h>
#include <iostream>
using namespace std;

char *min_ptr = NULL, *max_ptr;

void spray_paint(char *x, int sz, char tok, char *t)
{
	printf("%p %3d %x %s\n", x, sz, (unsigned char)tok, t);

	if (min_ptr == NULL){
		min_ptr = x;
		max_ptr = (x + sz - 1);
	}
	else if (min_ptr > x){
		min_ptr = x;
	}
	else if(max_ptr < (x+sz-1)) {
		max_ptr = (x + sz - 1);
	}

	for (int i = 0; i < sz; i++){
		*(x + i) = tok;
	}

}

void dumper(char *x, int n, int sgn)
{
	uintptr_t tempp = (uintptr_t)x;
	tempp -= tempp % 16;
	char* pointer = (char*)tempp;
	int k = 0;
	for (int t = 0; t < n; t++){
		printf("%8p", pointer);
		printf("  ");
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				k = (*(pointer + i + j) + 256)%256; //this happens to output incorrecty if not added 512
				printf("%.2x",k);
			}
			printf("  ");
		}
		printf("\n");
		pointer += 16;
	}


}

void sub2()
{
	int i;
	char* a;
	char x[20];

	spray_paint((char *)&i, sizeof(i), 0xf1, "sub2.i");
	spray_paint((char *)&a, sizeof(a), 0xf2, "sub2.a");
	spray_paint((char *)&x, sizeof(x), 0xf3, "sub2.x");
	printf("Min= %p Max= %p\n", min_ptr, max_ptr);
	dumper(min_ptr, (int)(max_ptr - min_ptr) / 16 + 1, 1);
	//
	//   save/destroy the stack here (don't forget to use an external save area)
	//
	printf(" destroyed stack\n");
	dumper(min_ptr, (int)(max_ptr - min_ptr) / 16 + 1, 1);
	//
	//    restore the stack here
	//
	printf(" restored stack\n");
	dumper(min_ptr, (int)(max_ptr - min_ptr) / 16 + 1, 1);
}

void sub1()
{
	float i;
	short a;
	int x[20];

	spray_paint((char *)&i, sizeof(i), 0xe1, "sub1.i");
	spray_paint((char *)&a, sizeof(a), 0xe2, "sub1.a");
	spray_paint((char *)&x, sizeof(x), 0xe3, "sub1.x");
	sub2();
}
int main()
{
	struct mine {
		char* a;
		char x[20];
		float z;
	} i;

	union crazy {
		float a;
		char b;
		int s;
	} a;

	char x[50];

	spray_paint((char *)&i, sizeof(i), 0xd1, "main.i");
	spray_paint((char *)&a, sizeof(a), 0xd2, "main.a");
	spray_paint((char *)&x, sizeof(x), 0xd3, "main.x");

	sub1();

	return 0;
}

