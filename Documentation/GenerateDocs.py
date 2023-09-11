import subprocess

# List of Doxygen configuration files and their respective names
doxygen_configs = [
    ("Doxyfile_Packer", "Packer"),
    ("Doxyfile_Console", "Console"),
    ("Doxyfile_Tests", "Tests"),
]

for config, component in doxygen_configs:
    print(f"Generating {component} documentation...")
    subprocess.run(["doxygen", config])
    print(f"{component} documentation generated.")

print("Documentation generation complete.")
