#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "my_library.h"

int DEL;

void printf_b(symbol_with_freq b[], int n) {
	int i;
	for(i =0 ; i< n; i++) {
		printf("symbol: %c freq: %d\n", b[i].symbol, b[i].frequency);
	}
}

int count_frequency(char *s, symbol_with_freq b[]) {
	int i;
	int n = 1;
	b[0].symbol = '-';
	b[0].frequency = 0;
	for(i = 0; s[i]; i++) {
		int k;
		for(k = 1; k < n; k++) {
			if(s[i] == b[k].symbol) {
				b[k].frequency+=1;
				break;
			}
		}

		if(k == n) {
			b[k].symbol = s[i];
			b[k].frequency = 1;
			n++;
		}
	}

	QuickSort(b, 1, n-1);
	for(i = 1; i<n; i++) {
		b[i].frequency += b[i-1].frequency;
	}
	DEL = b[n-1].frequency;
	printf_b(b,n);
	b[n].symbol = -1;
	b[n].frequency = -1;
	return 0;
}

int search(symbol_with_freq b[], char c) {
	int i = 0;
	while(b[i].symbol != c) {
		if(-1 == b[i].symbol) {
			printf("NOT FOUND\n");
			exit(4567543);
		}
		i++;
	}
	return i;
}

void add_bit(int bit, char* code) {
	int n = strlen(code);
	if(bit == 0) {
		code[n] = '0';
	}
	else {
		code[n] = '1';
	}

	code[n+1] = '\0';
}

int write_bits(int bit, int bits_to_foll, char* code) {
	
	add_bit(bit, code);

	while(bits_to_foll > 0) {
		add_bit(!bit, code);
		bits_to_foll -= 1;	   	   
	}
}

int encode(char *s, symbol_with_freq b[]) {
	int l[strlen(s)]; //исходный рабочий интервал
	int h[strlen(s)];
	h[0] = 65535;
	l[0] = 0;
	int first_qtr = h[0]/4+1;
	int half = 2*first_qtr;
	int third_qtr = 3*first_qtr;
	char code[500];
	code[0] = '\0';	
	int i = 1;
	int range = 0;
	int c_index;
	int bits_to_foll = 0;
	printf("DEL = %d\n\n\n", DEL);
	
	while(s[i-1]) {
		printf("symbol:%c\n", s[i-1]);
		c_index = search(b, s[i-1]);
		printf("friquency: %d\n", b[c_index].frequency);


		range = h[i-1] - l[i-1] + 1;

		l[i] = l[i-1] + (range * b[c_index-1].frequency)/DEL;
		h[i] = l[i-1] + (range * b[c_index].frequency)/DEL - 1;
		printf("[%d;%d]\n", l[i], h[i]);	
		for(;;) {
			if(h[i] < half) {
				write_bits(0, bits_to_foll, code);
				bits_to_foll = 0;
			}
			else if(l[i] >= half) {
				write_bits(1, bits_to_foll, code);
				bits_to_foll = 0;
				l[i] -= half;
				h[i] -= half;
			}
			else if(l[i] >= first_qtr && h[i] < third_qtr) {
				bits_to_foll +=1;
				l[i] -= first_qtr;
				h[i] -= first_qtr;
			}
			else break;
			l[i] = 2*l[i];
			h[i] = 2*h[i] + 1;
		}
		printf("[%d;%d]\n", l[i], h[i]);	
		i++;
		printf("code = %s\n", code);
		printf("-----------------\n");
	}

	printf("%s", code);
	return 0;
}


int main(int argc, char* argv[]) {
	if(argc < 3) {
		printf("please, use ./haffman <filename> <filename>\n");
		return 0;
	}

	FILE *fd;
	fd = fopen(argv[1], "r");
	if(fd == NULL) {
		printf("SOME ERROR:(\n");
		return 0;
	}

	char* s = malloc( sizeof(char)*1000 );
	
	int i = 0;
	do {
		s[i] = fgetc(fd);
		if(s[i] == EOF) {
			if(feof(fd)) {
				printf("Считывание завершено\n");
				break;
			}
		}
		i++;
	} while(1);
	s[i] = '\0';
	
	printf("считанное = %s\n", s);
	fclose(fd);

	s = "baaa\0";
	printf("s = %s\n", s);

	symbol_with_freq b[strlen(s)];
	count_frequency(s, b);
	encode(s,b);

//	free(s);
	return 0;
}