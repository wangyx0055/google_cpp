// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_OZONE_EVDEV_LIBGESTURES_GLUE_GESTURE_INTERPRETER_LIBEVDEV_CROS_H_
#define UI_EVENTS_OZONE_EVDEV_LIBGESTURES_GLUE_GESTURE_INTERPRETER_LIBEVDEV_CROS_H_

#include <gestures/gestures.h>
#include <libevdev/libevdev.h>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "ui/events/ozone/evdev/cursor_delegate_evdev.h"
#include "ui/events/ozone/evdev/event_device_util.h"
#include "ui/events/ozone/evdev/event_dispatch_callback.h"
#include "ui/events/ozone/evdev/events_ozone_evdev_export.h"
#include "ui/events/ozone/evdev/libgestures_glue/event_reader_libevdev_cros.h"

namespace ui {

class EventDeviceInfo;
class EventModifiersEvdev;
class MouseButtonMapEvdev;
class CursorDelegateEvdev;
class KeyboardEvdev;
struct GestureDeviceProperties;
class GesturePropertyProvider;

// Convert libevdev-cros events to ui::Events using libgestures.
//
// This builds a GestureInterpreter for an input device (touchpad or
// mouse).
//
// Raw input events must be preprocessed into a form suitable for
// libgestures. The kernel protocol only emits changes to the device state,
// so changes must be accumulated until a sync event. The full device state
// at sync is then processed by libgestures.
//
// Once we have the state at sync, we convert it to a HardwareState object
// and forward it to libgestures. If any gestures are produced, they are
// converted to ui::Events and dispatched.
class EVENTS_OZONE_EVDEV_EXPORT GestureInterpreterLibevdevCros
    : public EventReaderLibevdevCros::Delegate {
 public:
  GestureInterpreterLibevdevCros(int id,
                                 EventModifiersEvdev* modifiers,
                                 MouseButtonMapEvdev* button_map,
                                 CursorDelegateEvdev* cursor,
                                 KeyboardEvdev* keyboard,
                                 GesturePropertyProvider* property_provider,
                                 const EventDispatchCallback& callback);
  ~GestureInterpreterLibevdevCros() override;

  // Overriden from ui::EventReaderLibevdevCros::Delegate
  void OnLibEvdevCrosOpen(Evdev* evdev, EventStateRec* evstate) override;
  void OnLibEvdevCrosEvent(Evdev* evdev,
                           EventStateRec* evstate,
                           const timeval& time) override;

  // Handler for gesture events generated from libgestures.
  void OnGestureReady(const Gesture* gesture);

  // Accessors.
  int id() { return id_; }
  GesturePropertyProvider* property_provider() { return property_provider_; }
  Evdev* evdev() { return evdev_; }

 private:
  void OnGestureMove(const Gesture* gesture, const GestureMove* move);
  void OnGestureScroll(const Gesture* gesture, const GestureScroll* move);
  void OnGestureButtonsChange(const Gesture* gesture,
                              const GestureButtonsChange* move);
  void OnGestureContactInitiated(const Gesture* gesture);
  void OnGestureFling(const Gesture* gesture, const GestureFling* fling);
  void OnGestureSwipe(const Gesture* gesture, const GestureSwipe* swipe);
  void OnGestureSwipeLift(const Gesture* gesture,
                          const GestureSwipeLift* swipelift);
  void OnGesturePinch(const Gesture* gesture, const GesturePinch* pinch);
  void OnGestureMetrics(const Gesture* gesture, const GestureMetrics* metrics);

  void Dispatch(scoped_ptr<Event> event);
  void DispatchMouseButton(unsigned int modifier, bool down);
  void DispatchChangedKeys(Evdev* evdev, const timeval& time);

  // The unique device id.
  int id_;

  // True if the device may be regarded as a mouse. This includes normal mice
  // and multi-touch mice.
  bool is_mouse_;

  // Shared modifier state.
  EventModifiersEvdev* modifiers_;

  // Shared mouse button map.
  MouseButtonMapEvdev* button_map_;

  // Shared cursor state.
  CursorDelegateEvdev* cursor_;

  // Shared keyboard state.
  KeyboardEvdev* keyboard_;

  // Shared gesture property provider.
  GesturePropertyProvider* property_provider_;

  // Callback for dispatching events.
  EventDispatchCallback dispatch_callback_;

  // Gestures interpretation state.
  gestures::GestureInterpreter* interpreter_;

  // Last key state from libevdev.
  unsigned long prev_key_state_[EVDEV_BITS_TO_LONGS(KEY_CNT)];

  // Device pointer.
  Evdev* evdev_;

  // Gesture lib device properties.
  scoped_ptr<GestureDeviceProperties> device_properties_;

  DISALLOW_COPY_AND_ASSIGN(GestureInterpreterLibevdevCros);
};

}  // namspace ui

#endif  // UI_EVENTS_OZONE_EVDEV_LIBGESTURES_GLUE_GESTURE_INTERPRETER_LIBEVDEV_CROS_H_