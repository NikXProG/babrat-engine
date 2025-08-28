#pragma once
#include <type_traits>
#include <cstdint>
#include <atomic>
#include <memory>

#define BABRAT_PREDICT_TRUE(x) (__builtin_expect(false || (x), true))

#if defined(NDEBUG)
#define BABRAT_ASSERT(expr) \
(false ? static_cast<void>(expr) : static_cast<void>(0))
#else
#define BABRAT_ASSERT(expr)                           \
(BABRAT_PREDICT_TRUE((expr)) ? static_cast<void>(0) \
: [] { assert(false && #expr); }())  // NOLINT
#endif

using json = nlohmann::ordered_json;

namespace babrat {

    namespace serialization {

        typedef uint16_t field_id_t;
        const field_id_t MESSAGE_TERMINATOR_FIELD_ID = 0xFFFF;

        // Backport to c++11
        template <class...>
        using void_t = void;

        // Check if T is a vector, and provide access to the inner type
        template <typename T>
        struct is_vector : std::false_type {};
        template <typename T>
        struct is_vector<typename std::vector<T>> : std::true_type {
	        typedef T ELEMENT_TYPE;
        };


        template <typename T>
        struct is_pair : std::false_type {};
        template <typename T, typename U>
        struct is_pair<std::pair<T, U>> : std::true_type {
	        typedef T FIRST_TYPE;
	        typedef U SECOND_TYPE;
        };

        template <typename T>
        struct is_atomic : std::false_type {};

        template <typename T>
        struct is_atomic<std::atomic<T>> : std::true_type {
	        typedef T TYPE;
        };

        struct SerializationDefaultValue {

	        template <typename T = void>
	        static inline typename std::enable_if<is_atomic<T>::value, T>::type GetDefault() {
		        using INNER = typename is_atomic<T>::TYPE;
		        return static_cast<T>(GetDefault<INNER>());
	        }

	        template <typename T = void>
	        static inline bool IsDefault(const typename std::enable_if<is_atomic<T>::value, T>::type &value) {
		        using INNER = typename is_atomic<T>::TYPE;
		        return value == GetDefault<INNER>();
	        }

	        template <typename T = void>
	        static inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type GetDefault() {
		        return static_cast<T>(0);
	        }

	        template <typename T = void>
	        static inline bool IsDefault(const typename std::enable_if<std::is_arithmetic<T>::value, T>::type &value) {
		        return value == static_cast<T>(0);
	        }

	        template <typename T = void>
	        static inline typename std::enable_if<is_vector<T>::value, T>::type GetDefault() {
		        return T();
	        }

	        template <typename T = void>
	        static inline bool IsDefault(const typename std::enable_if<is_vector<T>::value, T>::type &value) {
		        return value.empty();
	        }

	        template <typename T = void>
	        static inline typename std::enable_if<std::is_same<T, std::string>::value, T>::type GetDefault() {
		        return T();
	        }

	        template <typename T = void>
	        static inline bool IsDefault(const typename std::enable_if<std::is_same<T, std::string>::value, T>::type &value) {
		        return value.empty();
	        }

        };

    } // namespace serialization

} // nnamespace babrat