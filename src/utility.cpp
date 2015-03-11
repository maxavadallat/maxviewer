
#include <QDebug>

#include "utility.h"


//==============================================================================
// Compare Images Pixel By Pixel
//==============================================================================
bool compareImagesByPixel(const QImage& aLeftImage, const QImage& aRightImage)
{
    // Check Sizes First
    if (aLeftImage.size() == aRightImage.size()) {
        // Go Thru Image Pixels
        for (int y=0; y<aLeftImage.height(); y++) {
            for (int x=0; x<aLeftImage.width(); x++) {
                // Check Pixels
                if (aLeftImage.pixel(x, y) != aRightImage.pixel(x, y)) {
                    return false;
                }
            }
        }

        return true;
    }

    return false;
}


