#pragma once

#include <vector>
#include <memory>
#include <bits/types/struct_iovec.h>
#include <string.h>

class BaseRemote {

public:

    BaseRemote(uintptr_t _address, size_t _size);

    BaseRemote(uintptr_t _address, size_t _size, BaseRemote *rm);

    ~BaseRemote();

    size_t size = 0;
    uintptr_t address = 0;




    static void ReadAll(pid_t pid);

    static void WriteAll(pid_t pid);

    bool bValuedChanged = false;
    bool bValid = false;
protected:
    uintptr_t *value = nullptr;
    uintptr_t *pOther = nullptr;

    iovec local{nullptr, 0};
    iovec remote{nullptr, 0};

};

template <typename T>
class RemoteValue : public BaseRemote {

public:
    explicit RemoteValue<T>(uintptr_t addy, size_t _size = sizeof(T)) : BaseRemote(addy, _size) {}
    RemoteValue<T>(BaseRemote* rm, uintptr_t offset, size_t _size = sizeof(T)) : BaseRemote(offset, _size, rm) {}


     T &Get() {

         return *(T *) value;
     };

     uintptr_t GetAddress()  {

        if (this->pOther != nullptr) {
            return (uintptr_t) (*this->pOther + address);
        }
        else
            return address;

    }

    void Set(const T& newVal) {

        memcpy(value, &newVal, sizeof(T));
        bValuedChanged = true;

    }

    RemoteValue<T>& operator=(const T& other){
         Set(other);
        return *this;
     }


    bool operator==(const T& other){
         return Get() == other;
    }


    bool operator>(const T& other) {
        return Get() > other;
    }


    bool operator<(const T& other) {
        return Get() < other;
    }

    bool operator>=(const T& other) {
        return Get() >= other;
    }

    bool operator<=(const T& other) {
        return Get() < other;
    }


#if T == bool

    explicit operator bool()  {
        return Get() == 1;
    }

#endif

};

