#!/usr/bin/env python

from __future__ import print_function

import time
import zmq

import sh
import io
import sys
import json
import time

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

success = { False: 0, True: 0 }

def done(cmd, s, exit_code):
    success[s] = success[s]+1

try:
    while True:
        socket.send('Request') # Request a job from the sender
        body = socket.recv_string()
        print('Job: %s'%(body))
        out = io.StringIO()
        err = io.StringIO()
        start = time.time()
        command = sh.bash(['-c', body], _out = out, _err = err, _done = done)
        end = time.time()
        result = { 
            "command" : body, 
            "output": out.getvalue().splitlines(), 
            "error": err.getvalue().splitlines(), 
            "success": True, 
            "time": round(end-start, 3) }
        print(json.dumps(result))

except KeyboardInterrupt as e:
    print('%d jobs completed, %s succeeded'%(success[True]+success[False], success[True]))
