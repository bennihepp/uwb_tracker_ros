# -*- coding: utf-8 -*-
"""
Created on Fri Nov 13 14:22:53 2015

@author: Benjamin Hepp
"""

import struct
from collections import namedtuple

class UWBStruct(object):

    UWB_TYPE = 1
    UWB_TUPLE = namedtuple('UWB_TUPLE', ['type', 'device_id', 'stamp_us', 'range', 'target_id'])
    STRUCT_FORMAT = '=BHifH'

    def __init__(self):
        self.struct = struct.Struct(self.STRUCT_FORMAT)

    def decode(self, message, print_error=False):
        try:
            fields = self.struct.unpack(message)
            return self.UWB_TUPLE._make(fields)
        except struct.error:
            if print_error:
                print struct.error
            return None

    def encode(self, device_id, stamp_us, range_value, target_id):
        return self.struct.pack(self.UWB_TYPE, device_id, stamp_us, range_value, target_id)

    def size(self):
        return self.struct.size

class UWBTextStruct(UWBStruct):

    TEXT_PREAMBLE = '^R> '

    def __init__(self):
        pass

    def decode(self, message):
        try:
            if len(message) > 4 and message[:4] == self.TEXT_PREAMBLE:
                message = message.rstrip()
                fields = message[4:].split(' ')
                fields[0] = int(fields[0])
                if fields[0] == self.UWB_TYPE:
                    for i in [1, 2, 4]:
                        fields[i] = int(fields[i])
                    fields[3] = float(fields[3])
                    return self.UWB_TUPLE._make(fields)
        except ValueError, e:
            print("Message had invalid format: {}".format(e))
            print("Message string: {}".format(message))
        except:
            import sys
            print("Failed to decode message: {}".format(sys.exc_info()[0]))
            print("Message string: {}".format(message))
        return None

    def encode(self, device_id, stamp_us, range_value, target_id):
        return self.struct.pack(self.UWB_TYPE, device_id, stamp_us, range_value, target_id)
