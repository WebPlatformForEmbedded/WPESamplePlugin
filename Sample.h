#ifndef SAMPLE_H
#define SAMPLE_H

#include "Module.h"

namespace WPEFramework {
namespace Plugin {

    class Sample : public PluginHost::IPlugin, public PluginHost::IWeb {
    private:
        Sample(const Sample&) = delete;
        Sample& operator=(const Sample&) = delete;

    public:
        class Data : public Core::JSON::Container {
        private:
            Data(const Data&) = delete;
            Data& operator=(const Data&) = delete;

        public:
            Data()
                : Core::JSON::Container()
                , Str()
            {
                Add(_T("Str"), &Str);
            }
            ~Data()
            {
            }

        public:
            Core::JSON::String Str;
        };

    public:
        Sample()
            : _skipURL(0)
            , _service(nullptr)
            , _testStr("Nothing Set")
        {
        }

        virtual ~Sample()
        {
        }

        BEGIN_INTERFACE_MAP(Sample)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IWeb)
        END_INTERFACE_MAP

    public:
        //   IPlugin methods
        // -------------------------------------------------------------------------------------------------------
        virtual const string Initialize(PluginHost::IShell* service) override;
        virtual void Deinitialize(PluginHost::IShell* service) override;
        virtual string Information() const override;

        //   IWeb methods
        // -------------------------------------------------------------------------------------------------------
        virtual void Inbound(Web::Request& request) override;
        virtual Core::ProxyType<Web::Response> Process(const Web::Request& request) override;

    private:
        void AsyncStatus(const string& status) {
            TRACE_L1("Sending async status %s", status.c_str());
            string message(string("{ \"Status\": \"" + status + "\" }"));
            _service->Notify(message);  // Send a message to the websocket
        }

    private:
        uint8_t _skipURL;
        PluginHost::IShell* _service;
        std::string _testStr;
    };

} // namespace Plugin
} // namespace WPEFramework

#endif // SAMPLE_H
