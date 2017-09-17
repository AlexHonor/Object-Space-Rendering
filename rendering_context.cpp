#include "rendering_context.h"

using namespace boost;

RenderingContext::RenderingContext() = default;

bool RenderingContext::ApplyContext(const Program& prog) const {
    prog.Use();
    prog.TrySetUniform("u_projection", proj.data.top());
    prog.TrySetUniform("u_model"     , model.data.top());
    prog.TrySetUniform("u_view"      , view.data.top());
    return true;
}

RenderingContext::~RenderingContext() = default;

MatrixStack::MatrixStack() {
    data.push(float44());
    Identity();
}


void MatrixStack::Perspective(float rad_fov, float aspect, float near, float far) {
    data.top() = boost::qvm::perspective_rh<float>(rad_fov, aspect, near, far);
}

void MatrixStack::Rotate(float3 axis, float rad_angle) {
    boost::qvm::rotate(data.top(), axis, rad_angle);
}

void MatrixStack::RotateX(float rad_angle) {
    boost::qvm::rotate_x(data.top(), rad_angle);
}

void MatrixStack::RotateY(float rad_angle) {
    boost::qvm::rotate_y(data.top(), rad_angle);
}

void MatrixStack::RotateZ(float rad_angle) {
    boost::qvm::rotate_z(data.top(), rad_angle);
}

void MatrixStack::Translate(float3 vec) {
    auto mat = (float44)boost::qvm::translation_mat(vec);
    boost::qvm::operator*=(data.top(), mat);
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
    boost::qvm::set_identity(data.top());
}

MatrixStack::~MatrixStack() = default;