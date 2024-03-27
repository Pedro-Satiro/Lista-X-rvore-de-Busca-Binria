from concurrent.futures import ThreadPoolExecutor, as_completed
from ctypes import CDLL, POINTER, Structure, c_double, c_int, c_longlong
from numpy import random
import numpy as np
import time
from tqdm import tqdm
import gc

# Define the struct in Python corresponding to the C struct
class Result(Structure):
    _fields_ = [("time", c_double), ("steps", c_int)]

# Load the shared library
lib_lista = CDLL('./lista_encadeada.dll')  # Update the path as needed
lib_arvore = CDLL('./arvore_busca_binaria.dll')  # Update the path as needed

# Call the C function and get the result
procurar_lista = lib_lista.procurar_lista
procurar_lista.argtypes = [POINTER(c_int), c_int, c_int]
procurar_lista.restype = Result

procura_arvore = lib_arvore.procura_arvore
procura_arvore.argtypes = [POINTER(c_int), c_int, c_int]
procura_arvore.restype = Result

# # Create a list with x elements with random values
# size = 99999999 
# my_list = random.randint(32767, size=(size), dtype=np.int64)
# value_to_find = random.choice(my_list)
# c_array = my_list.ctypes.data_as(POINTER(c_longlong))

# result = procura_arvore(c_array, size, value_to_find)
# print(f"Time: {result.time} s", )
# print("Steps:", result.steps)

# result = procurar_lista(c_array, size, value_to_find)
# print(f"Time: {result.time} s", )
# print("Steps:", result.steps)

import matplotlib.pyplot as plt

# Create a list to store the results
sizes = []
arvore_times = []
arvore_steps = []
lista_times = []
lista_steps = []

# Define the range of sizes to test
start_size = 10
size = start_size
end_size = 10000000
# step_size = 10000

# Iterate over the sizes
while size <= end_size:
    # Create a list to store the times and steps for each run
    arvore_times_per_run = []
    arvore_steps_per_run = []
    lista_times_per_run = []
    lista_steps_per_run = []

    # Run the tests multiple times
    num_runs = 50 #acaso 
    range_max = 2147483647 # tamanho maximo do int gerado no array 
    my_list = random.randint(range_max, size=(size), dtype=np.int64)
    c_array = my_list.ctypes.data_as(POINTER(c_int))

    def run_tests(value_to_find):
        result_arvore = procura_arvore(c_array, size, value_to_find)
        result_lista = procurar_lista(c_array, size, value_to_find)

        return result_arvore, result_lista

    with ThreadPoolExecutor(max_workers=10) as executor:
        future_to_row = {
            executor.submit(run_tests, random.choice(my_list)): _
            for _ in range(num_runs)
        }
    # for _ in tqdm(range(num_runs), desc=f"Size: {size}", unit='run'):
        # Create a list with x elements with random values
        # value_to_find = random.choice(my_list)

        # Call the C functions and get the results
        # result_arvore = procura_arvore(c_array, size, value_to_find)
        # result_lista = procurar_lista(c_array, size, value_to_find)

        # Store the results for each run
        for future in tqdm(as_completed(future_to_row), total=num_runs, desc=f"Size: {size}", unit='run'):
            result = future.result()
            result_arvore, result_lista = result
            arvore_times_per_run.append(result_arvore.time)
            arvore_steps_per_run.append(result_arvore.steps)
            lista_times_per_run.append(result_lista.time)
            lista_steps_per_run.append(result_lista.steps)

    # Calculate the median values for time and steps
    arvore_median_time = np.median(arvore_times_per_run)
    arvore_median_steps = np.median(arvore_steps_per_run)
    lista_median_time = np.median(lista_times_per_run)
    lista_median_steps = np.median(lista_steps_per_run)
    

    # Store the median values
    sizes.append(size)
    arvore_times.append(arvore_median_time)
    arvore_steps.append(arvore_median_steps)
    lista_times.append(lista_median_time)
    lista_steps.append(lista_median_steps)

    size *= 10

# Plot the graph
plt.figure(figsize=(15, 5))
plt.subplot(1, 3, 1)
plt.plot(sizes, lista_steps, label='Lista', color='red', linewidth=2)
plt.xlabel('Tamanho', fontsize=12)
plt.ylabel('Passos', fontsize=12)
plt.title('Tamanho X Passos | lista | O(n)', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True)

plt.subplot(1, 3, 2)
plt.plot(sizes, arvore_steps, label='Arvore', color='blue', linewidth=2)

plt.xlabel('Tamanho', fontsize=12)
plt.ylabel('Passos', fontsize=12)
plt.title('Tamanho X Passos | Arvore | O(Log(n))', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True)
 
plt.subplot(1, 3, 3)
plt.plot(sizes, arvore_steps, label='Arvore', color='blue', linewidth=2)
plt.plot(sizes, lista_steps, label='Lista', color='red', linewidth=2)
plt.xlabel('Tamanho', fontsize=12)
plt.ylabel('Passos', fontsize=12)
plt.title('Arvore X Lista', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True)

plt.tight_layout()
plt.show()


