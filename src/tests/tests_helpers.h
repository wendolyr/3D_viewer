#ifndef CPP4_3DVIEWER_V2_0_TESTS_TESTS_HELPERS_H
#define CPP4_3DVIEWER_V2_0_TESTS_TESTS_HELPERS_H

#include <gtest/gtest.h>

#include "../controller/facade.h"

void CreateCorrectFile();

void CreateInvalidVerticesOneFile();
void CreateInvalidVerticesTwoFile();
void CreateInvalidVerticesThreeFile();

void CreateInvalidPolygonsOneFile();
void CreateInvalidPolygonsTwoFile();
void CreateInvalidPolygonsThreeFile();
void CreateInvalidPolygonsFourFile();

void CreateIncorrectLastSettingsFile();
void CreateIncorrectLastObjectFile();

#endif