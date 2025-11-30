The BMP Image Editing System in C is a low-level image processing application designed to perform essential editing operations on bitmap (BMP) image files. Built entirely from scratch using core C concepts, the project demonstrates how digital images can be manipulated at the pixel level through careful interpretation of binary data.
The program begins by manually reading and decoding BMP file headers, interpreting image dimensions, color depth, row padding, and pixel arrangement. After loading the image into a custom data structure, the system allows the user to apply a variety of transformations, including:
1. Cropping a selected region
2. Rotating the image clockwise or counter-clockwise
3. Flipping the image horizontally or vertically
4. Adjusting opacity (alpha channel)
5. Inverting colors to create a negative effect

Each editing tool is implemented using its own algorithm for pixel coordinate transformation, ensuring correctness and efficiency. The program uses dynamic memory allocation to store pixel data, and each transformation generates a new image structure to prevent data corruption or memory leaks.
A simple, menu-driven user interface guides the user through each operation, enabling interactive editing without requiring any prior technical knowledge of image formats. The final edited image can be saved back as a valid BMP file using a manually constructed header and pixel writing routine.
This project demonstrates strong understanding of:
1. File handling in C
2. Structs, pointers, and dynamic memory
3. Image representation and pixel-level computation
4. Modular program design
5. Command-line user interaction

The system is lightweight, extensible, and can serve as a foundation for more advanced features such as brightness/contrast control, blur filters, histogram operations, or a GUI version using Raylib.
