#!/usr/bin/env python3
import os
import sys
from pathlib import Path

try:
    from rembg import remove
    from PIL import Image
except ImportError:
    print("Error: Required packages not installed.")
    print("Please install them with:")
    print("  pip install rembg pillow")
    sys.exit(1)

def remove_bg_from_pngs(directory=".", output_dir=None):
    """
    Remove background from all PNG files in a directory.
    
    Args:
        directory: Path to the directory containing PNG files (default: current directory)
        output_dir: Optional output directory (default: creates 'no_bg' subfolder)
    """
    # Convert to Path object
    input_path = Path(directory)
    
    if not input_path.exists():
        print(f"Error: Directory '{directory}' not found")
        sys.exit(1)
    
    if not input_path.is_dir():
        print(f"Error: '{directory}' is not a directory")
        sys.exit(1)
    
    # Set up output directory
    if output_dir is None:
        output_path = input_path / "no_bg"
    else:
        output_path = Path(output_dir)
    
    output_path.mkdir(exist_ok=True)
    
    # Find all PNG files
    png_files = list(input_path.glob("*.png"))
    
    if not png_files:
        print(f"No PNG files found in '{directory}'")
        sys.exit(1)
    
    print(f"Found {len(png_files)} PNG files")
    print(f"Output directory: {output_path}")
    print("-" * 50)
    
    # Process each PNG file
    success_count = 0
    for i, png_file in enumerate(png_files, 1):
        try:
            print(f"[{i}/{len(png_files)}] Processing: {png_file.name}... ", end="", flush=True)
            
            # Read input image
            with open(png_file, 'rb') as input_file:
                input_data = input_file.read()
            
            # Remove background
            output_data = remove(input_data)
            
            # Save output image
            output_file = output_path / png_file.name
            with open(output_file, 'wb') as out_file:
                out_file.write(output_data)
            
            print("✓ Done")
            success_count += 1
            
        except Exception as e:
            print(f"✗ Error: {str(e)}")
    
    print("-" * 50)
    print(f"Successfully processed {success_count}/{len(png_files)} images")
    print(f"Output saved to: {output_path.absolute()}")

def main():
    """Main function to handle command line arguments."""
    if len(sys.argv) > 2:
        target_dir = sys.argv[1]
        output_dir = sys.argv[2]
        remove_bg_from_pngs(target_dir, output_dir)
    elif len(sys.argv) > 1:
        target_dir = sys.argv[1]
        remove_bg_from_pngs(target_dir)
    else:
        remove_bg_from_pngs(".")

if __name__ == "__main__":
    main()