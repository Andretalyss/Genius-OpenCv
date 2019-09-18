#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>

#include "headers/CascadeDetectorAdapter.hpp"
#include "util.cpp"

using namespace std;
using namespace cv;

const string WindowName = "LP1 Final Project";

int main(int argc, char** argv)
{
        namedWindow(WindowName);

        VideoCapture VideoStream(0);
        RNG rng(0xFFFFFFFF);

        VideoStream.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        VideoStream.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

        if (!VideoStream.isOpened())
        {
                printf("Error: Cannot open video stream from camera\n");
                return 1;

        } // endi f

        string cascadeFrontalfilename = samples::findFile("data/lbpcascades/lbpcascade_frontalface.xml");
        Ptr<CascadeClassifier> cascade = makePtr<CascadeClassifier>(cascadeFrontalfilename);
        Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
        if ( cascade->empty() )
        {
                printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
                return 2;

        } // end if

        cascade = makePtr<CascadeClassifier>(cascadeFrontalfilename);
        Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
        if ( cascade->empty() )
        {
                printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
                return 2;

        } // end if

        DetectionBasedTracker::Parameters params;
        DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

        if (!Detector.run())
        {
                printf("Error: Detector initialization failed\n");
                return 2;

        } // end if

        Mat ReferenceFrame;
        Mat GrayFrame;
        vector<Rect> Faces;

        do
        {
                VideoStream >> ReferenceFrame;
                cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
                Detector.process(GrayFrame);
                Detector.getObjects(Faces);

                string position;

                for (size_t i = 0; i < Faces.size(); i++)
                {
                        //Rect r = Faces[i];
                        rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
                        //printf("xy face = %d x %d\n", r.x, r.y);

                } // end for

                flip(ReferenceFrame, ReferenceFrame, 1);

                for (size_t i = 0; i < Faces.size(); i++)
                {
                        Rect r = Faces[i];

                        Point pos;
                        pos.x = ReferenceFrame.cols - 1250;
                        pos.y = ReferenceFrame.rows - 650;

                        string text = "X = " + to_string(r.x);

                        putText(ReferenceFrame, text, pos, 0,
                                25*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                        pos.x = ReferenceFrame.cols - 1250;
                        pos.y = ReferenceFrame.rows - 600;

                        text = "Y = " + to_string(r.y);

                        putText(ReferenceFrame, text, pos, 0,
                                25*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                        int x_pos = r.x;

                        if(x_pos > 950) position = "Green";

                        if(x_pos > 640 && x_pos < 700) position = "Red";

                        if(x_pos > 325 && x_pos < 370) position = "Yellow";

                        if(x_pos < 60) position = "Blue";

                } // end for

                line(ReferenceFrame, Point(ReferenceFrame.cols / 4, 0),
                     Point(ReferenceFrame.cols / 4, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 2, 0),
                     Point((ReferenceFrame.cols / 4) * 2, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 3, 0),
                     Point((ReferenceFrame.cols / 4) * 3, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                Point recA, recB;

                // Green rectangle
                recA.x = (ReferenceFrame.cols - ReferenceFrame.cols) + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 0), MAX(80, 0), LINE_AA);

                // Red rectangle
                recA.x = (ReferenceFrame.cols / 4) + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) * 2 - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(0, 0, 255), MAX(80, 0), LINE_AA);

                // Yellow rectangle
                recA.x = (ReferenceFrame.cols / 4) * 2 + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) * 3 - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 255), MAX(80, 0), LINE_AA);

                // Blue rectangle
                recA.x = (ReferenceFrame.cols / 4) * 3 + 80;
                recA.y = ReferenceFrame.rows - 60;
                recB.x = (ReferenceFrame.cols / 4) * 4 - 80;
                recB.y = ReferenceFrame.rows - 60;

                rectangle(ReferenceFrame, recA, recB, Scalar(255, 0, 0), MAX(80, 0), LINE_AA);

                Point pos;
                pos.x = (ReferenceFrame.cols / 4) + 100;
                pos.y = ReferenceFrame.rows - 620;

                putText(ReferenceFrame, "GENIUS", pos, 3,
                        70*0.05+0.1, Scalar(255, 255, 255), 10, LINE_AA);

                pos.x = (ReferenceFrame.cols / 4) * 3;
                pos.y = ReferenceFrame.rows - 680;

                putText(ReferenceFrame, "Score: ", pos, 0,
                        25*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                pos.x = (ReferenceFrame.cols / 4) * 3;
                pos.y = ReferenceFrame.rows - 600;

                putText(ReferenceFrame, "Position: ", pos, 0,
                        20*0.05+0.1, Scalar(255, 255, 255), 4, LINE_AA);

                pos.x = ((ReferenceFrame.cols / 4) * 3) + 160;
                pos.y = ReferenceFrame.rows - 600;

                if(position == "Green")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(0, 255, 0), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 4) + 100;
                        pos.y = ReferenceFrame.rows - 620;

                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                70*0.05+0.1, Scalar(0, 255, 0), 10, LINE_AA);

                } // end if

                else if(position == "Red")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(0, 0, 255), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 4) + 100;
                        pos.y = ReferenceFrame.rows - 620;

                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                70*0.05+0.1, Scalar(0, 0, 255), 10, LINE_AA);

                } // end else/if

                else if(position == "Yellow")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(0, 255, 255), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 4) + 100;
                        pos.y = ReferenceFrame.rows - 620;

                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                70*0.05+0.1, Scalar(0, 255, 255), 10, LINE_AA);

                } // end else/if

                else if(position == "Blue")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(255, 0, 0), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 4) + 100;
                        pos.y = ReferenceFrame.rows - 620;

                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                70*0.05+0.1, Scalar(255, 0, 0), 10, LINE_AA);

                } // end else/if

                imshow(WindowName, ReferenceFrame);

        } while (waitKey(30) < 0);

        Detector.stop();

        return 0;

} // end main
