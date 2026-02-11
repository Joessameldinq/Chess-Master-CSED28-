#!/usr/bin/env python3
import os
import sys

def generate_folder_array(directory="."):
    """
    Scans a directory for folders and generates a C array declaration.
    
    Args:
        directory: Path to the directory to scan (default: current directory)
    """
    # Get all items in the directory
    try:
        items = os.listdir(directory)
    except FileNotFoundError:
        print(f"Error: Directory '{directory}' not found")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied for directory '{directory}'")
        sys.exit(1)
    
    # Filter only directories (folders)
    folders = sorted([item for item in items if os.path.isdir(os.path.join(directory, item))])
    
    if not folders:
        print(f"No folders found in '{directory}'")
        sys.exit(1)
    
    # Generate C array
    output_lines = ["const char *fullPaths[] = {"]
    
    for i, folder in enumerate(folders):
        path = f"assets/{folder}"
        # Add comma for all but the last item
        if i < len(folders) - 1:
            output_lines.append(f'    "{path}",')
        else:
            output_lines.append(f'    "{path}"')
    
    output_lines.append("};")
    
    # Write to file
    output_file = "folder_paths.txt"
    with open(output_file, 'w') as f:
        f.write('\n'.join(output_lines))
    
    print(f"Generated C array with {len(folders)} folders")
    print(f"Output written to: {output_file}")
    print("\nPreview:")
    print('\n'.join(output_lines))

if __name__ == "__main__":
    # Check if directory argument is provided
    if len(sys.argv) > 1:
        target_dir = sys.argv[1]
    else:
        target_dir = "."
    
    generate_folder_array(target_dir)