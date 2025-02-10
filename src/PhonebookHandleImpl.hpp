/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#ifndef __YP_PHONEBOOK_HANDLE_IMPL_H
#define __YP_PHONEBOOK_HANDLE_IMPL_H

#include "ClientImpl.hpp"

namespace YP {

class PhonebookHandleImpl {

    public:

    std::shared_ptr<ClientImpl> m_client;
    tl::provider_handle         m_ph;

    PhonebookHandleImpl() = default;

    PhonebookHandleImpl(std::shared_ptr<ClientImpl> client,
                       tl::provider_handle&& ph)
    : m_client(std::move(client))
    , m_ph(std::move(ph)) {}
};

}

#endif
