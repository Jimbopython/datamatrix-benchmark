#pragma once

#include <coroutine>

namespace sfdm {
    template<typename ResultKind>
    class ResultStream {
    public:
        struct promise_type {
            ResultKind current;

            ResultStream get_return_object() {
                return ResultStream{std::coroutine_handle<promise_type>::from_promise(*this)};
            }

            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            std::suspend_always yield_value(ResultKind value) {
                current = std::move(value);
                return {};
            }

            void return_void() {}
            void unhandled_exception() {}
        };

        using handle_t = std::coroutine_handle<promise_type>;

        explicit ResultStream(handle_t h) : m_handle(h) {}
        ResultStream(ResultStream &&other) noexcept : m_handle(other.m_handle) { other.m_handle = nullptr; }

        ~ResultStream() {
            if (m_handle) {
                m_handle.destroy();
            }
        }

        bool next() {
            if (!m_handle || m_handle.done())
                return false;
            m_handle.resume();
            return !m_handle.done();
        }

        const ResultKind &value() const { return m_handle.promise().current; }

    private:
        handle_t m_handle;
    };
} // namespace sfdm
