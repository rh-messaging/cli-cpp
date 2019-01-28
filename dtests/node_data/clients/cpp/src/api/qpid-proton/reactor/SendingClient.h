/*
 * SendingClient.h
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_

#include <sstream>
#include <string>

#include <proton/message.hpp>
#include <proton/container.hpp>
#include <proton/types.hpp>
#include <proton/duration.hpp>

#include "ModernClient.h"
#include "handler/SenderHandler.h"
#include "options/modern/SenderOptionsParser.h"
#include "options/OptionsHelper.h"
#include "ContentType.h"
#include "TypeUtils.h"
#include "UriParser.h"

using proton::message;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::proton::common::ModernClient;

using namespace dtests::common;

/**
 * Implements a connecting client using the proton reactive API
 */
class SendingClient : public ModernClient {
  public:
    SendingClient();
    virtual ~SendingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef ModernClient super;

    void setMessageOptions(const OptionsSetter &setter, message &msg) const;
    bool nameVal(const string& in, string& name, string& value, string& separator) const;
    void setMessageProperty(message *msg, const string &property) const;
    void setMessageListItem(message *msg, const string &property, std::list<value> &messageList) const;
    void setMessageMapItem(message *msg, const string &property, std::map<std::string, value> &messageMap) const;
    void setMessageProperties(StringAppendCallback &callbackProperty, message *msg) const;
    void setMessageList(StringAppendCallback &callbackList, message *msg) const;
    void setMessageMap(StringAppendCallback &callbackMap, message *msg) const;
    void setMessageText(string content, message *msg) const;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_ */
