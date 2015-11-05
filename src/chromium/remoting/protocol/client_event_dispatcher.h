// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_CLIENT_EVENT_DISPATCHER_H_
#define REMOTING_PROTOCOL_CLIENT_EVENT_DISPATCHER_H_

#include "base/memory/ref_counted.h"
#include "remoting/protocol/buffered_socket_writer.h"
#include "remoting/protocol/channel_dispatcher_base.h"
#include "remoting/protocol/input_stub.h"

namespace remoting {
namespace protocol {

// ClientEventDispatcher manages the event channel on the client
// side. It implements InputStub for outgoing input messages.
class ClientEventDispatcher : public ChannelDispatcherBase, public InputStub {
 public:
  ClientEventDispatcher();
  ~ClientEventDispatcher() override;

  // InputStub implementation.
  void InjectKeyEvent(const KeyEvent& event) override;
  void InjectTextEvent(const TextEvent& event) override;
  void InjectMouseEvent(const MouseEvent& event) override;

 protected:
  // ChannelDispatcherBase overrides.
  void OnInitialized() override;

 private:
  BufferedSocketWriter writer_;

  DISALLOW_COPY_AND_ASSIGN(ClientEventDispatcher);
};

}  // namespace protocol
}  // namespace remoting

#endif  // REMOTING_PROTOCOL_CLIENT_EVENT_DISPATCHER_H_