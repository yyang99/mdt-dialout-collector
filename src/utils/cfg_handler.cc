#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <memory>
#include <map>
#include <string>
#include <libconfig.h++>
#include "cfg_handler.h"


MainCfgHandler::MainCfgHandler()
{
    if (!lookup_main_parameters(this->mdt_dialout_collector_conf,
                                this->parameters)) {
        this->iface = parameters.at("iface");
        this->ipv4_socket_cisco = parameters.at("ipv4_socket_cisco");
        this->ipv4_socket_huawei = parameters.at("ipv4_socket_huawei");
    } else {
        throw std::exception();
    }
}

int MainCfgHandler::lookup_main_parameters(std::string cfg_path,
                                std::map<std::string, std::string>& params)
{
    std::unique_ptr<libconfig::Config> main_params(new libconfig::Config());

    try {
        main_params->readFile(cfg_path.c_str());
    } catch (const libconfig::FileIOException &fioex) {
        std::cout << "libconfig::FileIOException" << std::endl;
        return(EXIT_FAILURE);
    } catch(const libconfig::ParseException &pex) {
        std::cout << "libconfig::ParseException" << std::endl;
        return(EXIT_FAILURE);
    }

    // Main parameters evaluation
    bool iface = main_params->exists("iface");
    if (iface) {
        libconfig::Setting& iface = main_params->lookup("iface");
        std::string iface_s = iface;
        if (!iface_s.empty()) {
            params.insert({"iface", iface_s});
        } else {
            std::cout << "empty iface not allowed" << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        std::cout << "mdt-dialout-collector: iface mandatory" << std::endl;
        throw libconfig::SettingNotFoundException("iface");
    }

    bool ipv4_socket_cisco = main_params->exists("ipv4_socket_cisco");
    if (ipv4_socket_cisco) {
        libconfig::Setting& ipv4_socket_cisco =
            main_params->lookup("ipv4_socket_cisco");
        std::string ipv4_socket_cisco_s = ipv4_socket_cisco;
        if (!ipv4_socket_cisco_s.empty()) {
            params.insert({"ipv4_socket_cisco", ipv4_socket_cisco_s});
        } else {
            std::cout << "ipv4_socket_cisco: valid value not empty"
                                                                << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        params.insert({"ipv4_socket_cisco", ""});
    }

    bool ipv4_socket_huawei = main_params->exists("ipv4_socket_huawei");
    if (ipv4_socket_huawei) {
        libconfig::Setting& ipv4_socket_huawei =
            main_params->lookup("ipv4_socket_huawei");
        std::string ipv4_socket_huawei_s = ipv4_socket_huawei;
        if (!ipv4_socket_huawei_s.empty()) {
            params.insert({"ipv4_socket_huawei", ipv4_socket_huawei_s});
        } else {
            std::cout << "ipv4_socket_huawei: valid value not empty"
                                                                << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        params.insert({"ipv4_socket_huawei", ""});
    }

    return EXIT_SUCCESS;
}

KafkaCfgHandler::KafkaCfgHandler()
{
    if (!lookup_kafka_parameters(this->mdt_dialout_collector_conf,
                                this->parameters)) {
        this->topic = parameters.at("topic");
        this->bootstrap_servers = parameters.at("bootstrap_servers");
        this->enable_idempotence = parameters.at("enable_idempotence");
        this->client_id = parameters.at("client_id");
        this->security_protocol = parameters.at("security_protocol");
        this->ssl_key_location = parameters.at("ssl_key_location");
        this->ssl_certificate_location =
            parameters.at("ssl_certificate_location");
        this->ssl_ca_location = parameters.at("ssl_ca_location");
        this->log_level = parameters.at("log_level");
    } else {
        throw std::exception();
    }
}

int KafkaCfgHandler::lookup_kafka_parameters(std::string cfg_path,
                                std::map<std::string, std::string>& params)
{
    std::unique_ptr<libconfig::Config> kafka_params(new libconfig::Config());

    try {
        kafka_params->readFile(cfg_path.c_str());
    } catch (const libconfig::FileIOException &fioex) {
        std::cout << "libconfig::FileIOException" << std::endl;
        return(EXIT_FAILURE);
    } catch(const libconfig::ParseException &pex) {
        std::cout << "libconfig::ParseException" << std::endl;
        return(EXIT_FAILURE);
    }

    // Kafka arameters evaluation
    bool topic = kafka_params->exists("topic");
    if (topic) {
        libconfig::Setting& topic = kafka_params->lookup("topic");
        std::string topic_s = topic.c_str();
        if (!topic_s.empty()) {
            params.insert({"topic", topic_s});
        } else {
            std::cout << "empty topic not allowed" << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        std::cout << "kafka-producer: topic mandatory" << std::endl;
        throw libconfig::SettingNotFoundException("topic");
    }

    bool bootstrap_servers = kafka_params->exists("bootstrap_servers");
    if (bootstrap_servers) {
        libconfig::Setting& bootstrap_servers =
            kafka_params->lookup("bootstrap_servers");
        std::string bootstrap_servers_s = bootstrap_servers.c_str();
        if (!bootstrap_servers_s.empty()) {
            params.insert({"bootstrap_servers", bootstrap_servers_s});
        } else {
            std::cout << "empty bootstrap_servers not allowed" << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        std::cout << "kafka-producer: bootstrap_servers mandatory" << std::endl;
        throw libconfig::SettingNotFoundException("bootstrap_servers");
    }

    bool enable_idempotence = kafka_params->exists("enable_idempotence");
    if (enable_idempotence) {
        libconfig::Setting& enable_idempotence =
            kafka_params->lookup("enable_idempotence");
        std::string enable_idempotence_s = enable_idempotence.c_str();
        if (!enable_idempotence_s.empty() and
            (enable_idempotence_s.compare("true") == 0 or
                enable_idempotence_s.compare("false") == 0)) {
            params.insert({"enable_idempotence", enable_idempotence_s});
        } else {
            std::cout << "enable_idempotence: valid value <true | false>"
                                                                << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        params.insert({"enable_idempotence", "true"});
    }

    bool client_id = kafka_params->exists("client_id");
    if (client_id) {
        libconfig::Setting& client_id = kafka_params->lookup("client_id");
        std::string client_id_s = client_id.c_str();
        if (!client_id_s.empty()) {
            params.insert({"client_id", client_id_s});
        } else {
            std::cout << "client_id: valid value not empty" << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        params.insert({"client_id", "mdt-dialout-collector"});
    }

    bool log_level = kafka_params->exists("log_level");
    if (log_level) {
        libconfig::Setting& log_level = kafka_params->lookup("log_level");
        std::string log_level_s = log_level.c_str();
        if (!log_level_s.empty()) {
            params.insert({"log_level", log_level_s});
        } else {
            std::cout << "log_level: valid value 0..7" << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        params.insert({"log_level", "6"});
    }

    bool security_protocol = kafka_params->exists("security_protocol");
    if (security_protocol) {
        libconfig::Setting& security_protocol =
            kafka_params->lookup("security_protocol");
        std::string security_protocol_s = security_protocol.c_str();
        if (!security_protocol_s.empty() and
            (security_protocol_s.compare("ssl") == 0 or
            security_protocol_s.compare("plaintext") == 0)) {
            params.insert({"security_protocol", security_protocol_s});
        } else {
            std::cout << "security_protocol: valid values <ssl | plaintext>"
                                                                << std::endl;
            return(EXIT_FAILURE);
        }
    } else {
        std::cout << "kafka-producer: security_protocol mandatory" << std::endl;
        throw libconfig::SettingNotFoundException("security_protocol");
    }

    if (params.at("security_protocol").compare("ssl") == 0) {
        bool ssl_key_location = kafka_params->exists("ssl_key_location");
        bool ssl_certificate_location =
            kafka_params->exists("ssl_certificate_location");
        bool ssl_ca_location = kafka_params->exists("ssl_ca_location");

        if (ssl_key_location and ssl_certificate_location and ssl_ca_location) {
            libconfig::Setting& ssl_key_location =
                kafka_params->lookup("ssl_key_location");
            libconfig::Setting& ssl_certificate_location =
                kafka_params->lookup("ssl_certificate_location");
            libconfig::Setting& ssl_ca_location =
                kafka_params->lookup("ssl_ca_location");

            std::string ssl_key_location_s = ssl_key_location.c_str();
            std::string ssl_certificate_location_s =
                ssl_certificate_location.c_str();
            std::string ssl_ca_location_s = ssl_ca_location.c_str();
            if (!ssl_key_location_s.empty() and
                !ssl_certificate_location_s.empty() and
                !ssl_ca_location_s.empty()) {
                params.insert({"ssl_key_location", ssl_key_location_s});
                params.insert({"ssl_certificate_location",
                    ssl_certificate_location_s});
                params.insert({"ssl_ca_location", ssl_ca_location_s});
            } else {
                std::cout << "security_protocol: valid values not empty"
                                                                << std::endl;
                return(EXIT_FAILURE);
            }
        } else {
            std::cout << "kafka-producer: security_protocol options mandatory"
                                                                << std::endl;
            throw libconfig::SettingNotFoundException(
                                                "security_protocol options");
        }
    } else {
        params.insert({"ssl_key_location", "NULL"});
        params.insert({"ssl_certificate_location", "NULL"});
        params.insert({"ssl_ca_location", "NULL"});
    }

    return EXIT_SUCCESS;
}
