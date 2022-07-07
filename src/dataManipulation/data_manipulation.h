#ifndef _DATA_MANIPULATION_H_
#define _DATA_MANIPULATION_H_

#include <iostream>
#include <json/json.h>
#include "cisco_telemetry.pb.h"
#include "juniper_gnmi.pb.h"
#include "juniper_telemetry_header_extension.pb.h"


class DataManipulation {
public:
    // Handling data manipulation functions
    int append_label_map(const std::string& json_str,
            std::string& json_str_out);
    int cisco_gpbkv2json(
            const std::unique_ptr<cisco_telemetry::Telemetry>& cisco_tlm,
            std::string& json_str_out);
    Json::Value cisco_gpbkv_field2json(
            const cisco_telemetry::TelemetryField& field);
    int juniper_extension(gnmi::SubscribeResponse& juniper_stream,
        const std::unique_ptr<GnmiJuniperTelemetryHeaderExtension>&
            juniper_tlm_header_ext,
        Json::Value& root);
    int juniper_update(gnmi::SubscribeResponse& juniper_stream,
        std::string& json_str_out,
        Json::Value& root);
};

#endif
