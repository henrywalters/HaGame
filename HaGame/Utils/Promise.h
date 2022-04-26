#ifndef PROMISE_H
#define PROMISE_H

#include <iostream>
#include <functional>
#include <future>

namespace hagame {
	namespace utils {

		template <class T>
		using callback_t = std::function<void(T)>;

		template <class S, class E>
		using promise_function_t = std::function<void(callback_t<S>, callback_t<E>)>;

		enum class PromiseStatus {
			Pending,
			Resolved,
			Rejected
		};

		/*
			Based on the JavaScript promise, it is a class which represents the eventual completion or failure of an operation.
			The main difference with this class is the ability to run synchronously or asynchronously.

			Usage:

			Promise<bool, String, false>([x](auto resolve, auto reject) {
				if (isPrime(x)) resolve(true);

				reject("Is not prime");
			}).then([](bool isPrime) { std::cout << "Its prime!"; }), [](String error) { std::cout << "ERROR"; }));

		*/
		template <class S, class E, bool Async>
		class Promise {
		public:

			Promise(promise_function_t<S, E> functions):
				m_functions(functions),
				m_status(PromiseStatus::Pending),
				m_resolve_promise_fn([this](S successValue) {
					m_status = PromiseStatus::Resolved;
					m_resolve_callback(successValue);
				}),
				m_reject_promise_fn([this](E errorValue) {
					m_status = PromiseStatus::Rejected;
					m_reject_callback(errorValue);
				})
			{}

			// For synchronous promises, this must be run to execute the functions
			void update() {
				m_functions(m_resolve_promise_fn, m_reject_promise_fn);
			}

			void then(callback_t<S> resolve, callback_t<E> reject) {
				m_resolve_callback = resolve;
				m_reject_callback = reject;

				if (Async) {
					m_future = std::async(m_functions, m_resolve_promise_fn, m_reject_promise_fn);
				}
			}

			void then(callback_t<S> resolve) {
				m_resolve_callback = resolve;

				if (Async) {
					m_future = std::async(m_functions, m_resolve_promise_fn, m_reject_promise_fn);
				}
			}

			PromiseStatus getStatus() {
				return m_status;
			}

			bool isPending() {
				return m_status == PromiseStatus::Pending;
			}

		private:

			PromiseStatus m_status;
			promise_function_t<S, E> m_functions;
			callback_t<S> m_resolve_callback;
			callback_t<E> m_reject_callback;
			callback_t<S> m_resolve_promise_fn;
			callback_t<E> m_reject_promise_fn;

			std::future<void> m_future;
		};

	}
}


#endif
