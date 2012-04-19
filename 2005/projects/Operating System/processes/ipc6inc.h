#define M_SIZE 0x8000
#define P 6 
char f1(int i) {
	return i % 26 + 'A';
}
char f2(int i) {
	return i%26 + 'a';
}

char f0(int i) { 
	return i*0;
}

void fill(char *arr, char (*func)(int)) {
	int i;
	for (i = 0; i < M_SIZE; i++) {
		arr[i] = func(i);
	}
	//verify(arr,func);
	return;
}
 
int handle;

int verify(char *arr, char (*func)(int)) {
	int i;
	for (i = 0; i < M_SIZE; i++) {
		if (arr[i] != func(i)) {
			crt_printf(handle,"ERROR: Got (%c = %d) but was expecting (%c = %d) at index %d\n",arr[i],arr[i],func(i),func(i),i);
			return 0;
		}
	}
	return 1;
}

