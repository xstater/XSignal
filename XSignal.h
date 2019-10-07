#ifndef _XSIGNAL_H_
#define _XSIGNAL_H_

#include <functional>
#include <type_traits>
#include <memory>
#include <list>

namespace xsignal{
    template<class Type>
    class Signal{};

    template <class Type>
    class Connection{};

    template<class ReturnType,class...ArgsType>
    class Connection<ReturnType(ArgsType...)>{
    public:

        using SignalType = Signal<ReturnType(ArgsType...)>;
        using iter_type = typename SignalType::iter_type;

        explicit Connection(SignalType &signal,iter_type itr)
                :m_signal(signal),m_itr(itr){}
        Connection(const Connection &) = delete;
        Connection(Connection &&) = default;
        ~Connection() = default;

        Connection &operator=(const Connection &) = delete;
        Connection &operator=(Connection &&) = default;

        void disconnect()noexcept{
            m_signal.__remove(m_itr);
        }

        auto value()
        -> typename std::enable_if<!std::is_void<ReturnType>::value,ReturnType>::type{
            return m_itr->ret_value;
        }
    protected:
    private:
        SignalType &m_signal;
        iter_type m_itr;
    };

    template<class...ArgsType>
    class Connection<void(ArgsType...)>{
    public:

        using SignalType = Signal<void(ArgsType...)>;
        using iter_type = typename SignalType::iter_type;

        explicit Connection(SignalType &signal,iter_type itr)
                :m_signal(signal),m_itr(itr){}
        Connection(const Connection &) = delete;
        Connection(Connection &&) = default;
        ~Connection() = default;

        Connection &operator=(const Connection &) = delete;
        Connection &operator=(Connection &&) = default;

        void disconnect()noexcept{
            m_signal.__remove(m_itr);
        }
    protected:
    private:
        SignalType &m_signal;
        iter_type m_itr;
    };

///ReturnType must have a default constructor!!!

    template<class ReturnType,class...ArgsType>
    class Signal<ReturnType(ArgsType...)>{
    public:
        struct Slot{
            template <class Function>
            explicit Slot(Function &&f)
                    :func(std::forward<Function>(f)){}
            std::function<ReturnType(ArgsType...)> func;
            ReturnType ret_value;
        };

        using iter_type = typename std::list<Slot>::iterator;
        using return_type = ReturnType;

        Signal() = default;
        ~Signal() = default;

        void __remove(iter_type itr)noexcept{
            m_slots.erase(itr);
        }

        template<class Function>
        auto connect(Function &&func)
        -> Connection<ReturnType(ArgsType...)>{
            m_slots.emplace_back(std::forward<Function>(func));
            return Connection<ReturnType(ArgsType...)>(*this,std::prev(m_slots.end()));
        }
        template<class Function>
        auto operator<<(Function &&func)
        -> Signal &{
            m_slots.emplace_back(std::forward<Function>(func));
            return *this;
        }

        template <class...ArgsType2>
        auto emit(ArgsType2&&...args)
        -> typename std::enable_if<!std::is_void<ReturnType>::value,ReturnType>::type{
            for(auto &slot:m_slots){
                slot.ret_value = slot.func(std::forward<ArgsType2>(args)...);
            }
            return m_slots.begin()->ret_value;
        }

        template <class...ArgsType2>
        auto operator()(ArgsType2&&...args)
        -> typename std::enable_if<!std::is_void<ReturnType>::value,ReturnType>::type{
            return emit(std::forward<ArgsType2>(args)...);
        }
    protected:
    private:
        std::list<Slot> m_slots;
    };

    template<class...ArgsType>
    class Signal<void(ArgsType...)>{
    public:
        struct Slot{
            template <class Function>
            explicit Slot(Function &&f)
                    :func(std::forward<Function>(f)){}
            std::function<void(ArgsType...)> func;
        };

        using iter_type = typename std::list<Slot>::iterator;
        using return_type = void;

        Signal() = default;
        ~Signal(){
            m_slots.clear();
        }

        void __remove(iter_type itr){
            m_slots.erase(itr);
        }

        template<class Function>
        auto connect(Function &&func)
        -> Connection<void(ArgsType...)>{
            m_slots.emplace_back(std::forward<Function>(func));
            return Connection<void(ArgsType...)>(*this,std::prev(m_slots.end()));
        }
        template<class Function>
        auto operator<<(Function &&func)
        -> Signal &{
            m_slots.emplace_back(std::forward<Function>(func));
            return *this;
        }

        template <class...ArgsType2>
        void emit(ArgsType2&&...args) {
            for(auto &itr:m_slots){
                itr.func(std::forward<ArgsType2>(args)...);
            }
        }
        template <class...ArgsType2>
        void operator()(ArgsType2&&...args) {
            emit(std::forward<ArgsType2>(args)...);
        }
    protected:
    private:
        std::list<Slot> m_slots;
    };
}

#endif