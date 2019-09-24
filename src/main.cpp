/*


        Project for Object Oriented Programming Language Laboratory;
        Game Genius with Opencv4;

        Authors: André Talysson [Computer engineering in formation at UFPB];
        Carlos Henrique [Computer engineering in formation at UFPB];
        Rafael Farias [Computer engineering in formation at UFPB];


*/

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <vector>
#include <chrono>
#include <thread>
#include "headers/CascadeDetectorAdapter.hpp"
#include "headers/Genius.hpp"

using namespace std;
using namespace cv;

const string WindowName = "LP1 Final Project";

int main(int argc, char** argv)
{
        system ("tput clear");

        bool game_continue = true;

        while(game_continue)
        {
                Genius game;

                vector<int> sequencia;
                vector<int> captured;

                int start_delay = 2000;
                int showing_delay = 1500;
                int capture_delay = 25;

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
                bool somGameOver = true;
                double alpha[2] = {0.7, 0.9};
                string score;
                int pont;

                namedWindow(WindowName);

                VideoCapture VideoStream(0);

                //VideoStream.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
                //VideoStream.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

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

                system ("tput clear");

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
                                pont = game.getPont();
                                next_round = false;
                                g_start = false;
                                showing = false;
                                capturing = false;
                                tamSequencia = 0;
                                captured.clear();

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

                        pos.x = (ReferenceFrame.cols / 20) * 17;
                        pos.y = (ReferenceFrame.rows / 40) * 2;

                        putText(ReferenceFrame, score, pos, 0,
                                20*0.05+0.1, Scalar(255, 255, 255), 2, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 80) * 1;
                        pos.y = (ReferenceFrame.rows / 40) * 2;

                        putText(ReferenceFrame, "Voltar: ESC", pos, 3,
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

                                        if(showing && g_start && time_taken[0] > showing_delay)
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

                        if(capturing && !game_over)
                        {
                                for (size_t i = 0; i < Faces.size(); i++)
                                {
                                        Rect r = Faces[i];

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

                                        for(unsigned long int i = 0; i < captured.size(); i++)
                                        {
                                                if(!captured.empty() && captured[i] != (game.getSequencia())[i]) {

                                                        game_over = true;
                                                }

                                        } // end for

                                        if(captured.size() == game.getNumeroSequencia())
                                        {
                                                capturing = false;

                                                if(!game_over) {
                                                        next_round = true;
                                                        system("canberra-gtk-play -f sound/Win.wav &");
                                                }


                                        } // end if

                                } // end for

                        } // end if

                        if(game_over) {

                                pos.x = (ReferenceFrame.cols / 80) * 27;
                                pos.y = (ReferenceFrame.rows / 10) * 1;

                                putText(ReferenceFrame, "GAME OVER", pos, 3,
                                        40*0.05+0.1, Scalar(0, 0, 255), 5, LINE_AA);

                                if(somGameOver) {
                                        system("canberra-gtk-play -f sound/Gameover.wav &");
                                        somGameOver = false;
                                }
                        }

                        imshow(WindowName, ReferenceFrame);

                } while (waitKey(30) < 0);

                Detector.stop();
                game.Salvar(name,pont);

                while(true)
                {
                        system ("tput clear");

                        cout << "[1] Rank | [2] Jogar novamente | [3] Sair" << endl;
                        int opt;
                        cin >> opt;

                        if(opt==1)
                        {
                                system ("tput clear");

                                game.Ler();

                                cout << "----- RANK -----" << endl << endl;

                                for(unsigned long int i = 0; i < (game.rank).size(); i++)
                                {
                                        cout << "- " << i + 1 << ":" << endl;
                                        cout << "Nome: " << game.getRankName(i) << endl;
                                        cout << "Score: " << game.getRankScore(i) << endl << endl;

                                } // end for

                                cout << "---------------" << endl << endl;

                                cout << "ENTER para continuar.";

                                setbuf(stdin, 0);
                                getchar();

                        } // end if

                        if(opt == 2)
                        {
                                game_continue = true;
                                break;

                        } // end if

                        if(opt == 3)
                        {
                                game_continue = false;
                                break;

                        } // end if

                } // end while

        } // end while

        return 0;

} // end main
