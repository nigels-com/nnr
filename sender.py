#!/usr/bin/env python

import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

# 42355950 for ./nnr -n 5 -l 16

start = 0
end = 42355950
size = 1000000

for i in xrange(start, end, size):
  request = socket.recv() # Wait for a request from a worker
  command = './nnr -n 5 -l 16 --start %d --end %d | ./pes -n 5 -k 4'%(i, i+size)
  socket.send_string(command)
  print('Sent job: %s'%(command))
