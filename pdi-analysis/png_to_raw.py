from PIL import Image

def png_to_raw(input_png_path, output_raw_path, size=(256, 256)):
    """
    Converts a PNG image to raw pixel data without a header.
    
    Args:
        input_png_path (str): The path to the input PNG file.
        output_raw_path (str): The path for the output raw file.
        size (tuple): The desired output size (width, height).
    """
    try:
        # Open the image using Pillow
        img = Image.open(input_png_path)
        
        # Convert to RGB to ensure a consistent number of channels (removes alpha)
        # For grayscale, use `.convert('L')`
        img = img.convert('RGB')
        
        # Resize the image to 256x256
        img = img.resize(size, Image.Resampling.LANCZOS)
        
        # Get the raw pixel data as a bytes object
        raw_data = img.tobytes()
        
        # Write the raw data to a new file
        with open(output_raw_path, 'wb') as f:
            f.write(raw_data)
        
        print(f"Successfully converted '{input_png_path}' to '{output_raw_path}'")
        print(f"Raw file size: {len(raw_data)} bytes")
        
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage:
input_file = 'uea.png'  # Replace with your PNG file name
output_file = 'uea.raw'
png_to_raw(input_file, output_file)
