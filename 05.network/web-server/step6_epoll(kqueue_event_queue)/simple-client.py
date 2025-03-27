# Simple client used to interact with concurrent servers.
#
# Launches N concurrent client connections, each executing a pre-set sequence of
# sends to the server, and logs what was received back.
#
# Tested with Python 3.6
#
# Eli Bendersky [https://eli.thegreenplace.net]
# This code is in the public domain.

# 1. goal: N개의 동시 클라이언트 연결을 생성하여 서버에 부하를 주는 것
# 2. 작동 방식:
#   - 각 클라이언트는 미리 정의된 메시지 시퀀스를 서버에 전송
#   - 각 클라이언트는 별도의 읽기 스레드를 통해 서버 응답을 로깅
#   - 모든 클라이언트는 동일한 패턴으로 동작
# 3. 특징:
#   - 서버로부터 '*' 문자를 받는지 확인
#   - 세 번의 메시지 전송 (각 전송 사이에 대기 시간 존재)
#   - '1111'이 수신되면 클라이언트 종료
#   - 총 실행 시간 측정

import argparse
import logging
import socket
import sys
import threading
import time


class ReadThread(threading.Thread):
    def __init__(self, name, sockobj):
        super().__init__()
        self.sockobj = sockobj
        self.name = name
        self.bufsize = 8 * 1024

    def run(self):
        fullbuf = b''
        while True:
            buf = self.sockobj.recv(self.bufsize)
            logging.info('{0} received {1}'.format(self.name, buf))
            fullbuf += buf
            if b'1111' in fullbuf:
                break


def make_new_connection(name, host, port):
    """Creates a single socket connection to the host:port.

    Sets a pre-set sequence of messages to the server with pre-set delays; in
    parallel, reads from the socket in a separate thread.
    """
    sockobj = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sockobj.connect((host, port))
    if sockobj.recv(1) != b'*':
        logging.error('Something is wrong! Did not receive *')
    logging.info('{0} connected...'.format(name))

    rthread = ReadThread(name, sockobj)
    rthread.start()

    s = b'^abc$de^abte$f'
    logging.info('{0} sending {1}'.format(name, s))
    sockobj.send(s)
    time.sleep(1.0)

    s = b'xyz^123'
    logging.info('{0} sending {1}'.format(name, s))
    sockobj.send(s)
    time.sleep(1.0)

    # The 0000 sent to the server here will result in an echo of 1111, which is
    # a sign for the reading thread to terminate.
    # Add WXY after 0000 to enable kill-switch in some servers.
    s = b'25$^ab0000$abab'
    logging.info('{0} sending {1}'.format(name, s))
    sockobj.send(s)
    time.sleep(0.2)

    rthread.join()
    sockobj.close()
    logging.info('{0} disconnecting'.format(name))


def main():
    argparser = argparse.ArgumentParser('Simple TCP client')
    argparser.add_argument('host', help='Server host name')
    argparser.add_argument('port', type=int, help='Server port')
    argparser.add_argument('-n', '--num_concurrent', type=int,
                           default=1,
                           help='Number of concurrent connections')
    args = argparser.parse_args()

    logging.basicConfig(
        level=logging.DEBUG,
        format='%(levelname)s:%(asctime)s:%(message)s')

    t1 = time.time()
    connections = []
    for i in range(args.num_concurrent):
        name = 'conn{0}'.format(i)
        tconn = threading.Thread(target=make_new_connection,
                                 args=(name, args.host, args.port))
        tconn.start()
        connections.append(tconn)

    for conn in connections:
        conn.join()

    print('Elapsed:', time.time() - t1)


if __name__ == '__main__':
    main()
