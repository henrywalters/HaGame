#ifndef ASTAR_H
#define ASTAR_H

#include "./../Aliases.h"

namespace hagame {
	namespace utils {

		enum AStarHeuristic {
			Euclidean,
			Manhatten,
		};

		struct AStarNode {

			float g, h;

			// The cost to traverse the node from (0 to 1). 0 Means there is no cost, 1 means it is impossible to traverse.
			double cost = 0.0;

			// The position of the node for heuristics
			Vec3 position;

			Ptr<AStarNode> parent;

			float getF() {
				return h + g;
			}

			bool operator==(const AStarNode& node) {
				return position == node.position;
			}

			bool operator!=(const AStarNode& node) {
				return position != node.position;
			}
		};

		using AStarNeighborFn = std::function<Array<Ptr<AStarNode>>(Ptr<AStarNode>)>;

		class AStar {
		private:

			float getDistance(Ptr<AStarNode> a, Ptr<AStarNode> b) {
				switch (heuristic) {
				case AStarHeuristic::Euclidean:
					return (b->position - a->position).magnitude();
				case AStarHeuristic::Manhatten:
					Vec3 delta = b->position - a->position;
					return delta[0] + delta[1] + delta[2];
				}
			}

			void retracePath(Ptr<AStarNode> a, Ptr<AStarNode> b) {
				Ptr<AStarNode> current = b;
				while (current != a) {
					path.push_back(current);
					if (current->parent == NULL) {
						break;
					}
					current = current->parent;
				}
			}

		public:

			AStarHeuristic heuristic = AStarHeuristic::Euclidean;
			Ptr<AStarNode> start;
			Ptr<AStarNode> goal;
			Array<Ptr<AStarNode>> path;

			Map<float, Ptr<AStarNode>> closedList;
			Array<Ptr<AStarNode>> openList;
			//std::priority_queue<std::tuple<double, unsigned int>, Array<std::tuple<double, unsigned int>>, std::greater<std::tuple<double, unsigned int>>> openList;
			
			bool pathExists = false;
			bool complete = false;
			int iterations = 0;

			AStarNeighborFn getNeighbors;

			AStar() :
				getNeighbors([](Ptr<AStarNode> node) { throw new std::exception("getNeighbors is not defined on AStar instance"); return Array<Ptr<AStarNode>>(); }),
				start(NULL),
				goal(NULL)
			{}

			AStar(AStarNeighborFn _getNeighbors, Ptr<AStarNode> _start, Ptr<AStarNode> _goal):
				getNeighbors(_getNeighbors),
				start(_start),
				goal(_goal)
			{}

			static Optional<Array<Ptr<AStarNode>>> solve(AStarNeighborFn _getNeighbors, Ptr<AStarNode> _start, Ptr<AStarNode> _goal) {
				AStar astar = AStar(_getNeighbors, _start, _goal);
				while (!astar.complete) {
					astar.iterate();
				}
				return astar.pathExists ? Optional<Array<Ptr<AStarNode>>>(astar.path) : std::nullopt;
			}

			void init(Ptr<AStarNode> _start, Ptr<AStarNode> _goal) {
				start = _start;
				goal = _goal;
				path = Array<Ptr<AStarNode>>();
				openList.push_back(start);
			}

			void reset() {
				iterations = 0;
				complete = false;
				path.clear();
				openList.clear();
				closedList.clear();
			}

			void iterate() {
				if (!complete) {

					if (openList.size() == 0) {
						pathExists = false;
						complete = true;
					}
					else {
						Ptr<AStarNode> current = openList[0];
						int index = 0;
						for (int i = 0; i < openList.size(); i++) {
							if (openList[i]->getF() < current->getF() || openList[i]->getF() == current->getF() && openList[i]->h < current->h) {
								current = openList[i];
								index = i;
							}
						}
						openList.erase(openList.begin() + index);
						closedList.insert(std::make_pair(current->getF(), current));

						if (current == goal) {
							complete = true;
							pathExists = true;
							// retracePath(start, goal);
						}

						auto neighbors = getNeighbors(current);

						std::cout << current->position.toString() << " " << neighbors.size() << "\n";

						for (Ptr<AStarNode> neighbor : neighbors) {
							bool inClosed = false;
		
							for (auto& [f, node] : closedList) {
								if (node == neighbor) {
									inClosed = true;
									break;
								}
							}

							if (inClosed || neighbor->cost == 1.0) {
								continue;
							}
							float costToNeighbor = current->g + getDistance(current, neighbor) * neighbor->cost;

							bool inOpen = false;

							for (auto node : openList) {
								if (node == neighbor) {
									inOpen = true;
									break;
								}
							}

							if (costToNeighbor < neighbor->g || !inOpen) {
								neighbor->g = costToNeighbor;
								neighbor->h = getDistance(neighbor, goal);
								neighbor->parent = current;

								if (!inOpen) {
									openList.push_back(neighbor);
								}
							}
						}

						iterations++;
					}


				}
			}

		};
	}
}

#endif 
