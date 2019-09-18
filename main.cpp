#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>

using namespace std;
using namespace cv;

const string WindowName = "LP1 Final Project";

static Scalar randomColor(RNG& rng)
{
        int icolor = (unsigned)rng;
        return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

class CascadeDetectorAdapter : public DetectionBasedTracker::IDetector
{
public:
CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector) :
        IDetector(),
        Detector(detector)
{
        CV_Assert(detector);
}

void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
{
        Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
}

virtual ~CascadeDetectorAdapter() CV_OVERRIDE
{
}

private:
CascadeDetectorAdapter();
cv::Ptr<cv::CascadeClassifier> Detector;
};

int main(int, char** )
{
        namedWindow(WindowName);

        VideoCapture VideoStream(0);
        RNG rng(0xFFFFFFFF);

        if (!VideoStream.isOpened())
        {
                printf("Error: Cannot open video stream from camera\n");
                return 1;
        }

        std::string cascadeFrontalfilename = samples::findFile("data/lbpcascades/lbpcascade_frontalface.xml");
        cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
        cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
        if ( cascade->empty() )
        {
                printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
                return 2;
        }

        cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
        cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
        if ( cascade->empty() )
        {
                printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
                return 2;
        }

        DetectionBasedTracker::Parameters params;
        DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

        if (!Detector.run())
        {
                printf("Error: Detector initialization failed\n");
                return 2;
        }

        Mat ReferenceFrame;
        Mat GrayFrame;
        vector<Rect> Faces;

        do
        {
                VideoStream >> ReferenceFrame;
                cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
                Detector.process(GrayFrame);
                Detector.getObjects(Faces);

                for (size_t i = 0; i < Faces.size(); i++)
                {
                        Rect r = Faces[i];
                        rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
                        printf("xy face = %d x %d\n", r.x, r.y);

                        Point org;
                        org.x = ReferenceFrame.cols - 1250;
                        org.y = ReferenceFrame.rows - 650;

                        string text = "X = " + to_string(r.x);

                        putText(ReferenceFrame, text, org, 0,
                                25*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                        org.x = ReferenceFrame.cols - 1250;
                        org.y = ReferenceFrame.rows - 600;

                        text = "Y = " + to_string(r.y);

                        putText(ReferenceFrame, text, org, 0,
                                25*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                }

                line(ReferenceFrame, Point(ReferenceFrame.cols / 4, 0),
                     Point(ReferenceFrame.cols / 4, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 2, 0),
                     Point((ReferenceFrame.cols / 4) * 2, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 3, 0),
                     Point((ReferenceFrame.cols / 4) * 3, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                Point org;
                org.x = (ReferenceFrame.cols / 4) + 100;
                org.y = ReferenceFrame.rows - 620;

                Point recA, recB;

                // Green rectangle
                recA.x = (ReferenceFrame.cols - ReferenceFrame.cols) + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 0), MAX(100, 1), LINE_AA);

                // Red rectangle
                recA.x = (ReferenceFrame.cols / 4) + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) * 2 - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(0, 0, 255), MAX(100, 1), LINE_AA);

                // Yellow rectangle
                recA.x = (ReferenceFrame.cols / 4) * 2 + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) * 3 - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 255), MAX(100, 1), LINE_AA);

                // Blue rectangle
                recA.x = (ReferenceFrame.cols / 4) * 3 + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) * 4 - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(255, 0, 0), MAX(100, 1), LINE_AA);

                putText(ReferenceFrame, "GENIUS", org, 3,
                        70*0.05+0.1, randomColor(rng), 10, LINE_AA);

                org.x = ReferenceFrame.cols - 200;
                org.y = ReferenceFrame.rows - 680;

                putText(ReferenceFrame, "Score: ", org, 0,
                        25*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                imshow(WindowName, ReferenceFrame);

        } while (waitKey(30) < 0);

        Detector.stop();

        return 0;
}
