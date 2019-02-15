import os
import pyshark
import sys
import glob
import logging
import re
from termcolor import colored, cprint
import time
from collections import OrderedDict

import tarfile
import subprocess
import shutil
import plistlib
import biplist
import getopt


global source_port, destination_port
source_port = 0
destination_port = 0
global source_IP, dest_IP

global list_SourcePort, list_DestinationPort
global videoSourcePort, audioSourcePort, videoDestinationPort, audioDestinationPort
videoSourcePort = 0
audioSourcePort = 0
videoDestinationPort = 0
audioDestinationPort = 0
global homekitFilePath
global pcapFilePath
global configFilePath
global homekitFileFound, pcapFileFound, configFileFound

global sourceIPLog, destIPLog, sourcePortLog, destPortLog
global __location__
__location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))


#Find homekit log file and pcap file

homekitFileFound = False
pcapFileFound = False
configFileFound = False

for file in os.listdir(__location__):
    if file.endswith(".log") or file.endswith(".txt"):
        homekitFilePath = os.path.join(__location__, file)
        homekitFileFound = True
    elif file.endswith(".pcap") or file.endswith("pcapng"):
        pcapFilePath = os.path.join(__location__, file)
        pcapFileFound = True
    elif file.endswith(".plist"):
        configFilePath = os.path.join(__location__, file)
        configFileFound = True

if (homekitFileFound == False and pcapFileFound == False) or (configFileFound == False and pcapFileFound == False):
    print('There are no log files, plists or pcap files to process. Please copy the HomeKit log file or the StreamsConfiguration plist file and the corresponding PCAP file in the script folder and run the script again. \n','red')
    sys.exit()

timest = str(time.strftime('%Y%m%d-%H%M%S'))
#global str

global RTPParserLog
logDirectoryPath =  str((__location__)) + '/Logs'
if not os.path.exists(logDirectoryPath):
    os.mkdir(logDirectoryPath)
RTPParserLog = open(__location__ + '/Logs/RTPParser' + str(timest) + '.log', 'w')

global testsPassed, testsFailed, TotalTests
testsPassed = 0
testsFailed = 0
TotalTests = 12
global dictRTPTests
dictRTPTests = {}
dictRTPTests = OrderedDict()
src_port = 0
dest_port = 0
#Initial Config for camera stream


def getInitialConfig():
    countPorts = 0
    countDestPorts = 0
    global source_IP
    source_IP = 0
    dest_IP = 0
    list_SourcePort = []
    list_DestinationPort = []

    if homekitFileFound == True:

        sourcePortLog = open(homekitFilePath)
        for sourcePortLine in sourcePortLog:
            sourcePortLine = sourcePortLine.rstrip()
            if('remoteAddress.port' in sourcePortLine):
                list_SourcePort.append(re.findall(r'remoteAddress.port = ([0-9]+)',sourcePortLine))
                countPorts += 1
            if countPorts == 2:
                break
        sourcePortLog.close()


        videoSourcePort = list_SourcePort[0]
        global audioSourcePort
        audioSourcePort = list_SourcePort[1]

        destPortLog = open(homekitFilePath)

        for destPortLine in destPortLog:
            destPortLine = destPortLine.rstrip()
            if ('localAddress.port' in destPortLine):
                list_DestinationPort.append(re.findall(r'localAddress.port = ([0-9]+)', destPortLine))
                countDestPorts += 1
            if countDestPorts == 2:
                break

        destPortLog.close()

        global videoDestinationPort
        videoDestinationPort = list_DestinationPort[0]
        global audioDestinationPort
        audioDestinationPort = list_DestinationPort[1]

        sourceIPLog = open(homekitFilePath)

        for sourceIPLine in sourceIPLog:
            sourceIPLine = sourceIPLine.rstrip()
            if 'remoteAddress.ip' in sourceIPLine:
                source_IP = re.findall(r'remoteAddress.ip = ([0-9]+.[0-9]+.[0-9]+.[0-9]+)', sourceIPLine)
                break
        sourceIPLog.close()


        destIPLog = open(homekitFilePath)

        for destIPLine in destIPLog:
        # destIPLine = destIPLine.rstrip()
            if 'localAddress.ip' in destIPLine:
                dest_IP = re.findall(r'localAddress.ip = ([0-9]+.[0-9]+.[0-9]+.[0-9]+)', destIPLine)

                break
        destIPLog.close()

        videoSSRCLog = open(homekitFilePath)

        global videoSSRC
        for videoSSRCLine in videoSSRCLog:
            if 'videoSSRC' in videoSSRCLine:
                videoSSRC = re.findall(r'videoSSRC = ([0-9]+)', videoSSRCLine)
                break

        videoSSRCLog.close()

        audioSSRCLog = open(homekitFilePath)
        global audioSSRC
        for audioSSRCLine in audioSSRCLog:
            if 'audioSSRC' in audioSSRCLine:
                audioSSRC = re.findall(r'audioSSRC = ([0-9]+)', audioSSRCLine)
                break
        audioSSRCLog.close()

    elif configFileFound == True:
        '''
        global videoSourcePort
        global audioSourcePort
        global videoDestinationPort
        global audioDestinationPort
        '''

        global config_plist
        config_plist = biplist.readPlist(configFilePath)

        global streamConfigNum
        streamConfigNum = 0
        streamConfigNum = max(re.findall(r'StreamConfig-([0-9]+)', str(config_plist)))
        global audiopTime

        audiopTime = 20
        global audioCodec
        audioCodec = ''
        list_SourcePort.append(config_plist['StreamConfig-'+str(streamConfigNum)]['video']['sourcePort'])
        list_DestinationPort.append(config_plist['StreamConfig-'+str(streamConfigNum)]['video']['destinationPort'])
        list_SourcePort.append(config_plist['StreamConfig-'+str(streamConfigNum)]['audio']['sourcePort'])
        list_DestinationPort.append(config_plist['StreamConfig-'+str(streamConfigNum)]['audio']['destinationPort'])
        source_IP = config_plist['StreamConfig-'+str(streamConfigNum)]['sourceIP']
        dest_IP = config_plist['StreamConfig-'+str(streamConfigNum)]['destinationIP']
        videoSSRC = config_plist['StreamConfig-'+str(streamConfigNum)]['video']['ssrc']
        audioSSRC = config_plist['StreamConfig-'+str(streamConfigNum)]['audio']['ssrc']
        audiopTime = config_plist['StreamConfig-'+str(streamConfigNum)]['audio']['pTime']
        audioCodec = config_plist['StreamConfig-' + str(streamConfigNum)]['audio']['codecName']


        videoSourcePort = list_SourcePort[0]


        audioSourcePort = list_SourcePort[1]


        #print 'audioSourcePort = '+ str(audioSourcePort)

        videoDestinationPort = list_DestinationPort[0]

        #print 'video destination port = ' + str(videoDestinationPort)

        audioDestinationPort = list_DestinationPort[1]

        #print 'audio destination port = ' + str(audioDestinationPort)



