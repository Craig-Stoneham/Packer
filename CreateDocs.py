import subprocess
import os
import tempfile

# Directory where this script is located
script_dir = os.path.dirname(os.path.abspath(__file__))
documentation_dir = os.path.join(script_dir, "Documentation")

# Ensure the documentation directory exists, create it if it doesn't
if not os.path.exists(documentation_dir):
    os.makedirs(documentation_dir)

# List of projects to document
projects = [
    {
        "name": "Packer",
        "input": [
            os.path.join(script_dir, "Source/Packer"),
            os.path.join(script_dir, "Source/Console")
        ],
        "output": ".",
        "file_patterns": "*.h"
    }
]

for project in projects:
    print(f"Generating documentation for {project['name']}...")
    
    # Create a temporary Doxyfile
    with tempfile.NamedTemporaryFile(mode='w+', delete=False) as temp:
        temp.write(f"PROJECT_NAME = {project['name']}\n")
        temp.write(f"OUTPUT_DIRECTORY = {project['output']}\n")  # Set to current directory
        input_dirs = " ".join(project['input'])
        temp.write(f"INPUT = {input_dirs}\n")
        temp.write(f"FILE_PATTERNS = {project['file_patterns']}\n")
        temp_file_name = temp.name
    
    # Run Doxygen using the temporary Doxyfile
    subprocess.run(["doxygen", temp_file_name], cwd=documentation_dir)
    
    # Delete the temporary Doxyfile
    os.remove(temp_file_name)
    
    print(f"Documentation generated for {project['name']}.")

print("Documentation generation complete.")
