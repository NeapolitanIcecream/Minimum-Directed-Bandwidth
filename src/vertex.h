#include <bits/stdc++.h>

class Vertex {
public:
    uint32_t id;
    std::vector<std::shared_ptr<Vertex> > prev;
    std::vector<std::shared_ptr<Vertex> > next;
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

    void Reset() {
        visited = false;
        order = -1;
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

    std::vector<std::shared_ptr<Vertex> > GetUnvisitedNext() {
        std::vector<std::shared_ptr<Vertex> > result;
        for (auto v : next) {
            if (!v->visited) {
                result.push_back(v);
            }
        }
        return result;
    }

    void Visit() {
        visited = true;
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
};

typedef std::shared_ptr<Vertex> VertexPtr;

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
        if (!*i->visited) {
            v->next.erase(i);
            *i->RemovePrev(v);
            AddEdge(result, *i);
        }
    }
    v->AddNext(result);
    return result;
}
