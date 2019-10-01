#ifndef _XSIGNAL_H_
#define _XSIGNAL_H_

#include <vector>

#define XSignalGroup(name) \
struct name
#define __signal__ virtual
#define XSignal(name,signal_group) \
    xsignal::Signal<signal_group> name

#define XSlotGroup(name,signal_group) \
struct name : public signal_group
#define __slot__ virtual
#define XSlot(name,slot_group) slot_group name

#define XConnect(slot,signal) signal += &slot
#define XDisconnect(slot,signal) signal -= &slot
#define XEmit(signal,func) \
    for(auto ptr:signal)ptr->func

namespace xsignal{
    template<class Type>
    class Signal{
    public:
        using iterator = typename std::vector<Type*>::iterator;
        //using const_iterator = std::list<Type*>::const_iterator;
    
        Signal() = default;
        ~Signal() = default;
        
        Signal &operator+=(Type *ptr){
            m_slots.push_back(ptr);
            return *this;
        }
        Signal &operator-=(Type *ptr){
            for(auto itr = m_slots.begin();itr != m_slots.end();++itr){
                if(*itr == ptr){
                    m_slots.erase(itr);
                    break;
                }
            }
            return *this;
        }
        
        iterator begin(){
            return m_slots.begin();
        }
        iterator end(){
            return m_slots.end();
        }
    protected:
    private:
        std::vector<Type*> m_slots;
    };
}

#endif
