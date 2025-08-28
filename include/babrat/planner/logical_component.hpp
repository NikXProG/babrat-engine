#pragma once

namespace babrat {

    enum LogicalComponentType {

    };

    class LogicalComponentBase {

    public:

        virtual ~LogicalComponentBase() noexcept = default;

    public:

        explicit LogicalComponentBase(LogicalComponentType logicalComponentType) :
                type(logicalComponentType) {}

    private:

        LogicalComponentType type [[maybe_unused]];

    };

    class LogicalComponent : LogicalComponentBase {

    public:

        explicit LogicalComponent(LogicalComponentType type) :
            LogicalComponentBase(type) {}

    };



} // namespace babrat