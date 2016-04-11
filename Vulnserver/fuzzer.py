import boofuzz
import socket

TARGET_IP                    =    "192.168.1.62"
TARGET_PORT                  =    9999
LOGGER                       =    boofuzz.FuzzLogger(fuzz_loggers=[boofuzz.FuzzLoggerText()])
SESSION                      =    boofuzz.sessions.Session(sleep_time=0.0,fuzz_data_logger
                                                           =LOGGER)
CONNECTION                   =    boofuzz.SocketConnection(TARGET_IP, TARGET_PORT, 
                                                           proto="tcp")
TARGET                       =    boofuzz.sessions.Target(CONNECTION)

SESSION.add_target(TARGET)

boofuzz.s_initialize("trunc")
boofuzz.s_string("TRUN")
boofuzz.s_delim(" ")
boofuzz.s_string("anonymous")
boofuzz.s_static("\r\n")    


SESSION.connect(boofuzz.s_get("trunc"))
SESSION.fuzz()
