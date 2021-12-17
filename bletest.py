import pygatt

#commands:     
#A/AO/O Command to get the next log page. 
#   Response Format is A/log index,timestamp,Event code . 
#   Each time you send this command, the log page in the response will be incremented by 1.
#    Such that if the size of the log is 600 entries, then you need to send this
#    command and get the response 600 times. to download all of the data. 
#   Event Codes: LOGGER_EVT_MOVEMENT_DETECTED=(0), LOGGER_EVT_LIGHTS_ON=(1), LOGGER_EVT_LIGHTS_OFF=(2),
#
#A/AO/G Command to read the size of the log, get current timestamp, and get lux threshold. 
#   Response format: A/number of log pages,current timestamp,current lux threshold
#
#A/AO/A Clear the log. Response: A/OK



class HB_BLE_Terminal():

    def __init__(self):
        self.flag_waiting_for_noti = False
        self.last_noti_val = ''

    def hb_ble_command(self,cmd,device):
            #s = "A/AO/G"
            b = bytearray()
            b.extend(map(ord, cmd))
            device.char_write('e093f3b7-00a3-a9e5-9eca-40036e0edc24',b,wait_for_response=False)
            self.flag_waiting_for_noti = True
            while(self.flag_waiting_for_noti == True):
                pass
            return self.last_noti_val

    def notification_received(self,handle,value):
        valstr = str(value, 'UTF-8')
        print(valstr)
        self.flag_waiting_for_noti = False
        self.last_noti_val = valstr
    

def main():
    t = HB_BLE_Terminal()
    adapter = pygatt.BGAPIBackend(serial_port='COM77')
    adapter.start()
    result = adapter.scan()
    for res in result:
        if(res['name'] == 'HB_BLE_Terminal'):
            print(res['address'])
            device = adapter.connect(res['address'])
            device.discover_characteristics()
            device.subscribe('e093f3b6-00a3-a9e5-9eca-40026e0edc24', callback=t.notification_received, indication=False,wait_for_response=False)
            
            #get the log size
            stats = t.hb_ble_command('A/AO/G',device)
            log_size = stats.split(',')[0]
            
            #print everything in the log
            for i in range(int(log_size[2:])):
                t.hb_ble_command('A/AO/O',device)
            
            # at the end, clear the log
            t.hb_ble_command('A/AO/A',device)
            
            device.disconnect()

main()
            
        