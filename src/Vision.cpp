#include "Vision.hpp"
#include <cmath>
//#include <iostream>
#include <vector>
#include <string>

// Datos de la boya
const double PERIMETRO_BOYA = 0.62; // metros
const double DIAMETRO_BOYA = PERIMETRO_BOYA / CV_PI;
// Parámetro focal obtenido experimentalmente
const double FX = 533.67; // pixeles

bool detectarBoya(
    const cv::Mat& frame,
    cv::Mat& mascara,
    cv::Rect& caja
    ){
    cv::Mat hsv;

    // Convertir de BGR a HSV
    cv::cvtColor(
        frame,
        hsv,
        cv::COLOR_BGR2HSV
    );

    cv::Mat mascara1;
    cv::Mat mascara2;

    // Primer rango de rojo
    cv::inRange(
        hsv,
        cv::Scalar(0, 156, 173),
        cv::Scalar(10, 255, 255),
        mascara1
    );

    // Segundo rango de rojo
    cv::inRange(
        hsv,
        cv::Scalar(170, 156, 173),
        cv::Scalar(179, 255, 255),
        mascara2
    );

    // Unir rangos
    cv::bitwise_or(
        mascara1,
        mascara2,
        mascara
    );

    // Kernel
    cv::Mat kernel = cv::getStructuringElement(
            cv::MORPH_ELLIPSE,
            cv::Size(5, 5));

    cv::Mat mascaraLimpia;

    // Eliminar ruido
    cv::morphologyEx(
        mascara,
        mascaraLimpia,
        cv::MORPH_OPEN,
        kernel
    );

    // Expandir la zona detectada
    cv::morphologyEx(
        mascaraLimpia,
        mascara,
        cv::MORPH_DILATE,
        kernel
    );

    // Buscar contornos
    std::vector<std::vector<cv::Point>> contornos;

    cv::findContours(
        mascara,
        contornos,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
    );

    if (contornos.empty()) return false;

    // Buscar el contorno más grande
    int indiceMayor = 0;
    for(int i=1; i < static_cast<int>(contornos.size()); i++){
        if (cv::contourArea(contornos[i]) > cv::contourArea(contornos[indiceMayor])){
            indiceMayor = i;
        }
    }

    // Obtener area
    double area = cv::contourArea(contornos[indiceMayor]);

    // Comprobar tamaño
    if (area <= 500){
        return false;
    }

    // Obtener bounding box
    caja = cv::boundingRect(contornos[indiceMayor]);

    return true;
}

double calcularDistanciaZ(double diametroPx){
    return (FX * DIAMETRO_BOYA) / diametroPx;
}

double calcularDesplazamientoX(
    double distanciaZ,
    double u,
    double cx
){
    return distanciaZ * (u - cx) / FX;
}


double calcularDesplazamientoY(
    double distanciaZ,
    double v,
    double cy
){
    return distanciaZ * (v - cy) / FX;
}

double calcularDistancia(
    double x,
    double y,
    double z
){
    return std::sqrt(x * x + y * y + z * z);
}

void dibujarResultados(
    cv::Mat& frame,
    const cv::Rect& caja,
    double distancia
){
    int x = caja.x;
    int y = caja.y;
    int w = caja.width;
    int h = caja.height;

    // Centro del objecto
    int u = x + w / 2;
    int v = y + h / 2;

    // Dibujar bounding box
    cv::rectangle(
        frame,
        caja,
        cv::Scalar(0, 255, 0),
        2
    );

    // Dibujar centro
    cv::circle(
        frame,
        cv::Point(u, v),
        5,
        cv::Scalar(255, 0, 0),
        -1
    );

    // U y V
    std::string texto1 =
        "U: " +
        std::to_string(u) +
        " V: " +
        std::to_string(v);

    // W y H
    std::string texto2 =
        "W: " +
        std::to_string(w) +
        " H: " +
        std::to_string(h);

    // Distancia
    std::string texto3 =
        "Distancia: " +
        std::to_string(distancia) +
        " m";

    // Mostrar U y V
    cv::putText(
        frame,
        texto1,
        cv::Point(x, y - 45),
        cv::FONT_HERSHEY_SIMPLEX,
        0.6,
        cv::Scalar(0, 255, 0),
        2
    );

    // Mostrar W y H
    cv::putText(
        frame,
        texto2,
        cv::Point(x, y - 20),
        cv::FONT_HERSHEY_SIMPLEX,
        0.6,
        cv::Scalar(0, 255, 0),
        2
    );

    // Mostrar distancia
    cv::putText(
        frame,
        texto3,
        cv::Point(x, y + h + 25),
        cv::FONT_HERSHEY_SIMPLEX,
        0.7,
        cv::Scalar(0, 255, 0),
        2
    );
}