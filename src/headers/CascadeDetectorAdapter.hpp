/*
 *   Author: Carlos Henrique Silva Correia de Araujo
 *   UFPB Computer engineering student
 *   https://github.com/ch94ca
 *
 */

#ifndef CASCADEDETECTORADAPTER_HPP
#define CASCADEDETECTORADAPTER_HPP

#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>

class CascadeDetectorAdapter : public cv::DetectionBasedTracker::IDetector
{

public:

CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector) :
        IDetector(),
        Detector(detector){

        CV_Assert(detector);

} // end CascadeDetectorAdapter

void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
{
        Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);

} // end detect

virtual ~CascadeDetectorAdapter() CV_OVERRIDE
{

} // end CascadeDetectorAdapter

private:

CascadeDetectorAdapter();

cv::Ptr<cv::CascadeClassifier> Detector;

}; // end CascadeDetectorAdapter

#endif
