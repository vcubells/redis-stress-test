import redis
import threading

n_inserts = 1000000
n_threads = 8

def t(tid):
    #r = redis.StrictRedis(host='localhost', port=6379, db=0, password="QOTGFCJDKOWJZKNH")
    r = redis.StrictRedis(host='sl-us-south-1-portal.16.dblayer.com', port=29954, db=0, password="HQIUDWQMOBXRIIGR")

    inicio = tid * n_inserts
    fin = (tid + 1) * n_inserts

    for i in range(inicio, fin):
        v = r.get(i)
        print("{} : {}".format(tid, i))

for tid in range(n_threads):
    thread = threading.Thread(target=t, args=(tid,))    
    thread.start()