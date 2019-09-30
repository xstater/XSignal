#ifndef _XSIGNAL_H_
#define _XSIGNAL_H_

#include <list>

#define XSignalGroup(name) \
struct name
#define __signal__ virtual
#define XSignal(name,signal_group) signal_group

#define XSlotGroup(name,signal_group) \
struct name : public signal_group
#define __slot__ virtual

#define XConnect
#define XDisconnect
#define XEmit

namespace xsignal{
    template<class Type>
    class Signal{
    public:
        Signal
    protected:
    private:
        std::list<Type *> m_slots;
    };
}

#endif
