<p align="center"><img width="100%" src="resources/branding/logo.png" /></p>

# Packer - File Management Utility

Packer is a versatile file management utility designed to facilitate the organized copying and moving of files based on their extensions while preserving the original folder hierarchy. Its primary purpose is to streamline the process of copying or moving specific file types from source folders, particularly in scenarios where these files coexist with project-related materials.

## **Use Cases:**

Imagine maintaining a consolidated workspace where your photo and audio editing projects reside side by side. With Packer, you can seamlessly extract only the exported images and sounds, leaving behind the original project files. This approach empowers you to transfer these files to another location while retaining the familiar folder structure, ensuring the preservation of your source files. Game developers, in particular, stand to benefit from this tool, as it optimizes resource organization and time efficiency.

## **Key Features:**

Packer boasts a range of features designed to enhance your file management experience:

* **Ignore File:** Packer allows you to specify an "Ignore File" in any directory. When an "Ignore File" is found in a directory, Packer will skip that directory and any sub-directories within it during file copying or moving operations. This flexible feature gives you granular control over which directories are processed.

* **Extension Case Adjustment:** Maintain consistent extension casing across your projects. Packer offers the ability to convert extensions to upper or lower case, accommodating the varying conventions of different software.

* **Extension Case Insensitivity:** Packer enables seamless copying and moving of files, regardless of the case of their extensions. This feature eliminates the need to worry about matching the extension case precisely.

* **Suffix Removal:** Easily eliminate suffixes from file names during copying or moving operations.

## **Using CMake for Building:**

Packer can be built using CMake, a popular build system generator. Follow these steps to build Packer using CMake:

1. Clone the Packer repository to your local machine.

2. Create a build directory within the cloned repository:

   ```bash
   mkdir build
   cd build
3. Generate build files with CMake:

   ```bash
   cmake ..

4. Build the project using your preferred build system (e.g., Make, Visual Studio, Xcode):

   ```bash
   cmake --build .

Once the build is complete, you'll find the Packer executable in the appropriate build directory.

## **Available Executables:**

In the [Releases](https://github.com/Craig-Stoneham/packer/releases) section, you will find the Release executable for Packer. This executable is built for the x64 architecture, ensuring compatibility with a wide range of systems. It's optimized for performance and efficiency, making it the recommended choice for everyday use.

## **Contribute and Report Bugs:**

We welcome contributions from the community to improve and enhance Packer. If you'd like to contribute, please follow these steps:

1. Fork the repository and create your branch from `master`.
2. Make your changes, whether it's adding features, fixing bugs, or improving documentation.
3. Open a pull request describing your changes for review.

If you come across any bugs, issues, or have suggestions for improvements, feel free to open an issue in the GitHub repository. Your feedback helps us make Packer even better!

## **License**

This project is licensed under the [MIT License](LICENSE.md).