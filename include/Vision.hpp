#ifndef VISION_BOYA_HPP
#define VISION_BOYA_HPP
#include <opencv2/opencv.hpp>

bool detectarBoya(
    const cv::Mat& frame,
    cv::Mat& mascara,
    cv::Rect& caja
);

double calcularDistanciaZ(
    double diametroPx
);

double calcularDesplazamientoX(
    double distanciaZ,
    double u,
    double cx
);

double calcularDesplazamientoY(
    double distanciaZ,
    double v,
    double cy
);

double calcularDistancia(
    double x,
    double y,
    double z
);

void dibujarResultados(
    cv::Mat& frame,
    const cv::Rect& caja,
    double distancia
);

#endif