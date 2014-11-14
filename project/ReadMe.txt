We are using OpenCV, tesseract and leptonica libraries. So, all these libraries should be installed in order to run this project.

1.) Run Makefile
    We have tested it on Linux machine and it was loading as required. 
    If for some reason, it's not loading, then use following commands:
    To compile OpenCV code:
    g++ -ggdb `pkg-config --cflags opencv` -o `basename project.cpp .cpp` project.cpp `pkg-config --libs opencv`
    ./project

    To compile tesseract code:
    g++ tess.cpp -o tess \
  -I/usr/include/leptonica \
  -I/usr/local/include/tesseract \
  -llept -ltesseract
   ./tess

2.) Tess file doesn't require any user input. It is using an image from images folder. It is working for single character digit files. The resultant digit will be displayed.

3.) For OpenCV approach, we will be required to provide inputs
    First you will be asked to select the operation. Please select option 1, as transpose is working.
    Then you will be asked to enter the image name, enter numbers.png
    Then enter number of rows and cols (value less than 10)
4.) Resultant transpose of matrix will be displayed.
5.) Hit Ctrl + C to exit out of this program.

If you face any issue while loading or executing project, please email us on dlalwani@indiana.edu and vidjoshi@indiana.edu

