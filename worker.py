#!/usr/bin/env python

from __future__ import print_function

import pika
import sh
import io
import sys
import json
import time

credentials = pika.PlainCredentials('test', 'test')
connection = pika.BlockingConnection(pika.ConnectionParameters(host='10.0.0.21', credentials=credentials))

#def exit():
#    raise StopIteration()
#connection.add_timeout(10.0, exit)

success = { False: 0, True: 0 }

try:
    channel = connection.channel()
    channel.queue_declare(queue='worker')
    channel.basic_qos(prefetch_count=1)

    def done(cmd, s, exit_code):
        success[s] = success[s]+1

    def callback(ch, method, properties, body):
        out = io.StringIO()
        err = io.StringIO()
        print('Job: %s'%(body))
        start = time.time()
        command = sh.bash(['-c', unicode(body, 'utf-8')], _out = out, _err = err, _done = done)
        end = time.time()
        ch.basic_ack(delivery_tag = method.delivery_tag)
        print(json.dumps({ 
            "command" : body, 
            "output": out.getvalue().splitlines(), 
            "error": err.getvalue().splitlines(), 
            "success": True, 
            "time": round(end-start, 3) }))

    channel.basic_consume(callback, queue='worker')
    channel.start_consuming()

#except StopIteration as e:
except KeyboardInterrupt as e:
    print('%d jobs completed, %s succeeded'%(success[True]+success[False], success[True]))
