/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2013-2014 - Jason Fetters
 *  Copyright (C) 2011-2016 - Daniel De Matteis
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string/stdstring.h>

#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>

#include "../connect/joypad_connection.h"
#include "../input_autodetect.h"
#include "../input_hid_driver.h"
#include "../../configuration.h"
#include "../../verbosity.h"

typedef struct apple_hid
{
   IOHIDManagerRef ptr;
   joypad_connection_t *slots;
   uint32_t buttons[MAX_USERS];
   int16_t axes[MAX_USERS][6];
} iohidmanager_hid_t;

struct iohidmanager_hid_adapter
{
   uint32_t slot;
   IOHIDDeviceRef handle;
   char name[PATH_MAX_LENGTH];
   uint8_t data[2048];
};

static bool iohidmanager_hid_joypad_query(void *data, unsigned pad)
{
   return pad < MAX_USERS;
}

static const char *iohidmanager_hid_joypad_name(void *data, unsigned pad)
{
   /* TODO/FIXME - implement properly */
   if (pad >= MAX_USERS)
      return NULL;

   return NULL;
}

static uint64_t iohidmanager_hid_joypad_get_buttons(void *data, unsigned port)
{
   iohidmanager_hid_t        *hid   = (iohidmanager_hid_t*)data;
   if (hid)
      return pad_connection_get_buttons(&hid->slots[port], port);
   return 0;
}

static bool iohidmanager_hid_joypad_button(void *data,
      unsigned port, uint16_t joykey)
{
   uint64_t buttons          = 
      iohidmanager_hid_joypad_get_buttons(data, port);
   iohidmanager_hid_t *hid   = (iohidmanager_hid_t*)data;

   if (joykey == NO_BTN)
      return false;

   /* Check hat. */
   if (GET_HAT_DIR(joykey))
      return false;

   /* Check the button. */
   if ((port < MAX_USERS) && (joykey < 32))
      return ((buttons & (1 << joykey)) != 0) 
         || ((hid->buttons[port] & (1 << joykey)) != 0);
   return false;
}

static bool iohidmanager_hid_joypad_rumble(void *data, unsigned pad,
      enum retro_rumble_effect effect, uint16_t strength)
{
   iohidmanager_hid_t        *hid   = (iohidmanager_hid_t*)data;
   if (!hid)
      return false;
   return pad_connection_rumble(&hid->slots[pad], pad, effect, strength);
}

static int16_t iohidmanager_hid_joypad_axis(void *data,
      unsigned port, uint32_t joyaxis)
{
   iohidmanager_hid_t          *hid = (iohidmanager_hid_t*)data;
   int16_t               val = 0;

   if (joyaxis == AXIS_NONE)
      return 0;

   if (AXIS_NEG_GET(joyaxis) < 6)
   {
      val += hid->axes[port][AXIS_NEG_GET(joyaxis)];
      val += pad_connection_get_axis(&hid->slots[port],
            port, AXIS_NEG_GET(joyaxis));

      if (val >= 0)
         val = 0;
   }
   else if(AXIS_POS_GET(joyaxis) < 6)
   {
      val += hid->axes[port][AXIS_POS_GET(joyaxis)];
      val += pad_connection_get_axis(&hid->slots[port],
            port, AXIS_POS_GET(joyaxis));

      if (val <= 0)
         val = 0;
   }

   return val;
}

static void iohidmanager_hid_device_send_control(void *data,
      uint8_t* data_buf, size_t size)
{
   struct iohidmanager_hid_adapter *adapter = 
      (struct iohidmanager_hid_adapter*)data;

   if (adapter)
      IOHIDDeviceSetReport(adapter->handle,
            kIOHIDReportTypeOutput, 0x01, data_buf + 1, size - 1);
}

static void iohidmanager_hid_device_report(void *data,
      IOReturn result, void *sender,
      IOHIDReportType type, uint32_t reportID, uint8_t *report,
      CFIndex reportLength)
{
   struct iohidmanager_hid_adapter *adapter = 
      (struct iohidmanager_hid_adapter*)data;
   iohidmanager_hid_t *hid = (iohidmanager_hid_t*)hid_driver_get_data();

   if (hid && adapter)
      pad_connection_packet(&hid->slots[adapter->slot], adapter->slot,
            adapter->data, reportLength + 1);
}

