import subprocess
import os
import tempfile
import sys  # Don't forget to import sys

script_dir = os.path.dirname(os.path.abspath(__file__))
documentation_dir = os.path.join(script_dir, "Documentation")

if not os.path.exists(documentation_dir):
    os.makedirs(documentation_dir)

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
    
    with tempfile.NamedTemporaryFile(mode='w+', delete=False) as temp:
        temp.write(f"PROJECT_NAME = {project['name']}\n")
        temp.write(f"OUTPUT_DIRECTORY = {project['output']}\n")
        input_dirs = " ".join(project['input'])
        temp.write(f"INPUT = {input_dirs}\n")
        temp.write(f"FILE_PATTERNS = {project['file_patterns']}\n")
        temp_file_name = temp.name
    
    result = subprocess.run(["doxygen", temp_file_name], cwd=documentation_dir)
    
    if result.returncode != 0:
        print(f"Failed to generate documentation for {project['name']}.")
        sys.exit(1)  # Exit with an error code
    
    os.remove(temp_file_name)
    
    print(f"Documentation generated for {project['name']}.")

print("Documentation generation complete.")
sys.exit(0)  # Exit successfully
