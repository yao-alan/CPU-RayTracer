#ifndef MODEL_H
#define MODEL_H

#include "vmath.h"

#include <cstdint>
#include <vector>
#include <utility>

class Model
{
    public:
        Model() { ; }
        ~Model() { ; }

        void addVertex(double x, double y, double z) { m_vertices.push_back(vmath::Vec3<double>(x, y, z)); }
        void addTri   (uint32_t x, uint32_t y, uint32_t z) { m_tris.push_back(vmath::Vec3<uint32_t>(x, y, z)); }
        // returns a range of faces; iterate from .first to .second
        std::pair<std::vector<vmath::Vec3<uint32_t>>::iterator,
                  std::vector<vmath::Vec3<uint32_t>>::iterator> 
            getTriRange() { return std::make_pair(m_tris.begin(), m_tris.end()); }
        vmath::Vec3<double> 
            getVertex (uint32_t idx) { return m_vertices[idx]; }
    private:
        std::vector<vmath::Vec3<double>> m_vertices;  // vertex in 3D space as (x, y, z)
        std::vector<vmath::Vec3<uint32_t>> m_tris;    // indices of the three vertices that make up a face
};

#endif