/* NOTE: I pieced this together through trial and error,
 * any corrections are welcome. */

static void iohidmanager_hid_device_input_callback(void *data, IOReturn result,
      void* sender, IOHIDValueRef value)
{
   iohidmanager_hid_t *hid                  = (iohidmanager_hid_t*)
      hid_driver_get_data();
   struct iohidmanager_hid_adapter *adapter = 
      (struct iohidmanager_hid_adapter*)data;
   IOHIDElementRef element                  = IOHIDValueGetElement(value);
   uint32_t type                            = IOHIDElementGetType(element);
   uint32_t page                            = IOHIDElementGetUsagePage(element);
   uint32_t use                             = IOHIDElementGetUsage(element);

   if (type != kIOHIDElementTypeInput_Misc)
      if (type != kIOHIDElementTypeInput_Button)
         if (type != kIOHIDElementTypeInput_Axis)
            return;

   /* Joystick handler.
    * TODO: Can GamePad work the same? */

   switch (page)
   {
      case kHIDPage_GenericDesktop:
         switch (type)
         {
            case kIOHIDElementTypeInput_Misc:
               switch (use)
               {
                  case kHIDUsage_GD_Hatswitch:
                     break;
                  default:
                     {
                        int i;
                        // +0/-0   =>   Left Stick Horizontal       => 48
                        // +1/-1   =>   Left Stick Vertical         => 49
                        // +2/-2   =>   Right Stick Horizontal      => 51
                        // +3/-3   =>   Right Stick Vertical        => 52
                        // +4/-4   =>   Left Trigger (if exists)    => 50
                        // +5/-5   =>   Right Trigger (if exists)   => 53
                        static const uint32_t axis_use_ids[6] = 
                        { 48, 49, 51, 52, 50, 53 };

                        for (i = 0; i < 6; i ++)
                        {
                           CFIndex min   = IOHIDElementGetPhysicalMin(element);
                           CFIndex state = IOHIDValueGetIntegerValue(value) - min;
                           CFIndex max   = IOHIDElementGetPhysicalMax(element) - min;
                           float val     = (float)state / (float)max;

                           if (use != axis_use_ids[i])
                              continue;

                           hid->axes[adapter->slot][i] =
                              ((val * 2.0f) - 1.0f) * 32767.0f;
                        }
                     }
                     break;
               }
               break;
         }
         break;
      case kHIDPage_Button:
         switch (type)
         {
            case kIOHIDElementTypeInput_Button:
               {
                  CFIndex state = IOHIDValueGetIntegerValue(value);
                  unsigned   id = use - 1;

                  if (state)
                     BIT64_SET(hid->buttons[adapter->slot], id);
                  else
                     BIT64_CLEAR(hid->buttons[adapter->slot], id);
               }
               break;
         }
         break;
   }
}

static void iohidmanager_hid_device_remove(void *data,
      IOReturn result, void* sender)
{
   struct iohidmanager_hid_adapter *adapter = 
      (struct iohidmanager_hid_adapter*)data;
   iohidmanager_hid_t *hid = (iohidmanager_hid_t*)
      hid_driver_get_data();

   if (hid && adapter && (adapter->slot < MAX_USERS))
   {
      input_config_autoconfigure_disconnect(adapter->slot, adapter->name);

      hid->buttons[adapter->slot] = 0;
      memset(hid->axes[adapter->slot], 0, sizeof(hid->axes));

      pad_connection_pad_deinit(&hid->slots[adapter->slot], adapter->slot);
      free(adapter);
   }
}

static int32_t iohidmanager_hid_device_get_int_property(
      IOHIDDeviceRef device, CFStringRef key)
{
   int32_t value;
   CFNumberRef ref = (CFNumberRef)IOHIDDeviceGetProperty(device, key);

   if (ref)
   {
      if (CFGetTypeID(ref) == CFNumberGetTypeID())
      {
         CFNumberGetValue((CFNumberRef)ref, kCFNumberIntType, &value);
         return value;
      }
   }

   return 0;
}

