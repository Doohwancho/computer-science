# python3
# file: server.py
import json
import random
import sys
from socketserver import BaseRequestHandler, TCPServer
from uuid import uuid4


class Handler(BaseRequestHandler):
  users = {}
  accounts = {}

  def handle(self):
    client = f'client {self.client_address}'
    req = self.request.recv(1024) #recv()는 TCP protocol에서 인자로 받은 데이터의 길이만큼 받게 되며, 이 메소드는 소켓이 열려 있는 동안 데이터를 수신할 것이라는 보장이 있다.
    if not req:
      print(f'{client} unexpectedly disconnected')
      return

    print(f'{client} < {req}')
    req = req.decode('utf8')
    if req[-1] != '\n':
      raise Exception('Max request length exceeded')

    method, entity_kind, entity_id = req[:-1].split(' ', 3)
    if (method != 'GET'
       or entity_kind not in ('user', 'account')
       or not entity_id.isdigit()):
      raise Exception('Bad request')


    # user 객체에 정보를 담기 시작
    if entity_kind == 'user':
      user = self.users.get(entity_id) or {'id': entity_id}
      self.users[entity_id] = user

      if 'name' not in user:
        user['name'] = str(uuid4()).split('-')[0]

      if 'account_id' not in user:
        account_id = str(len(self.accounts) + 1)
        account = {'id': account_id,
               'balance': random.randint(0, 100)}
        self.accounts[account_id] = account
        user['account_id'] = account_id
      self.send(user)
      return

    if entity_kind == 'account':
      account = self.accounts[entity_id]
      self.send(account)
      return

  def send(self, data):
    resp = json.dumps(data).encode('utf8') # user 객체를 json.dumps() 함
    print(f'client {self.client_address} > {resp}')
    self.request.sendall(resp) # 다시 client로 flush()


if __name__ == '__main__':
  port = int(sys.argv[1])
  with TCPServer(('127.0.0.1', port), Handler) as server: # reactive model이라 handler를 callback함수로 보낸다.
    server.serve_forever()


# to run,
# run eventloop.py first
# python3 server.py 53210
