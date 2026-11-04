#ifndef D1137048_1F02_43F9_851D_5115BF6A6ADC
#define D1137048_1F02_43F9_851D_5115BF6A6ADC

#include <memory>

template<typename V, typename E>
class Result {
private:
    std::unique_ptr<V> value_;
    std::unique_ptr<E> error_;

    Result(std::unique_ptr<V> value, std::unique_ptr<E> error)
        : value_(std::move(value)),
          error_(std::move(error)) {}

public:
    static Result success(V value) {
        return Result(std::make_unique<V>(std::move(value)), nullptr);
    }

    static Result failure(E error) {
        return Result(nullptr, std::make_unique<E>(std::move(error)));
    }

    bool hasValue() const {
        return value_ != nullptr;
    }

    explicit operator bool() const {
        return hasValue();
    }

    const V& value() const {
        if (!value_) {
            throw std::logic_error("Result does not contain a value");
        }
        return *value_;
    }

    const E& error() const {
        if (!error_) {
            throw std::logic_error("Result does not contain an error");
        }
        return *error_;
    }
};

#endif /* D1137048_1F02_43F9_851D_5115BF6A6ADC */
