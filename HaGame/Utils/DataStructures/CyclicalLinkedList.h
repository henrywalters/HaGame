#ifndef CYCLICAL_LINKED_LIST_H
#define CYCLICAL_LINKED_LIST_H

#include "./Node.h"

namespace hagame {
	namespace utils {

		template <class T>
		class CyclicalLinkedList {
		public:

			// Adds an element at the current tail
			void push(T value) {
				auto node = std::make_shared<Node<T>>();
				node->value = value;
				if (m_tail == nullptr) {
					m_tail = node;
					m_tail->next = m_tail;
					m_tail->prev = m_tail;
				}
				else {
					node->next = m_tail->next;
					m_tail->next = node;
					node->prev = m_tail;
					m_tail = node;
				}

				m_size++;
			}

			// Adds an element directly before the current tail
			void pushBefore(T value) {
				auto node = std::make_shared<Node<T>>();
				node->value = value;
				if (m_tail == nullptr) {
					m_tail = node;
					m_tail->next = m_tail;
					m_tail->prev = m_tail;
				}
				else {
					m_tail->prev->next = node;
					node->prev = m_tail->prev;
					m_tail->prev = node;
					node->next = m_tail;
				}

				m_size++;
			}

			// Removes the current tail
			void pop() {

				if (m_size == 0) return;

				auto node = m_tail;

				m_tail->prev->next = m_tail->next;
				m_tail->next->prev = m_tail->prev;

				m_tail = node->prev;

				node.reset();

				m_size--;
			}

			T value() {
				return m_tail->value;
			}

			Ptr<Node<T>> raw() {
				return m_tail;
			}

			size_t size() { return m_size; }

			bool isEmpty() {
				return m_size == 0;
			}

			CyclicalLinkedList& operator++() {
				m_tail = m_tail->next;
				return *this;
			}

			CyclicalLinkedList& operator--() {
				m_tail = m_tail->prev;
				return *this;
			}

			~CyclicalLinkedList() {
				auto node = m_tail;
				while (node != nullptr) {
					auto tmp = node;
					node = tmp->next;
					node.reset();
				}
			}

		private:

			size_t m_size;
			Ptr<Node<T>> m_tail;

		};
	}
}

#endif
