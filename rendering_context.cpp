#include "rendering_context.h"

#include <iostream>

using namespace boost;

RenderingContext::RenderingContext() = default;

bool RenderingContext::ApplyContext(const std::shared_ptr<Program>& prog) const {
    prog->Use();
    prog->TrySetUniform("u_projection", proj.data.top());
    prog->TrySetUniform("u_model"     , model.data.top());
    prog->TrySetUniform("u_view"      , view.data.top());
       
    float44 mv = qvm::operator*(model.data.top(), view.data.top());
    
    float33 normal = qvm::inverse(qvm::del_row_col<3, 3, float44>(mv));
    normal = qvm::transposed(normal);
    


    prog->TrySetUniform("u_normal", normal);
    return true;
}

RenderingContext::~RenderingContext() = default;

MatrixStack::MatrixStack() {
    data.push(float44());
    Identity();
}


void MatrixStack::Perspective(float rad_fov, float aspect, float near, float far) {
    data.top() = qvm::perspective_rh<float>(rad_fov, aspect, near, far);
}

void MatrixStack::Rotate(float3 axis, float rad_angle) {
    qvm::rotate(data.top(), axis, rad_angle);
}

void MatrixStack::RotateX(float rad_angle) {
    qvm::rotate_x(data.top(), rad_angle);
}

void MatrixStack::RotateY(float rad_angle) {
    qvm::rotate_y(data.top(), rad_angle);
}

void MatrixStack::RotateZ(float rad_angle) {
    qvm::rotate_z(data.top(), rad_angle);
}

void MatrixStack::Translate(float3 vec) {
    auto mat = (float44)qvm::translation_mat(vec);
    qvm::operator*=(data.top(), mat);
}

void MatrixStack::Scale(float3 scale) {
    float4 scale_final;
    qvm::XYZ(scale_final) = scale;
    qvm::W  (scale_final) = 1.0f;

    qvm::operator*=(data.top(), qvm::diag_mat(scale_final));
}

void MatrixStack::Push() {
    data.push(data.top());
}

void MatrixStack::Pop() {
    data.pop();
}

void MatrixStack::Identity() {
    qvm::set_identity(data.top());
}

MatrixStack::~MatrixStack() = default;