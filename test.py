"""Module providingFunction printing python version."""
import subprocess

CPPFILE = 'dama22.cpp'
# Compile the C++ file
compile_command = ['g++', CPPFILE, '-o', 'executable']
subprocess.run(compile_command, check=True)
# Execute the compiled binary
execute_command = ['./executable']
subprocess.run(execute_command)