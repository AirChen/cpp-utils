#include <iostream>
const static int SIZE = 4;
const static int NNZ = 9;
const static int NUM_ROWS = 4;
const static int X = -1;
typedef int DTYPE;
void matrixvector(DTYPE A[SIZE][SIZE], DTYPE* y, DTYPE* x) {
	for (int i = 0; i < SIZE; i++) {
		DTYPE y0 = 0;
		for (int j = 0; j < SIZE; j++)
			y0 += A[i][j] * x[j];
		y[i] = y0;
	}
}
// 坐标存储 COO
void spmv_v1(int row[NNZ], int col[NNZ],DTYPE values[NNZ], DTYPE y[SIZE], DTYPE x[SIZE]) {
	int i;
	for (i = 0; i < SIZE; i++) {
		y[i] = 0;
	}
	for ( i = 0; i < NNZ; i++)
		y[row[i]] += values[i] * x[col[i]];
}
// 行存储 CSR
void spmv_v2(int ptr[NUM_ROWS + 1], int col[NNZ], DTYPE data[NNZ], DTYPE y[SIZE], DTYPE x[SIZE]) {
	for (int i = 0; i < NUM_ROWS; i++) {
		DTYPE y0 = 0;
		for (int k = ptr[i]; k < ptr[i + 1]; k++) {
			y0 += data[k] * x[col[k]];
		}
		y[i] = y0;
	}
}
// 对角存储
void spmv_v3(DTYPE data[12], int offsets[SIZE - 1], DTYPE y[SIZE], DTYPE x[SIZE]) {
	int i, j, k, N;
	int Istart, Jstart, stride = 4;
	for (i = 0; i < SIZE - 1; i++) {
		k = offsets[i];
		Istart = std::max(0, -k);
		Jstart = std::max(0, k);
		N = std::min(SIZE - Istart, SIZE - Jstart);
		for (j = 0; j < N; j++) {
			if (data[Istart + i * stride + j] != X)
				y[Istart + j] += data[Istart + i * stride + j] * x[Jstart + j];
		}
	}
}
// 埃尔帕克存储 ELLPACK
void spmv_v4(DTYPE data[12], int indices[12], DTYPE y[SIZE], DTYPE x[SIZE]) {
	int n, i, k, N;
	int max_ncols = SIZE - 1, num_rows = SIZE;
	for (n = 0; n < max_ncols; n++)
		for (i = 0; i < num_rows; i++)
			if (data[n * num_rows + i] != X)
				y[i] += data[n * num_rows + i] * x[indices[n * num_rows + i]];
}
int main() {
	int fail = 0;
	DTYPE M[SIZE][SIZE] = { {1,5,0,0},{0,2,6,0},{8,0,3,7},{0,9,0,4} };
	DTYPE x[SIZE] = { 1,2,3,4 };
	DTYPE y_sw[SIZE];
	matrixvector(M, y_sw, x);
	
	DTYPE data_v1[] = { 1,5,2,6,8,3,7,9,4 };
	int col_v1[] = { 0,1,1,2,0,2,3,1,3 };
	int row_v1[] = { 0,0,1,1,2,2,2,3,3};
	DTYPE y_v1[SIZE];
	spmv_v1(row_v1, col_v1, data_v1, y_v1, x);
	
	DTYPE data_v2[] = { 1,5,2,6,8,3,7,9,4 };
	int col_v2[] = { 0,1,1,2,0,2,3,1,3 };
	int ptr_v2[] = { 0,2,4,7,9 };
	DTYPE y_v2[SIZE];
	spmv_v2(ptr_v2, col_v2, data_v2, y_v2, x);

	DTYPE data_v3[12] = { X,X,8,9,1,2,3,4,5,6,7,X };
	int offsets_v3[SIZE - 1] = { -2,0,1 };
	DTYPE y_v3[SIZE] = { 0 };
	spmv_v3(data_v3, offsets_v3, y_v3, x);

	DTYPE data_v4[12] = { 1,2,8,9,5,6,3,4,X,X,7,X };
	DTYPE indices_v4[12] = { 0,1,0,1,1,2,2,3,X,X,3,X };
	DTYPE y_v4[SIZE] = { 0 };
	spmv_v4(data_v4, indices_v4, y_v4, x);

	for (int i = 0; i < SIZE; i++)
		if (y_sw[i] != y_v1[i] || y_sw[i] != y_v2[i] || y_sw[i] != y_v3[i] || y_sw[i] != y_v4[i])
			fail = 1;
	    if (fail == 1)
		    printf("FAILED\n");
	else
		printf("PASS\n");

	printf("The orin matrix is\n");
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%d ", M[i][j]);
		}
		printf("\n");
	}
	printf("The vector is\n");
	for (int i = 0; i < SIZE; i++) {
		printf("%d\n", x[i]);
	}

	printf("The mul result of matrix is\n");
	for (int i = 0; i < SIZE; i++) {
		printf("%d\n", y_sw[i]);
	}
	return fail;
}