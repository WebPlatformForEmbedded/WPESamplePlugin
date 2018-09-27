#include "Module.h"
#include <interfaces/ISample.h>
#include <interfaces/IMemory.h>

namespace WPEFramework {
namespace Plugin {

    class SampleImplementation : public Exchange::ISample {
    private:

        class Config : public Core::JSON::Container {
        private:
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

        public:
            Config()
                : Core::JSON::Container()
                , TestNum(0)
            {
                Add(_T("testNum"), &TestNum);
                Add(_T("testStr"), &TestStr);
            }
            ~Config()
            {
            }

        public:
            Core::JSON::DecUInt16 TestNum;
            Core::JSON::String TestStr;
        };

    private:
        SampleImplementation(const SampleImplementation&) = delete;
        SampleImplementation& operator=(const SampleImplementation&) = delete;

    public:
        SampleImplementation()
            : _observers()
            , str("Nothing set")
        {
        }

        virtual ~SampleImplementation()
        {
        }

        virtual uint32_t Configure(PluginHost::IShell* service)
        {
            ASSERT(service != nullptr);

            Config config;
            config.FromString(service->ConfigLine());

            uint32_t result = 0;

            return (result);
        }

        virtual void SampleSet(const string& str)
        {
            this->str = str;
        }
        virtual string SampleGet() const
        {
            return (str);
        }

        BEGIN_INTERFACE_MAP(SampleImplementation)
        INTERFACE_ENTRY(Exchange::ISample)
        END_INTERFACE_MAP

    private:
        std::list<PluginHost::IStateControl::INotification*> _observers;
        string str;
    };

    SERVICE_REGISTRATION(SampleImplementation, 1, 0);

} // namespace Plugin

} // namespace WPEFramework
