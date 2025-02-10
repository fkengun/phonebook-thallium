/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#ifndef __YP_PHONEBOOK_INTERFACE_HPP
#define __YP_PHONEBOOK_INTERFACE_HPP

#include <YP/Result.hpp>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include <thallium.hpp>

/**
 * @brief Helper class to register backend types into the backend factory.
 */
template<typename PhonebookInterfaceType>
class __YpBackendRegistration;

namespace YP {

/**
 * @brief Interface for phonebook backends. To build a new backend,
 * implement a class MyPhonebookInterface that inherits from PhonebookInterface, and put
 * YP_REGISTER_BACKEND(mybackend, MyPhonebookInterface); in a cpp file
 * that includes your backend class' header file.
 *
 * Your backend class should also have two static functions to
 * respectively create and open a phonebook:
 *
 * std::unique_ptr<PhonebookInterface> create(const json& config)
 * std::unique_ptr<PhonebookInterface> attach(const json& config)
 */
class PhonebookInterface {

    template<typename PhonebookInterfaceType>
    friend class ::__YpBackendRegistration;

    std::string m_name;

    public:

    /**
     * @brief Constructor.
     */
    PhonebookInterface() = default;

    /**
     * @brief Move-constructor.
     */
    PhonebookInterface(PhonebookInterface&&) = default;

    /**
     * @brief Copy-constructor.
     */
    PhonebookInterface(const PhonebookInterface&) = default;

    /**
     * @brief Move-assignment operator.
     */
    PhonebookInterface& operator=(PhonebookInterface&&) = default;

    /**
     * @brief Copy-assignment operator.
     */
    PhonebookInterface& operator=(const PhonebookInterface&) = default;

    /**
     * @brief Destructor.
     */
    virtual ~PhonebookInterface() = default;

    /**
     * @brief Return the name of backend.
     */
    const std::string& name() const {
        return m_name;
    }

    /**
     * @brief Returns a JSON-formatted configuration string.
     */
    virtual std::string getConfig() const = 0;

    /**
     * @brief Compute the sum of two integers.
     *
     * @param x first integer
     * @param y second integer
     *
     * @return a Result containing the result.
     */
    virtual Result<int32_t> computeSum(int32_t x, int32_t y) = 0;

    /**
     * @brief Destroys the underlying phonebook.
     *
     * @return a Result<bool> instance indicating
     * whether the database was successfully destroyed.
     */
    virtual Result<bool> destroy() = 0;

};

/**
 * @brief The PhonebookFactory contains functions to create
 * or open phonebooks.
 */
class PhonebookFactory {

    template<typename PhonebookInterfaceType>
    friend class ::__YpBackendRegistration;

    using json = nlohmann::json;

    public:

    PhonebookFactory() = delete;

    /**
     * @brief Creates a phonebook and returns a unique_ptr to the created instance.
     *
     * @param backend_name Name of the backend to use.
     * @param engine Thallium engine.
     * @param config Configuration object to pass to the backend's create function.
     *
     * @return a unique_ptr to the created Phonebook.
     */
    static std::unique_ptr<PhonebookInterface> createPhonebook(
            const std::string& backend_name,
            const thallium::engine& engine,
            const json& config);

    /**
     * @brief Opens an existing phonebook and returns a unique_ptr to the
     * created backend instance.
     *
     * @param backend_name Name of the backend to use.
     * @param engine Thallium engine.
     * @param config Configuration object to pass to the backend's open function.
     *
     * @return a unique_ptr to the created PhonebookInterface.
     */
    static std::unique_ptr<PhonebookInterface> openPhonebook(
            const std::string& backend_name,
            const thallium::engine& engine,
            const json& config);

    private:

    static std::unordered_map<std::string,
                std::function<std::unique_ptr<PhonebookInterface>(const thallium::engine&, const json&)>> create_fn;

    static std::unordered_map<std::string,
                std::function<std::unique_ptr<PhonebookInterface>(const thallium::engine&, const json&)>> open_fn;
};

} // namespace YP


#define YP_REGISTER_BACKEND(__backend_name, __backend_type) \
    static __YpBackendRegistration<__backend_type> __YP ## __backend_name ## _backend( #__backend_name )

template<typename PhonebookInterfaceType>
class __YpBackendRegistration {

    using json = nlohmann::json;

    public:

    __YpBackendRegistration(const std::string& backend_name)
    {
        YP::PhonebookFactory::create_fn[backend_name] = [backend_name](const thallium::engine& engine, const json& config) {
            auto p = PhonebookInterfaceType::create(engine, config);
            p->m_name = backend_name;
            return p;
        };
        YP::PhonebookFactory::open_fn[backend_name] = [backend_name](const thallium::engine& engine, const json& config) {
            auto p = PhonebookInterfaceType::open(engine, config);
            p->m_name = backend_name;
            return p;
        };
    }
};

#endif
