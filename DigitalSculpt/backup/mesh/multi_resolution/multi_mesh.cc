#include <cstdint>
#include <vector>

#include "../klib/kgl_lib_pch.h"

#include "mesh/multi_resolution/multi_mesh.h"

namespace DigitalSculpt
{
    std::uint32_t Multimesh::RENDER_HINT = 0;

    Multimesh::Multimesh(MeshSPtr mesh)
    {
        this->setID(mesh->getID());
        this->setRenderData(mesh->getRenderData());
        this->setTransformData(mesh->getTransformData());

        //this->_meshes = [new MeshResolution(mesh, true)];
        this->_meshes.clear();
        this->_meshes.emplace_back(std::make_shared<MeshResolution>(new MeshResolution(mesh, true));

        this->setSelection(0);

        this->_indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
        this->_wireframeBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    }

    void Multimesh::setSelection(std::uint32_t sel)
    {
        this->_sel = sel;
        this->setMeshData(this->getCurrentMesh()->getMeshData());
    }

    MeshSPtr Multimesh::addLevel()
    {
        if ((this->_meshes.size() - 1) != this->_sel)
            return this->getCurrentMesh();

        std::uint32_t nbFaces = this->getCurrentMesh()->getNbFaces();
        // var strTimer = 'addLevel : ' + nbFaces + ' -> ' + nbFaces * 4;
        // console.time(strTimer);

        MeshSPtr baseMesh = this->getCurrentMesh();
        MeshResolutionSPtr newMesh = std::make_shared<MeshResolution>(new MeshResolution(baseMesh, false));
        baseMesh->setVerticesMapping(nullptr);

        Subdivision::fullSubdivision(baseMesh, newMesh);
        newMesh->initTopology();

        this->pushMesh(newMesh);
        this->initRender();

        //console.timeEnd(strTimer);

        return newMesh;
    }

    MeshSPtr Multimesh::computeReverse()
    {
        if (this->_sel != 0)
            return this->getCurrentMesh();

        MeshSPtr baseMesh = this->getCurrentMesh();
        MeshSPtr newMesh = std::make_shared<MeshResolution>(new MeshResolution(baseMesh, false));

        bool status = Reversion::computeReverse(baseMesh, newMesh);
        if (!status)
            return std::make_shared<MeshResolution>(nullptr);

        newMesh->initTopology();

        this->unshiftMesh(newMesh);
        this->initRender();
        return newMesh;
    }

    MeshSPtr Multimesh::lowerLevel()
    {
        if (this->_sel == 0)
            return this->_meshes[0];

        this->_meshes[this->_sel - 1]->lowerAnalysis(this->getCurrentMesh());
        this->setSelection(this->_sel - 1);
        this->updateResolution();

        return this->getCurrentMesh();
    }

    MeshSPtr Multimesh::higherLevel()
    {
        if (this->_sel == this->_meshes.size() - 1)
            return this->getCurrentMesh();

        this->_meshes[this->_sel + 1]->higherSynthesis(this->getCurrentMesh());
        this->setSelection(this->_sel + 1);
        this->updateResolution();

        return this->getCurrentMesh();
    }

    void Multimesh::updateResolution()
    {
        this->updateGeometry();
        this->updateDuplicateColorsAndMaterials();
        this->updateBuffers();

        MeshSPtr mesh = this->_meshes[this->getLowIndexRender()];

        const Uint32Array& tri = mesh->getTriangles();
        const Uint32Array wf = mesh->getWireframe();
        this->_indexBuffer->update(tri, tri.size());
        this->_wireframeBuffer->update(wf, wf.size());
    }

    void Multimesh::selectResolution(std::uint32_t sel)
    {
        while (this->_sel > sel)
        {
            this->lowerLevel();
        }

        while (this->_sel < sel)
        {
            this->higherLevel();
        }
    }

    std::uint32_t Multimesh::findIndexFromMesh(MeshSPtr mesh)
    {
        std::uint32_t l = _meshes.size();
        for (std::uint32_t i = 0; i < l; ++i) {
            if (mesh == _meshes[i])
                return i;
        }

        return -1;
    }

    void Multimesh::selectMesh(MeshSPtr mesh)
    {
        std::uint32_t val = this->findIndexFromMesh(mesh);
        this->selectResolution(val);
    }

    void Multimesh::pushMesh(MeshSPtr mesh)
    {
        _meshes.emplace_back(mesh);//this->_meshes.push(mesh);
        this->setSelection(this->_meshes.length - 1);
        this->updateResolution();
    }

    void Multimesh::unshiftMesh(MeshSPtr mesh) {
        this->_meshes.unshift(mesh);
        this->setSelection(1);
        this->lowerLevel();
    }

    void Multimesh::popMesh() {
        this->_meshes.pop();
        this->setSelection(this->_meshes.length - 1);
        this->updateResolution();
    }

    void Multimesh::shiftMesh()
    {
        this->_meshes.shift();
        this->setSelection(0);
        this->updateResolution();
    }

    void Multimesh::deleteLower()
    {
        this->_meshes.splice(0, this->_sel);
        this->setSelection(0);
    }

    void Multimesh::deleteHigher()
    {
        this->_meshes.splice(this->_sel + 1);
    }

    std::uint32_t Multimesh::getLowIndexRender()
    {
        std::uint32_t limit = 500000;
        std::uint32_t sel = this->_sel;
        while (this->_sel >= 0)
        {
            MeshSPtr mesh = this->_meshes[this->_sel];
            // we disable low rendering for lower resolution mesh with
            // an index indirection for even vertices
            if (mesh->getEvenMapping() == true)
                return sel == this->_sel ? sel : sel + 1;
            if (mesh->getNbTriangles() < limit)
                return sel;
            --sel;
        }
        return 0;
    }

    void Multimesh::_renderLow(Scene* main)
    {
        RenderData* render = this->getRenderData();
        //var tmpSel = this->_sel;
        //Buffer* tmpIndex = this->getIndexBuffer();
        this->setSelection(this->getLowIndexRender());
        render->_indexBuffer = this->_indexBuffer;

        Mesh::render(main);//super.render(main);

        render->_indexBuffer = this->getIndexBuffer();
        this->setSelection(this->_sel);
    }

    void Multimesh::_renderWireframeLow(Scene* main)
    {
        RenderData* render = this->getRenderData();
        // var tmpSel = this->_sel;
        // var tmpWire = this->getWireframeBuffer();
        this->setSelection(this->getLowIndexRender());
        render->_wireframeBuffer = this->_wireframeBuffer;

        Mesh::renderWireframe(main);

        render._wireframeBuffer = this->getWireframeBuffer();
        this->setSelection(this->_sel);
    }

    bool Multimesh::_canUseLowRender(Scene* main)
    {
        if (this->isUsingTexCoords() || this->isUsingDrawArrays())
            return false;

        if (Multimesh::RENDER_HINT == Multimesh::PICKING() || Multimesh::RENDER_HINT == Multimesh::NONE())
            return false;

        if (main->getMesh() == this && Multimesh::RENDER_HINT != Multimesh::CAMERA())
            return false;

        if (this->getLowIndexRender() == this->_sel)
            return false;

        return true;
    }

    void Multimesh::render(Scene* main)
    {
        return this->_canUseLowRender(main) ? this->_renderLow(main) : super.render(main);
    }

    void Multimesh::renderWireframe(Scene* main)
    {
        return this->_canUseLowRender(main) ? this->_renderWireframeLow(main) : Mesh::renderWireframe(main);
    }

}