static uint16_t iohidmanager_hid_device_get_vendor_id(IOHIDDeviceRef device)
{
   return iohidmanager_hid_device_get_int_property(device,
         CFSTR(kIOHIDVendorIDKey));
}

static uint16_t iohidmanager_hid_device_get_product_id(IOHIDDeviceRef device)
{
   return iohidmanager_hid_device_get_int_property(device,
         CFSTR(kIOHIDProductIDKey));
}

static void iohidmanager_hid_device_get_product_string(
      IOHIDDeviceRef device, char *buf, size_t len)
{
   CFStringRef ref = (CFStringRef)
      IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));

   if (ref)
      CFStringGetCString(ref, buf, len, kCFStringEncodingUTF8);
}

static void iohidmanager_hid_device_add_autodetect(unsigned idx,
      const char *device_name, const char *driver_name,
      uint16_t dev_vid, uint16_t dev_pid)
{
   autoconfig_params_t params = {{0}};

   params.idx = idx;
   params.vid = dev_vid;
   params.pid = dev_pid;

   strlcpy(params.name, device_name, sizeof(params.name));
   strlcpy(params.driver, driver_name, sizeof(params.driver));

   input_config_autoconfigure_joypad(&params);
   RARCH_LOG("Port %d: %s.\n", idx, device_name);
}

static void iohidmanager_hid_device_add(void *data, IOReturn result,
      void* sender, IOHIDDeviceRef device)
{
   IOReturn ret;
   uint16_t dev_vid, dev_pid;

   settings_t *settings = config_get_ptr();
   iohidmanager_hid_t     *hid = (iohidmanager_hid_t*)hid_driver_get_data();
   struct iohidmanager_hid_adapter *adapter = (struct iohidmanager_hid_adapter*)
      calloc(1, sizeof(*adapter));

   if (!adapter || !hid)
      return;

   adapter->handle        = device;

   ret = IOHIDDeviceOpen(device, kIOHIDOptionsTypeNone);

   if (ret != kIOReturnSuccess)
   {
      free(adapter);
      return;
   }

   /* Move the device's run loop to this thread. */
   IOHIDDeviceScheduleWithRunLoop(device, CFRunLoopGetCurrent(),
         kCFRunLoopCommonModes);
   IOHIDDeviceRegisterRemovalCallback(device, 
         iohidmanager_hid_device_remove, adapter);

#ifndef IOS
   iohidmanager_hid_device_get_product_string(device, adapter->name,
         sizeof(adapter->name));
#endif

   dev_vid = iohidmanager_hid_device_get_vendor_id  (device);
   dev_pid = iohidmanager_hid_device_get_product_id (device);

   adapter->slot = pad_connection_pad_init(hid->slots,
         adapter->name, dev_vid, dev_pid, adapter, 
         &iohidmanager_hid_device_send_control);

   if (adapter->slot == -1)
       return;

   if (pad_connection_has_interface(hid->slots, adapter->slot))
      IOHIDDeviceRegisterInputReportCallback(device,
            adapter->data + 1, sizeof(adapter->data) - 1,
            iohidmanager_hid_device_report, adapter);
   else
      IOHIDDeviceRegisterInputValueCallback(device,
            iohidmanager_hid_device_input_callback, adapter);

   if (string_is_empty(adapter->name))
      return;

   strlcpy(settings->input.device_names[adapter->slot],
         adapter->name, sizeof(settings->input.device_names[adapter->slot]));

   iohidmanager_hid_device_add_autodetect(adapter->slot,
         adapter->name, iohidmanager_hid.ident, dev_vid, dev_pid);
}

