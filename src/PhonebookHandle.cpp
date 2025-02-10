/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#include "YP/PhonebookHandle.hpp"
#include "YP/Result.hpp"
#include "YP/Exception.hpp"

#include "ClientImpl.hpp"
#include "PhonebookHandleImpl.hpp"

#include <thallium/serialization/stl/string.hpp>
#include <thallium/serialization/stl/pair.hpp>

namespace YP {

PhonebookHandle::PhonebookHandle() = default;

PhonebookHandle::PhonebookHandle(const std::shared_ptr<PhonebookHandleImpl>& impl)
: self(impl) {}

PhonebookHandle::PhonebookHandle(const PhonebookHandle&) = default;

PhonebookHandle::PhonebookHandle(PhonebookHandle&&) = default;

PhonebookHandle& PhonebookHandle::operator=(const PhonebookHandle&) = default;

PhonebookHandle& PhonebookHandle::operator=(PhonebookHandle&&) = default;

PhonebookHandle::~PhonebookHandle() = default;

PhonebookHandle::operator bool() const {
    return static_cast<bool>(self);
}

Client PhonebookHandle::client() const {
    return Client(self->m_client);
}

Future<int32_t> PhonebookHandle::computeSum(
        int32_t x, int32_t y) const
{
    if(not self) throw Exception("Invalid YP::PhonebookHandle object");
    auto& rpc = self->m_client->m_compute_sum;
    auto& ph  = self->m_ph;
    auto async_response = rpc.on(ph).async(x, y);
    return Future<int32_t>{std::move(async_response)};
}

}
