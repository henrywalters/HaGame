#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include "CyclicalLinkedList.h"

namespace hagame {
	namespace utils {
		
		/*
			The ArrayQueue provides a queue interface overtop a contiguous chunk of memory with O(1) push / pop operations.

			The cost of this is O(2N) memory consumption.
		*/
		template<class T, size_t Size>
		class ArrayQueue {
		public:

			ArrayQueue() {
				m_list.push(-1);
				m_head = m_list.raw();
				m_tail = m_list.raw();
			}

			void enqueue(T value) {
				if (m_size == Size) {
					throw new std::exception("ArrayQueue Overflow! Exceeded Size");
				}
			}

			T dequeue() {

			}

		private:

			CyclicalLinkedList<int> m_list;
			std::array<T, Size> m_data;

			Ptr<Node<int>> m_head, m_tail;
			size_t m_size = 0;
		};
	}
}

#endif
