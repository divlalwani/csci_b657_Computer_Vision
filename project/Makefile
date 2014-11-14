project : project.cpp 
	g++ -ggdb `pkg-config --cflags opencv` -o `basename project.cpp .cpp` project.cpp `pkg-config --libs opencv`

tess : tess.cpp g++ tess.cpp -o tess \
  -I/usr/include/leptonica \
  -I/usr/local/include/tesseract \
  -llept -ltesseract
