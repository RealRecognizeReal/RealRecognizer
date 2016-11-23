//
//  main.cpp
//  real-recognizer
//
//  Created by 김동이 on 2016. 10. 3..
//  Copyright © 2016년 김동이. All rights reserved.
//


#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>


#include "ImgProc.hpp"

using namespace std;
//
//
//string runOCR(const string& filename);
//Mat simplify(Mat mat);
//
//
//
//void extractContours(Mat& image,vector< vector<Point> > contours_poly){
//    
//    
//    
//    //Sort contorus by x value going from left to right
//    sort(contours_poly.begin(),contours_poly.end(), [](vector<Point> &c1, vector<Point> &c2){
//         return boundingRect( Mat(c1)).x<boundingRect( Mat(c2)).x;
//    });
//    
//    
//    //Loop through all contours to extract
//    for( int i = 0; i< contours_poly.size(); i++ ){
//        
//        Rect r = boundingRect( Mat(contours_poly[i]) );
//        
//        
//        Mat mask = Mat::zeros(image.size(), CV_8UC1);
//        //Draw mask onto image
//        drawContours(mask, contours_poly, i, Scalar(255), CV_FILLED);
//        
//        //Check for equal sign (2 dashes on top of each other) and merge
//        if(i+1<contours_poly.size()){
//            Rect r2 = boundingRect( Mat(contours_poly[i+1]) );
//            if(abs(r2.x-r.x)<20){
//                //Draw mask onto image
//                drawContours(mask, contours_poly, i+1, Scalar(255), CV_FILLED);
//                i++;
//                int minX = min(r.x,r2.x);
//                int minY = min(r.y,r2.y);
//                int maxX = max(r.x+r.width,r2.x+r2.width);
//                int maxY = max(r.y+r.height,r2.y+r2.height);
//                r = Rect(minX,minY,maxX - minX,maxY-minY);
//                
//            }
//        }
//        //Copy
//        Mat extractPic;
//        //Extract the character using the mask
//        image.copyTo(extractPic,mask);
//        Mat resizedPic = extractPic(r);
//        
//        cv::Mat image=resizedPic.clone();
//        
//        //Show image
//        imshow("image",image);
//        char ch  = waitKey(0);
//        stringstream searchMask;
//        searchMask<<i<<".jpg";
//        imwrite(searchMask.str(),resizedPic);
//        
//    }
//}
//
//void getContours(Mat &img)
//{
//    //Apply blur to smooth edges and use adapative thresholding
//    cv::Mat img2 = img.clone();
//
//    std::vector<cv::Point> points;
//    cv::Mat_<uchar>::iterator it = img.begin<uchar>();
//    cv::Mat_<uchar>::iterator end = img.end<uchar>();
//    for (; it != end; ++it)
//        if (*it)
//            points.push_back(it.pos());
//    
//    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
//    
//    double angle = box.angle;
//    if (angle < -45.)
//        angle += 90.;
//    
//    cv::Point2f vertices[4];
//    box.points(vertices);
//    for(int i = 0; i < 4; ++i)
//        cv::line(img, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), 1, CV_AA);
//    
//    
//    
//    cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);
//    
//    cv::Mat rotated;
//    cv::warpAffine(img2, rotated, rot_mat, img.size(), cv::INTER_CUBIC);
//    
//    
//    
//    cv::Size box_size = box.size;
//    if (box.angle < -45.)
//        std::swap(box_size.width, box_size.height);
//    cv::Mat cropped;
//    
//    cv::getRectSubPix(rotated, box_size, box.center, cropped);
//    cv::imshow("Cropped", cropped);
//    imwrite("example5.jpg",cropped);
//    
//    Mat cropped2=cropped.clone();
//    cvtColor(cropped2,cropped2,CV_GRAY2RGB);
//    
//    Mat cropped3 = cropped.clone();
//    cvtColor(cropped3,cropped3,CV_GRAY2RGB);
//    
//    vector<vector<Point> > contours;
//    vector<Vec4i> hierarchy;
//    
//    /// Find contours
//    cv:: findContours( cropped, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS, Point(0, 0) );
//    
//    
//    
//    /// Approximate contours to polygons + get bounding rects and circles
//    vector<vector<Point> > contours_poly( contours.size() );
//    vector<Rect> boundRect( contours.size() );
//    vector<Point2f>center( contours.size() );
//    vector<float>radius( contours.size() );
//    
//    
//    //Get poly contours
//    for( int i = 0; i < contours.size(); i++ )
//    {
//        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
//    }
//    
//    
//    //Get only important contours, merge contours that are within another
//    vector<vector<Point> > validContours;
//    for (int i=0;i<contours_poly.size();i++){
//        
//        Rect r = boundingRect(Mat(contours_poly[i]));
//        if(r.area()<100)continue;
//        bool inside = false;
//        for(int j=0;j<contours_poly.size();j++){
//            if(j==i)continue;
//            
//            Rect r2 = boundingRect(Mat(contours_poly[j]));
//            if(r2.area()<100||r2.area()<r.area())continue;
//            if(r.x>r2.x&&r.x+r.width<r2.x+r2.width&&
//               r.y>r2.y&&r.y+r.height<r2.y+r2.height){
//                
//                inside = true;
//            }
//        }
//        if(inside)continue;
//        validContours.push_back(contours_poly[i]);
//    }
//    
//    
//    //Get bounding rects
//    for(int i=0;i<validContours.size();i++){
//        boundRect[i] = boundingRect( Mat(validContours[i]) );
//    }
//    
//    
//    //Display
//    Scalar color = Scalar(0,255,0);
//    for( int i = 0; i< validContours.size(); i++ )
//    {
//        if(boundRect[i].area()<100)continue;
//        drawContours( cropped2, validContours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//        rectangle( cropped2, boundRect[i].tl(), boundRect[i].br(),color, 2, 8, 0 );
//    }
//    
//    //imwrite("example6.jpg",cropped2);
//    imshow("Contours",cropped2);
//    cv::waitKey(0);
////    extractContours(cropped3,validContours);
//    
//    
//}
//
//string recognize(Mat img)
//{
//    getContours(img);
//    return "";
//}
//
//void run(const char * filename)
//{
//    
//    //file check
//
//    //scale up for segmentation
//   
//    //rotate image with various angle
////    const double lower_angle = -10.;
////    const double upper_angle = 10.;
////    const double unit_angle = 1;
////    for(double angle = lower_angle; angle <= upper_angle; angle+= unit_angle)
////    {
////        cv::Mat rot_mat = cv::getRotationMatrix2D(Point(psize, psize), angle, 1);
////        cv::Mat rotated;
////        cv::warpAffine(panel, rotated, rot_mat, panel.size());
////        string latex = recognize(rotated);
////    }
//}
//
//
//
//vector<Mat> splitHorizon(Mat img)
//{
//    vector<Mat> ret;
//    
//    bool isOn = false;
//    int left = 0;
//    int right = 0 ;
//    for(int x = 0 ; x < img.cols; x++)
//    {
//        bool isWhite = false;
//        for(int y = 0 ; y < img.rows; y++)
//        {
//            if( img.at<uchar>(y, x) != 0 )
//            {
//                isWhite = true;
//                break;
//            }
//        }
//        
//        if(isOn == false && isWhite == true)
//        {
//            left = x;
//            right = x;
//            isOn = true;
//        }
//        if(isOn == true && isWhite == true)
//        {
//            right = x;
//        }
//        if(isOn == true && isWhite == false)
//        {
//            ret.push_back(img(Rect(left, 0, right-left+1, img.rows)));
//            isOn = false;
//        }
//    }
//    return ret;
//}
//string runOCR(const string & filename)
//{
//    cv::Mat img = cv::imread(filename.c_str(), 0);
//    if(img.empty())
//    {
//        printf(" - Couldn't read image\n");
//        exit(-1);
//    }
//    
//    img = simplify(img);
//    
////    imshow("Simplifed",img);
////    cv::waitKey(0);
//    
//    auto vh = splitHorizon(img);
//    for(int i = 0 ; i < vh.size(); i++)
//    {
//        imshow("vh", vh[i]);
//        cv::waitKey(0);
//    }
//    
//    
//    
//    return "";
//}
//
//Mat simplify(Mat img)
//{
//    static const int SCALE_LEVEL = 4;
//    
//    int rows = img.rows * SCALE_LEVEL;
//    int cols = img.cols * SCALE_LEVEL;
//    resize(img, img, Size( cols, rows ));
//    
//    //give blurring to image
//    cv::Size b_size(3,3);
//    cv::GaussianBlur(img,img,b_size,0);
//    adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);
//    cv::bitwise_not(img, img);
//    
//    return img;
////    //
////    
////    imshow("Contours",img);
////    cv::waitKey(0);
////    
////    
////    int minx = 1e9;
////    int maxx = -1e9;
////    int miny = 1e9;
////    int maxy = -1e9;
////    for(int x = 0 ; x < cols; x++)
////    {
////        for(int y = 0 ; y < rows; y++)
////        {
////            if(img.at<uchar>(y, x) != 0)
////            {
////                minx = min(minx, x);
////                miny = min(miny, y);
////                maxx = max(maxx, x);
////                maxy = max(maxy, y);
////            }
////        }
////    }
////    
////    //generate image panel square
////    
////    rows = maxy - miny + 1;
////    cols = maxx - minx + 1;
////    int psize = max(rows, cols);
////    Mat panel;
////    resize(img, panel, Size(psize, psize));
////    for(int r = 0 ; r < psize; r++)
////        for(int c = 0 ;c < psize; c++)
////            panel.at<uchar>(r, c) = 0;
////    
////    //copy image into panel
////    int dr = (psize - rows)/2;
////    int dc = (psize - cols)/2;
////    img = img(Rect(minx, miny, cols, rows));
////    img.copyTo(panel.rowRange(dr, dr + rows).colRange(dc, dc + cols));
////    return panel;
//}

int main(int argc, const char * argv[])
{
    
    vector<string> files = {
        "/Users/waps12b/Documents/resource/example1.jpg",
        "/Users/waps12b/Documents/resource/example1.jpg"
    };
    
    for(int i = 0 ; i < files.size(); i++)
    {
        printf("[Case #%d] file : \"%s\"\n", i+1, files[i].c_str());
        rr::runOCR(files[i])
        ;
    }
    
    return 0;
}
