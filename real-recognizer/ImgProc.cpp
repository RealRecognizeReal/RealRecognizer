//
//  ImgProc.cpp
//  real-recognizer
//
//  Created by 김동이 on 2016. 10. 4..
//  Copyright © 2016년 김동이. All rights reserved.
//



#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "ImgProc.hpp"

tesseract::TessBaseAPI api;

void show(Mat img, const string &title)
{
    imshow(title.c_str(), img);
    waitKey(0);
}

void configTesseract()
{
    api.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    api.SetVariable("tessedit_char_whitelist", "1234567890abcdefghijklmniopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-=!");
    api.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
}


//
Mat simplify(Mat img);

//
Mat getMinimumBox(Mat img, float angle = 0);

//splits all elements horizontally
vector<Mat> splitHorizon(Mat img);

//
rr::OCRResult recognizeElement(Mat element);

namespace rr
{
    OCRResult runOCR(const string& filename)
    {
        configTesseract();
       
        //load image and check file errors
        Mat img = cv::imread(filename.c_str(), 0);
        if(img.empty())
        {
            OCRResult result;
            result.message = "failed to load image";
            return result;
        }
        
        //simplify image
        img = simplify(img);
        img = getMinimumBox(img);

        const float mina = -1.0f;
        const float maxa = 1.0f;
        const float unit = 0.2f;
        for(float angle = mina; angle <= maxa; angle += unit)
        {
            Mat panel = getMinimumBox(img,angle);
            auto elements = splitHorizon(panel);
            
            OCRResult result;
            for(int i = 0 ; i < elements.size(); i++)
            {
                OCRResult e_res = recognizeElement(elements[i]);
                if(e_res.success == false)
                {
                    result.success = false;
                    break;
                }
                result.appendLatex(e_res.latex);
            }
            
            if(result.success == false)
                continue;
            
            return result;
        }
        
        return OCRResult();
    }
}



Mat simplify(Mat img)
{
    static const int SCALE_LEVEL = 4;
    
    //scale up image
    int rows = img.rows * SCALE_LEVEL;
    int cols = img.cols * SCALE_LEVEL;
    resize(img, img, Size( cols, rows ));
    
    //give blurring to
    cv::Size b_size(3,3);
    cv::GaussianBlur(img,img,b_size,0);
    adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);
    cv::bitwise_not(img, img);

    return img;
}

Mat getMinimumBox(Mat img, float angle)
{
    int size = max(img.cols, img.rows) * 2;
    int sr = (size - img.rows ) / 2;
    int sc = (size - img.cols ) / 2;
    
    if(angle != 0.0f)
    {
        Mat scaled = Mat::zeros(size, size, CV_8UC1);
        img.copyTo(scaled.rowRange(sr, sr+img.rows).colRange(sc, sc+img.cols));
        
        Mat rotated;
        Mat rot_mat = getRotationMatrix2D( Point2f(size/2, size/2), angle, 1);
        cv::warpAffine(scaled, rotated, rot_mat, scaled.size(), cv::INTER_CUBIC);
        img = rotated;
    }

    int minx = 1e9;
    int maxx = -1e9;
    int miny = 1e9;
    int maxy = -1e9;
    
    int rows = img.rows;
    int cols = img.cols;
    
    for(int x = 0 ; x < cols; x++)
    {
        for(int y = 0 ; y < rows; y++)
        {
            if(img.at<uchar>(y, x) != 0)
            {
                minx = min(minx, x);
                miny = min(miny, y);
                maxx = max(maxx, x);
                maxy = max(maxy, y);
            }
        }
    }
    //generate image panel square
    
    rows = maxy - miny + 1;
    cols = maxx - minx + 1;
    
    return img(Rect(minx, miny, cols, rows));
}

vector<Mat> splitHorizon(Mat img)
{
    static const int MINIMUM_WIDTH = 5;
    
    vector<Mat> ret;
    
    bool isOn = false;
    int left = 0;
    int right = 0 ;
    for(int x = 0 ; x < img.cols; x++)
    {
        bool isWhite = false;
        for(int y = 0 ; y < img.rows; y++)
        {
            if( img.at<uchar>(y, x) != 0 )
            {
                isWhite = true;
                break;
            }
        }
        
        if(isOn == false && isWhite == true)
        {
            left = x;
            right = x;
            isOn = true;
        }
        if(isOn == true && isWhite == true)
        {
            right = x;
        }
        if(isOn == true && isWhite == false)
        {
            int width = right - left + 1;
            if(width >= MINIMUM_WIDTH)
                ret.push_back(img(Rect(left, 0, right-left+1, img.rows)));
            isOn = false;
        }
    }
    return ret;
}

rr::OCRResult recognizeElement(Mat element)
{
    rr::OCRResult result;
    api.SetImage((uchar*)element.data, element.cols, element.rows, 1, element.cols);
    int code = api.Recognize(0);
//    String utf8 = api.GetUTF8Text();
    return result;
}
