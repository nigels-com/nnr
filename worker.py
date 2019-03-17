#!/usr/bin/env python

import pika
import sh
import io
import sys

credentials = pika.PlainCredentials('test', 'test')
connection = pika.BlockingConnection(pika.ConnectionParameters(host='10.0.0.21', credentials=credentials))

def exit():
    raise StopIteration()

connection.add_timeout(2.0, exit)
success = { False: 0, True: 0 }

try:
    channel = connection.channel()
    channel.queue_declare(queue='worker')

    def done(cmd, s, exit_code):
        success[s] = success[s]+1

    def callback(ch, method, properties, body):
        sh.bash(['-c',body], _out = sys.stdout, _done = done)
        ch.basic_ack(delivery_tag = method.delivery_tag)

    channel.basic_consume(callback, queue='worker')
    channel.start_consuming()

except StopIteration as e:
    print('%d jobs completed, %s succeeded'%(success[True]+success[False], success[True]))