#run initial setup scripts to validate IP address, Ports and SSRC


def intialSettings():

    cprint('Running Initial Setup Tests', 'yellow')
    RTPParserLog.write('Running Initial Setup Tests\n')
    global source_port
    videoSourcePort = config_plist['StreamConfig-'+str(streamConfigNum)]['video']['sourcePort']
    source_port = videoSourcePort
    global destination_port
    destination_port = videoDestinationPort

    count_InitialConfig = 0
    list_ = []
    initConfig_cap = pyshark.FileCapture(pcapFilePath, True, 'h264', False, None, 'wpa-pwk',{'udp.port== ' +str(source_port) : 'rtp', 'udp.port==' +str(destination_port) : 'rtp'})
    #print str(initConfig_cap)
    porttestsPassed = False
    ssrctestsPassed = False
    rtpandrtcpPortstestsPassed = False
    global src_port
    global dest_port
    src_port = 0
    dest_port = 0
    for pkt_IC in initConfig_cap:

        src_port = pkt_IC[pkt_IC.transport_layer].srcport
        #print src_port
        #print videoSourcePort
        dest_port = pkt_IC[pkt_IC.transport_layer].dstport
        #print dest_port
        #print videoDestinationPort

        if (int(videoSourcePort) == int(src_port) and int(videoDestinationPort == int(dest_port))):
            porttestsPassed = True
            continue
        elif (videoSourcePort != src_port) or (destination_port != dest_port):

            porttestsPassed = False
            break


    for pkt_SSRC in initConfig_cap:
        ssrc = pkt_SSRC.rtp.ssrc

        if(int(ssrc, 0) == int(int(videoSSRC))):
            ssrctestsPassed = True
            continue
        else:
            ssrctestsPassed = False
            break



    initConfig_cap.close()

    initRTCP_cap = pyshark.FileCapture(pcapFilePath, True, 'rtp', False, None, 'wpa-pwk',{'udp.port== ' +str(source_port) : 'rtp', 'udp.port==' +str(destination_port) : 'rtp'})
    for pkt_RTCPPort in initRTCP_cap:


        src_port_RTCP = pkt_RTCPPort[pkt_RTCPPort.transport_layer].srcport

        dest_port_RTCP = pkt_RTCPPort[pkt_RTCPPort.transport_layer].dstport


        if int(src_port_RTCP) == int(videoSourcePort) and int(dest_port_RTCP) == int(videoDestinationPort):
            rtpandrtcpPortstestsPassed = True

        break
    initRTCP_cap.close()
    global testsPassed, testsFailed
    if porttestsPassed == True and ssrctestsPassed == True and rtpandrtcpPortstestsPassed == True:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('Test Passed!\n')
        RTPParserLog.write('RTP, RTCP ports are correct, Video SSRC is correct. \n\n')

        testsPassed = 3
        dictRTPTests['RTP Ports'] = True
        dictRTPTests['Video SSRC'] = True
        dictRTPTests['RTP and RTCP Ports'] = True
    else:
        if porttestsPassed == False:
            cprint('Test Failed! Ports are incorrect\n', 'red')
            RTPParserLog.write('RTP ports do NOT match HomeKit configuration. \n')
            testsFailed +=1
        if ssrctestsPassed == False:
            cprint('Test Failed! VideoSSRC does not match HomeKit configuration!\n', 'red')
            RTPParserLog.write('Video SSRC does NOT match HomeKit configuration. \n')
            testsFailed +=1
        if rtpandrtcpPortstestsPassed == False:
            cprint('Test Failed! RTP and RTCP ports are incorrect', 'red')
            RTPParserLog.write('RTP and RTCP ports do NOT match. \n')
            testsFailed+=1
        cprint('Exiting script as config is incorrect!', 'red')
        RTPParserLog.write('Exiting script. \n\n')
        #sys.exit()

