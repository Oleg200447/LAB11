#pragma once

void makePictureNegative(const Picture* picture, RGBQUAD** palitr);

void makePictureWhiteAndBlack(const Picture* picture, RGBQUAD** palitr);

void makeMedianFiltration(Picture* picture,int x_time, RGBQUAD* palitr);

void makeGammaCorrection(const Picture* picture, int gamma_value, RGBQUAD** palitr);
