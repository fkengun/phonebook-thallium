/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#ifndef __YP_PHONEBOOK_HANDLE_HPP
#define __YP_PHONEBOOK_HANDLE_HPP

#include <thallium.hpp>
#include <memory>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YP/Client.hpp>
#include <YP/Exception.hpp>
#include <YP/Future.hpp>

namespace YP {

class Client;
class PhonebookHandleImpl;

/**
 * @brief A PhonebookHandle object is a handle for a remote phonebook
 * on a server. It enables invoking the phonebook's functionalities.
 */
class PhonebookHandle {

    friend class Client;

    public:

    /**
     * @brief Constructor. The resulting PhonebookHandle handle will be invalid.
     */
    PhonebookHandle();

    /**
     * @brief Copy-constructor.
     */
    PhonebookHandle(const PhonebookHandle&);

    /**
     * @brief Move-constructor.
     */
    PhonebookHandle(PhonebookHandle&&);

    /**
     * @brief Copy-assignment operator.
     */
    PhonebookHandle& operator=(const PhonebookHandle&);

    /**
     * @brief Move-assignment operator.
     */
    PhonebookHandle& operator=(PhonebookHandle&&);

    /**
     * @brief Destructor.
     */
    ~PhonebookHandle();

    /**
     * @brief Returns the client this phonebook has been opened with.
     */
    Client client() const;


    /**
     * @brief Checks if the PhonebookHandle instance is valid.
     */
    operator bool() const;

    /**
     * @brief Requests the target phonebook to compute the sum of two numbers.
     * If result is null, it will be ignored. If req is not null, this call
     * will be non-blocking and the caller is responsible for waiting on
     * the request.
     *
     * @param[in] x first integer
     * @param[in] y second integer
     *
     * @return a Future<int32_t> that can be awaited to get the result.
     */
    Future<int32_t> computeSum(int32_t x, int32_t y) const;

    private:

    /**
     * @brief Constructor is private. Use a Client object
     * to create a PhonebookHandle instance.
     *
     * @param impl Pointer to implementation.
     */
    PhonebookHandle(const std::shared_ptr<PhonebookHandleImpl>& impl);

    std::shared_ptr<PhonebookHandleImpl> self;
};

}

#endif