#Validate MTU size test
def largestMTUSize():

    cprint('Running MTU Size test', 'yellow')
    RTPParserLog.write('Running MTU Size test. \n')
    MTUSize_cap = pyshark.FileCapture(pcapFilePath, True,
                                       'h264', True, None, 'wpa-pwk',
                                       {'udp.port== '+str(source_port) : 'rtp',
                                        'udp.port== ' +str(destination_port) : 'rtp'})

    global testsPassed, testsFailed
    hasPassed = False
    for mtuPkt in MTUSize_cap:
        if int(mtuPkt.length) > 1420:

            hasPassed = False
            break
        else:
            hasPassed = True

    if hasPassed == True:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('MTU size is correct. \n\n')
        dictRTPTests['MTU Size'] = True
        testsPassed +=1
    else:
        cprint('Test Failed! MTU Size is greater than 1420\n', 'red')
        RTPParserLog.write('MTU size is greater than 1420. \n\n')
        testsFailed+=1
        dictRTPTests['MTU Size'] = False
    MTUSize_cap.close()

#Find average framerate
def frameRate(maxFramerate):

    cprint('Running Framerate Test', 'yellow')
    RTPParserLog.write('User put max framerate value: ' + str(maxFramerate) + ' FPS\n')
    RTPParserLog.write("Running Framerate Test \n")
    global listTimeElapsed
    listTimeElapsed = []
    framerate_cap = pyshark.FileCapture(pcapFilePath, True,
                                       '(h264 && rtp.marker == 1)', True, None, 'wpa-pwk',
                                       {'udp.port== '+str(source_port) : 'rtp',
                                        'udp.port== ' +str(destination_port) : 'rtp'})
    countTimeStamp = 0
    listTimeStamp = []
    countTime = 0
    startTime = 0.0
    endTime = 1.0
    countFrames = 0
    global listFramerate
    global listTimeGone
    listFramerate = []
    listTimeGone = []
    for framePkt in framerate_cap:
        # print (pkt2.time)
        listTimeElapsed.append(framePkt.time)
        countTime += 1
        if startTime < float(framePkt.time) <= endTime:
            countFrames += 1
        elif float(framePkt.time) > endTime:
            #print countFrames
            listFramerate.append(countFrames)
            listTimeGone.append(endTime)
            RTPParserLog.write(str(countFrames) +'\n')
            countFrames = 0
            startTime += 1
            endTime += 1
    global testsPassed, testsFailed
    averageFramerate = float(countTime)/float(listTimeElapsed[countTime-1])

    if averageFramerate < (maxFramerate - 5):
        cprint('Test Failed! Average framerate for the session is less than ' + str(maxFramerate), 'red')
        cprint('Your average framerate is:', 'red')
        cprint(averageFramerate, 'red')
        RTPParserLog.write('Tests Failed! Average framerate for the session is less than ' + str(maxFramerate) +'. \n')
        RTPParserLog.write('Your average framerate is: . \n')
        RTPParserLog.write(str(float(averageFramerate)) +str('\n\n'))
        testsFailed += 1
        dictRTPTests['Average Framerate'] = False
        print('\n')
    else:
        cprint('Test Passed!' ,'green')
        cprint('Your average framerate is: '+str(float(averageFramerate)) +'\n', 'green')
        RTPParserLog.write('Test Passed!.\n')
        RTPParserLog.write('Your average framerate is: ' + str(averageFramerate) + '\n\n')
        testsPassed += 1
        dictRTPTests['Average Framerate'] = True



    framerate_cap.close()

#validate timestamp increments for video stream
def timeStampIncrements():

    cprint ('Running Timestamp incremental test', 'yellow')
    RTPParserLog.write("Running Timestamp incremental test. \n")
    timeStamp_cap = pyshark.FileCapture(pcapFilePath, True,
                                       '(h264 && rtp.marker == 1)', False, None, 'wpa-pwk',
                                       {'udp.port== '+str(source_port) : 'rtp',
                                        'udp.port== ' +str(destination_port) : 'rtp'})
    countTime = 0
    countTimeStamp = 0
    listTimeStamp = []
    #listTimeElapsed=[]
    for pktTS in timeStamp_cap:
        listTimeStamp.append(pktTS.rtp.timestamp)
        countTimeStamp += 1


    markedFrameTime_1 = float(listTimeElapsed[countTime-2])
    markedFrameTime_2 = float(listTimeElapsed[countTime-31])

    packetTimeDifference = float(markedFrameTime_1) - float(markedFrameTime_2)
    timeStamp_1 = float(listTimeStamp[countTimeStamp -2])
    timeSTamp_2 = float(listTimeStamp[countTimeStamp - 31])
    packetTimeStampDifference = float(timeStamp_1) - float(timeSTamp_2)

    RTPTSDiff = float(packetTimeDifference) * 90000

    TSDifference = (packetTimeStampDifference/1000) - (RTPTSDiff/1000)
    global testsPassed, testsFailed
    if (TSDifference) > 100:
        cprint('Test Failed! Timestamp increments are greater than required.\n', 'red')
        RTPParserLog.write('Test Failed! Timestamp increments are greater than required\n\n')
        testsFailed +=1
        dictRTPTests['Timestamp Increments'] = False
    else:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('Test Passed \n\n')
        testsPassed+=1
        dictRTPTests['Timestamp Increments'] = True
    timeStamp_cap.close()


