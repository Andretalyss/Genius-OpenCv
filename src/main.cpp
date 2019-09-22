#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>
#include <vector>
#include <chrono>
#include "headers/CascadeDetectorAdapter.hpp"
#include "headers/Genius.hpp"

using namespace std;
using namespace cv;

const string WindowName = "LP1 Final Project";

int main(int argc, char** argv)
{
        Genius game;

        vector<int> sequencia;
        vector<int> captured;

        int start_delay = 2000;
        int showing_delay = 1500;
        int capture_delay = 20;

        srand(time(NULL));

        cout << "GENIUS GAME" << endl;
        cout << "--------------------------" << endl << endl;

        cout << "Nome do Jogador: ";

        string name;
        setbuf(stdin, 0);
        getline(cin, name);

        game.setName(name);

        int tamSequencia = 0;
        int cont[5] = {0, 0, 0, 0, 0};
        double time_taken[5] = {0, 0, 0, 0, 0};
        bool res_time = false;
        bool g_start = false;
        bool showing = false;
        bool capturing = false;
        bool next_round = true;
        bool game_over = false;
        double alpha[2] = {0.7, 0.9};
        string score;

        namedWindow(WindowName);

        VideoCapture VideoStream(0);

        VideoStream.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        VideoStream.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

        if (!VideoStream.isOpened())
        {
                printf("Error: Cannot open video stream from camera\n");
                return 1;

        } // end if

        string cascadeFrontalfilename = samples::findFile("data/lbpcascades/lbpcascade_frontalface.xml");
        Ptr<CascadeClassifier> cascade = makePtr<CascadeClassifier>(cascadeFrontalfilename);
        Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);

        if (cascade->empty())
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
        Mat ReferenceFrameCopy;
        Mat GrayFrame;
        vector<Rect> Faces;

        do
        {
                VideoStream >> ReferenceFrame;
                cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
                Detector.process(GrayFrame);
                Detector.getObjects(Faces);

                if(next_round)
                {
                        game.setSequencia();
                        sequencia = game.getSequencia();
                        game.setPont();
                        score = "score: " + to_string(game.getPont());
                        next_round = false;
                        g_start = false;
                        showing = false;
                        capturing = false;
                        tamSequencia = 0;
                        captured.clear();

                        cout << "Sequencia: ";

                        for(unsigned long int i = 0; i < sequencia.size(); i++)
                        {
                                cout << sequencia[i] << " ";

                        } // end for

                        cout << endl;

                } // end if

                for (size_t i = 0; i < Faces.size(); i++)
                {
                        //Rect r = Faces[i];
                        rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
                        //printf("xy face = %d x %d\n", r.x, r.y);

                } // end for

                flip(ReferenceFrame, ReferenceFrame, 1);

                line(ReferenceFrame, Point(ReferenceFrame.cols / 4, 0),
                     Point(ReferenceFrame.cols / 4, ReferenceFrame.rows - 1),
                     Scalar(255, 255, 255));

                line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 2, 0),
                     Point((ReferenceFrame.cols / 4) * 2, ReferenceFrame.rows - 1),
                     Scalar(255, 255, 255));

                line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 3, 0),
                     Point((ReferenceFrame.cols / 4) * 3, ReferenceFrame.rows - 1),
                     Scalar(255, 255, 255));

                Point recA, recB;

                ReferenceFrame.copyTo(ReferenceFrameCopy);

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

                addWeighted(ReferenceFrameCopy, alpha[0], ReferenceFrame, 1 - alpha[0], 0, ReferenceFrame);

                Point pos;

                pos.x = (ReferenceFrame.cols / 4) * 3;
                pos.y = (ReferenceFrame.rows / 10) * 1;

                putText(ReferenceFrame, score, pos, 0,
                        20*0.05+0.1, Scalar(255, 255, 255), 2, LINE_AA);

                pos.x = ((ReferenceFrame.cols / 4) * 3) + 160;
                pos.y = ReferenceFrame.rows - 600;

                if(capturing == false) showing = true;
                else if (capturing == true) showing = false;

                if(showing)
                {
                        if(!g_start)
                        {
                                auto start = chrono::high_resolution_clock::now();

                                if(time_taken[0] > start_delay)
                                {
                                        g_start = true;
                                        time_taken[0] = 0.0;

                                } // end if

                                auto end = chrono::high_resolution_clock::now();

                                time_taken[0] += chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                        } // end if

                        if(g_start)
                        {
                                if(sequencia[tamSequencia] == 1)
                                {
                                        pos.x = (ReferenceFrame.cols / 10) * 4;
                                        pos.y = (ReferenceFrame.rows / 10) * 1;

                                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                                40*0.05+0.1, Scalar(0, 255, 0), 5, LINE_AA);

                                        // Green rectangle
                                        recA.x = (ReferenceFrame.cols - ReferenceFrame.cols) + 80;
                                        recA.y = ReferenceFrame.rows - 60;
                                        recB.x = (ReferenceFrame.cols / 4) - 80;
                                        recB.y = ReferenceFrame.rows - 60;

                                        rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 0), MAX(80, 0), LINE_AA);

                                } // end else/if

                                else if(sequencia[tamSequencia] == 2)
                                {
                                        pos.x = (ReferenceFrame.cols / 10) * 4;
                                        pos.y = (ReferenceFrame.rows / 10) * 1;

                                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                                40*0.05+0.1, Scalar(0, 0, 255), 5, LINE_AA);

                                        // Red rectangle
                                        recA.x = (ReferenceFrame.cols / 4) + 80;
                                        recA.y = ReferenceFrame.rows - 60;
                                        recB.x = (ReferenceFrame.cols / 4) * 2 - 80;
                                        recB.y = ReferenceFrame.rows - 60;

                                        rectangle(ReferenceFrame, recA, recB, Scalar(0, 0, 255), MAX(80, 0), LINE_AA);

                                } // end else/if

                                else if(sequencia[tamSequencia] == 3)
                                {
                                        pos.x = (ReferenceFrame.cols / 10) * 4;
                                        pos.y = (ReferenceFrame.rows / 10) * 1;

                                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                                40*0.05+0.1, Scalar(0, 255, 255), 5, LINE_AA);

                                        // Yellow rectangle
                                        recA.x = (ReferenceFrame.cols / 4) * 2 + 80;
                                        recA.y = ReferenceFrame.rows - 60;
                                        recB.x = (ReferenceFrame.cols / 4) * 3 - 80;
                                        recB.y = ReferenceFrame.rows - 60;

                                        rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 255), MAX(80, 0), LINE_AA);

                                } // end else/if

                                if(sequencia[tamSequencia] == 4)
                                {
                                        pos.x = (ReferenceFrame.cols / 10) * 4;
                                        pos.y = (ReferenceFrame.rows / 10) * 1;

                                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                                40*0.05+0.1, Scalar(255, 0, 0), 5, LINE_AA);

                                        // Blue rectangle
                                        recA.x = (ReferenceFrame.cols / 4) * 3 + 80;
                                        recA.y = ReferenceFrame.rows - 60;
                                        recB.x = (ReferenceFrame.cols / 4) * 4 - 80;
                                        recB.y = ReferenceFrame.rows - 60;

                                        rectangle(ReferenceFrame, recA, recB, Scalar(255, 0, 0), MAX(80, 0), LINE_AA);

                                } // end if

                        } // end if

                        if(showing)
                        {
                                auto start = chrono::high_resolution_clock::now();

                                if(g_start && time_taken[0] > showing_delay)
                                {
                                        tamSequencia++;
                                        res_time = true;

                                        pos.x = (ReferenceFrame.cols / 10) * 4;
                                        pos.y = (ReferenceFrame.rows / 10) * 1;

                                        putText(ReferenceFrame, "GENIUS", pos, 3,
                                                40*0.05+0.1, Scalar(255, 255, 255), 5, LINE_AA);

                                } // end if

                                auto end = chrono::high_resolution_clock::now();

                                time_taken[0] += chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                                if(res_time)
                                {
                                        time_taken[0] = 0.0;
                                        res_time = false;

                                } // end if

                        } // end if

                        if(tamSequencia == game.getNumeroSequencia())
                        {
                                capturing = true;
                                showing = false;

                        } // end if

                } // end if

                if(capturing)
                {
                        for (size_t i = 0; i < Faces.size(); i++)
                        {
                                Rect r = Faces[i];

                                Point pos;
                                pos.x = 0;
                                pos.y = (ReferenceFrame.rows / 10) * 1;

                                string text = "X = " + to_string(r.x);

                                putText(ReferenceFrame, text, pos, 0,
                                        20*0.05+0.1, Scalar(255, 255, 255), 2, LINE_AA);

                                pos.x = 0;
                                pos.y = (ReferenceFrame.rows / 10) * 2;

                                text = "Y = " + to_string(r.y);

                                putText(ReferenceFrame, text, pos, 0,
                                        20*0.05+0.1, Scalar(255, 255, 255), 2, LINE_AA);

                                int x_pos = r.x;

                                // detection cordinates

                                if(x_pos > 950)
                                {
                                        cont[1]++;
                                        cont[2] = 0;
                                        cont[3] = 0;
                                        cont[4] = 0;

                                        if(cont[1] > capture_delay)
                                        {
                                                cont[1] = 0;
                                                captured.push_back(1);

                                                // Green rectangle
                                                recA.x = (ReferenceFrame.cols - ReferenceFrame.cols) + 80;
                                                recA.y = ReferenceFrame.rows - 60;
                                                recB.x = (ReferenceFrame.cols / 4) - 80;
                                                recB.y = ReferenceFrame.rows - 60;

                                                rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 0), MAX(80, 0), LINE_AA);

                                        } // end if

                                } // end if

                                else if(x_pos > 630 && x_pos < 820)
                                {
                                        cont[1] = 0;
                                        cont[2]++;
                                        cont[3] = 0;
                                        cont[4] = 0;

                                        if(cont[2] > capture_delay)
                                        {
                                                cont[2] = 0;
                                                captured.push_back(2);

                                                // Red rectangle
                                                recA.x = (ReferenceFrame.cols / 4) + 80;
                                                recA.y = ReferenceFrame.rows - 60;
                                                recB.x = (ReferenceFrame.cols / 4) * 2 - 80;
                                                recB.y = ReferenceFrame.rows - 60;

                                                rectangle(ReferenceFrame, recA, recB, Scalar(0, 0, 255), MAX(80, 0), LINE_AA);

                                        } // end if

                                } // end if

                                else if(x_pos > 320 && x_pos < 515)
                                {
                                        cont[1] = 0;
                                        cont[2] = 0;
                                        cont[3]++;
                                        cont[4] = 0;

                                        if(cont[3] > capture_delay)
                                        {
                                                cont[3] = 0;
                                                captured.push_back(3);

                                                // Yellow rectangle
                                                recA.x = (ReferenceFrame.cols / 4) * 2 + 80;
                                                recA.y = ReferenceFrame.rows - 60;
                                                recB.x = (ReferenceFrame.cols / 4) * 3 - 80;
                                                recB.y = ReferenceFrame.rows - 60;

                                                rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 255), MAX(80, 0), LINE_AA);

                                        } // end if

                                } // end if

                                else if(x_pos < 180)
                                {
                                        cont[1] = 0;
                                        cont[2] = 0;
                                        cont[3] = 0;
                                        cont[4]++;

                                        if(cont[4] > capture_delay)
                                        {
                                                cont[4] = 0;
                                                captured.push_back(4);

                                                // Blue rectangle
                                                recA.x = (ReferenceFrame.cols / 4) * 3 + 80;
                                                recA.y = ReferenceFrame.rows - 60;
                                                recB.x = (ReferenceFrame.cols / 4) * 4 - 80;
                                                recB.y = ReferenceFrame.rows - 60;

                                                rectangle(ReferenceFrame, recA, recB, Scalar(255, 0, 0), MAX(80, 0), LINE_AA);

                                        } // end if

                                } // end if

                                for(unsigned long int i = 0; i < captured.size(); i++)
                                {
                                        if(!captured.empty() && captured[i] != (game.getSequencia())[i]) game_over = true;

                                } // end for

                                if(captured.size() == game.getNumeroSequencia())
                                {
                                        capturing = false;

                                        if(!game_over) next_round = true;

                                } // end if

                        } // end for

                } // end if

                cout << "Captured: ";

                for(unsigned long int i = 0; i < captured.size(); i++)
                {
                        cout << captured[i] << " ";

                } // end for

                cout << endl;

                if(game_over) cout << "----- game over -----" << endl;

                imshow(WindowName, ReferenceFrame);

        } while (waitKey(30) < 0);

        Detector.stop();

        return 0;

} // end main
