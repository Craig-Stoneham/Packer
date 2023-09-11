import xml.etree.ElementTree as ET
from pathlib import Path

# Function to add Doxygen-style comments to a C++ source file
def add_doxygen_comments(source_file, class_name, methods):
    with open(source_file, 'r') as f:
        lines = f.readlines()

    # Find the class declaration and its position in the file
    class_declaration = f"class {class_name} {{\n"
    class_start_line = lines.index(class_declaration)

    # Insert Doxygen comments above the class declaration
    comment_lines = ['/**\n', f' * @class {class_name}\n', ' * @brief Class description goes here.\n', ' */\n']
    lines[class_start_line:class_start_line] = comment_lines

    # Find and comment each method in the class
    for method in methods:
        method_declaration = f"    {method['signature']} {{\n"
        method_start_line = lines.index(method_declaration, class_start_line)
        comment_lines = ['    /**\n', f'     * @brief {method["description"]}\n']

        # Add parameter descriptions if available
        if 'params' in method:
            for param in method['params']:
                comment_lines.append(f'     * @param {param["name"]} {param["description"]}\n')

        comment_lines.append('     */\n')
        lines[method_start_line:method_start_line] = comment_lines

    # Write the modified content back to the file
    with open(source_file, 'w') as f:
        f.writelines(lines)

# Function to parse XML documentation and extract class and method information
def parse_xml(xml_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()

    class_name = root.find('class/name').text
    methods = []

    for method_elem in root.findall('class/methods/function'):
        method = {
            'signature': method_elem.find('signature').text,
            'description': method_elem.find('description').text,
        }

        params = []
        for param_elem in method_elem.findall('param'):
            param = {
                'name': param_elem.attrib['name'],
                'description': param_elem.text,
            }
            params.append(param)

        if params:
            method['params'] = params

        methods.append(method)

    return class_name, methods

# Main script
if __name__ == '__main__':
    xml_file = 'D:\Dev\Apps\Packer\Documentation\Color.xml'
    cpp_file = 'D:\Dev\Apps\Packer\Source\Packer\Color.h'

    class_name, methods = parse_xml(xml_file)
    add_doxygen_comments(cpp_file, class_name, methods)

    print(f'Doxygen comments added to {cpp_file}')
