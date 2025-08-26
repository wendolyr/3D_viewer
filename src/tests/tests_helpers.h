#ifndef CPP4_3DVIEWER_V2_0_TESTS_TESTS_HELPERS_H
#define CPP4_3DVIEWER_V2_0_TESTS_TESTS_HELPERS_H

#include <gtest/gtest.h>

#include "../controller/facade.h"

void CreateCorrectFile();
void CreateFileWithoutVerticesEdges();

void CreateInvalidVerticesOneFile();
void CreateInvalidVerticesTwoFile();
void CreateInvalidVerticesThreeFile();

void CreateInvalidEdgesOneFile();
void CreateInvalidEdgesTwoFile();
void CreateInvalidEdgesThreeFile();
void CreateInvalidEdgesFourFile();

void CreateIncorrectLastSettingsFileOne();
void CreateIncorrectLastSettingsFileTwo();
void CreateIncorrectLastSettingsFileThree();

void CreateEmptyLastSettingsFile();

void CreateIncorrectLastObjectFile();
void CreateEmptyLastObjectFile();

std::vector<std::vector<float>> CreateMatrix();

#endif