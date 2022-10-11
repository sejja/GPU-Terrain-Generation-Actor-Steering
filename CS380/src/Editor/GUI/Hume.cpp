#include <Shared.h>

namespace BaleaEditor {

    template<class T>
    T clamp(T x, T a, T b)
    {
        return min(b, max(x, a));
    }

    static float current_time_seconds = 0.f;
    static Graph<Node>* graph_ = nullptr;

    void SelectGraph(Graph<Node>* graph) {

        graph_ = graph;

    }

    class ColorNodeEditor
    {
    public:
        ColorNodeEditor() {}

        void show()
        {

            if (!graph_)
                return;

            // The node editor window
            ImGui::Begin("Hume Editor WIP");

            imnodes::BeginNodeEditor();

            // Handle new nodes
            // These are driven by the user, so we place this code before rendering the nodes
            {
                const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                    imnodes::IsEditorHovered() &&
                    InputMgr->isKeyPressed('A');

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
                if (!ImGui::IsAnyItemHovered() && open_popup)
                {
                    ImGui::OpenPopup("add node");
                }

                if (ImGui::BeginPopup("add node"))
                {
                    const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

                    if (ImGui::MenuItem("add"))
                    {
                        const Node value(NodeType::value, 0.f);
                        const Node op(NodeType::add);

                        UiNode ui_node;
                        ui_node.type = UiNodeType::add;
                        ui_node.add.lhs = graph_->insert_node(value);
                        ui_node.add.rhs = graph_->insert_node(value);
                        ui_node.id = graph_->insert_node(op);

                        graph_->insert_edge(ui_node.id, ui_node.add.lhs);
                        graph_->insert_edge(ui_node.id, ui_node.add.rhs);

                        graph_->UInodes_.push_back(ui_node);
                        imnodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                    }

                    if (ImGui::MenuItem("multiply"))
                    {
                        const Node value(NodeType::value, 0.f);
                        const Node op(NodeType::multiply);

                        UiNode ui_node;
                        ui_node.type = UiNodeType::multiply;
                        ui_node.multiply.lhs = graph_->insert_node(value);
                        ui_node.multiply.rhs = graph_->insert_node(value);
                        ui_node.id = graph_->insert_node(op);

                        graph_->insert_edge(ui_node.id, ui_node.multiply.lhs);
                        graph_->insert_edge(ui_node.id, ui_node.multiply.rhs);

                        graph_->UInodes_.push_back(ui_node);
                        imnodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                    }

                    if (ImGui::MenuItem("modulus"))
                    {
                        const Node value(NodeType::value, 0.f);
                        const Node op(NodeType::modulus);

                        UiNode ui_node;
                        ui_node.type = UiNodeType::modulus;
                        ui_node.modulus.lhs = graph_->insert_node(value);
                        ui_node.modulus.rhs = graph_->insert_node(value);
                        ui_node.id = graph_->insert_node(op);

                        graph_->insert_edge(ui_node.id, ui_node.modulus.lhs);
                        graph_->insert_edge(ui_node.id, ui_node.modulus.rhs);

                        graph_->UInodes_.push_back(ui_node);
                        imnodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                    }

                    if (ImGui::MenuItem("output"))
                    {
                        const Node value(NodeType::value, 0.f);
                        const Node out(NodeType::output);

                        UiNode ui_node;
                        ui_node.type = UiNodeType::output;
                        ui_node.output.r = graph_->insert_node(value);
                        ui_node.output.g = graph_->insert_node(value);
                        ui_node.output.b = graph_->insert_node(value);
                        ui_node.output.a = graph_->insert_node(value);
                        ui_node.id = graph_->insert_node(out);

                        graph_->insert_edge(ui_node.id, ui_node.output.r);
                        graph_->insert_edge(ui_node.id, ui_node.output.g);
                        graph_->insert_edge(ui_node.id, ui_node.output.b);
                        graph_->insert_edge(ui_node.id, ui_node.output.a);

                        graph_->UInodes_.push_back(ui_node);
                        imnodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                        graph_->root_node = ui_node.id;

                    }

                    if (ImGui::MenuItem("sine"))
                    {
                        const Node value(NodeType::value, 0.f);
                        const Node op(NodeType::sine);

                        UiNode ui_node;
                        ui_node.type = UiNodeType::sine;
                        ui_node.sine.input = graph_->insert_node(value);
                        ui_node.id = graph_->insert_node(op);

                        graph_->insert_edge(ui_node.id, ui_node.sine.input);

                        graph_->UInodes_.push_back(ui_node);
                        imnodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                    }

                    if (ImGui::MenuItem("time"))
                    {
                        UiNode ui_node;
                        ui_node.type = UiNodeType::time;
                        ui_node.id = graph_->insert_node(Node(NodeType::time));

                        graph_->UInodes_.push_back(ui_node);
                        imnodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                    }

                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar();
            }

            for (const UiNode& node : graph_->UInodes_)
            {
                switch (node.type)
                {
                case UiNodeType::add:
                {
                    const float node_width = 100.f;
                    imnodes::BeginNode(node.id);

                    imnodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted("add");
                    imnodes::EndNodeTitleBar();
                    {
                        imnodes::BeginInputAttribute(node.add.lhs);
                        const float label_width = ImGui::CalcTextSize("left").x;
                        ImGui::TextUnformatted("left");
                        if (graph_->num_edges_from_node(node.add.lhs) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat("##hidelabel", &graph_->node(node.add.lhs).value, 0.01f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    {
                        imnodes::BeginInputAttribute(node.add.rhs);
                        const float label_width = ImGui::CalcTextSize("right").x;
                        ImGui::TextUnformatted("right");
                        if (graph_->num_edges_from_node(node.add.rhs) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat("##hidelabel", &graph_->node(node.add.rhs).value, 0.01f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginOutputAttribute(node.id);
                        const float label_width = ImGui::CalcTextSize("result").x;
                        ImGui::Indent(node_width - label_width);
                        ImGui::TextUnformatted("result");
                        imnodes::EndOutputAttribute();
                    }

                    imnodes::EndNode();
                }
                break;
                case UiNodeType::multiply:
                {
                    const float node_width = 100.0f;
                    imnodes::BeginNode(node.id);

                    imnodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted("multiply");
                    imnodes::EndNodeTitleBar();

                    {
                        imnodes::BeginInputAttribute(node.multiply.lhs);
                        const float label_width = ImGui::CalcTextSize("left").x;
                        ImGui::TextUnformatted("left");
                        if (graph_->num_edges_from_node(node.multiply.lhs) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.multiply.lhs).value, 0.01f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }


                    {
                        imnodes::BeginInputAttribute(node.multiply.rhs);
                        const float label_width = ImGui::CalcTextSize("right").x;
                        ImGui::TextUnformatted("right");
                        if (graph_->num_edges_from_node(node.multiply.rhs) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.multiply.rhs).value, 0.01f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginOutputAttribute(node.id);
                        const float label_width = ImGui::CalcTextSize("result").x;
                        ImGui::Indent(node_width - label_width);
                        ImGui::TextUnformatted("result");
                        imnodes::EndOutputAttribute();
                    }

                    imnodes::EndNode();
                }
                break;

                case UiNodeType::modulus:
                {
                    const float node_width = 100.0f;
                    imnodes::BeginNode(node.id);

                    imnodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted("modulus");
                    imnodes::EndNodeTitleBar();

                    {
                        imnodes::BeginInputAttribute(node.modulus.lhs);
                        const float label_width = ImGui::CalcTextSize("left").x;
                        ImGui::TextUnformatted("left");
                        if (graph_->num_edges_from_node(node.modulus.lhs) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.modulus.lhs).value, 0.01f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }


                    {
                        imnodes::BeginInputAttribute(node.modulus.rhs);
                        const float label_width = ImGui::CalcTextSize("right").x;
                        ImGui::TextUnformatted("right");
                        if (graph_->num_edges_from_node(node.modulus.rhs) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.modulus.rhs).value, 0.01f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginOutputAttribute(node.id);
                        const float label_width = ImGui::CalcTextSize("result").x;
                        ImGui::Indent(node_width - label_width);
                        ImGui::TextUnformatted("result");
                        imnodes::EndOutputAttribute();
                    }

                    imnodes::EndNode();
                }
                break;

                case UiNodeType::output:
                {
                    const float node_width = 100.0f;
                    imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(11, 109, 191, 255));
                    imnodes::PushColorStyle(
                        imnodes::ColorStyle_TitleBarHovered, IM_COL32(45, 126, 194, 255));
                    imnodes::PushColorStyle(
                        imnodes::ColorStyle_TitleBarSelected, IM_COL32(81, 148, 204, 255));
                    imnodes::BeginNode(node.id);

                    imnodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted("output");
                    imnodes::EndNodeTitleBar();

                    ImGui::Dummy(ImVec2(node_width, 0.f));
                    {
                        imnodes::BeginInputAttribute(node.output.r);
                        const float label_width = ImGui::CalcTextSize("r").x;
                        ImGui::TextUnformatted("r");
                        if (graph_->num_edges_from_node(node.output.r) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.output.r).value, 0.01f, 0.f, 1.0f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginInputAttribute(node.output.g);
                        const float label_width = ImGui::CalcTextSize("g").x;
                        ImGui::TextUnformatted("g");
                        if (graph_->num_edges_from_node(node.output.g) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.output.g).value, 0.01f, 0.f, 1.f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginInputAttribute(node.output.b);
                        const float label_width = ImGui::CalcTextSize("b").x;
                        ImGui::TextUnformatted("b");
                        if (graph_->num_edges_from_node(node.output.b) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.output.b).value, 0.01f, 0.f, 1.0f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginInputAttribute(node.output.a);
                        const float label_width = ImGui::CalcTextSize("a").x;
                        ImGui::TextUnformatted("a");
                        if (graph_->num_edges_from_node(node.output.a) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.output.a).value, 0.01f, 0.f, 1.0f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    imnodes::EndNode();
                    imnodes::PopColorStyle();
                    imnodes::PopColorStyle();
                    imnodes::PopColorStyle();
                }
                break;
          
                case UiNodeType::sine:
                {
                    const float node_width = 100.0f;
                    imnodes::BeginNode(node.id);

                    imnodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted("sine");
                    imnodes::EndNodeTitleBar();

                    {
                        imnodes::BeginInputAttribute(node.sine.input);
                        const float label_width = ImGui::CalcTextSize("number").x;
                        ImGui::TextUnformatted("number");
                        if (graph_->num_edges_from_node(node.sine.input) == 0ull)
                        {
                            ImGui::SameLine();
                            ImGui::PushItemWidth(node_width - label_width);
                            ImGui::DragFloat(
                                "##hidelabel", &graph_->node(node.sine.input).value, 0.01f, 0.f, 1.0f);
                            ImGui::PopItemWidth();
                        }
                        imnodes::EndInputAttribute();
                    }

                    ImGui::Spacing();

                    {
                        imnodes::BeginOutputAttribute(node.id);
                        const float label_width = ImGui::CalcTextSize("output").x;
                        ImGui::Indent(node_width - label_width);
                        ImGui::TextUnformatted("output");
                        imnodes::EndInputAttribute();
                    }

                    imnodes::EndNode();
                }
                break;
                case UiNodeType::time:
                {
                    imnodes::BeginNode(node.id);

                    imnodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted("time");
                    imnodes::EndNodeTitleBar();

                    imnodes::BeginOutputAttribute(node.id);
                    ImGui::Text("output");
                    imnodes::EndOutputAttribute();

                    imnodes::EndNode();
                }
                break;
                }
            }

            for (const auto& edge : graph_->edges())
            {
                // If edge doesn't start at value, then it's an internal edge, i.e.
                // an edge which links a node's operation to its input. We don't
                // want to render node internals with visible links.
                if (graph_->node(edge.from).type != NodeType::value)
                    continue;

                imnodes::Link(edge.id, edge.from, edge.to);
            }

            imnodes::EndNodeEditor();

            // Handle new links
            // These are driven by Imnodes, so we place the code after EndNodeEditor().

            {
                int start_attr, end_attr;
                if (imnodes::IsLinkCreated(&start_attr, &end_attr))
                {
                    const NodeType start_type = graph_->node(start_attr).type;
                    const NodeType end_type = graph_->node(end_attr).type;

                    const bool valid_link = start_type != end_type;
                    if (valid_link)
                    {
                        // Ensure the edge is always directed from the value to
                        // whatever produces the value
                        if (start_type != NodeType::value)
                        {
                            std::swap(start_attr, end_attr);
                        }
                        graph_->insert_edge(start_attr, end_attr);
                    }
                }
            }

            // Handle deleted links

            {
                int link_id;
                if (imnodes::IsLinkDestroyed(&link_id))
                {
                    graph_->erase_edge(link_id);
                }
            }

            {
                const int num_selected = imnodes::NumSelectedLinks();
                if (num_selected > 0 && InputMgr->isKeyPressed('X'))
                {
                    static std::vector<int> selected_links;
                    selected_links.resize(static_cast<size_t>(num_selected));
                    imnodes::GetSelectedLinks(selected_links.data());
                    for (const int edge_id : selected_links)
                    {
                        graph_->erase_edge(edge_id);
                    }
                }
            }

            {
                const int num_selected = imnodes::NumSelectedNodes();
                if (num_selected > 0 && InputMgr->isKeyPressed('X'))
                {
                    static std::vector<int> selected_nodes;
                    selected_nodes.resize(static_cast<size_t>(num_selected));
                    imnodes::GetSelectedNodes(selected_nodes.data());
                    for (const int node_id : selected_nodes)
                    {
                        graph_->erase_node(node_id);
                        auto iter = std::find_if(
                            graph_->UInodes_.begin(), graph_->UInodes_.end(), [node_id](const UiNode& node) -> bool {
                                return node.id == node_id;
                            });
                        // Erase any additional internal nodes
                        switch (iter->type)
                        {
                        case UiNodeType::add:
                            graph_->erase_node(iter->add.lhs);
                            graph_->erase_node(iter->add.rhs);
                            break;
                        case UiNodeType::multiply:
                            graph_->erase_node(iter->multiply.lhs);
                            graph_->erase_node(iter->multiply.rhs);
                            break;
                        case UiNodeType::modulus:
                            graph_->erase_node(iter->modulus.lhs);
                            graph_->erase_node(iter->modulus.rhs);
                            break;
                        case UiNodeType::output:
                            graph_->erase_node(iter->output.r);
                            graph_->erase_node(iter->output.g);
                            graph_->erase_node(iter->output.b);
                            graph_->erase_node(iter->output.a);
                            graph_->root_node = -1;
                            break;
                        case UiNodeType::sine:
                            graph_->erase_node(iter->sine.input);
                            break;
                        default:
                            break;
                        }
                        graph_->UInodes_.erase(iter);
                    }
                }
            }

            ImGui::End();

        }
    };

    static ColorNodeEditor color_editor;
} // namespace example

namespace BaleaEditor {

    void HumeWindow::Initialize() {
        imnodes::IO& io = imnodes::GetIO();
        io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;

    }

    void HumeWindow::Present() {

        BaleaEditor::color_editor.show();

    }
}