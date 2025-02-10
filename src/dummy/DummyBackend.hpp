/*
 * (C) 2020 The University of Chicago
 *
 * See COPYRIGHT in top-level directory.
 */
#ifndef __DUMMY_BACKEND_HPP
#define __DUMMY_BACKEND_HPP

#include <YP/PhonebookInterface.hpp>

using json = nlohmann::json;

/**
 * Dummy implementation of an YP Backend.
 */
class DummyPhonebook : public YP::PhonebookInterface {

    thallium::engine m_engine;
    json             m_config;

    public:

    /**
     * @brief Constructor.
     */
    DummyPhonebook(thallium::engine engine, const json& config);

    /**
     * @brief Move-constructor.
     */
    DummyPhonebook(DummyPhonebook&&) = default;

    /**
     * @brief Copy-constructor.
     */
    DummyPhonebook(const DummyPhonebook&) = default;

    /**
     * @brief Move-assignment operator.
     */
    DummyPhonebook& operator=(DummyPhonebook&&) = default;

    /**
     * @brief Copy-assignment operator.
     */
    DummyPhonebook& operator=(const DummyPhonebook&) = default;

    /**
     * @brief Destructor.
     */
    virtual ~DummyPhonebook() = default;

    /**
     * @brief Get the phonebook's configuration as a JSON-formatted string.
     */
    std::string getConfig() const override;

    /**
     * @brief Compute the sum of two integers.
     *
     * @param x first integer
     * @param y second integer
     *
     * @return a Result containing the result.
     */
    YP::Result<int32_t> computeSum(int32_t x, int32_t y) override;

    /**
     * @brief Destroys the underlying phonebook.
     *
     * @return a Result<bool> instance indicating
     * whether the database was successfully destroyed.
     */
    YP::Result<bool> destroy() override;

    /**
     * @brief Static factory function used by the PhonebookFactory to
     * create a DummyPhonebook.
     *
     * @param engine Thallium engine
     * @param config JSON configuration for the phonebook
     *
     * @return a unique_ptr to a phonebook
     */
    static std::unique_ptr<YP::PhonebookInterface> create(const thallium::engine& engine, const json& config);

    /**
     * @brief Static factory function used by the PhonebookFactory to
     * open a DummyPhonebook.
     *
     * @param engine Thallium engine
     * @param config JSON configuration for the phonebook
     *
     * @return a unique_ptr to a phonebook
     */
    static std::unique_ptr<YP::PhonebookInterface> open(const thallium::engine& engine, const json& config);
};

#endif
