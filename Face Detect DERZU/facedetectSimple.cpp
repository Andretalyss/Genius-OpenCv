#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <exception>
#include <unistd.h>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale );

string cascadeName;
const char *title = "Genius";
Mat fruta;

int main( int argc, const char** argv )
{
        VideoCapture capture;
        Mat frame;
        CascadeClassifier cascade;
        double scale;

        fruta = cv::imread("laranja.png", IMREAD_UNCHANGED);

        if (fruta.empty())
                printf("Error opening file laranja.pn\n");
        cascadeName       = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
        scale = 1;

        if( !cascade.load( cascadeName ) )
        {
                cerr << "ERROR: Could not load classifier cascade:\n " << cascadeName << endl;
                return -1;
        }

        try {
                const char * device = "/dev/video0";
                if(!capture.open(device))
                        cout << "Capture from camera #0 didn't work" << endl;
        }
        catch(std::exception& e)
        {
                std::cout << " Excecao capturada open: " << e.what() << std::endl;
        }

        if( capture.isOpened() )
        {
                cout << "Video capturing has been started ..." << endl;
                namedWindow(title, cv::WINDOW_NORMAL);
                for(;;)
                {
                        try {
                                capture >> frame;
                        }
                        catch (cv::Exception& e)
                        {
                                std::cout << " Excecao2 capturada frame: " << e.what() << std::endl;
                                usleep(1000000);
                                continue;
                        }
                        catch (std::exception& e)
                        {
                                std::cout << " Excecao3 capturada frame: " << e.what() << std::endl;
                                usleep(1000000);
                                continue;
                        }

                        if( frame.empty() )
                                break;

                        detectAndDraw( frame, cascade, scale );

                        char c = (char)waitKey(10);
                        if( c == 27 || c == 'q' || c == 'Q' )
                                break;
                }
        }

        return 0;
}

/**
 * @brief Draws a transparent image over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawTransparency(Mat frame, Mat transp, int xPos, int yPos) {
        Mat mask;
        vector<Mat> layers;

        split(transp, layers); // seperate channels
        Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3]; // png's alpha channel used as mask
        merge(rgb, 3, transp); // put together the RGB channels, now transp insn't transparent
        transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}

void drawTransparency2(Mat frame, Mat transp, int xPos, int yPos) {
        Mat mask;
        vector<Mat> layers;

        split(transp, layers); // seperate channels
        Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3]; // png's alpha channel used as mask
        merge(rgb, 3, transp); // put together the RGB channels, now transp insn't transparent
        Mat roi1 = frame(Rect(xPos, yPos, transp.cols, transp.rows));
        Mat roi2 = roi1.clone();
        transp.copyTo(roi2.rowRange(0, transp.rows).colRange(0, transp.cols), mask);
        printf("%p, %p\n", roi1.data, roi2.data);
        double alpha = 0.2;
        addWeighted(roi2, alpha, roi1, 1.0 - alpha, 0.0, roi1);
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale)
{
        vector<Rect> faces;
        Scalar color = Scalar(255,0,0);
        Mat gray, smallImg;

        cvtColor( img, gray, COLOR_BGR2GRAY );
        double fx = 1 / scale;
        resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
        equalizeHist( smallImg, smallImg );

        cascade.detectMultiScale( smallImg, faces,
                                  1.3, 2, 0
                                  //|CASCADE_FIND_BIGGEST_OBJECT
                                  //|CASCADE_DO_ROUGH_SEARCH
                                  |CASCADE_SCALE_IMAGE,
                                  Size(40, 40) );

        for ( size_t i = 0; i < faces.size(); i++ )
        {
                Rect r = faces[i];
                Point center;

                printf("xy face = %d x %d\n", r.x, r.y);

                rectangle( img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                           Point(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                           color, 3, 8, 0);

                if(!fruta.empty()) drawTransparency2(img, fruta, r.x, r.y);

                //void drawTransparency(Mat frame, Mat transp, int xPos, int yPos)

        }

        imshow( title, img );
}
