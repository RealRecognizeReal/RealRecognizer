//
//  ImgProc.hpp
//  real-recognizer
//
//  Created by 김동이 on 2016. 10. 4..
//  Copyright © 2016년 김동이. All rights reserved.
//

#ifndef ImgProc_hpp
#define ImgProc_hpp

#include <stdio.h>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml.hpp>


#include <tesseract/baseapi.h>


using namespace cv;
using namespace std;

namespace rr
{
    struct OCRResult
    {
        bool success;
        string latex;
        string message;
        OCRResult() : success(false), latex(""), message("")
        {}
        
        void setLatex(const string &utf8latex)
        {
            this->latex = utf8latex;
            this->success = true;
        }
        void appendLatex(const string &utf8latex)
        {
            this->latex += utf8latex;
            this->success = true;
        }
    };
    
    OCRResult runOCR(const string& filename);
}

#endif /* ImgProc_hpp */
