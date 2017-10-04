#pragma once

#include "inc.h"
#include "utility.h"
#include "program.h"

#include <stack>
#include <memory>

class RenderingContext;

class MatrixStack {
public:
    MatrixStack();

    void Perspective(float rad_fov, float aspect, float near, float far);

    void Rotate(float3 axis, float rad_angle);

    void RotateX(float rad_angle);
    void RotateY(float rad_angle);
    void RotateZ(float rad_angle);

    void Translate(float3 vec);
    void Scale(float3 scale);

    void Push();
    void Pop();

    void Identity();

    ~MatrixStack();

    friend class RenderingContext;
private:
    stack<float44> data;
};

class RenderingContext {
public:
    RenderingContext();

    bool ApplyContext(const std::shared_ptr<Program>& prog) const;
    
    ~RenderingContext();

    MatrixStack proj;
    MatrixStack model;
    MatrixStack view;
};
