/*
    This file skeleton (without the algorithms implementation) was originally
   part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#include "utils/sampling/warp.h"
#include "utils/consts.h"

namespace Neon {

Point2f Warp::squareToUniformSquare(const Point2f &sample) { return sample; }

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
  if (sample[0] >= 0 && sample[0] <= 1 && sample[1] >= 0 && sample[1] <= 1)
    return 1.0f;
  return 0.0f;
}

float tentInverse(float x) {
  if (x < 0.5)
    return sqrt(2 * x) - 1;
  return 1 - sqrt(2 * (1 - x));
}

Point2f Warp::squareToTent(const Point2f &sample) {
  return {tentInverse(sample.x()), tentInverse(sample.y())};
}

float Warp::squareToTentPdf(const Point2f &p) {
  return (1 - std::abs(p.x())) * (1 - std::abs(p.y()));
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
  float r = sqrt(sample.x()), theta = 2 * M_PI * sample.y();
  return {static_cast<float>(r * cos(theta)),
          static_cast<float>(r * sin(theta))};
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
  return p.normSq() <= 1.0f ? INV_PI : 0;
}

Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
  float sinTheta = 2 * sqrt(sample.x() * (1 - sample.x()));
  float phi = 2 * M_PI * sample.y();
  return {static_cast<float>(sinTheta * cos(phi)),
          static_cast<float>(sinTheta * sin(phi)), 1 - 2 * sample.x()};
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
  return v.lengthSq() <= 1.0f ? INV_FOURPI : 0;
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
  float sinTheta = sqrt(1 - sample.x() * sample.x());
  float phi = 2 * M_PI * sample.y();
  return {static_cast<float>(sinTheta * cos(phi)),
          static_cast<float>(sinTheta * sin(phi)), 1 - sample.x()};
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
  return v.lengthSq() <= 1.0f && v.z() > 0 ? INV_TWOPI : 0;
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
  Point2f d = squareToUniformDisk(sample);
  float z = sqrt(std::max(0.f, 1 - d.x() * d.x() - d.y() * d.y()));
  return Vector3f(d.x(), d.y(), z);
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
  return v.z() >= 0 ? v.z() * INV_PI : 0;
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
  float theta = atan(alpha * sqrt(-log(sample.x())));
  float phi = 2 * M_PI * sample.y();
  return {static_cast<float>(sin(theta) * cos(phi)),
          static_cast<float>(sin(theta) * sin(phi)), std::cos(theta)};
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
  float cosTheta = m.z();
  if (cosTheta <= 0) {
    return 0;
  }

  float tanSqTheta = (1 - pow(cosTheta, 2)) / pow(cosTheta, 2);
  float result = INV_PI * exp(-tanSqTheta / pow(alpha, 2)) /
                 (pow(alpha, 2) * pow(cosTheta, 3));
  return result;
}
} // namespace Neon