#validate all packets are non-interleaved and the camera follows the spec
def nonInterleavedVideo():

    cprint('Running Non-Interleaved Video Test', 'yellow')
    RTPParserLog.write("Running Non-Interleaved Video test \n")
    nonIL_cap = pyshark.FileCapture(pcapFilePath, True,
                                       'h264', True, None, 'wpa-pwk',
                                       {'udp.port== '+str(source_port) : 'rtp',
                                        'udp.port== ' +str(destination_port) : 'rtp'})

    countInterleaved = 0
    listFrameType = []
    hasPassed = False
    countSTAPA = 0
    countFUA = 0

    for pktIL in nonIL_cap:
        # print (pkt2.time)
        listFrameType.append(pktIL.info)
        if re.findall('STAP-B', listFrameType[countInterleaved]) or re.findall('FU-B', listFrameType[countInterleaved]):
            hasPassed = False
            break

        else:
            hasPassed = True
            countInterleaved += 1
    #if countSTAPA == 0 or countFUA == 0:
        #hasPassed = False
    global testsPassed, testsFailed
    if hasPassed == True:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('Test Passed \n\n')
        testsPassed +=1
        dictRTPTests['Non Interleaved Video'] = True
    else:
        '''
        if countSTAPA == 0:
            cprint('Test Failed! No STAP-A found.\n', 'red')
            RTPParserLog.write('Test Failed! No STAP-A found.\n')
            testsFailed += 1
            dictRTPTests['Non Interleaved Video'] = False
        elif countFUA == 0:
            cprint('Test Failed! No FU-A found.\n', 'red')
            RTPParserLog.write('Test Failed! No FU-A found.\n')
            testsFailed += 1
            dictRTPTests['Non Interleaved Video'] = False
            '''
    #else:
        cprint('Test Failed! Only STAP-A, FU-A and NAL units allowed for Non-Interleaved Video. Make sure the stream is unencrypted and try again.\n', 'red')
        RTPParserLog.write('Test Failed! Only STAP-A, FU-A and NAL units allowed for Non-Interleaved Video. Make sure the stream is unencrypted and try again. \n\n')
        testsFailed +=1
        dictRTPTests['Non Interleaved Video'] = False

    nonIL_cap.close()



#Validate camera TMMBN feedback

def RTCPFBCheck():

    cprint('Running RTCP-FB TMMBR Test', 'yellow')
    RTPParserLog.write('Running RTCP-FB TMMBR Test \n')
    RTCPFB_cap = pyshark.FileCapture(pcapFilePath, True,
                                       '(rtcp.rtpfb.fmt == 3 || rtcp.rtpfb.fmt == 4)', False, None, 'wpa-pwk',
                                       {'udp.port== '+str(source_port) : 'rtp',
                                        'udp.port== ' +str(destination_port) : 'rtp'})

    countRTCPFB = 0
    listRTCPFB = []

    tmmbnArrivalTime = 0
    countFailureRate = 0
    hasRTCPFBPassed = True
    countPasses = 0
    for pktFB in RTCPFB_cap:
        try:
            listRTCPFB.append(int(pktFB.layers[5].rtpfb_fmt))

            if int(pktFB.layers[5].rtpfb_fmt) == 4:
                tmmbnArrivalTime = float(pktFB.frame_info.time_delta_displayed)
                if tmmbnArrivalTime > 0.250:

                    cprint ('Delay in receiving tmmbn is: ' +str(tmmbnArrivalTime), 'red')
                    cprint ('Frame number: ' +str(pktFB.frame_info.number), 'red')
                    countFailureRate +=1
                elif countFailureRate > 20:

                    hasRTCPFBPassed = False
                    break

            elif int(pktFB.layers[5].rtpfb_fmt) == 3:
                continue
        except AttributeError as e:
                # ignore packets with bad padding
            pass
    global testsPassed, testsFailed
    if hasRTCPFBPassed == False:
        cprint('Test Failed! TMMBN feedback should follow TMMBR within 250 ms\n', 'red')
        RTPParserLog.write('Test Failed! TMMBN feedback should follow TMMBR within 250 ms \n\n')

        testsFailed +=1
        dictRTPTests['TMMBN Feedback'] = False
    else:
        cprint ('Test Passed!','green')
        RTPParserLog.write('Test Passed \n\n')
        dictRTPTests['TMMBN Feedback'] = True
        testsPassed +=1
    RTCPFB_cap.close()


#Validate camera PLI implementation

