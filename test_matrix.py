import numpy as np
import random as rnd
import subprocess
import argparse

'''
def cut_matrix(matrix, index) -> list:
    return np.delete(np.delete(matrix, 0, 0), index, 1)

def calc_det(matrix):
    det = 0
    if len(matrix) == 1:
        return matrix[0][0]

    for idx, cell in enumerate(matrix[0].tolist()):
        det += (-1) ** (idx % 2) * cell * calc_det(cut_matrix(matrix, idx))
    return det
'''

class Colored:
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    @staticmethod
    def ok(message):
        return Colored.GREEN + message + Colored.ENDC
    
    @staticmethod
    def fail(message):
        return Colored.RED + message + Colored.ENDC
    
    @staticmethod
    def warning(message):
        return Colored.YELLOW + message + Colored.ENDC
    
    @staticmethod
    def print(color, *args):
        print(color)
        for arg in args:
            print(arg, end=" ")
        print(Colored.ENDC)



class Tester:
    def __init__(self, total):
        self.passed = total
        self.total = total

    @staticmethod
    def assert_eq(expected, actual, message=""):
        if expected == actual:
            print(Colored.ok("Pass:"), message)
            print(f"{Colored.ok('Actual:')} [{actual}]\n")
            return True
        else:
            print(Colored.fail("Fail: "), message)
            print(f"{Colored.warning('Expected:')} [{expected}]")
            print(f"{Colored.warning('Actual:')} [{actual}]\n")

        return False

    def run(self, command, input, expected_output):
        try:
            process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)
            actual_output, error = process.communicate(input=input)

            stripped_input = input
            if len(input) > 30:
                stripped_input = input[:15:] + " ... " + input[len(input) - 15::]
            res = Tester.assert_eq(expected_output, actual_output, f"{command[0].strip('./')}: {stripped_input}")
            if not res:
                self.passed -= 1

        except Exception as e:
            print("An error occurred:", str(e))


    def __del__(self):
        if self.passed != self.total:
            print(Colored.fail(f"Passed: {self.passed}/{self.total}"))
        else:
            print(Colored.ok("All tests passed"))

    @staticmethod
    def convert_args(test_name, *args) -> list:
        return [test_name] * 2 + list(args)

    @staticmethod
    def convert_input(matrix_size, matrix) -> str:
        matrix_in = str(matrix_size) + ' '
        for row in matrix: 
            for cell in row:
                matrix_in += str(cell) + ' '
        
        return matrix_in



class MatrixGen:
    @staticmethod
    def get_upper_triangular(size, determinant):
        matrix = np.array([[0]*size for _ in range(size)])

        for i in range(size):
            for j in range(size):
                if i == j:
                    matrix[i][j] = 1
                elif j > i:
                    matrix[i][j] = rnd.randint(1, 10)

        cell_idx: int = rnd.randint(0, size-1)
        matrix[cell_idx][cell_idx] = determinant
        return matrix

    @staticmethod
    def get(size):
        matrix1 = MatrixGen.get_upper_triangular(size, 1).transpose()
        determinant2 = rnd.randint(10, 400)
        matrix2 = MatrixGen.get_upper_triangular(size, determinant2)
        return (np.dot(matrix1, matrix2), determinant2)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
                    prog='Matrix tester',
                    description='Tests determinant calculations')
    parser.add_argument('--file', dest='file', required=True, help='path to program under tests')
    parser.add_argument('--size', dest='matrix_size', help='size of matrix', default=5, type=int)
    parser.add_argument('-n', dest='ntest', help='number of tests', default=5, type=int)
    args = parser.parse_args()

    tester = Tester(args.ntest)
    for i in range(args.ntest):
        matrix, determinant = MatrixGen.get(args.matrix_size)
        tester.run(Tester.convert_args(args.file), Tester.convert_input(args.matrix_size, matrix), str(determinant))