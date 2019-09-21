#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <chrono>
#include <iostream>
#include <vector>
#include <ctime>
#include <thread>

#include <stdio.h>

#include "headers/Genius.hpp"
#include "headers/CascadeDetectorAdapter.hpp"
#include "util.cpp"

#define AZUL 0
#define VERMELHO 1
#define VERDE 2
#define AMARELO 3

using namespace std;
using namespace cv;

int i=0;
const string WindowName = "LP1 Final Project";


int main(int argc, char** argv)
{       

        namedWindow(WindowName,WINDOW_NORMAL);

        VideoCapture VideoStream(0);
        RNG rng(0xFFFFFFFF);

        //VideoStream.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        //VideoStream.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

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
        Genius *game = new Genius();

        do
        {      
                VideoStream >> ReferenceFrame;
                cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
                Detector.process(GrayFrame);
                Detector.getObjects(Faces);

                string position;

                for (size_t i = 0; i < Faces.size(); i++)
                {
                       // Rect r = Faces[i];
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
                        int y_pos = r.y;

                        // detection cordinates
                        if(x_pos > (ReferenceFrame.cols / 10) * 6 && y_pos < (ReferenceFrame.rows / 10) * 3) position = "Blue";

                        if(x_pos < (ReferenceFrame.cols / 10) * 4 && y_pos < (ReferenceFrame.rows / 10) * 3) position = "Red";

                        if(x_pos > (ReferenceFrame.cols / 10) * 6 && y_pos > (ReferenceFrame.rows / 10) * 7) position = "Yellow";

                        if(x_pos < (ReferenceFrame.cols / 10) * 4 && y_pos > (ReferenceFrame.rows / 10) * 7) position = "Green";

                } // end for

                // square
                Point grid_a, grid_b;

                grid_a.x = (ReferenceFrame.cols / 10) * 4;
                grid_a.y = (ReferenceFrame.rows / 10) * 3;

                grid_b.x = (ReferenceFrame.cols / 10) * 6;
                grid_b.y = (ReferenceFrame.rows / 10) * 7;

                rectangle(ReferenceFrame, grid_a, grid_b, Scalar(255,255,255), MAX(2, 0), LINE_AA  );

                // lines
                grid_a.x = (ReferenceFrame.cols / 2);
                grid_a.y = 0;

                grid_b.x = (ReferenceFrame.cols / 2);
                grid_b.y = (ReferenceFrame.rows / 10) * 3;

                rectangle(ReferenceFrame, grid_a, grid_b, Scalar(255,255,255), MAX(2, 0), LINE_AA);

                grid_a.x = (ReferenceFrame.cols / 2);
                grid_a.y = (ReferenceFrame.rows / 10) * 7;

                grid_b.x = (ReferenceFrame.cols / 2);
                grid_b.y = ReferenceFrame.rows;

                rectangle(ReferenceFrame, grid_a, grid_b, Scalar(255,255,255), MAX(2, 0), LINE_AA);

                grid_a.x = 0;
                grid_a.y = (ReferenceFrame.rows / 2);

                grid_b.x = (ReferenceFrame.cols / 10) * 4;
                grid_b.y = (ReferenceFrame.rows / 2);

                rectangle(ReferenceFrame, grid_a, grid_b, Scalar(255,255,255), MAX(2, 0), LINE_AA);

                grid_a.x = (ReferenceFrame.cols / 10) * 6;
                grid_a.y = (ReferenceFrame.rows / 2);

                grid_b.x = ReferenceFrame.cols;
                grid_b.y = (ReferenceFrame.rows / 2);

                rectangle(ReferenceFrame, grid_a, grid_b, Scalar(255,255,255), MAX(2, 0), LINE_AA);

                /*line(ReferenceFrame, Point(ReferenceFrame.cols / 4, 0),
                     Point(ReferenceFrame.cols / 4, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                   line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 2, 0),
                     Point((ReferenceFrame.cols / 4) * 2, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));

                   line(ReferenceFrame, Point((ReferenceFrame.cols / 4) * 3, 0),
                     Point((ReferenceFrame.cols / 4) * 3, ReferenceFrame.rows - 1),
                     Scalar(255, 0, 0));
*/
                   Point recA, recB;

                    //Green rectangle
                   recA.x = (ReferenceFrame.cols/6) - 60;
                   recA.y = (ReferenceFrame.rows/6) - 40;
                   recB.x = (ReferenceFrame.cols/6)*1 - 60;
                   recB.y = (ReferenceFrame.rows/6) - 40;

                   rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 0), MAX(80, 0), LINE_AA);

                   // Red rectangle
                   recA.x = (ReferenceFrame.cols / 6)*5 + 60;
                   recA.y = (ReferenceFrame.rows/6) - 40;
                   recB.x = (ReferenceFrame.cols / 6) * 5 + 60;
                   recB.y = (ReferenceFrame.rows/6) - 40;

                   rectangle(ReferenceFrame, recA, recB, Scalar(0, 0, 255), MAX(80, 0), LINE_AA);

                   // Yellow rectangle
                   recA.x = (ReferenceFrame.cols / 6) - 60;
                   recA.y = (ReferenceFrame.rows/6)*5 + 40;
                   recB.x = (ReferenceFrame.cols / 6) - 60;
                   recB.y = (ReferenceFrame.rows/6)*5 + 40;

                   rectangle(ReferenceFrame, recA, recB, Scalar(0, 255, 255), MAX(80, 0), LINE_AA);

                   // Blue rectangle
                   recA.x = (ReferenceFrame.cols / 6) * 5 + 60;
                   recA.y = (ReferenceFrame.rows/6) * 5 + 40;
                   recB.x = (ReferenceFrame.cols / 6) * 5 + 60;
                   recB.y = (ReferenceFrame.rows/6) * 5 + 40;

                   rectangle(ReferenceFrame, recA, recB, Scalar(255, 0, 0), MAX(80, 0), LINE_AA);


                Point pos;
                pos.x = (ReferenceFrame.cols / 10) * 4.1;
                pos.y = (ReferenceFrame.rows / 8) * 3;

                //putText(ReferenceFrame, "GENIUS", pos, 3,
                  //15*0.05+0.2, Scalar(255, 255, 255), 5, LINE_AA);

                pos.x = (ReferenceFrame.cols / 10) * 4.5;
                pos.y = (ReferenceFrame.rows/8) * 5.5;

                putText(ReferenceFrame, "Score: ", pos, 0,
                        10*0.05+0.2, Scalar(255, 255, 255), 4, LINE_AA);

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

                        pos.x = (ReferenceFrame.cols / 10) * 4;
                        pos.y = (ReferenceFrame.rows / 8) * 3;

                       // putText(ReferenceFrame, "GENIUS", pos, 3,
                         //       40*0.05+0.1, Scalar(0, 255, 0), 5, LINE_AA);

                } // end if

                else if(position == "Red")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(0, 0, 255), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 10) * 4;
                        pos.y = (ReferenceFrame.rows / 8) * 3;

                        //putText(ReferenceFrame, "GENIUS", pos, 3,
                          //      40*0.05+0.1, Scalar(0, 0, 255), 5, LINE_AA);

                } // end else/if

                else if(position == "Yellow")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(0, 255, 255), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 10) * 4;
                        pos.y = (ReferenceFrame.rows / 8) * 3;

                        //putText(ReferenceFrame, "GENIUS", pos, 3,
                          //      40*0.05+0.1, Scalar(0, 255, 255), 5, LINE_AA);

                } // end else/if

                else if(position == "Blue")
                {
                        putText(ReferenceFrame, position, pos, 0,
                                20*0.05+0.1, Scalar(255, 0, 0), 4, LINE_AA);

                        pos.x = (ReferenceFrame.cols / 10) * 4;
                        pos.y = (ReferenceFrame.rows / 8) * 3;

                       // putText(ReferenceFrame, "GENIUS", pos, 3,
                         //       40*0.05+0.1, Scalar(255, 0, 0), 5, LINE_AA);

                } // end else/if
                

               game->setSequencia();
               int tam_sequencia = game->getNumeroSequencia();
        
               double time_token = 0;
               vector<int> results = game->getSequencia();

                imshow(WindowName, ReferenceFrame);

                
                while(i < tam_sequencia){

                         auto start = chrono::high_resolution_clock::now();
                         Point posc;

                        posc.x = (ReferenceFrame.cols / 10) * 4.1;
                        posc.y = (ReferenceFrame.rows / 8) * 3;

                        if(results[i] == AZUL){
                        putText(ReferenceFrame, "GENIUS", posc, 3,
                         15*0.05+0.2, Scalar(255, 0, 0), 5, LINE_AA);
                         cout << "Entrou azul " << endl;
                         imshow(WindowName, ReferenceFrame);
                        }
                        else if(results[i] == VERMELHO){
                        putText(ReferenceFrame, "GENIUS", posc, 3,
                         15*0.05+0.2, Scalar(0, 0, 255), 5, LINE_AA);   
                         cout << "Entrou vermelho" << endl;
                         imshow(WindowName, ReferenceFrame);
                        }
                        else if(results[i] == VERDE){
                        putText(ReferenceFrame, "GENIUS", posc, 3,
                         15*0.05+0.2, Scalar(0, 255, 0), 5, LINE_AA);
                        cout << "entrou verde" << endl;
                        imshow(WindowName, ReferenceFrame);
                        }
                        else if(results[i] == AMARELO){
                        putText(ReferenceFrame, "GENIUS", posc, 3,
                         15*0.05+0.2, Scalar(0, 255, 255), 5, LINE_AA);
                                cout << "entrou amarelo" << endl;
                                imshow(WindowName, ReferenceFrame);
                        }

                        auto finish = chrono::high_resolution_clock::now();
                        time_token += chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
        
                        imshow(WindowName, ReferenceFrame);

                        if((time_token * 1e-9 ) >= 1){
                                i++;
                                cout << "TEMPO: " << time_token << endl;
                                time_token = 0;
                        }

           
           
                 }

                 
                 //Tentando captar sequência
         /*        
                       imshow(WindowName, ReferenceFrame);
                  
                        vector<int> compara;
                        int k=0;
        
         do{       

                        VideoStream >> ReferenceFrame;
                        cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
                        Detector.process(GrayFrame);
                        Detector.getObjects(Faces);

                 for (size_t i = 0; i < Faces.size(); i++){
                        
                                Rect r = Faces[i];
                                rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));   

                                int x_posc = r.x;
                                int y_posc = r.y;
                
                        if(x_posc > 310 && y_posc < 50){ //verde
                                compara.push_back(2);
                                continue;
                        }else if(x_posc < 110 && y_posc < 50){//vermelho
                                compara.push_back(1);
                                continue;
                        }else if(x_posc < 150 && y_posc > 200){ //azul
                                compara.push_back(0);
                                continue;
                        }else if(x_posc > 340 && y_posc > 210){//amarelo
                                compara.push_back(3);
                                continue;
                        }
                        
                        for(int j=0;j<tam_sequencia;j++){
                                if(results[j] == compara[j]){
                                        k++;
                                }
                        }

                        }
                        if(k==tam_sequencia)
                                break;
                        
                        flip(ReferenceFrame, ReferenceFrame, 1);
                        imshow(WindowName, ReferenceFrame);   


                        }while(1);

                */
        } while (waitKey(30) < 0);

        Detector.stop();

        return 0;

} // end main