def RTCPPLICheck():

    cprint('Running PLI Response Verification Test', 'yellow')
    RTPParserLog.write('Running PLI Response Verification Test \n')
    RTCPPLI_cap = pyshark.FileCapture(pcapFilePath, True,
                                       '(h264.nal_unit_type == 5 && rtp.marker == 1) || (rtcp.psfb.fmt == 1)', False, None, 'wpa-pwk',
                                       {'udp.port== '+str(source_port) : 'rtp',
                                        'udp.port== ' +str(destination_port) : 'rtp'})

    iOSSentPLI = False
    countPLIs = 0
    listRTCPPLI = []
    listRTCP = []
    pliResponseTime = 0
    countFailureRate = 0
    hasRTCPPLIPassed = True
    countPackets = 0
    countPasses = 0
    for pktPLI in RTCPPLI_cap:
        try:
            listRTCPPLI.append(int(pktPLI.layers[3].p_type) or int(pktPLI.layers[5].psfb_fmt) or int(pktPLI.layers[5].rtpfb_fmt) )



            if int(pktPLI.layers[3].p_type) == 99 and iOSSentPLI == True:
                pliResponseTime = float(pktPLI.frame_info.time_delta_displayed)
                if pliResponseTime > 0.250 :
                    cprint ('Delay in receiving Key Frame after PLI is: ' +str(pliResponseTime), 'red')
                    cprint ('Frame number: ' +str(pktPLI.frame_info.number), 'red')
                    countFailureRate += 1
                    #hasRTCPPLIPassed = False
                elif countFailureRate > 10:

                    hasRTCPPLIPassed = False
                    break
                else:
                    if countFailureRate <= 10:
                        hasRTCPPLIPassed = True

            iOSSentPLI = False

        except AttributeError as e:
                # ignore packets with bad padding

            listRTCPPLI.append(int(pktPLI.layers[6].rtpfb_fmt) or int(pktPLI.layers[5].rtpfb_fmt))
            if int(pktPLI.layers[5].psfb_fmt) or int(pktPLI.layers[5].rtpfb_fmt) == 1:
                iOSSentPLI = True
                countPLIs += 1
            pass
    global testsPassed, testsFailed

    if hasRTCPPLIPassed == False or countPLIs >=10:
        cprint('Test Failed! Camera should send a Key Frame within 250 ms of a PLI\n', 'red')
        RTPParserLog.write('Test Failed! Camera should send a key frame within 250 ms of a PLI \n\n')
        testsFailed+=1
        dictRTPTests['PLI Response'] = False

    elif countPLIs == 0:
        cprint('Could not test! Please test with 1% packet loss to get valid data\n', 'Yellow')
        RTPParserLog.write('Test Failed! Please test with 1% packet loss to get valid data! \n\n')
        testsFailed+=1
        dictRTPTests['PLI Response'] = False
    else:
        cprint ('Test Passed!\n','green')
        RTPParserLog.write('Test Passed \n\n')
        dictRTPTests['PLI Response'] = True
        testsPassed+=1
    RTCPPLI_cap.close()


#Validate RTCP SR for Video
def RTCPSR():

    cprint('Running RTCP Video SR Test', 'yellow')
    RTPParserLog.write('Running RTCP Video SR test\n')
    RTCPSR_cap = pyshark.FileCapture(pcapFilePath, True,
                                     'rtcp.senderssrc == '+str(videoSSRC), False, None, 'wpa-pwk',
                                     {'udp.port== ' + str(source_port): 'rtp',
                                      'udp.port== ' + str(destination_port): 'rtp'})

    countRTCPFB = 0
    listRTCPFB = []
    global testsPassed, testsFailed
    hasPassed = False
    for pktSR in RTCPSR_cap:
        try:
            listRTCPFB.append(pktSR.layers[3].rc)
            if int(listRTCPFB[countRTCPFB]) != 0:
                hasPassed = False
                break
            else:
                countRTCPFB +=1
                hasPassed = True
        except AttributeError as e:
            # ignore packets with bad padding
            pass
    if hasPassed == True:
        cprint('Test Passed!\n' ,'green')
        RTPParserLog.write('Test Passed \n\n')
        testsPassed+=1
        dictRTPTests['RTCP-Video SR'] = True
    else:
        cprint('Test Failed! Reception Report count is not 0\n', 'red')
        RTPParserLog.write('Test Failed! Reception Report count is not 0 or RTCP FB does not exist \n\n')
        testsFailed+=1
        dictRTPTests['RTCP-Video SR'] = False


    RTCPSR_cap.close()

#Validate periodic Stap-A and periodic set (Stap-A, FU-A and Mark FU-A End)

