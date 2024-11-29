Prerequisites
-----------------
1. Java Runtime
2. Xcode
3. cmake
4. nodejs (>= 18.0)

One time-setup
--------------
Install node using nvm
```
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.1/install.sh | bash
nvm install 18
```


Build using Xcode
-------------------
1. mkdir build-xcode && cd build-xcode
2. nvm use 18
2. cmake .. -G Xcode
3. open *.xcodeproj
4. Open hello.cc and set a breakpoint in Init (Cmd + Shift + O to search hello.cc)
5. Run target node-addon (the choice of target is on the top)