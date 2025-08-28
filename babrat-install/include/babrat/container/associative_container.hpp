#pragma once

#include <utility>

namespace babrat {

    namespace container {

        template<typename t_key, typename t_value>
        class AssociativeContainer
        {

        public:

            class KeyValuePair final
            {

            public:

                t_key key;
                t_value value;

            public:

                KeyValuePair(
                        t_key const &key,
                        t_value const &value):
                        key(key),
                        value(value)
                {

                }

                KeyValuePair(
                        t_key const &key,
                        t_value &&value):
                        key(key),
                        value(std::move(value))
                {

                }

                KeyValuePair(
                        t_key &&k,
                        t_value &&v):
                        key(std::forward<t_key>(k)),
                        value(std::forward<t_value>(v))
                {

                }

                KeyValuePair(
                        t_key &&k,
                        t_value const &v):
                        key(std::forward<t_key>(k)),
                        value(v)
                {

                }

                friend bool operator==(
                        const KeyValuePair &first,
                        const KeyValuePair &second) noexcept
                {
                    return first.key == second.key && first.value == second.value;
                }

                friend bool operator!=(
                        const KeyValuePair &first,
                        const KeyValuePair &second) noexcept
                {
                    return !(first == second);
                }

                explicit operator std::pair<const t_key&, t_value&>() noexcept
                {
                    return {key, value};
                }

            };

        public:

            virtual ~AssociativeContainer() noexcept = default;

        public:

            virtual void insert(
                    t_key const &key,
                    t_value const &value) = 0;

            virtual void insert(
                    t_key const &key,
                    t_value &&value) = 0;

            virtual void update(
                    t_key const &key,
                    t_value const &value) = 0;

            virtual void update(
                    t_key const &key,
                    t_value &&value) = 0;

            virtual t_value &search(
                    t_key const &key) = 0;

            virtual t_value remove(
                    t_key const &key) = 0;

            [[nodiscard]] virtual size_t size() const = 0;

        public:


        };

    } // namespace storage

} // namespace babrat