def periodicKeyFrames():

    cprint('Running Periodic Key Frames Test', 'yellow')
    RTPParserLog.write('Running Periodic Key Frames Test \n')
    periodicKF_cap = pyshark.FileCapture(pcapFilePath, True,
                                     '(h264.nal_unit_hdr == 24)', False, None, 'wpa-pwk',
                                     {'udp.port== ' + str(source_port): 'rtp',
                                      'udp.port== ' + str(destination_port): 'rtp'})

    countperiodicKF = 0
    listPeriodicKF = []
    hasPeriodicKFPassed = False
    for pktPKF in periodicKF_cap:
        listPeriodicKF.append(pktPKF.layers[4].nal_unit_hdr)
        if int(listPeriodicKF[countperiodicKF]) != 24:
            hasPeriodicKFPassed = False
            break
        else:
            countperiodicKF +=1
            hasPeriodicKFPassed = True

    global testsPassed, testsFailed

    periodicKF_cap.close()

    periodicStapandFUA_cap = pyshark.FileCapture(pcapFilePath, True,
                                         '(h264.nal_unit_hdr == 24) || (h264.nal_unit_type == 5)', False, None, 'wpa-pwk',
                                         {'udp.port== ' + str(source_port): 'rtp',
                                          'udp.port== ' + str(destination_port): 'rtp'})

    countperiodicSF = 0
    listPeriodicSF = []
    listPeriodicTimeSTamp = []
    listPeriodicSeq = []
    global rtpTimeStamp
    rtpTimeStamp = 0
    global rtpSeqNumber
    rtpSeqNumber = 0
    hasPeriodicSFPassed = False
    countSequence = 0
    for pktPSF in periodicStapandFUA_cap:
        listPeriodicSF.append(pktPSF.layers[4].nal_unit_hdr)

        if int(pktPSF.layers[4].nal_unit_hdr) == 24 and countSequence == 0:
            listPeriodicTimeSTamp.append(pktPSF.rtp.timestamp)
            listPeriodicSeq.append(pktPSF.rtp.seq)
            rtpTimeStamp = int(pktPSF.rtp.timestamp)
            rtpSeqNumber = int(pktPSF.rtp.seq)
            countSequence+=1
        elif int(pktPSF.layers[4].nal_unit_hdr) == 28:
            listPeriodicTimeSTamp.append(pktPSF.rtp.timestamp)
            listPeriodicSeq.append(pktPSF.rtp.seq)
            if int(pktPSF.rtp.timestamp) != rtpTimeStamp or int(pktPSF.rtp.seq) <= rtpSeqNumber:
                hasPeriodicSFPassed = False
                break
            else:
                rtpSeqNumber = int(pktPSF.rtp.seq)
                hasPeriodicSFPassed = True
        else:
            break


    if hasPeriodicSFPassed==False or hasPeriodicKFPassed == False:
        if hasPeriodicSFPassed == False:
            cprint('Test Failed! RTP TimeStamp is not the same for the same sequence set or the set does not have an increasing sequence number\n', 'red')
            RTPParserLog.write('Test Failed! RTP TimeStamp is not the same for the same sequence set or the set does not have an increasing sequence number \n\n')
            testsFailed+=1
            dictRTPTests['Periodic Stap and FU'] = False
        else:
            cprint('Test Failed! No periodic Stap-A found\n', 'red')
            RTPParserLog.write('Test Failed! No periodic Stap-A found \n\n')
            testsFailed +=1
            dictRTPTests['Periodic Stap-A'] = False
    elif hasPeriodicSFPassed==True and hasPeriodicKFPassed == True:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('Test Passed \n')
        RTPParserLog.write('Test passed for periodic Stap and FU \n')
        RTPParserLog.write('Test Passed for periodic Stap-A \n\n')
        testsPassed += 2
        dictRTPTests['Periodic Stap and FU'] = True
        dictRTPTests['Periodic Stap-A'] = True

    periodicStapandFUA_cap.close()

#Validate audio settings

def intialAudioSettings():
    cprint('Running Audio Setup Tests', 'yellow')
    RTPParserLog.write('Running Audio Setup Tests \n')
    global audsource_port
    audsource_port = audioSourcePort
    global auddestination_port
    auddestination_port = 0
    auddestination_port = audioDestinationPort
    count_InitialConfig = 0
    source_port = audsource_port
    destination_port = auddestination_port
    #initAudioConfig_cap = pyshark.FileCapture(pcapFilePath, True, 'rtp.p_type == 110 && ip.src == ' +str(source_IP), False, None, 'wpa-pwk',{'udp.port== ' +str(source_port[0]) : 'rtp', 'udp.port==' +str(destination_port[0]) : 'rtp'})
    displayFilter = '(rtp.p_type == 110 && ip.src == ' +str(source_IP) +')'
    initAudioConfig_cap = pyshark.FileCapture(pcapFilePath, True, str(displayFilter), False, None, 'wpa-pwk', {'udp.port== ' + str(audioSourcePort): 'rtp','udp.port== ' + str(destination_port): 'rtp'})

    porttestsPassed = True
    ssrctestsPassed = True


    for pktAudio_IC in initAudioConfig_cap:


        src_port = pktAudio_IC[pktAudio_IC.transport_layer].srcport

        dest_port = pktAudio_IC[pktAudio_IC.transport_layer].dstport


        if (int(audioSourcePort) == int(src_port)) and (int(audioDestinationPort) == int(dest_port)):

            porttestsPassed = True
            break
        else:

            porttestsPassed = False
        break


    for pkt_SSRC in initAudioConfig_cap:
        ssrc = pkt_SSRC.rtp.ssrc

        if(int(ssrc, 0) == int(audioSSRC)):
            ssrctestsPassed = True
            break
        else:
            ssrctestsPassed = False
            break



    #initRTCP_cap = pyshark.FileCapture(pcapFilePath, True, 'rtp', False, None, 'wpa-pwk',{'udp.port== ' +str(source_port[0]) : 'rtp', 'udp.port==' +str(destination_port[0]) : 'rtp'})



    global testsPassed, testsFailed
    if porttestsPassed == True and ssrctestsPassed == True:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('Test Passed \n')
        RTPParserLog.write('RTP ports for Audio are correct \n')
        RTPParserLog.write('Audio SSRC is correct \n\n')
        testsPassed = 2
        dictRTPTests['Audio RTP Ports'] = True
        dictRTPTests['Audio SSRC'] = True
        #dictRTPTests['RTP and RTCP Ports'] = True
    else:
        if porttestsPassed == False:
            cprint('Test Failed! Audio Ports are incorrect\n', 'red')
            RTPParserLog.write('Test Failed! Audio ports are incorrect and do not meet HomeKit configuration \n\n')
            testsFailed +=1
        elif ssrctestsPassed == False:
            cprint('Test Failed! Audio SSRC does not match HomeKit configuration!\n', 'red')
            RTPParserLog.write('Test Failed! Audio SSRC does not match HomeKit configuration \n\n')
            testsFailed +=1
        cprint('Exiting script as config is incorrect!', 'red')

        sys.exit()
    initAudioConfig_cap.close()

#validate audio timestamp increments

