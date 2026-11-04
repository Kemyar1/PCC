#include "../core/library/Result.h"
#include "vendor/catch.hpp"

#include <stdexcept>
#include <utility>

namespace {
enum class TestError { InvalidValue };
using IntResult = Result<int, TestError>;
}

TEST_CASE("success_IntegerValue42_HasValueAndReturns42", "[result]") {
    const int expectedValue = 42;
    const bool expectedHasValue = true;
    const int inputValue = 42;

    auto result = IntResult::success(inputValue);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const int actualValue = result.value();
    REQUIRE(actualValue == expectedValue);
}

TEST_CASE("success_FalseValue_IsStillSuccessful", "[result]") {
    const bool expectedValue = false;
    const bool expectedHasValue = true;
    const bool expectedBoolValue = true;
    const bool inputValue = false;

    auto result = Result<bool, TestError>::success(inputValue);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const bool actualValue = result.value();
    const bool actualBoolValue = static_cast<bool>(result);
    REQUIRE(actualValue == expectedValue);
    REQUIRE(actualBoolValue == expectedBoolValue);
}

TEST_CASE("failure_ErrorCode_HasNoValueAndReturnsError", "[result]") {
    const TestError expectedError = TestError::InvalidValue;
    const bool expectedHasValue = false;
    const TestError inputError = TestError::InvalidValue;

    auto result = IntResult::failure(inputError);

    const bool actualHasValue = result.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const TestError actualError = result.error();
    REQUIRE(actualError == expectedError);
}

TEST_CASE("value_FailedResult_ThrowsLogicError", "[result]") {
    const bool expectedLogicError = true;
    const TestError inputError = TestError::InvalidValue;

    auto result = IntResult::failure(inputError);

    bool actualLogicError = false;
    try {
        result.value();
    } catch (const std::logic_error&) {
        actualLogicError = true;
    }

    REQUIRE(actualLogicError == expectedLogicError);
}

TEST_CASE("error_SuccessfulResult_ThrowsLogicError", "[result]") {
    const bool expectedLogicError = true;
    const int inputValue = 42;

    auto result = IntResult::success(inputValue);

    bool actualLogicError = false;
    try {
        result.error();
    } catch (const std::logic_error&) {
        actualLogicError = true;
    }

    REQUIRE(actualLogicError == expectedLogicError);
}

TEST_CASE("operatorBool_SuccessAndFailure_MatchesHasValue", "[result]") {
    const bool expectedSuccessBool = true;
    const bool expectedFailureBool = false;
    const int inputValue = 42;
    const TestError inputError = TestError::InvalidValue;

    auto success = IntResult::success(inputValue);
    auto failure = IntResult::failure(inputError);

    const bool actualSuccessBool = static_cast<bool>(success);
    const bool actualFailureBool = static_cast<bool>(failure);
    REQUIRE(actualSuccessBool == expectedSuccessBool);
    REQUIRE(actualFailureBool == expectedFailureBool);
}

TEST_CASE("moveConstructor_SuccessfulResult_TransfersValue", "[result]") {
    const int expectedValue = 42;
    const bool expectedMovedHasValue = true;
    const bool expectedOriginalHasValue = false;
    const bool expectedValueLogicError = true;
    const bool expectedErrorLogicError = true;
    const int inputValue = 42;

    auto original = IntResult::success(inputValue);
    auto moved = std::move(original);

    const bool actualMovedHasValue = moved.hasValue();
    REQUIRE(actualMovedHasValue == expectedMovedHasValue);

    const int actualValue = moved.value();
    const bool actualOriginalHasValue = original.hasValue();
    REQUIRE(actualValue == expectedValue);
    REQUIRE(actualOriginalHasValue == expectedOriginalHasValue);

    bool actualValueLogicError = false;
    try {
        original.value();
    } catch (const std::logic_error&) {
        actualValueLogicError = true;
    }
    REQUIRE(actualValueLogicError == expectedValueLogicError);

    bool actualErrorLogicError = false;
    try {
        original.error();
    } catch (const std::logic_error&) {
        actualErrorLogicError = true;
    }
    REQUIRE(actualErrorLogicError == expectedErrorLogicError);
}

TEST_CASE("moveConstructor_FailedResult_TransfersError", "[result]") {
    const TestError expectedError = TestError::InvalidValue;
    const bool expectedHasValue = false;
    const bool expectedLogicError = true;
    const TestError inputError = TestError::InvalidValue;

    auto original = IntResult::failure(inputError);
    auto moved = std::move(original);

    const bool actualHasValue = moved.hasValue();
    REQUIRE(actualHasValue == expectedHasValue);

    const TestError actualError = moved.error();
    REQUIRE(actualError == expectedError);

    bool actualLogicError = false;
    try {
        original.error();
    } catch (const std::logic_error&) {
        actualLogicError = true;
    }
    REQUIRE(actualLogicError == expectedLogicError);
}
