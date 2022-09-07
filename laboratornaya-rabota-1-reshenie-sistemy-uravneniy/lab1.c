#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addWithOut(int *i, const int y)
{
	if (*i == y - 1) *i += 2;
	else *i = *i + 1;
}

void copyMas(const float *mas1, float *mas2, const int size)
{
	for (int i = 0; i < size * size; i++){
		mas2[i] = mas1[i];
	}
}

void reduceMatrix(const float *mas, float *newMas, const int size, const int index)
{
	int startJ = -1;
	addWithOut(&startJ, index);
	int coordinate = 0;
	for (int i = 1; i < size + 1; i++) {
		for (int j = startJ; j < size + 1; addWithOut(&j, index)) {
			newMas[coordinate] = mas[i * (size + 1) + j];
			coordinate++;
		}
	}
}

float determinant(float *mas, int size, int *stop)
{	
	if (*stop == 1){
		free(mas);
		return 0;
	}
	if (size == 1) {
		float res = mas[0];
		free(mas);
		return res;
	}
	size--;
	float result = 0;
	float sign = 1;
	for (int i = 0; i < size + 1; i++) {
		float *newMas = malloc(size * size * sizeof(float));
		if (newMas == NULL){
			printf("No more memory can be allocated");
			*stop = 1;
			free(mas);
			return 0;
		}
		reduceMatrix(mas, newMas, size, i);
		float det = determinant(newMas, size, stop);
		if (*stop == 1){
			free(mas);
			return 0;
		}
		result += mas[i] * det * sign;
		sign *= -1;
	}
	free(mas);
	return result;
}

int main(int argc, char *argv[])
{
	FILE *input = fopen(argv[1], "r");
	if (input == NULL){
		printf("Can`t open this file for reading %s", argv[1]);
		return 1;
	}
	const int size;
	fscanf(input, "%d", &size);
	float *mas = malloc(size * size * sizeof(float));
	if (mas == NULL) {
		printf("No more memory can be allocated");
		fclose(input);
		return 2;
	}
	float *results = malloc(size * sizeof(float));
	if (results == NULL) {
		printf("No more memory can be allocated");
		free(mas);
		fclose(input);
		return 2;
	}
	float* dopMas = malloc(size * size * sizeof(float));
	if (dopMas == NULL) {
		printf("No more memory can be allocated");
		free(mas);
		free(results);
		fclose(input);
		return 2;
	}
	int index = 0;
	for (int i = 0; i < size * (size + 1); i++){
		if ((i + 1) % (size + 1) == 0) fscanf(input, "%f", &(results[(i + 1) / (size + 1) - 1]));
		else fscanf(input, "%f", &mas[index++]);
	}
	fclose(input);
	FILE *output = fopen(argv[2], "w");
	if (output == NULL) {
		printf("No more memory can be allocated for this writing file %s", argv[2]);
		free(dopMas);
		free(mas);
		free(results);
		return 1;
	}
	copyMas(mas, dopMas, size);
	int stop = 0;
	const float detMain = determinant(dopMas, size, &stop);
	if (stop == 1) {
		free(dopMas);
		free(mas);
		free(results);
		fclose(output);
		return 2;
	}
	int oneSolution = 1;
	if (detMain == 0){
		oneSolution = 0;
	}
	for (int i = 0; i < size; i++){
		dopMas = malloc(size * size * sizeof(float));
		if (dopMas == NULL) {
			printf("No more memory can be allocated");
			free(mas);
			free(results);
			fclose(output);
			return 2;
		}
		copyMas(mas, dopMas, size);
		for (int j = 0; j < size; j++){
			dopMas[j * size + i] = results[j];
		}
		float nextDet = determinant(dopMas, size, &stop);
		if (stop == 1){
			free(mas);
			free(results);
			fclose(output);
			return 2;
		}
		if (oneSolution == 0){
	        if (nextDet != 0){
				oneSolution = -1;
				fprintf(output, "no solution\n");
				break;
			}
		} else fprintf(output, "%g\n", nextDet / detMain);
	}
	if (oneSolution == 0){
		fprintf(output, "many solutions\n");
	}
	free(results);
	free(mas);
	fclose(output);
}

