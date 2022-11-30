Sockets allow communication between two different processes on the same or different machines. 
To be more precise, it's a way to talk to other computers using standard Unix file descriptors. 
In Unix, every I/O action is done by writing or reading a file descriptor. 
A file descriptor is just an integer associated with an open file and it can be a network connection, 
a text file, a terminal, or something else.

To a programmer, a socket looks and behaves much like a low-level file descriptor. 
This is because commands such as read() and write() work with sockets in the same way they do with files and pipes.

Socket types:
1) Stream Sockets - Доставка гарантирована. Если мы отправляем, A, B, C - они придут в том же порядке в целости и сохранности.
Этот тип сокетов использует TCP. Если доставка невозможна, отправитель получает ошибку. Объем данных безграничен.

2) Datagram Sockets - Доставка негарантирована. Если в TCP активная сторона устанавливает соединения с пассивной стороной,
то здесь в пакете просто указываются данные получателя и пакет отправляется.
Такие сокеты используют UDP. Благодаря этому протоколу возможен Multicast - т.е. , когда отправитель отправляет пакет, 
конкретным получателям, относящимся к конкретной группе(группа здесь - это какое-то адресное пространство). Это не тоже самое
что Broadcast. Difference: Broadcast - one to all, Multicast - one to many.

Как сделать клиента:
1) Создаём сокет используя системный вызов socket()
2) Подключаем сокет к адресу сервера, используя системный вызов connect()
3) Отсылаем и получаем данные, при помощи системных вызовов write() и read() соответственно.

Как сделать сервер:
1) Создаём сокет используя системный вызов socket()
2) Биндим сокет к адресу, используя системный вызов bind().
3) Слушаем соединения, используя системный вызов listen()
4) Принимаем соединения используя системный вызов accept(). Этот вызов обычно блокирует соединения, до тех пор, пока клиент
не подключится к серверу.
5) Отсылаем и принимаем данные при помощи read() и write().

