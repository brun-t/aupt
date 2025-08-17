import sys
import os

def to_cpp_array(data, var_name):
    lines = []
    line = ""
    for i, b in enumerate(data):
        line += f"0x{b:02x}, "
        if (i + 1) % 12 == 0:
            lines.append(line)
            line = ""
    if line:
        lines.append(line)
    array_content = "\n  ".join(lines)
    return f"const unsigned char {var_name}[] = {{\n  {array_content}\n}};\n\nconst unsigned int {var_name}_size = {len(data)};\n"

def main(files_str, output_header, var_names_str):
    files = [f.strip() for f in files_str.split(',')]
    var_names = [v.strip() for v in var_names_str.split(',')]

    if len(var_names) != len(files):
        print("Error: The number of variable names must match the number of files.")
        sys.exit(1)

    arrays = []
    for filepath, var_name in zip(files, var_names):
        if not os.path.isfile(filepath):
            print(f"Warning: '{filepath}' is not a file, skipping.")
            continue
        with open(filepath, "rb") as f:
            data = f.read()
        cpp_array = to_cpp_array(data, var_name)
        arrays.append(cpp_array)

    with open(output_header, "w") as f:
        f.write("#pragma once\n\n")
        f.write("// Auto-generated file data for multiple files\n\n")
        f.write("\n".join(arrays))

    print(f"Generated {output_header} with {len(arrays)} embedded files.")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python gen_files.py <file1,file2,...> <output_header.hpp> <varname1,varname2,...>")
        sys.exit(1)
    files_str = sys.argv[1]
    output_header = sys.argv[2]
    var_names_str = sys.argv[3]
    main(files_str, output_header, var_names_str)
