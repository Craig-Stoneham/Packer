import subprocess
import os

# Directory where this script is located
script_dir = os.path.dirname(os.path.abspath(__file__))

# List of Doxygen configuration files
doxygen_configs = [
    "Doxyfile_Packer",
    "Doxyfile_Console",
    "Doxyfile_Tests",
]

for config in doxygen_configs:
    config_path = os.path.join(script_dir, "Documentation", config)
    print(f"Generating documentation for {config}...")
    subprocess.run(["doxygen", config_path], cwd=os.path.join(script_dir, "Documentation"))
    print(f"Documentation generated for {config}.")

print("Documentation generation complete.")
