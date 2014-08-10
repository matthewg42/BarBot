To run the web interface:

1. Install go (package "golang" on Debian-based distros). If using Raspbian install go version 1.3:
  http://xivilization.net/~marek/blog/2014/07/05/go-1-dot-3-for-raspberry-pi/

2. Choose a directory to put GO libraries in. I chose ~/project/go
3. Set GOHOME in your .basrc or similar:

    export GOHOME=$HOME/project/go

4. Install sqlite3 & goserial go libraries:

    $ cd $GOHOME
    $ go get github.com/mattn/go-sqlite3
    $ go get github.com/tarm/goserial

5. Run the web server like this:

    $ cd ~/project/barbot/src/web
    $ go run bb1.go -serial /dev/ttyS0

6. Point your browser at http://localhost:8080/


