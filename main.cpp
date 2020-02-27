#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <Magick++.h>
#include <iostream>
#include <cstdio>

using namespace Magick;
using namespace std;

int main(int argc, char *argv[])
{
    //If program is launched without entry file specified
    if (argc < 2){
        printf("Usage: %s <Input file>", argv[0]);
        return 1;
    }

    //will be the path of the entry file
    string input = argv[1];

    //initializing lib magick
    InitializeMagick("");

    //opening the file with lib Magick
    printf("Opening...\n");
    Image img(input.c_str());

    //declaration & defining the output
    string output = input.substr(0,input.size()-4) + ".tif";

    printf("write...\n");
    img.write(output.c_str());

    //declaration of the array which will contains pdf text
    char *outText;

    //creating instance of the api
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with French, without specifying tessdata path
    if (api->Init(NULL, "fra")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix *image = pixRead(output.c_str());
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    //Remove tif converted image
    remove(output.c_str());

    // Destroy used object and release memory
    api->End();
    delete api;
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
