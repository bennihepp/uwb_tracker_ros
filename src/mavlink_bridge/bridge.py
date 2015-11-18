import pymavlink.dialects.v10.uwb as uwb


class MAVLinkBridge(object):

    def __init__(self, serial):
        self._serial = serial
        if self._serial.timeout != 0:
            self._serial.timeout = 0
            print("Warning: Setting serial port timeout to 0")
        self._mav = uwb.MAVLink(self._serial)

    def send_message(self, msg):
        self._mav.send(msg)
        self._serial.flushOutput()

    def try_receive_message(self):
        c = self._serial.read(1)
        while len(c) > 0:
            msg = self._mav.parse_char(c)
            if msg is not None:
                return msg
            c = self._serial.read(1)
        return None
