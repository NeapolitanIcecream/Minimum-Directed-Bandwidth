#include <memory>
#include <vector>
#include <iostream>
#include <list>

namespace mdb {
class Vertex {
public:
    uint32_t id;
    std::vector<std::shared_ptr<Vertex>> prev;
    std::vector<std::shared_ptr<Vertex>> next;
    bool visited;
    uint32_t order;
    uint32_t unvisitedNextCount;

    Vertex() {
        static uint32_t id_counter = 0;
        id = id_counter++;
        visited = false;
        order = -1;
    }

    void AddPrev(std::shared_ptr<Vertex> v) {
        prev.push_back(v);
    }

    void AddNext(std::shared_ptr<Vertex> v) {
        next.push_back(v);
        unvisitedNextCount++;
    }

    void Print() {
        std::cout << "Vertex " << id << ":" << std::endl;
        for (auto v : prev) {
            std::cout << "  prev: " << v->id << std::endl;
        }
        for (auto v : next) {
            std::cout << "  next: " << v->id << std::endl;
        }
        std::cout << "  visited: " << visited << std::endl;
        std::cout << "  order: " << order << std::endl;
    }

    void SetOrder(uint32_t order) {
        visited = true;
        this->order = order;
    }

    std::vector<std::shared_ptr<Vertex>> GetUnvisitedNext() {
        std::vector<std::shared_ptr<Vertex>> result;
        for (auto v : next) {
            if (!v->visited) {
                result.push_back(v);
            }
        }
        return result;
    }

    void Visit() {
        static uint32_t globalOrder = 0;
        visited = true;
        order = globalOrder++;
        for (auto v : prev) {
            v->unvisitedNextCount--;
        }
    }

    bool AllNextVisited() {
        return unvisitedNextCount == 0;
    }

    bool OneNextUnvisited() {
        return unvisitedNextCount == 1;
    }

    bool MoreThanOneNextUnvisited() {
        return unvisitedNextCount > 1;
    }

    bool AllPrevVisited() {
        for (auto v : prev) {
            if (!v->visited) {
                return false;
            }
        }
        return true;
    }
};

typedef std::shared_ptr<Vertex> VertexPtr;

namespace {

void RemovePrev(VertexPtr v, VertexPtr prev) {
    for (auto i = v->prev.begin(); i != v->prev.end(); i++) {
        if (*i == prev) {
            v->prev.erase(i);
            break;
        }
    }
}

void RemoveNext(VertexPtr v, VertexPtr next) {
    for (auto i = v->next.begin(); i != v->next.end(); i++) {
        if (*i == next) {
            v->next.erase(i);
            break;
        }
    }
}

void AddEdge(VertexPtr v1, VertexPtr v2) {
    v1->AddNext(v2);
    v2->AddPrev(v1);
}

VertexPtr Copy(VertexPtr v) {
    VertexPtr result(new Vertex());
    result->AddPrev(v);
    for (auto i = v->next.begin(); i != v->next.end(); i++) {
        if (!(*i)->visited) {
            v->next.erase(i);
            RemovePrev(*i, v);
            AddEdge(result, *i);
        }
    }
    v->AddNext(result);
    return result;
}

VertexPtr CreateVertexPtr() {
    VertexPtr result(new Vertex());
    return result;
}

} // namespace

template< std::size_t N >
class Graph {
public:
    std::vector<VertexPtr> vertices;
    std::list<VertexPtr> visitedVertices;

    Graph() {
        vertices.reserve(N);
        for (uint32_t i = 0; i < N; i++) {
            vertices.push_back(CreateVertexPtr());
        }
    }

    void Print() {
        for (auto v : vertices) {
            v->Print();
        }
    }

    void Print(uint32_t id) {
        vertices[id]->Print();
    }

    std::vector<std::shared_ptr<Vertex>> GetUnvisitedNext(uint32_t id) {
        return vertices[id]->GetUnvisitedNext();
    }

    void Visit(uint32_t id) {
        vertices[id]->Visit();
        visitedVertices.push_back(vertices[id]);
    }

    void AddEdge(uint32_t v1, uint32_t v2) {
        mdb::AddEdge(vertices[v1], vertices[v2]);
    }

    void AllNextVisited(uint32_t id) {
        vertices[id]->AllNextVisited();
    }

    void OneNextUnvisited(uint32_t id) {
        vertices[id]->OneNextUnvisited();
    }

    void MoreThanOneNextUnvisited(uint32_t id) {
        vertices[id]->MoreThanOneNextUnvisited();
    }

    void AllPrevVisited(uint32_t id) {
        vertices[id]->AllPrevVisited();
    }

    void Copy(uint32_t v) {
        vertices.push_back(mdb::Copy(vertices[v]));
    }
};
} // namespace mdb
