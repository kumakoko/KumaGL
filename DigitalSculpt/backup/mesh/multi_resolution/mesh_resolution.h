#ifndef digital_sculpt_mesh_resolution_h__
#define digital_sculpt_mesh_resolution_h__

#include <cstdint>
#include "misc/utils.h"
#include "editing/subdivision.h"
#include "mesh/mesh.h"
#include "mesh/mesh_data.h"

namespace DigitalSculpt
{
    class MeshResolution : public Mesh
    {
    private:
        Float32Array _detailsXYZ; // details vectors (Float32Array)
        Float32Array _detailsRGB; // details vectors (Float32Array)
        Float32Array _detailsPBR; // details vectors (Float32Array)
        Uint32Array _vertMapping; // vertex mapping to higher res (Uint32Array)
        bool _evenMapping; // if the even vertices are not aligned with higher res
    public:
        inline bool getEvenMapping() const
        {
            return this->_evenMapping;
        }

        inline Uint32Array& getVerticesMapping()
        {
            return this->_vertMapping;
        }

        inline const Uint32Array& getVerticesMapping() const
        {
            return this->_vertMapping;
        }

        inline void setVerticesMapping(const Uint32Array& vmAr)
        {
            this->_vertMapping = vmAr;
        }

        inline setEvenMapping(bool e)
        {
            this->_evenMapping = e;
        }

        MeshResolution(MeshSPtr mesh, bool keepMesh);

        void optimize();

        /** Go to one level above (down to up) */
        void higherSynthesis(std::shared_ptr<MeshResolution> meshDown);

        /** Go to one level below (up to down) */
        void lowerAnalysis(std::shared_ptr<MeshResolution>  meshUp);

        void copyDataFromHigherRes(std::shared_ptr<MeshResolution> meshUp);

        void computePartialSubdivision(Float32Array& subdVerts, Float32Array& subdColors, Float32Array& subdMaterials, std::size_t nbVerticesUp);

        /** Apply back the detail vectors */
        void applyDetails();

        /** Compute the detail vectors */
        void computeDetails(Float32Array& subdVerts, Float32Array& subdColors, Float32Array& subdMaterials, Float32Array& nbVerticesUp);
    };
    
    typedef std::shared_ptr<MeshResolution> MeshResolutionSPtr;
}
#endif // digital_sculpt_mesh_resolution_h__
