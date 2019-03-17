#!/usr/bin/env python
import pika

credentials = pika.PlainCredentials('test', 'test')
connection = pika.BlockingConnection(pika.ConnectionParameters(host='10.0.0.21', credentials=credentials))

channel = connection.channel()
channel.queue_declare(queue='worker')
#channel.basic_publish(exchange='', routing_key='worker', body='uname -a')
#channel.basic_publish(exchange='', routing_key='worker', body='pwd')

# 42355950 for ./nnr -n 5 -l 16

start = 0
end = 42355950
size = 1000000
for i in xrange(start, end, size):
  command = './nnr -n 5 -l 16 --start %d --end %d | ./pes -n 5 -k 4'%(i, i+size)
  channel.basic_publish(exchange='', routing_key='worker', body=command)
  print('Sent job: %s'%(command))

connection.close()

