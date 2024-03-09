#include "Poll.hxx"
#include "_PollData.hxx"

Poll::Poll(const EventDeviceDescriptor & fd, const int timeout_ms) :
    _data(new Poll::Data{fd, {fd, POLLIN}, timeout_ms})
{
}

Poll::~Poll() { delete _data; }

Poll::Result Poll::run()
{
    int ret = poll(&_data->_pfd, 1, _data->_timeout);

    if (ret == 0) [[likely]]
        return Result::TIMEOUT;
    else if (ret > 0)
        return Result::GOT_EVENT;
    else [[unlikely]]
        return Result::FAILURE;
}
