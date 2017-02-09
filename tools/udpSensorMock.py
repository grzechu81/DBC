#!/usr/bin/env python
# encoding: utf-8

import sys
import socket
import time
import random

from optparse import OptionParser

__all__ = []
__version__ = 0.1
__date__ = '2017-02-07'
__updated__ = '2017-02-07'

# voltage monitor
# @PWR,12.1,12.3,-01.3*<CRC>
# 
# level monitor
# @FLVL,0,83*<CRC>
# @WLVL,0,100*<CRC>
# 
# enviroment monitor
# @ENV,0,22.5,1024.5*<CRC>   


def main(argv=None):
    '''Command line options.'''
    
    if argv is None:
        argv = sys.argv[1:]
    try:
        # setup option parser
        parser = OptionParser()
        parser.add_option("-s", "--service", dest="service", help="sets the service to be mocked [default: battery]", metavar="SERVICE")
        parser.add_option("-i", "--interval", dest="interval", help="sets the interval between messages [default: 5]", metavar="INTERVAL")
        parser.add_option("-a", "--address", dest="address", help="ip address [default: 127.0.0.1]", metavar="ADDRESS")
        parser.add_option("-p", "--port", dest="port", help="ip port [default: 5555]", metavar="PORT")

        # set defaults
        parser.set_defaults(service="battery", interval=4, address="127.0.0.1", port=5555)

        # process options
        (opts, args) = parser.parse_args(argv)

        if opts.service:
            print("service: %s" % opts.service)
        if opts.interval:
            print("interval: %s s" % opts.interval)
        if opts.address:
            print("address: %s" % opts.address)
        if opts.port:
            print("port: %s" % opts.port)
            
        print("--------------------\n")

        # MAIN BODY #
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                     
        while True:
            dataToSend = createDataToSend(opts.service)
            sock.sendto(dataToSend, (opts.address, int(opts.port)))
            print("sent: %s" % dataToSend)
            time.sleep(int(opts.interval))

    except Exception as e:
        sys.stderr.write("Something went wrong")
        return 2
 
def createDataToSend(service):
    if service == "battery":
        return createBatteryStatus()
    elif service == 'fuel':
        return createFuelStatus()
    elif service == 'water':
        return createWaterStatus()
    return b''

def createBatteryStatus():
    vol1 = random.uniform(10, 14.5)
    vol2 = random.uniform(10, 14.5)
    current = random.uniform(-10, 10)
    
    answer = '@PWR,'
    answer += str("%.1f" % vol1) + ','
    answer += str("%.1f" % vol2) + ','
    answer += str("%.1f" % current) + '*'
    
    crc = calculateCrc(answer)
    answer += str("{:02x}".format(crc))
    return answer.encode('utf-8')

def createFuelStatus():
    answer = '@FLVL,0,'
    answer += str(random.randint(0,100)) + '*'
    crc = calculateCrc(answer)
    answer += str("{:02x}".format(crc))
    return answer.encode('utf-8')


def createWaterStatus():
    answer = '@WLVL,0,'
    answer += str(random.randint(0,100)) + '*'
    crc = calculateCrc(answer)
    answer += str("{:02x}".format(crc))
    return answer.encode('utf-8')
       

def calculateCrc(data):
    crc = 0;    
    for c in data:
        crc = crc ^ ord(c)
    return crc

if __name__ == "__main__":
    sys.exit(main())