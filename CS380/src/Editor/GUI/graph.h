#pragma once

namespace BaleaEditor {

    enum class UiNodeType {

        add,
        multiply,
        output,
        sine,
        time,
        modulus

    };

    struct UiNode {

        UiNodeType type;
        int id;

        union {

            struct {

                int lhs, rhs;
            } add;

            struct
            {
                int lhs, rhs;
            } multiply;

            struct
            {
                int lhs, rhs;
            } modulus;

            struct
            {
                int r, g, b, a;
            } output;

            struct
            {
                int input;
            } sine;
        };
    };

    // a very simple directional graph
    template<typename NodeType>
    class Graph
    {
    public:
        Graph() : current_id_(0), nodes_(), edges_from_node_(), node_neighbors_(), edges_() {}

        struct Edge
        {
            int id;
            int from, to;

            Edge() = default;
            Edge(const int id, const int f, const int t) : id(id), from(f), to(t) {}

            inline int opposite(const int n) const { return n == from ? to : from; }
            inline bool contains(const int n) const { return n == from || n == to; }
        };

        // Element access

        NodeType& node(int node_id);
        const NodeType& node(int node_id) const;
        stl::Span<const int> neighbors(int node_id) const;
        stl::Span<const Edge> edges() const;

        // Capacity

        size_t num_edges_from_node(int node_id) const;

        // Modifiers

        int insert_node(const NodeType& node);
        void erase_node(int node_id);

        int insert_edge(int from, int to);
        void erase_edge(int edge_id);
        BaleaEngine::Graphics::Color evaluate();
        unsigned root_node = -1;
        std::vector<UiNode> UInodes_;

    private:
        int current_id_;
        // These contains map to the node id
        stl::IdMap<NodeType> nodes_;
        stl::IdMap<int> edges_from_node_;
        stl::IdMap<std::vector<int>> node_neighbors_;

        // This container maps to the edge id
        stl::IdMap<Edge> edges_;
    };

    template<typename NodeType>
    NodeType& Graph<NodeType>::node(const int id)
    {
        return const_cast<NodeType&>(static_cast<const Graph*>(this)->node(id));
    }

    template<typename NodeType>
    const NodeType& Graph<NodeType>::node(const int id) const
    {
        const auto iter = nodes_.find(id);
        assert(iter != nodes_.end());
        return *iter;
    }

    template<typename NodeType>
    stl::Span<const int> Graph<NodeType>::neighbors(int node_id) const
    {
        const auto iter = node_neighbors_.find(node_id);
        assert(iter != node_neighbors_.end());
        return *iter;
    }

    template<typename NodeType>
    stl::Span<const typename Graph<NodeType>::Edge> Graph<NodeType>::edges() const
    {
        return edges_.elements();
    }

    template<typename NodeType>
    size_t Graph<NodeType>::num_edges_from_node(const int id) const
    {
        auto iter = edges_from_node_.find(id);
        assert(iter != edges_from_node_.end());
        return *iter;
    }

    template<typename NodeType>
    int Graph<NodeType>::insert_node(const NodeType& node)
    {
        const int id = current_id_++;
        assert(!nodes_.contains(id));
        nodes_.insert(id, node);
        edges_from_node_.insert(id, 0);
        node_neighbors_.insert(id, std::vector<int>());
        return id;
    }

    template<typename NodeType>
    void Graph<NodeType>::erase_node(const int id)
    {

        // first, remove any potential dangling edges
        {
            static std::vector<int> edges_to_erase;

            for (const Edge& edge : edges_.elements())
            {
                if (edge.contains(id))
                {
                    edges_to_erase.push_back(edge.id);
                }
            }

            for (const int edge_id : edges_to_erase)
            {
                erase_edge(edge_id);
            }

            edges_to_erase.clear();
        }

        nodes_.erase(id);
        edges_from_node_.erase(id);
        node_neighbors_.erase(id);
    }

