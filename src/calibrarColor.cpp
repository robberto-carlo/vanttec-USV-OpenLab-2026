#include <opencv2/opencv.hpp>
#include <iostream>

int hMin = 0;
int hMax = 179;

int sMin = 0;
int sMax = 255;

int vMin = 0;
int vMax = 255;

int main(){
    cv::VideoCapture camara(0);

    if (!camara.isOpened()){
        // No abrio la camara
        std::cerr << "Error: no se pudo abrir la webcam." << std::endl;
        return -1;
    }

    // Resolucion 640x480 px
    camara.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    camara.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    cv::namedWindow("Camara", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Mascara HSV", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Calibrar Color", cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("H Min", "Calibrar Color", &hMin, 179);
    cv::createTrackbar("H Max", "Calibrar Color", &hMax, 179);

    cv::createTrackbar("S Min", "Calibrar Color", &sMin, 255);
    cv::createTrackbar("S Max", "Calibrar Color", &sMax, 255);

    cv::createTrackbar("V Min", "Calibrar Color", &vMin, 255);
    cv::createTrackbar("V Max", "Calibrar Color", &vMax, 255);

    cv::Mat frame;
    cv::Mat hsv;
    cv::Mat mascara;

    while(true){
        camara >> frame;

        if(frame.empty()){
            std::cerr<< "Error: no se pudo obtener la imagen."<< std::endl;
            break;
        }

        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); // BGR => HSV

        // Leer valores del trackbar
        hMin = cv::getTrackbarPos("H Min", "Calibrar Color");
        hMax = cv::getTrackbarPos("H Max", "Calibrar Color");

        sMin = cv::getTrackbarPos("S Min", "Calibrar Color");
        sMax = cv::getTrackbarPos("S Max", "Calibrar Color");

        vMin = cv::getTrackbarPos("V Min", "Calibrar Color");
        vMax = cv::getTrackbarPos("V Max", "Calibrar Color");

        // Evitar rangos invalidos
        if (hMin >= hMax)
            hMax = hMin + 1;
        if (sMin >= sMax)
            sMax = sMin + 1;
        if (vMin >= vMax)
            vMax = vMin + 1;

        // Crear máscara
        cv::inRange(
            hsv,
            cv::Scalar(hMin, sMin, vMin),
            cv::Scalar(hMax, sMax, vMax),
            mascara
        );

        // Cámara original
        cv::imshow("Camara", frame);
        // Color detectado con HSV
        cv::imshow("Mascara HSV", mascara);

        // Mostrar valores en la terminal
        std::cout
            << "\rH: [" << hMin << ", " << hMax << "] "
            << "S: [" << sMin << ", " << sMax << "] "
            << "V: [" << vMin << ", " << vMax << "]"
            << std::flush;

        if (cv::waitKey(1) == 27){ // ESC para salir
            break;
        }
    }

    camara.release();
    cv::destroyAllWindows();

    return 0;
}
