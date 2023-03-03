/*
 * ModernClient.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include "ModernClient.h"

namespace dtests {
namespace proton {
namespace common {

using std::string;
using namespace dtests::common::log;

ModernClient::ModernClient()
    : super()
{
    // TODO Auto-generated constructor stub

}

ModernClient::~ModernClient()
{
    // TODO Auto-generated destructor stub
}

void ModernClient::logInfo() const
{
    Logger::LogConfiguration logConfiguration;

    logConfiguration.minimumLevel = info;
    logConfiguration.stream = stdout;

    LoggerWrapper::initLogger(logConfiguration);
}

void ModernClient::logDebug() const
{
    Logger::LogConfiguration logConfiguration;

    logConfiguration.minimumLevel = debug;
    logConfiguration.stream = stdout;

    LoggerWrapper::initLogger(logConfiguration);
}

void ModernClient::logTrace() const
{
    Logger::LogConfiguration logConfiguration;

    logConfiguration.minimumLevel = trace;
    logConfiguration.stream = stdout;

    LoggerWrapper::initLogger(logConfiguration);
}

void ModernClient::setLogLevel(const optparse::Values &options) const
{
    
    if (options.is_set("client-log-level")) {
        const string logLevel = options["client-log-level"];

        if (logLevel == "info") {
            logInfo();
        } else {
            if (logLevel == "debug") {
                logDebug();
            }
            else {
                if (logLevel == "trace") {
                    logTrace();
                }
            }
        }
    }

}

void ModernClient::enableTracing(std::string service_name) const
{

#ifdef __unix__
    opentelemetry::exporter::jaeger::JaegerExporterOptions opts;

    // Initialize Jaeger Exporter
    std::unique_ptr<opentelemetry::sdk::trace::SpanExporter> exporter = std::unique_ptr<opentelemetry::sdk::trace::SpanExporter>(
            new opentelemetry::exporter::jaeger::JaegerExporter(opts));

    // Set service-name
    auto resource_attributes = opentelemetry::sdk::resource::ResourceAttributes
            {
                    {"service.name", service_name}
            };

    // Creation of the resource for associating it with telemetry
    auto resource = opentelemetry::sdk::resource::Resource::Create(resource_attributes);

    auto processor = std::unique_ptr<opentelemetry::sdk::trace::SpanProcessor>(
            new opentelemetry::sdk::trace::SimpleSpanProcessor(std::move(exporter)));
    auto provider = opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
            new opentelemetry::sdk::trace::TracerProvider(std::move(processor), resource));

    // Set the global trace provider
    opentelemetry::trace::Provider::SetTracerProvider(provider);

    // Enable tracing in proton cpp
    ::proton::initOpenTelemetryTracer();
#endif

}


} /* namespace messenger */
} /* namespace proton */
} /* namespace dtests */
