#include <iostream>
#include <kafka/KafkaProducer.h>
#include <csignal>
#include <memory>
#include <vector>

class MessageBusProducer {

public:

    using MessageHandler = std::function<void(const std::string&)>;

    MessageBusProducer(const std::string& brokers)
    {

        kafka::Properties props(
                {
                        {"bootstrap.servers", {brokers}}
                });

        producer_ = std::make_unique<kafka::clients::producer::KafkaProducer>(props);

    }

    void send(const std::string& message) {

        std::string responseTopic = "db-responses";

        kafka::clients::producer::ProducerRecord responseRecord(
                responseTopic,
                kafka::NullKey,
                kafka::Value(message.c_str(), message.size())
        );

        std::cout << message << std::endl;
        producer_->send(responseRecord,
                        [](
                                const kafka::clients::producer::RecordMetadata& metadata,
                                const kafka::Error& error) {
                            if (error) {
                                std::cerr << "Failed to send response: " << error.message() << std::endl;
                            }
        });


         producer_->close();

    }


private:

    std::unique_ptr<kafka::clients::producer::KafkaProducer> producer_;

};