def audioTimeStamp():
    cprint('Running Audio Timestamp Tests', 'yellow')

    RTPParserLog.write('Running Audio Timestamp Tests\n')
    global audsource_port
    audsource_port = audioSourcePort
    global auddestination_port
    auddestination_port = audioDestinationPort
    count_InitialConfig = 0
    source_port = audsource_port
    destination_port = auddestination_port
    # initAudioConfig_cap = pyshark.FileCapture(pcapFilePath, True, 'rtp.p_type == 110 && ip.src == ' +str(source_IP), False, None, 'wpa-pwk',{'udp.port== ' +str(source_port[0]) : 'rtp', 'udp.port==' +str(destination_port[0]) : 'rtp'})
    displayFilter = '(rtp.p_type == 110 && ip.src == ' + str(source_IP) + ')'
    initAudioTimeStamp_cap = pyshark.FileCapture(pcapFilePath, True,
                                              displayFilter, False, None, 'wpa-pwk',
                                              {'udp.port== ' + str(audioSourcePort): 'rtp',
                                               'udp.port== ' + str(destination_port): 'rtp'})

    timestamptestPassed = True
    rtpTimeStamp = 0
    rtpTimeStampNext = 0
    list_audiopTime = []

    #audiopTime = 20
    #audiopTimeVal = int(list_audiopTime[0])
    timestampInterval = 0
    if audioCodec == 'kAVCCodecTypeOpus24K':
        timestampInterval = (int(audiopTime) * 24)
        print str(audioCodec) + ' timestamp interval should be: ' + str(timestampInterval)
    elif audioCodec == 'kAVCCodecTypeOpus16K':
        timestampInterval = (int(audiopTime) * 16)
        print str(audioCodec) + ' timestamp interval should be: ' + str(timestampInterval)
    elif audioCodec == 'kAVCCodecTypeAACELD16K':
        timestampInterval = (float(audiopTime)/30 * 480)
        print str(audioCodec) + ' timestamp interval should be: ' + str(timestampInterval)
    elif audioCodec == 'kAVCCodecTypeAACELD24K':
        timestampInterval = (float(audiopTime)/20 * 480)
        print str(audioCodec) + ' timestamp interval should be: ' + str(timestampInterval)


    countcorrectTS = 0
    countwrongTS = 0
    countskippedTS = 0
    for pktAudio_TS in initAudioTimeStamp_cap:
        try:
            if rtpTimeStamp == 0:
                rtpTimeStamp = int(pktAudio_TS.layers[3].timestamp)
            elif rtpTimeStamp != 0:

                rtpTimeStampNext = int(pktAudio_TS.layers[3].timestamp)
                timestampDifference = rtpTimeStampNext - rtpTimeStamp
                if timestampDifference == timestampInterval:
                    timestamptestPassed = True
                    cprint('Timestamp difference is: ' + str(timestampDifference), 'green')
                    #cprint('Timestamp difference should be as per spec', 'green')
                    countcorrectTS += 1
                    cprint('Timestamp difference is: ' + str(timestampDifference), 'green')
                    #cprint('Frame number: ' + str(pktAudio_TS.frame_info.number), 'green')
                    rtpTimeStamp = rtpTimeStampNext

                elif timestampDifference % timestampInterval == 0:
                    countskippedTS += 1

                else:
                    cprint('Timestamp difference is: ' +str(timestampDifference), 'red')


                    timestamptestPassed = False
                    countwrongTS += 1

                    cprint('Frame number: ' + str(pktAudio_TS.frame_info.number), 'red')
                    rtpTimeStamp = rtpTimeStampNext
                    cprint('Please refer to spec for correct timestamp increments', 'red')

        except AttributeError as e:
            # ignore packets with bad padding
            pass
    #print countcorrectTS
    #print countwrongTS
    if timestamptestPassed == True or countskippedTS <=50 or countwrongTS < 10:
        cprint('Test Passed!', 'green')
        RTPParserLog.write('Test Passed \n\n')
        dictRTPTests['Audio Time Stamps'] = True
    elif timestamptestPassed == True or countskippedTS > 50 or countwrongTS < 10:
        cprint('Test Failed! Too many audio packets skipped.\n', 'red')
        RTPParserLog.write('Test Failed! Too many audio packets skipped. \n')
        dictRTPTests['Audio Time Stamps'] = False
    elif timestamptestPassed == True or countskippedTS > 50 or countwrongTS > 10:
        cprint('Test Failed! Incorrect timestamp increments and too many audio packets skipped.\n', 'red')
        RTPParserLog.write('Test Failed! Incorrect timestamp increments and too many audio packets skipped. \n')
        dictRTPTests['Audio Time Stamps'] = False

    else:
        cprint('Test Failed! Audio Timestamp increments are not correct.\n', 'red')
        RTPParserLog.write(
            'Test Failed! Audio Timestamp increments are not correct. \n')
        dictRTPTests['Audio Time Stamps'] = False

    initAudioTimeStamp_cap.close()



