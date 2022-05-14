#ifndef NODE_H
#define NODE_H

#include <vector>
#include "../Aliases.h"

namespace hagame {
	namespace utils {
		template <class T>
		struct ForwardNode {
			Ptr<ForwardNode> next;
			T value;
		};

		template <class T>
		struct BackwardNode {
			Ptr<BackwardNode> prev;
			T value;
		};

		template <class T>
		struct Node {
			Ptr<Node> next;
			Ptr<Node> prev;
			T value;
		};

		template <class T>
		struct ParentNode {
			T value;
			std::vector<Ptr<ParentNode>> children;
		};
	}
}

#endif
