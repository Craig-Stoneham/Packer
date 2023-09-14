import subprocess
import os

# Directory where this script is located
script_dir = os.path.dirname(os.path.abspath(__file__))
documentation_dir = os.path.join(script_dir, "..", "Documentation")

# Ensure the documentation directory exists, create it if it doesn't
if not os.path.exists(documentation_dir):
    os.makedirs(documentation_dir)
    
# List of Doxygen configuration files
doxygen_configs = [
    "Packer",
    "Console",
    "Tests",
]

for config in doxygen_configs:
    config_path = os.path.join(script_dir, "Doxyfiles", config)
    print(f"Generating documentation for {config}...")
    subprocess.run(["doxygen", config_path], cwd=documentation_dir)
    print(f"Documentation generated for {config}.")

print("Documentation generation complete.")
