#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main()
{
    char *out;

    tesseract::TessBaseAPI *tessapi = new tesseract::TessBaseAPI();
    // Load dataset of English language
    if (tessapi->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    Pix *image = pixRead("images/4.png");

    tessapi->SetImage(image);
    
    out = tessapi->GetUTF8Text(); //store recognition result in char array
    printf("\n%s", out);

    tessapi->End();
    delete [] out;
    pixDestroy(&image);

    return 0;
}
