#!/usr/bin/env python
import pika

credentials = pika.PlainCredentials('test', 'test')
connection = pika.BlockingConnection(pika.ConnectionParameters(host='10.0.0.21', credentials=credentials))

channel = connection.channel()
channel.queue_declare(queue='worker')
#channel.basic_publish(exchange='', routing_key='worker', body='uname -a')
#channel.basic_publish(exchange='', routing_key='worker', body='pwd')

start = 0
end = 500
batchsize = 10000000
for i in xrange(start, end):
  command = './nnr -n 5 -l 16 --start %d --end %d | ./pes -n 5 -k 4'%(i*batchsize, (i+1)*batchsize)
  channel.basic_publish(exchange='', routing_key='worker', body=command)
  print('Sent job: %s'%(command))

connection.close()

