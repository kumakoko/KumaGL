#include "edge.h"

namespace DigitalSculpt
{
    Edge::Edge()
    {
    }

    Edge::~Edge()
    {
    }

    void Edge::cleanUp()
    {
        // using std::set;

        // set<KeyData> keys;
        // const auto sizeofedges = this->vertexEdges.size();



        // for (int i = 0; i < sizeofedges; i++)
        // {
        // 	keys.insert(this->vertexEdges[i]);
        // }
        // vertexEdges.reserve(keys.size());
        // vertexEdges.assign(keys.begin(), keys.end());

    }

    void Edge::printEdge()
    {
        for (auto& key : vertexEdges)
        {
            //say key << ' ';
        }
        //say "\n";
    }
}