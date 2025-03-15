1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

By checking the EOF chracter at the end of the transmission, the remove client can determine when a command's output is fully recieved. Partial reads are handled by using a loop that continuesly calls recv() unit EOF is detect. To make sure of complete message transmission, the server sends chunks of data at a time followed by a EOF. The client process each chuck and checking if the last bite is EOF. IF it is, it will exit. 

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

The networked shell protocol define and detect the beginning and end of a command by using a special EOF character RDSH_EOF_CHAR to mark the end of a transition. If it is not handled correctly the user could get misintrepreted data,and incomplete data. They could also get stuck waiting for more data which would lead to a corrupted output. By using proffer buffering and recv() loop, we can handle message fragments. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintin information across multiple client-server. It remebers pervious requests and client state. This is great for keeping the connections going, but it also requires more resources. Stateless protocol handles each request individually, with no stored client conetext. This is great because it saves resources, but it may require clients to send all necessary thinsg with each request. 

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used when speed and low latency is more important than reliablity. It does not ensure packet order or establish connections making it a great choice for applications where real time performence is crucial.  Ex: online gaming

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

It is the Berkeley Socket API. This allows application to send and recieve data over network using standard system calls. 