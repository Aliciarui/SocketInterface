## Custom Protocol Communication based on Socket Interface

#### Purpose

According to the self-defined protocol specification, use the Socket programming interface to write basic network application software.

- Master the usage of Socket programming interface in C language, and be able to send and receive network data packets correctly
- Develop a client to realize the human-computer interaction interface and communication with the server
- Develop a server to implement concurrent processing of multiple client requests
- The program interface is not required, just use the command line or the simplest form
- The functional requirements are as follows:
  - Transport layer protocol uses TCP
  - The client uses an interactive menu form, and the user can select the following functions:
    - Connection: request to connect to the server of the specified address and port
    - Disconnect: disconnect from the server
    - Get time: request the server to give the current time
    - Get the name: request the server to give the name of its machine
    - Active connection list: request the server to give all the client information (number, IP address, port, etc.) currently connected
    - Send a message: request the server to forward the message to the client with the corresponding number, and the client will display it on the screen after receiving it
    - Exit: Disconnect and exit the client program
  - After the server receives the client's request, it completes a specific task according to the instructions from the client:
    - Send the current time of the server's machine to the client
    - Send the name of the machine where the server is located to the client
    - Transmit all client information currently connected to the client
    - Forward the content sent by a client to other clients with a specified number
    - The asynchronous multi-threaded programming mode is adopted to correctly handle the situation where multiple clients connect at the same time and send messages at the same time
- According to the above functional requirements, design an application communication protocol between the client and the server



#### main equipment

- Networked PC, Wireshark software

- Visual C++, gcc and other C++ integrated development environments.



#### Experimental results

##### data pack

![1](/pic/1.png)(Request packet format)

![2](/pic/2.png)(Response packet format)

![3](/pic/3.png)(Indicated packet format)



##### Interface

![4](/pic/4.jpg)

![5](/pic/5.jpg)

![6](/pic/6.jpg)

![7](/pic/7.jpg)

![8](/pic/8.jpg)

(from Wireshark)

Three Handshake：

![9](/pic/9.jpg)

The first handshake, the type is SYN

![10](/pic/10.jpg)

The second handshake, the type is SYN and ACK

![11](/pic/11.jpg)

The second handshake, the type is ACK

![12](/pic/12.jpg)

![13](/pic/13.jpg)

![14](/pic/14.jpg)



##### Client's receiving data sub-thread loop

![15](/pic/15.jpg)

Use nested while statements to perform sub-thread loops, determine the received response data packet/indicating the boundary of the data packet through the feature of the fixed size of the data packet, call the recv function to determine the size of the received data packet, once the size and size are received If the corresponding/indicating data packet size is the same, then enter the judgment

![16](/pic/16.jpg)

Use if and else if statements to identify the keyword of the response/indication data packet, and output the corresponding information after identifying the corresponding Command character, and then enter the next sub-thread loop.



##### Send and Receive Message

![17](/pic/17.jpg)