static void iohidmanager_hid_append_matching_dictionary(
      CFMutableArrayRef array,
      uint32_t page, uint32_t use)
{
   CFNumberRef usen, pagen;
   CFMutableDictionaryRef matcher = CFDictionaryCreateMutable(
         kCFAllocatorDefault, 0,
         &kCFTypeDictionaryKeyCallBacks,
         &kCFTypeDictionaryValueCallBacks);

   pagen = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
   CFDictionarySetValue(matcher, CFSTR(kIOHIDDeviceUsagePageKey), pagen);
   CFRelease(pagen);

   usen = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &use);
   CFDictionarySetValue(matcher, CFSTR(kIOHIDDeviceUsageKey), usen);
   CFRelease(usen);

   CFArrayAppendValue(array, matcher);
   CFRelease(matcher);
}

static int iohidmanager_hid_manager_init(iohidmanager_hid_t *hid)
{
   if (!hid)
      return -1;
   if (hid->ptr) /* already initialized. */
      return 0;

   hid->ptr = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

   if (hid->ptr)
   {
      IOHIDManagerSetDeviceMatching(hid->ptr, NULL);
      IOHIDManagerScheduleWithRunLoop(hid->ptr, CFRunLoopGetCurrent(),
            kCFRunLoopDefaultMode);
      return 0;
   }

   return -1;
}


static int iohidmanager_hid_manager_free(iohidmanager_hid_t *hid)
{
   if (!hid || !hid->ptr)
      return -1;

   IOHIDManagerUnscheduleFromRunLoop(hid->ptr,
         CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
   IOHIDManagerClose(hid->ptr, kIOHIDOptionsTypeNone);
   CFRelease(hid->ptr);
   hid->ptr = NULL;

   return 0;
}

static int iohidmanager_hid_manager_set_device_matching(
      iohidmanager_hid_t *hid)
{
   CFMutableArrayRef matcher = CFArrayCreateMutable(kCFAllocatorDefault, 0,
         &kCFTypeArrayCallBacks);

   if (!matcher)
      return -1;

   iohidmanager_hid_append_matching_dictionary(matcher,
         kHIDPage_GenericDesktop,
         kHIDUsage_GD_Joystick);
   iohidmanager_hid_append_matching_dictionary(matcher,
         kHIDPage_GenericDesktop,
         kHIDUsage_GD_GamePad);

   IOHIDManagerSetDeviceMatchingMultiple(hid->ptr, matcher);
   IOHIDManagerRegisterDeviceMatchingCallback(hid->ptr,
         iohidmanager_hid_device_add, 0);

   CFRelease(matcher);

   return 0;
}

static void *iohidmanager_hid_init(void)
{
   iohidmanager_hid_t *hid_apple = (iohidmanager_hid_t*)
      calloc(1, sizeof(*hid_apple));

   if (!hid_apple)
      goto error;
   hid_apple->slots = pad_connection_init(MAX_USERS);
   if (!hid_apple->slots)
      goto error;
   if (iohidmanager_hid_manager_init(hid_apple) == -1)
      goto error;
   if (iohidmanager_hid_manager_set_device_matching(hid_apple) == -1)
      goto error;

   return hid_apple;

error:
   if (hid_apple->slots)
      free(hid_apple->slots);
   hid_apple->slots = NULL;
   if (hid_apple)
      free(hid_apple);
   return NULL;
}

static void iohidmanager_hid_free(void *data)
{
   iohidmanager_hid_t *hid_apple = (iohidmanager_hid_t*)data;

   if (!hid_apple || !hid_apple->ptr)
      return;

   pad_connection_destroy(hid_apple->slots);
   iohidmanager_hid_manager_free(hid_apple);

   if (hid_apple)
      free(hid_apple);
}

static void iohidmanager_hid_poll(void *data)
{
   (void)data;
}

hid_driver_t iohidmanager_hid = {
   iohidmanager_hid_init,
   iohidmanager_hid_joypad_query,
   iohidmanager_hid_free,
   iohidmanager_hid_joypad_button,
   iohidmanager_hid_joypad_get_buttons,
   iohidmanager_hid_joypad_axis,
   iohidmanager_hid_poll,
   iohidmanager_hid_joypad_rumble,
   iohidmanager_hid_joypad_name,
   "iohidmanager",
};
