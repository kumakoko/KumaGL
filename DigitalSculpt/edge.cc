#include <iterator>
#include <string>
#include "spdlog/spdlog.h"
#include "fmt/format.h"
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
        auto out = fmt::memory_buffer();
       
        for (auto& key : vertexEdges)
        {
            out.clear();
            fmt::format_to(std::back_inserter(out),"{} ", key);
            spdlog::info(out.data());
        }

        spdlog::info("\n");
    }
}