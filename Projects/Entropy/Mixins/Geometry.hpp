#include <vector>
#include <iterator>

#define GLM_ENABLE_EXPERIMENTAL
#include "Cute/cute_c2.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/vec2.hpp"

using glm::vec2;
using std::vector;

#pragma once

namespace Entropy {

    typedef struct Vector2D {
        union {
            struct {
                float x, y;
            };
            vec2 glmVector;
            c2v c2Vector;
        };

        Vector2D(float _x, float _y) : x(_x), y(_y) {}

        Vector2D(vec2 v) : glmVector(v) {}
        Vector2D(c2v v) : c2Vector(v) {}

        operator vec2() const { return glmVector; }
        operator c2v() const { return c2Vector; }

        Vector2D normalise() { return glm::normalize(glmVector); }
    } Vector2D;

    inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2) { return (v1.glmVector - v2.glmVector); }

    inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2) { return (v1.glmVector + v2.glmVector); }

    struct Vertex2D {
        Vector2D normal;
        union {
            struct {
                float x, y;
            };
            Vector2D position;
        };

        Vertex2D(const Vector2D& p) : position(p), normal(vec2(0)) {}
        Vertex2D(float _x, float _y) : x(_x), y(_y), normal(vec2(0)) {}
    };

    struct Ray {
        Vector2D origin;
        Vector2D direction;
        float distance;

        operator c2Ray() {
            c2Ray ray;

            ray.t = distance;
            ray.d = direction;
            ray.p = origin;

            return ray;
        }
    };

    namespace Mixin {
        class Geometry2D {
          public:
            virtual bool rayIntersect(c2Ray ray, c2x position) = 0;
            virtual bool rayCast(c2Ray ray, c2x position, c2Raycast& cast) = 0;
            virtual vector<Vertex2D> getGeometry() = 0;
            virtual vector<Vector2D> getEdgeNormals() = 0;
        };

    }  // namespace Mixin

    class Polygon2D : Mixin::Geometry2D {
      private:
        void computePolygon() {
            
            poly.count = vertices.size();
            std::transform(vertices.begin(), vertices.end(), std::begin(poly.norms),
                           [](const Vertex2D& a) { return a.normal.c2Vector; });

            std::transform(vertices.begin(), vertices.end(), std::begin(poly.verts),
                           [](const Vertex2D& a) { return a.position.c2Vector; });

        }

      public:
        c2Poly poly;
        vector<Vertex2D> vertices;

        Polygon2D(vector<Vertex2D> const& v) : vertices(v) { calculateVertexNormals(); computePolygon(); }
        Polygon2D() : vertices(vector<Vertex2D>()) {}

        inline bool rayCast(c2Ray ray, c2x position, c2Raycast& cast) override {
            
            
            return c2RaytoPoly((c2Ray) ray, &poly, 0, &cast);
        }

        inline bool rayIntersect(c2Ray ray, c2x position) override {
            // c2Raycast cast = rayCast(ray, position);
            
            // if (cast.t == 0)
            //     return false;
            return true;
        }

        inline vector<Vector2D> getEdgeNormals() override {
            vector<Vector2D> normals;

            for (size_t i = 0; i < vertices.size(); i++) {
                auto normal = vertices[i].position - vertices[(i + 1) % vertices.size()].position;
                normal = glm::rotate(normal.glmVector, 90.0f);
                normals.push_back(normal);
            }

            return normals;
        }

        inline void calculateVertexNormals() {
            auto edgeNormals = getEdgeNormals();

            for (size_t i = 0; i < edgeNormals.size(); i++) {
                vertices[i].normal = (edgeNormals[i] + edgeNormals[(i + 1) % edgeNormals.size()]).normalise();
            }
        }

        inline vector<Vertex2D> getGeometry() override { return vertices; }
    };

}  // namespace Entropy