#validate audio SR test
def audio_SR(twoWayAudio):
    cprint('Running Audio SR Test', 'yellow')
    RTPParserLog.write('Running Audio SR Test \n')
    RTPParserLog.write('User put twoWayAudio value as: ' + str(twoWayAudio))
    global audsource_port
    audsource_port = audioSourcePort
    global auddestination_port
    auddestination_port = audioDestinationPort
    count_InitialConfig = 0
    source_port = audsource_port
    destination_port = auddestination_port
    # initAudioConfig_cap = pyshark.FileCapture(pcapFilePath, True, 'rtp.p_type == 110 && ip.src == ' +str(source_IP), False, None, 'wpa-pwk',{'udp.port== ' +str(source_port[0]) : 'rtp', 'udp.port==' +str(destination_port[0]) : 'rtp'})

    displayFilter = '(rtcp.pt == 200 & rtcp.senderssrc == ' + str(audioSSRC) + ')'
    RTCPAudioSR_cap = pyshark.FileCapture(pcapFilePath, True, displayFilter, False, None, 'wpa-pwk', {'udp.port== ' + str(audioSourcePort): 'rtp', 'udp.port== ' + str(destination_port): 'rtp'})

    countRTCPFB = 0
    listRTCPFB = []
    countfailedRTCPFB = 0
    global testsPassed, testsFailed
    hasPassed = True
    i = 0
    for pktSR in RTCPAudioSR_cap:
        try:
            listRTCPFB.append(pktSR.layers[3].rc)
            if twoWayAudio == 0:
                if int(listRTCPFB[i]) !=0:
                    print 'RTCPFB is: ' + str(int(listRTCPFB[i]))
                    i += 1
                    countfailedRTCPFB +=1
                    if countfailedRTCPFB == 1:
                        hasPassed = False
                        break
                else:
                    countRTCPFB += 1
                    i+=1
                    hasPassed = True
            else:
                if int(listRTCPFB[i]) ==0:
                    #print 'RTCPFB is: ' + str(int(listRTCPFB[i]))
                    countfailedRTCPFB += 1
                    if countfailedRTCPFB == 1:
                        hasPassed = False
                        i+=1
                        #break
                else:
                    countRTCPFB += 1
                    i+=1
                    hasPassed = True

        except AttributeError as e:
            # ignore packets with bad padding
            pass
    if hasPassed == True:
        cprint('Test Passed!\n', 'green')
        RTPParserLog.write('Test Passed \n\n')
        testsPassed += 1
        dictRTPTests['RTCP-Audio SR'] = True
    else:
        cprint('Test Failed! Reception Report count should be 0 for one-way audio and 1 for two-way audio\n', 'red')
        RTPParserLog.write('Test Failed! Reception Report count should be 0 for one-way audio and 1 for two-way audio \n\n')
        testsFailed += 1
        dictRTPTests['RTCP-Audio SR'] = False

    RTCPAudioSR_cap.close()

#Validate RTP results

def RTPResults():
    global testsPassed, testsFailed, TotalTests
    # cprint('Total tests = ' +str(int(TotalTests)))
    # cprint('Total tests passed = ' +str(int(testsPassed)), 'green')
    # cprint('Total tests failed = ' +str(int(testsFailed)), 'red')
    cprint('Total tests = ' + str(len(dictRTPTests.keys())))
    cprint('Total tests passed = ' + str(sum(dictRTPTests.values())), 'green')
    cprint('Total tests failed = ' + str(len(dictRTPTests) - sum(dictRTPTests.values())), 'red')

    print '\n\n'
    RTPParserLog.write('Total tests = ' + str(len(dictRTPTests.keys())))
    RTPParserLog.write('\n')
    RTPParserLog.write('Total tests passed = ' + str(sum(dictRTPTests.values())))
    RTPParserLog.write('\n')
    RTPParserLog.write('Total tests failed = ' + str(len(dictRTPTests) - sum(dictRTPTests.values())))
    RTPParserLog.write('\n\n')

    countTests = 1
    for tests in dictRTPTests:
        if dictRTPTests[tests] == True:
            cprint(str(countTests) + '. '+tests +' : Passed\n', 'green')
            RTPParserLog.write(str(countTests) + str('. ')+tests +' : Passed\n')
            countTests += 1
        elif dictRTPTests[tests] == False:
            cprint(str(countTests) + '. '+tests +' : Failed\n', 'red')
            RTPParserLog.write(str(countTests) + str('. ')+tests +' : Failed\n')
            countTests +=1

    RTPParserLog.close()
    for testFile in os.listdir(__location__):
        if testFile.endswith(".plist") or testFile.endswith(".pcap"):
            os.rename(testFile, testFile+str('-org'))
    return dictRTPTests

def prompt_user_for_supported_framerate():
    try:
        prompt = '\n Enter max supported framerate? -> '
        max_framerate = int(raw_input(prompt))
        return max_framerate
    except ValueError:
        print 'Value Error: Please enter an appropriate number.'
        pass
    return prompt_user_for_supported_framerate()

def prompt_user_for_multiway_audio():
    try:
        prompt = '\n Enter 0 for one-way audio and 1 for two-way audio? -> '
        twoWayAudio = int(raw_input(prompt))
        return twoWayAudio
    except ValueError:
        print 'Value Error: Please enter an appropriate number.'
        pass
    return prompt_user_for_multiway_audio()

def main():

    max_framerate = 30
    twoWayAudio = 1

    max_framerate = prompt_user_for_supported_framerate()
    twoWayAudio = prompt_user_for_multiway_audio()


    getInitialConfig()

    intialSettings()

    largestMTUSize()

    frameRate(max_framerate)

    timeStampIncrements()

    nonInterleavedVideo()

    RTCPFBCheck()

    RTCPPLICheck()

    RTCPSR()
    periodicKeyFrames()

    intialAudioSettings()

    audioTimeStamp()

    audio_SR(twoWayAudio)
    RTPResults()


if __name__ == "__main__":
    main()
