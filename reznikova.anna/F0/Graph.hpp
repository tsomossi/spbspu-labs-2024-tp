#ifndef Graph_hpp
#define Graph_hpp
#include <vector>
#include <string>
#include "Vertex.hpp"

namespace reznikova
{
  struct Graph
  {
    Graph(std::string name);
    std::string getGraphName() const;
    size_t getCapacity() const;
    Vertex * findVertex(size_t index) const;
    bool isVertex(size_t index) const;
    bool isEdge(size_t first_index, size_t second_index) const;
  private:
    std::vector< Vertex * > graph_;
    std::string graphName_;
    size_t capacity_;
  };
}

#endif
