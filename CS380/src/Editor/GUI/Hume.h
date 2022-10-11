#pragma once

namespace BaleaEditor {

	struct Node;

	template<typename T>
	class Graph;

	void SelectGraph(Graph<Node>* graph);

	class HumeWindow : public IWindow {

	public:
		void Initialize();
		void Present() override;

	};
}
