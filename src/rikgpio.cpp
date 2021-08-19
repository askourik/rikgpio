/*
// Copyright (c) 2019 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "rikgpio.hpp"
#include <phosphor-logging/log.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <vector>
#include <unordered_map>
#include <xyz/openbmc_project/Common/error.hpp>

#include <filesystem>

namespace fs = std::filesystem;


RikgpioMgr::RikgpioMgr(boost::asio::io_service& io_,
                     sdbusplus::asio::object_server& srv_,
                     std::shared_ptr<sdbusplus::asio::connection>& conn_) :
    io(io_),
    server(srv_), conn(conn_)
{
    iface = server.add_interface("/xyz/openbmc_project/rikgpio", "xyz.openbmc_project.Rikgpio");

    iface->register_property(
        "rikgpioflag", std::string(""), 
        [](const std::string& req, std::string& propertyValue) { propertyValue = req; return 1; },
        [this](const std::string& property) {
            if (property!="")
                this->mode = property;
            auto now = std::chrono::system_clock::now();
            auto timePoint = std::chrono::system_clock::to_time_t(now);
            phosphor::logging::log<phosphor::logging::level::INFO>(
                ("!!!Rikgpio rikgpioflag set " + this->mode + " at " + std::ctime(&timePoint)).c_str());
            //if (this->mode == RikModeNTP)
            {
                int ret_code = system("/bin/sh /usr/sbin/gpioxxx.sh");
                if(ret_code)
                    throw std::runtime_error("Errors occurred while running gpio.sh at setgpio");
                phosphor::logging::log<phosphor::logging::level::INFO>("Rikgpio executed gpio.sh at setgpio");
            }
            return this->mode;
        });



    iface->initialize(true);


    phosphor::logging::log<phosphor::logging::level::INFO>(
        ("Rikgpio started mode " + mode).c_str());

    int ret_code = 0;
    ret_code += system("systemctl start rikgpio.service");
    if(ret_code)
        throw std::runtime_error("Errors occurred while setting timer");

}


