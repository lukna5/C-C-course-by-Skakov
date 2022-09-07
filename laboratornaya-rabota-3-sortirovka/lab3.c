#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Man{
	char lastname[42];
	char firstname[42];
	char father[42];
	long long number;
} Man;

static int realSize = 2;
int checkMas(Man *mas, int size) {
	if (size >= realSize - 1)
	{
		realSize *= 2;
		mas = (Man*) realloc(mas, realSize * sizeof(Man));
		if (mas == NULL)
		{
			return 0;
		}
	}
	return 1;
}

int compare(Man* first, Man* second) {
	if (strcmp(first->lastname, second->lastname) > 0) return 1;
	else if (strcmp(first->lastname, second->lastname) < 0) return -1;
	if (strcmp(first->firstname, second->firstname) > 0) return 1;
	else if (strcmp(first->firstname, second->firstname) < 0) return -1;
	if (strcmp(first->father, second->father) > 0) return 1;
	else if (strcmp(first->father, second->father) < 0) return -1;
	if (first->number > second->number) return 1;
	else if (first->number < second->number) return -1;
	return 0;
}

int bigger(Man* first, Man* second) {
	int comp = compare(first, second);
	if (comp >= 1) return 1;
	return 0;
}

int smaller(Man* first, Man* second) {
	int comp = compare(first, second);
	if (comp <= -1) return 1;
	return 0;
}

int equals(Man* first, Man* second) {
	int comp = compare(first, second);
	if (comp == 0) return 1;
	return 0;
}

void swap(Man* mas, int first, int second) {
	Man t = mas[first];
	mas[first] = mas[second];
	mas[second] = t;
}

void quickSort(Man* mas, int left, int right) {
	while (left < right) {
		int i = left, j = right;
		Man q = mas[i];
		while (i <= j){
			while (bigger(&mas[j], &q))
				j--;
			while (smaller(&mas[i], &q))
				i++;
			if (i > j) 
				break;
			swap(mas, i, j);
			i++;
			j--;
		} 
		quickSort(mas, left, j);
		left = i;
	}
}


int main(int argc, char **argv) {
	if (argc != 3)
	{
		printf("Not enough arguments");
		return 1;
	}
	FILE* input = fopen(argv[1], "r");
	if (input == NULL)
	{
		printf("Can`t open reading file");
		return 1;
	}
	int size = 0;
	Man* mas = malloc(2 * sizeof(Man));
	if (mas == NULL)
	{
		fclose(input);
		printf("Can`t allocate more memory");
		return 2;
	}
	while (!feof(input))
	{
		if (size >= realSize - 1)
		{
			realSize = size * 2;
			double per = 1.9;
			Man* temp = (Man*)realloc(mas, realSize * sizeof(Man));
			if (temp == NULL)
			{
				while (1) {
					realSize = (int) size * per;
					temp = (Man*) realloc(mas, realSize * sizeof(Man));
					if (temp == NULL)
					{
						if (per <= 1.1) {
							fclose(input);
							free(mas);
							printf("Can`t allocate more memory");
							return 2;
						}
						per -= 0.1;
						continue;
					}
					else
					{
						break;
					}
				}
				if (realSize <= size)
				{
					fclose(input);
					free(mas);
					printf("Can`t allocate more memory");
					return 2;
				}
			}
			mas = temp;
		}
		fscanf(input, "%s", &mas[size].lastname);
		fscanf(input, "%s", &mas[size].firstname);
		fscanf(input, "%s", &mas[size].father);
		fscanf(input, "%lld", &mas[size].number);
		size++;
	}
	fclose(input);
	FILE* output = fopen(argv[2], "w");
	if (output == NULL)
	{
		free(mas);
		printf("Can`t allocate more memory");
		return 2;
	}
	quickSort(mas, 0, size - 1);
	for (int i = 0; i < size; i++)
	{
		fprintf(output, "%s %s %s %lld\n", mas[i].lastname, mas[i].firstname, mas[i].father, mas[i].number);
	}
	free(mas);
	fclose(output);
}