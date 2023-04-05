#pragma once

void makePictureNegative(Picture* picture, RGBQUAD** palitr);

void makePictureWhiteAndBlack(Picture* picture, RGBQUAD** palitr);

void makeMedianFiltration(Picture* picture,int x_time, RGBQUAD* palitr);

void makeGammaCorrection(Picture* picture, int gamma_value, RGBQUAD** palitr);
