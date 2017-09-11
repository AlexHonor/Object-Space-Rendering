#pragma once

#include "inc.h"
#include "utility.h"

#include <stack>

using namespace std;

class RenderingContext {
public:
    RenderingContext();

    void Push(float44 mat);
    float44 Pop();

    float44 &Proj();
    float44 &Model();
    float44 &View();

    ~RenderingContext();
private:
    stack<float44> proj;
    stack<float44> model;
    stack<float44> view;
};