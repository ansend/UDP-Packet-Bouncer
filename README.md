# UDP-Packet-Bouncer
##Description
UDP Packet bouncer to measure latencys (posix). This program send a UDP packet to a specifed location assuming that it is bounced back. Inside the package is a timestamp of the local system and a counter. On return the of the packet the program can determine from the timestamp the latency and the number of lost packages of the counter values differ.

The measuring mechanism consist of two programs:

The Sender:
Will emit the UDP package and calculate upon return the latency. If no packet is return the Sender will no aquire data

The Bouncer:
Will bounce/reflect the package back to the sender. The same functionality could also be achieved by port-forwarding or NAT or redirections settings direct at the firewall.

##Build
Use of cmake as build system
Additional dependencies: libglib2.0

Checkout:
$>git clone https://github.com/ESATelerobotics/UDP-Packet-Bouncer.git

Change dir
$>cd UDP-Packet-Bouncer

Create build directory
$>mkdir build

Change into build directory
$>cd build

Invoke Cmake
$>cmake ../src

Build executables
$>make

The executables "Bender" and "Bouncer" will be in the local dir

##Usage
### Commandline help
For the standard help type:

$>Sender --help

or 

$>Bouncer --help

###Example simple
The connection between computer A IP:192.168.101.42 and computer B IP: 192.168.101.111 has to be tested.

On Computer A the Sender will be invoked with
$>Sender 192.168.101.111 25000 26000
specifies sending UDP packets to 192.168.101.42 port 25000 and awaits the return on port 26000

In order to work on computer B the Bouncer has to be started with
$>Bouncer 192.168.101.42 26000 25000
specifies that it is listening on port 25000 and bounce incoming upd packets to 192.168.101.42 port 26000

On the sender side in addtion the sample rate, the packet size, and the logfile can be specified

###Example advanced

n Computer A the Sender will be invoked with
$>Sender 192.168.101.111 25000 26000 -v -p 512 -s 1000 -l /home/Thomas/log.txt
specifies sending UDP packets to 192.168.101.42 port 25000 and awaits the return on port 26000
In addition a packet with the of 512 byte will be send every 1000 us, the output is displayed on the screen and data logged at /home/Thomas/log.txt

In order to work on computer B the Bouncer has to be started with
$>Bouncer 192.168.101.42 26000 25000
specifies that it is listening on port 25000 and bounce incoming upd packets to 192.168.101.42 port 26000

On the sender side in addtion the sample rate, the packet size, and the logfile can be specified


###Interpreting the logs
The log file or the verbose output on the Sender side contains the latency in ms, the counter value, accumulated the lost messages, Timer-misses and maxium latency

latency in ms => is the duration the packet needed to return

Counter => Increment to determine lost packages

Lost Messages => Accumulated how many packets have been lost (Determined by missing counters)

Timer-misses => The sending loop runs a constant rate (as good as possible) with real-time kernel. The value should not increase at all or just very slowly. If it increases it is a sign that the system is running into performance problems which then would give wrong measurements sinds it is unknown if the latency/jitter of the network link is measured or that of the operating system.

Max_Latency => Is the maximum measured latency

A typical log locks like this:

Latency ms | Counter | Lost messages | Timer-misses | Max_Latency ms
====================================================================
0.232711         0       0       0       0.232711
0.304229         1       0       0       0.304229
0.240813         2       0       0       0.304229
0.285511         3       0       0       0.304229
0.285511         4       0       0       0.304229
0.247238         5       0       0       0.304229
0.246400         6       0       0       0.304229
0.279645         7       0       0       0.304229
0.413460         8       0       0       0.413460

##Hints
To check the connection the packets can be made larger or the sending frequency could be increased. With the measured latencys it could be determined when the link goes into saturation (when latency increases or packet loss increases). However when this is done the Timer-misses value should be monitored because if this one also increases it is the OS and not the link running into performance problems.


