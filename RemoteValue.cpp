//
// Created by xrf on 12/12/18.
//

#include "RemoteValue.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <unistd.h>
#include <sys/uio.h>

static std::vector<BaseRemote*> allRemoteValues = std::vector<BaseRemote*>();

void BaseRemote::ReadAll(pid_t pid) {

    for (auto rd : allRemoteValues) {

        if(rd->pOther != nullptr) {
            rd->remote.iov_base = (void*)(*rd->pOther + rd->address);
        }

        process_vm_readv(pid, &rd->local, 1, &rd->remote, 1, 0);
        rd->bValuedChanged = false;
        rd->bValid = true;
    }
}

void BaseRemote::WriteAll(pid_t pid) {

    for (auto rd : allRemoteValues) {

        if(!rd->bValuedChanged) {
            continue;
        }

        if (rd->pOther != nullptr) {
            rd->remote.iov_base = (void *) (*rd->pOther + rd->address);
        }



        process_vm_writev(pid, &rd->local, 1, &rd->remote, 1, 0);
    }
}



BaseRemote::BaseRemote(uintptr_t _address, size_t _size) : size(_size), address(_address), pOther(nullptr) {
    value = (uintptr_t*)malloc(size);

    allRemoteValues.push_back(this);

    local = {(void*)value, size};
    remote = {(void*)address, size};

}


BaseRemote::BaseRemote(uintptr_t _address, size_t _size, BaseRemote* rm) : size(_size), address(_address), pOther(rm->value) {
    value = (uintptr_t*)malloc(size);

    allRemoteValues.push_back(this);
    local = {(void*)value, size};
    remote = {(void*)address, size};
}

BaseRemote::~BaseRemote() {
    auto index = std::find(allRemoteValues.begin(), allRemoteValues.end(), this);
    allRemoteValues.erase(index);
}




