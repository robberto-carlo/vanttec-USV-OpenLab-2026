#include <opencv2/opencv.hpp>
#include <iostream>
#include "Vision.hpp"

int main(){
    cv::VideoCapture camara(0); // Abrir cámara

    if(!camara.isOpened()){
        std::cerr<<"Error: no se pudo abrir la webcam."<< std::endl; // No abrio la camara
        return -1;
    }

    cv::Mat frame;
    cv::Mat mascara;

    camara >> frame;
    if(frame.empty()){
        std::cerr << "Error: no se pudo obtener la imagen."<< std::endl;
        return -1;
    }

    double CX = frame.cols / 2.0;
    double CY = frame.rows / 2.0;

    while(true){
        camara >> frame; // Capturar imagen en frame

        if(frame.empty()){
            std::cerr<<"Error: no se pudo obtener la imagen."<< std::endl; // No habia una imagen en frame
            break;
        }

        cv::Rect caja; // x, y, width, height
        bool boyaDetectada = detectarBoya(frame,mascara,caja); // Detectar objeto

        if(boyaDetectada){
            // Dimensiones del objecto
            int w = caja.width;
            int h = caja.height;

            // Centro del objecto
            double u = caja.x + w / 2.0;
            double v = caja.y + h / 2.0;

            double diametroPx = (w + h)/2.0; // Diametro en pixeles

            // Calcular profunidad Z
            double distanciaZ = calcularDistanciaZ(diametroPx);
            // Calcular desplazamiento en X
            double desplazamientoX = calcularDesplazamientoX(distanciaZ,u,CX);
            // Calcular desplazamiento en Y
            double desplazamientoY = calcularDesplazamientoY(distanciaZ,v,CY);

            // Calcular distancia Real
            double distancia = calcularDistancia(desplazamientoX,desplazamientoY,distanciaZ);

            // Mostrar datos
            dibujarResultados(
                frame,
                caja,
                distancia
            );

            // Mostrar en terminal
            std::cout
                << "U: " << u
                << " | V: " << v
                << " | W: " << w
                << " | H: " << h
                << " | Distancia: "
                << distancia
                << " m"
                << std::endl;
        }

        // Mostrar imagen
        cv::imshow("Camara",frame);
        // Mostrar máscara
        cv::imshow("Mascara",mascara);

        if (cv::waitKey(1) == 27){ //ESC para salir
            break;
        }
    }

    camara.release();
    cv::destroyAllWindows();
    return 0;
}
