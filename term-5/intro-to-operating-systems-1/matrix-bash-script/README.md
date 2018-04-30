matrix is a bash shell script that can perform various operations on a file that contains a matrix of integers.
The file must have rows of tab separated numbers that represent a matrix. The following matrix functions
can be called: "dims" returns the dimensions of the matrix, "transpose" transposes the matrix, "mean" produces
the mean of the matrix, "add" sums two matrices, and "multiply" outputs the product of two matrices.
The script is called with the function and matrix file name as argument for dims, transpose, and mean: matrix dims matrixFileName.
If calling add or multiply the arguments must be the function and two matrix files like so: matrix add matrixFileName1 matrixFileName2