    template<typename NodeType>
    int Graph<NodeType>::insert_edge(const int from, const int to)
    {
        const int id = current_id_++;
        assert(!edges_.contains(id));
        assert(nodes_.contains(from));
        assert(nodes_.contains(to));
        edges_.insert(id, Edge(id, from, to));

        // update neighbor count
        assert(edges_from_node_.contains(from));
        *edges_from_node_.find(from) += 1;
        // update neighbor list
        assert(node_neighbors_.contains(from));
        node_neighbors_.find(from)->push_back(to);

        return id;
    }

    template<typename NodeType>
    void Graph<NodeType>::erase_edge(const int edge_id)
    {
        // This is a bit lazy, we find the pointer here, but we refind it when we erase the edge based
        // on id key.
        assert(edges_.contains(edge_id));
        const Edge& edge = *edges_.find(edge_id);

        // update neighbor count
        assert(edges_from_node_.contains(edge.from));
        int& edge_count = *edges_from_node_.find(edge.from);
        assert(edge_count > 0);
        edge_count -= 1;

        // update neighbor list
        {
            assert(node_neighbors_.contains(edge.from));
            auto neighbors = node_neighbors_.find(edge.from);
            auto iter = std::find(neighbors->begin(), neighbors->end(), edge.to);
            assert(iter != neighbors->end());
            neighbors->erase(iter);
        }

        edges_.erase(edge_id);
    }

    template<typename NodeType, typename Visitor>
    void dfs_traverse(const Graph<NodeType>& graph, const int start_node, Visitor visitor)
    {
        std::stack<int> stack;

        stack.push(start_node);

        while (!stack.empty())
        {
            const int current_node = stack.top();
            stack.pop();

            visitor(current_node);

            for (const int neighbor : graph.neighbors(current_node))
            {
                stack.push(neighbor);
            }
        }
    }

    enum class NodeType
    {
        add,
        multiply,
        output,
        sine,
        time,
        value,
        modulus
    };

    struct Node
    {
        NodeType type;
        float value;

        explicit Node(const NodeType t) : type(t), value(0.f) {}

        Node(const NodeType t, const float v) : type(t), value(v) {}
    };

    template<typename T>
    BaleaEngine::Graphics::Color Graph<T>::evaluate()
    {
        std::stack<int> postorder;

        if (node_neighbors_.find(root_node) == node_neighbors_.end())
            return BaleaEngine::Graphics::Color(1, 1, 1, 1);

        dfs_traverse(
            *this, root_node, [&postorder](const int node_id) -> void { postorder.push(node_id); });

        std::stack<float> value_stack;
        while (!postorder.empty())
        {
            const int id = postorder.top();
            postorder.pop();
            const Node node_ = node(id);

            switch (node_.type)
            {
            case NodeType::add:
            {
                const float rhs = value_stack.top();
                value_stack.pop();
                const float lhs = value_stack.top();
                value_stack.pop();
                value_stack.push(lhs + rhs);
            }
            break;
            case NodeType::multiply:
            {
                const float rhs = value_stack.top();
                value_stack.pop();
                const float lhs = value_stack.top();
                value_stack.pop();
                value_stack.push(rhs * lhs);
            }
            break;
            case NodeType::modulus:
            {
                float rhs = value_stack.top();
                value_stack.pop();
                float lhs = value_stack.top();
                value_stack.pop();
                value_stack.push(static_cast<int>(rhs) % static_cast<int>(lhs));
            }
            break;
            case NodeType::sine:
            {
                const float x = value_stack.top();
                value_stack.pop();
                const float res = std::abs(std::sin(x));
                value_stack.push(res);
            }
            break;
            case NodeType::time:
            {
                value_stack.push(mTime->mFrames);
            }
            break;
            case NodeType::value: {

                if (num_edges_from_node(id) == 0ull)
                {
                    value_stack.push(node_.value);
                }
            }

                                break;
            default:
                break;
            }
        }

        const float a = value_stack.top();
        value_stack.pop();
        const float b = value_stack.top();
        value_stack.pop();
        const float g = value_stack.top();
        value_stack.pop();
        const float r = value_stack.top();
        value_stack.pop();

        return BaleaEngine::Graphics::Color(r, g, b, a);
    }
} // namespace example
