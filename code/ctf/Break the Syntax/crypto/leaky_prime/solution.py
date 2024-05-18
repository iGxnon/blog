import socket
from Crypto.Util.number import long_to_bytes, isPrime

tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcp.connect(('leaky-prime.ch.bts.wh.edu.pl', 1337))
data = tcp.recv(8192).decode().strip()
tcp.close()

print(f'Response: \n{data}\n')

lines = data.splitlines()
ct = int(lines[0])
n = int(lines[1])
e = 65537
msb_pad = int(lines[2] + '0' * 20, 2)

print(f'ct: {ct}')
print(f'n: {n}')
print(f'msb: {msb_pad}')

if bin(msb_pad >> 20)[2:] != lines[2]:
    raise "Error"

q = -1
p = -1

for i in range(msb_pad + 1, msb_pad + 2 ** 20, 2):
    if n % i != 0:
        continue
    if not isPrime(i):
        continue
    q = n // i
    if isPrime(q):
        p = i
        break

phi = (p - 1) * (q - 1)

d = -1
k = 1
while True:
    if (k * phi + 1) % e == 0:
        d = (k * phi + 1) // e
        break
    k += 1

print(f'\ngot it! \nq: {q}\np: {p}\nphi_n: {phi}\ne: {e}\nd: {d}\n')

pt = pow(ct, d, n)

print(f'flag!: {long_to_bytes(pt).decode()}')