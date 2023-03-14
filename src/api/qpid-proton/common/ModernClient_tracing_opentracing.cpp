#include "ModernClient.h"

#include <proton/tracing.hpp>

#include <opentelemetry/sdk/trace/simple_processor.h>
#include <opentelemetry/sdk/trace/tracer_provider.h>
#include <opentelemetry/trace/provider.h>
#include <opentelemetry/nostd/unique_ptr.h>
#include <opentelemetry/exporters/jaeger/jaeger_exporter.h>
#include <opentelemetry/exporters/ostream/span_exporter.h>
#include <opentelemetry/sdk/resource/resource.h>

#include <opentelemetry/trace/span.h>
#include <opentelemetry/trace/tracer.h>
#include <opentelemetry/trace/context.h>

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
