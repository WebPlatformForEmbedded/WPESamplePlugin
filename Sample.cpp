#include "Sample.h"

namespace WPEFramework {
namespace Plugin {

    SERVICE_REGISTRATION(Sample, 1, 0);

    static Core::ProxyPoolType<Web::JSONBodyType<Sample::Data> > jsonDataFactory(1);
    static Core::ProxyPoolType<Web::JSONBodyType<Sample::Data> > jsonBodyDataFactory(2);

    /* virtual */ const string Sample::Initialize(PluginHost::IShell* service)
    {
        ASSERT (_service == nullptr);
        ASSERT (service != nullptr);

        Config config;
        config.FromString(service->ConfigLine());
        _skipURL = static_cast<uint8_t>(service->WebPrefix().length());
        _service = service;

        AsyncStatus("UP");

        return (string());
    }

    /* virtual */ void Sample::Deinitialize(PluginHost::IShell* service)
    {
        ASSERT (_service == service);
        AsyncStatus("DOWN");

        _service = nullptr;
    }

    /* virtual */ string Sample::Information() const
    {
        return (string());          // No additional info to report.
    }

    /* virtual */ void Sample::Inbound(Web::Request& request )
    {
        if (request.Verb == Web::Request::HTTP_POST)
            request.Body(jsonBodyDataFactory.Element());
    }

    /*------------------------------------------------------------------------
     *  $ curl -X GET  http://<HOST>/Service/Sample/Test
     *  {"Str":"Nothing set"}
     *
     *  $ curl -X POST -d '{"Str":"TEST"}' http://bcm/Service/Sample/Test
     *
     *  $ curl -X GET  http://<HOST>/Service/Sample/Test
     *  {"Str":"TEST"}
     *------------------------------------------------------------------------*/
    /* virtual */ Core::ProxyType<Web::Response> Sample::Process(const WPEFramework::Web::Request& request)
    {
        ASSERT(_skipURL <= request.Path.length());

        TRACE(Trace::Information, (string(_T("Received Sample request"))));

        Core::ProxyType<Web::Response> result(PluginHost::Factories::Instance().Response());
        Core::TextSegmentIterator index(Core::TextFragment(request.Path, _skipURL, request.Path.length() - _skipURL), false, '/');

        if ((request.Verb == Web::Request::HTTP_GET) && ((index.Next()) && (index.Next()))) {
            result->ErrorCode = Web::STATUS_OK;
            result->Message = "OK";

            if (index.Current().Text() == _T("Test")) {
                Core::ProxyType<Web::JSONBodyType<Data> > data (jsonDataFactory.Element());
                data->Str = _testStr;
                result->ContentType = Web::MIMETypes::MIME_JSON;
                result->Body(data);
            } else {
                result->ErrorCode = Web::STATUS_BAD_REQUEST;
                result->Message = _T("Unsupported GET Request");
            }
        } else if ((request.Verb == Web::Request::HTTP_POST) && ((index.Next()) && (index.Next()))) {

            if (index.Current().Text() == _T("Test")) {
                _testStr = request.Body<const Data>()->Str.Value();
                result->ErrorCode = Web::STATUS_OK;
                result->Message = "OK";
            } else {
                result->ErrorCode = Web::STATUS_BAD_REQUEST;
                result->Message = _T("Unsupported POST Request");
            }
        } else {
            result->ErrorCode = Web::STATUS_BAD_REQUEST;
            result->Message = _T("Unsupported Method.");
        }
        return result;
    }

} // namespace Plugin
} // namespace WPEFramework
